#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

struct modeset_buf {
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	uint32_t size;
	uint32_t handle;
	uint8_t *vaddr;
	uint32_t fb_id;
};

struct modeset_dev {
	unsigned int front_index;
	struct modeset_buf bufs[2];

	drmModeModeInfo mode;
	uint32_t conn_id;
	uint32_t crtc_id;
};

static int modeset_create_fb(int fd, struct modeset_buf *buf)
{
	struct drm_mode_create_dumb create = {};
 	struct drm_mode_map_dumb map = {};

	create.width = buf->width;
	create.height = buf->height;
	create.bpp = 32;
	drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create);

	buf->stride = create.pitch;
	buf->size = create.size;
	buf->handle = create.handle;
	drmModeAddFB(fd, buf->width, buf->height, 24, 32, buf->stride,
			   buf->handle, &buf->fb_id);

	map.handle = buf->handle;
	drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map);

	buf->vaddr = mmap(0, buf->size, PROT_READ | PROT_WRITE, MAP_SHARED,
		        fd, map.offset);

	memset(buf->vaddr, 0, buf->size);

	return 0;
}

static void modeset_destroy_fb(int fd, struct modeset_buf *buf)
{
	struct drm_mode_destroy_dumb destroy = {};

	munmap(buf->vaddr, buf->size);

	drmModeRmFB(fd, buf->fb_id);

	destroy.handle = buf->handle;
	drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
}

static void modeset_page_flip_handler(int fd, unsigned int frame,
				    unsigned int sec, unsigned int usec,
				    void *data)
{
	struct modeset_dev *dev = data;
	struct modeset_buf *buf;
	unsigned int i, j, offset;
	static int index = 0;
	static const uint32_t color[] = {
		0xFF0000, 0x00FF00, 0x0000FF,
		0xFF00FF, 0xFFFF00, 0x00FFFF,
		0xFFFFFF, 0x000000
	};


	buf = &dev->bufs[dev->front_index ^ 1];
	for (i = 0; i < buf->height; i++) {
		for (j = 0; j < buf->width; j++) {
			offset = buf->stride * i + j * 4;
			*(uint32_t*)&buf->vaddr[offset] = color[index];
		}
	}
	index++;
	index %= 8;

	drmModePageFlip(fd, dev->crtc_id, buf->fb_id,
			      DRM_MODE_PAGE_FLIP_EVENT, dev);

	dev->front_index ^= 1;

	usleep(500000);
}

static void modeset_handle_event(int fd, struct modeset_dev *dev)
{
	fd_set fds;
	drmEventContext ev = {};

	ev.version = DRM_EVENT_CONTEXT_VERSION;
	ev.page_flip_handler = modeset_page_flip_handler;

	while (1) {
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(fd, &fds);

		select(fd + 1, &fds, NULL, NULL, NULL);

		if (FD_ISSET(0, &fds))
			break;

		if (FD_ISSET(fd, &fds))
			drmHandleEvent(fd, &ev);
	}
}

int main(int argc, char **argv)
{
	int fd;
	struct modeset_dev dev;
	drmModeConnector *conn;
	drmModeRes *res;

	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);

	res = drmModeGetResources(fd);
	conn = drmModeGetConnector(fd, res->connectors[0]);

	dev.crtc_id = res->crtcs[0];
	dev.conn_id = conn->connector_id;
	dev.bufs[0].width = conn->modes[0].hdisplay;
	dev.bufs[0].height = conn->modes[0].vdisplay;
	dev.bufs[1].width = conn->modes[0].hdisplay;
	dev.bufs[1].height = conn->modes[0].vdisplay;
	memcpy(&dev.mode, &conn->modes[0], sizeof(dev.mode));

	drmModeFreeConnector(conn);
	drmModeFreeResources(res);

	modeset_create_fb(fd, &dev.bufs[0]);
	modeset_create_fb(fd, &dev.bufs[1]);

	drmModeSetCrtc(fd, dev.crtc_id, dev.bufs[0].fb_id, 0, 0,
			     &dev.conn_id, 1, &dev.mode);

	drmModePageFlip(fd, dev.crtc_id, dev.bufs[0].fb_id,
			      DRM_MODE_PAGE_FLIP_EVENT, &dev);

	modeset_handle_event(fd, &dev);

	modeset_destroy_fb(fd, &dev.bufs[1]);
	modeset_destroy_fb(fd, &dev.bufs[0]);

	close(fd);

	return 0;
}
