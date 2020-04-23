/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drmP.h>
#include <drm/drm_encoder.h>

static struct drm_device drm;
static struct drm_plane primary;
static struct drm_crtc crtc;
static struct drm_encoder encoder;
static struct drm_connector connector;

static const struct drm_plane_funcs vkms_plane_funcs;
static const struct drm_crtc_funcs vkms_crtc_funcs;
static const struct drm_encoder_funcs vkms_encoder_funcs;
static const struct drm_connector_funcs vkms_connector_funcs;

static const u32 vkms_formats[] = {
	DRM_FORMAT_XRGB8888,
};

static void vkms_modeset_init(void)
{
	drm_mode_config_init(&drm);

	drm_universal_plane_init(&drm, &primary, 0, &vkms_plane_funcs,
				 vkms_formats, ARRAY_SIZE(vkms_formats),
				 NULL, DRM_PLANE_TYPE_PRIMARY, NULL);

	drm_crtc_init_with_planes(&drm, &crtc, &primary, NULL, &vkms_crtc_funcs, NULL);

	drm_encoder_init(&drm, &encoder, &vkms_encoder_funcs, DRM_MODE_ENCODER_VIRTUAL, NULL);

	drm_connector_init(&drm, &connector, &vkms_connector_funcs, DRM_MODE_CONNECTOR_VIRTUAL);
}

static const struct file_operations vkms_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.poll = drm_poll,
	.read = drm_read,
};

static struct drm_driver vkms_driver = {
	.driver_features = DRIVER_MODESET,
	.fops			= &vkms_fops,

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
