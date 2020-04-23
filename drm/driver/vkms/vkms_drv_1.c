/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drmP.h>

static struct drm_device drm;

static struct drm_driver vkms_driver = {
	.name			= "vkms",
	.desc			= "Virtual Kernel Mode Setting",
	.date			= "20180514",
	.major			= 1,
	.minor			= 0,
};

static int __init vkms_init(void)
{
	drm_dev_init(&drm, &vkms_driver, NULL);
	drm_dev_register(&drm, 0);

	return 0;
}

module_init(vkms_init);
