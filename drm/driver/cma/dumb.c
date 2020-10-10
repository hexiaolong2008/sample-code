/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drmP.h>
#include <drm/drm_gem.h>

struct drm_gem_cma_object {
	struct drm_gem_object base;
	dma_addr_t paddr;
	void *vaddr;
};

static struct drm_device drm;

static int drm_gem_cma_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct drm_gem_cma_object *cma_obj;

	drm_gem_mmap(filp, vma);

	cma_obj = vma->vm_private_data;

	return remap_pfn_range(vma, vma->vm_start, cma_obj->paddr >> PAGE_SHIFT,
			vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static int drm_gem_cma_dumb_create(struct drm_file *file_priv,
			    struct drm_device *drm,
			    struct drm_mode_create_dumb *args)
{
	struct drm_gem_cma_object *cma_obj;

	args->pitch = args->width * args->bpp / 8;
	args->size = args->pitch * args->height;

	cma_obj = kzalloc(sizeof(*cma_obj), GFP_KERNEL);
	drm_gem_object_init(drm, &cma_obj->base, args->size);

	drm_gem_handle_create(file_priv, &cma_obj->base, &args->handle);

	cma_obj->vaddr = dma_alloc_wc(drm->dev, args->size, &cma_obj->paddr,
				      GFP_KERNEL | __GFP_NOWARN);

	return 0;
}

static const struct vm_operations_struct drm_gem_cma_vm_ops = {};

static const struct file_operations mygem_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.mmap = drm_gem_cma_mmap,
};

static struct drm_driver mygem_driver = {
	.driver_features	= DRIVER_GEM,
	.fops			= &mygem_fops,

	.dumb_create		= drm_gem_cma_dumb_create,
	.gem_vm_ops		= &drm_gem_cma_vm_ops,

	.name			= "my-gem-cma",
	.desc			= "My GEM CMA Driver",
	.date			= "20200901",
	.major			= 1,
	.minor			= 0,
};

static int __init mygem_init(void)
{
	drm_dev_init(&drm, &mygem_driver, NULL);
	drm_dev_register(&drm, 0);

	return 0;
}

module_init(mygem_init);

