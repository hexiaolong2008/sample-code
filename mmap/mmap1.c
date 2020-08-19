/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>

static void *kaddr;

static int my_mmap(struct file *file, struct vm_area_struct *vma)
{
	return remap_pfn_range(vma, vma->vm_start,
				(virt_to_phys(kaddr) >> PAGE_SHIFT) + vma->vm_pgoff,
				vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static struct file_operations my_fops = {
	.owner	= THIS_MODULE,
	.mmap	= my_mmap,
};
 
static struct miscdevice mdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "my_dev",
	.fops = &my_fops,
};
 
static int __init my_init(void)
{
	kaddr = kzalloc(PAGE_SIZE * 3, GFP_KERNEL);
	return misc_register(&mdev);
}
module_init(my_init);

MODULE_AUTHOR("Leon He <hexiaolong2008@gmail.com");
MODULE_DESCRIPTION("mmap simple demo");
MODULE_LICENSE("GPL v2");
