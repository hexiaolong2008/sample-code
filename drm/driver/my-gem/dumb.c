/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drmP.h>
#include <drm/drm_gem_cma_helper.h>

static struct drm_device drm;

static const struct file_operations mygem_fops = {
        .owner = THIS_MODULE,
        .open = drm_open,
        .release = drm_release,
        .unlocked_ioctl = drm_ioctl,
        .poll = drm_poll,
        .read = drm_read,
        .mmap = drm_gem_cma_mmap,
};

static struct drm_driver mygem_driver = {
        .driver_features        = DRIVER_GEM,
        .fops                   = &mygem_fops,

        .dumb_create            = drm_gem_cma_dumb_create,
        .gem_vm_ops             = &drm_gem_cma_vm_ops,
        .gem_free_object_unlocked = drm_gem_cma_free_object,

        .name                   = "my-gem",
        .desc                   = "My GEM Driver",
        .date                   = "20200601",
        .major                  = 1,
        .minor                  = 0,
};

static int __init mygem_init(void)
{
        drm_dev_init(&drm, &mygem_driver, NULL);
        drm_dev_register(&drm, 0);

        return 0;
}

module_init(mygem_init);
