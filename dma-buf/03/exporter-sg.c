/*
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/slab.h>

struct dma_buf *dmabuf_exported;
EXPORT_SYMBOL(dmabuf_exported);

static int exporter_attach(struct dma_buf *dmabuf, struct device *dev,
			struct dma_buf_attachment *attachment)
{
	pr_info("dmabuf attach device: %s\n", dev_name(dev));
	return 0;
}

static void exporter_detach(struct dma_buf *dmabuf, struct dma_buf_attachment *attachment)
{
	pr_info("dmabuf detach device: %s\n", dev_name(attachment->dev));
}

static struct sg_table *exporter_map_dma_buf(struct dma_buf_attachment *attachment,
					 enum dma_data_direction dir)
{
	void *vaddr = attachment->dmabuf->priv;
	struct sg_table *table;
	int err;

	table = kmalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		return ERR_PTR(-ENOMEM);

	err = sg_alloc_table(table, 1, GFP_KERNEL);
	if (err) {
		kfree(table);
		return ERR_PTR(err);
	}

	sg_dma_len(table->sgl) = PAGE_SIZE;
	sg_dma_address(table->sgl) = dma_map_single(NULL, vaddr, PAGE_SIZE, dir);

	return table;
}

static void exporter_unmap_dma_buf(struct dma_buf_attachment *attachment,
			       struct sg_table *table,
			       enum dma_data_direction dir)
{
	dma_unmap_single(NULL, sg_dma_address(table->sgl), PAGE_SIZE, dir);
	sg_free_table(table);
	kfree(table);
}

static void exporter_release(struct dma_buf *dmabuf)
{
	kfree(dmabuf->priv);
}

static void *exporter_kmap_atomic(struct dma_buf *dmabuf, unsigned long page_num)
{
	return NULL;
}

static void *exporter_kmap(struct dma_buf *dmabuf, unsigned long page_num)
{
	return NULL;
}

static int exporter_mmap(struct dma_buf *dmabuf, struct vm_area_struct *vma)
{
	return -ENODEV;
}

static const struct dma_buf_ops exp_dmabuf_ops = {
	.attach = exporter_attach,
	.detach = exporter_detach,
	.map_dma_buf = exporter_map_dma_buf,
	.unmap_dma_buf = exporter_unmap_dma_buf,
	.release = exporter_release,
	.map = exporter_kmap,
	.map_atomic = exporter_kmap_atomic,
	.mmap = exporter_mmap,
};

static struct dma_buf *exporter_alloc_page(void)
{
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);
	struct dma_buf *dmabuf;
	void *vaddr;

	vaddr = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (!vaddr)
		return NULL;

	exp_info.ops = &exp_dmabuf_ops;
	exp_info.size = PAGE_SIZE;
	exp_info.flags = O_CLOEXEC;
	exp_info.priv = vaddr;

	dmabuf = dma_buf_export(&exp_info);
	if (IS_ERR(dmabuf)) {
		kfree(vaddr);
		return NULL;
	}

	sprintf(vaddr, "hello world!");

	return dmabuf;
}

static int __init exporter_init(void)
{
	dmabuf_exported = exporter_alloc_page();
	if (!dmabuf_exported) {
		pr_err("error: exporter alloc page failed\n");
		return -ENOMEM;
	}

	return 0; 
}

static void __exit exporter_exit(void)
{
	pr_info("exporter exit\n");
}

module_init(exporter_init);
module_exit(exporter_exit);

MODULE_AUTHOR("Leon He <343005384@qq.com>");
MODULE_DESCRIPTION("DMA-BUF Exporter example for dma-access");
MODULE_LICENSE("GPL v2");

