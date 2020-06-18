/**
 * Author: Leon He
 * e-mail: 343005384@qq.com
 *
 * Compilation for Ubuntu: gcc -o dumb dumb.c `pkg-config --cflags --libs libdrm`
 */

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int main(int argc, char **argv)
{
        int fd;
        char *vaddr;
        struct drm_mode_create_dumb create_req = {};
        struct drm_mode_destroy_dumb destroy_req = {};
        struct drm_mode_map_dumb map_req = {};

        fd = open("/dev/dri/card0", O_RDWR);

        create_req.bpp = 32;
        create_req.width = 240;
        create_req.height = 320;
        drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_req);
        printf("create dumb: handle = %u, pitch = %u, size = %llu\n",
                create_req.handle, create_req.pitch, create_req.size);

        map_req.handle = create_req.handle;
        drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map_req);
        printf("get mmap offset 0x%llx\n", map_req.offset);

        vaddr = mmap(0, create_req.size, PROT_WRITE, MAP_SHARED, fd, map_req.offset);
        strcpy(vaddr, "This is a dumb buffer!");
        munmap(vaddr, create_req.size);

        vaddr = mmap(0, create_req.size, PROT_READ, MAP_SHARED, fd, map_req.offset);
        printf("read from mmap: %s\n", vaddr);
        munmap(vaddr, create_req.size);

        getchar();

        destroy_req.handle = create_req.handle;
        drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy_req);
        close(fd);

        return 0;
}
