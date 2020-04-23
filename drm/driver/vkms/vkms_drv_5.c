/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drm_crtc_helper.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>

static struct drm_device drm;
static struct drm_plane primary;
static struct drm_crtc crtc;
static struct drm_encoder encoder;
static struct drm_connector connector;

static void vkms_crtc_dpms(struct drm_crtc *crtc, int mode)
{
}

static int vkms_crtc_mode_set(struct drm_crtc *crtc,
				struct drm_display_mode *mode,
				struct drm_display_mode *adjusted_mode,
				int x, int y, struct drm_framebuffer *old_fb)
{
	return 0;
}

static void vkms_crtc_prepare(struct drm_crtc *crtc)
{
}

static void vkms_crtc_commit(struct drm_crtc *crtc)
{
}

static int vkms_crtc_page_flip(struct drm_crtc *crtc,
				struct drm_framebuffer *fb,
				struct drm_pending_vblank_event *event,
				uint32_t page_flip_flags,
				struct drm_modeset_acquire_ctx *ctx)
{
	unsigned long flags;

	crtc->primary->fb = fb;
	if (event) {
		spin_lock_irqsave(&crtc->dev->event_lock, flags);
		drm_crtc_send_vblank_event(crtc, event);
		spin_unlock_irqrestore(&crtc->dev->event_lock, flags);
	}
	return 0;
}

static const struct drm_crtc_helper_funcs vkms_crtc_helper_funcs = {
	.dpms = vkms_crtc_dpms,
	.mode_set = vkms_crtc_mode_set,
	.prepare = vkms_crtc_prepare,
	.commit = vkms_crtc_commit,
};

static const struct drm_crtc_funcs vkms_crtc_funcs = {
	.set_config = drm_crtc_helper_set_config,
	.page_flip = vkms_crtc_page_flip,
	.destroy = drm_crtc_cleanup,
};

static const struct drm_plane_funcs vkms_plane_funcs = {
	.update_plane = drm_primary_helper_update,
	.disable_plane = drm_primary_helper_disable,
	.destroy = drm_plane_cleanup,
};

static int vkms_connector_get_modes(struct drm_connector *connector)
{
	int count;

	count = drm_add_modes_noedid(connector, 8192, 8192);
	drm_set_preferred_mode(connector, 1024, 768);

	return count;
}

static struct drm_encoder *vkms_connector_best_encoder(struct drm_connector *connector)
{
	return &encoder;
}

static const struct drm_connector_helper_funcs vkms_conn_helper_funcs = {
	.get_modes = vkms_connector_get_modes,
	.best_encoder = vkms_connector_best_encoder,
};


static const struct drm_connector_funcs vkms_connector_funcs = {
	.dpms = drm_helper_connector_dpms,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = drm_connector_cleanup,
};

static const struct drm_encoder_funcs vkms_encoder_funcs = {
	.destroy = drm_encoder_cleanup,
};

static const struct drm_mode_config_funcs vkms_mode_funcs = {
	.fb_create = drm_fb_cma_create,
};

static const u32 vkms_formats[] = {
	DRM_FORMAT_XRGB8888,
};

static void vkms_modeset_init(void)
{
	drm_mode_config_init(&drm);
	drm.mode_config.max_width = 8192;
	drm.mode_config.max_height = 8192;
	drm.mode_config.funcs = &vkms_mode_funcs;

	drm_universal_plane_init(&drm, &primary, 0, &vkms_plane_funcs,
				 		vkms_formats, ARRAY_SIZE(vkms_formats),
				 		NULL, DRM_PLANE_TYPE_PRIMARY, NULL);

	drm_crtc_init_with_planes(&drm, &crtc, &primary, NULL, &vkms_crtc_funcs, NULL);
	drm_crtc_helper_add(&crtc, &vkms_crtc_helper_funcs);

	drm_encoder_init(&drm, &encoder, &vkms_encoder_funcs, DRM_MODE_ENCODER_VIRTUAL, NULL);

	drm_connector_init(&drm, &connector, &vkms_connector_funcs, DRM_MODE_CONNECTOR_VIRTUAL);
	drm_connector_helper_add(&connector, &vkms_conn_helper_funcs);
	drm_mode_connector_attach_encoder(&connector, &encoder);
}

static const struct file_operations vkms_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.poll = drm_poll,
	.read = drm_read,
	.mmap = drm_gem_cma_mmap,
};

static struct drm_driver vkms_driver = {
	.driver_features	= DRIVER_MODESET | DRIVER_GEM,
	.fops			= &vkms_fops,

	.dumb_create	= drm_gem_cma_dumb_create,
	.gem_vm_ops		= &drm_gem_cma_vm_ops,
	.gem_free_object_unlocked = drm_gem_cma_free_object,

	.name			= "vkms",
	.desc			= "Virtual Kernel Mode Setting",
	.date			= "20180514",
	.major			= 1,
	.minor			= 0,
};

static int __init vkms_init(void)
{
	drm_dev_init(&drm, &vkms_driver, NULL);

	vkms_modeset_init();

	drm_dev_register(&drm, 0);

	return 0;
}

module_init(vkms_init);

