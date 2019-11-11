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

#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/slab.h>

extern struct dma_buf *dmabuf_exported;

static int importer_test(struct dma_buf *dmabuf)
{
        struct dma_buf_attachment *attachment;
        struct sg_table *table;
	struct device *dev;
	unsigned int reg_addr, reg_size;

	if (!dmabuf)
		return -EINVAL;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;
	dev_set_name(dev, "importer");

        attachment = dma_buf_attach(dmabuf, dev);
        if (IS_ERR(attachment)) {
		pr_err("dma_buf_attach() failed\n");
                return PTR_ERR(attachment);
	}

        table = dma_buf_map_attachment(attachment, DMA_BIDIRECTIONAL);
        if (IS_ERR(table)) {
		pr_err("dma_buf_map_attachment() failed\n");
		dma_buf_detach(dmabuf, attachment);
                return PTR_ERR(table);
        }

	reg_addr = sg_dma_address(table->sgl);
	reg_size = sg_dma_len(table->sgl);
	pr_info("reg_addr = 0x%08x, reg_size = 0x%08x\n", reg_addr, reg_size);

	dma_buf_unmap_attachment(attachment, table, DMA_BIDIRECTIONAL);
	dma_buf_detach(dmabuf, attachment);

        return 0;
}

static int __init importer_init(void)
{
	return importer_test(dmabuf_exported);
}

static void __exit importer_exit(void)
{
	pr_info("importer exit\n");
}

module_init(importer_init);
module_exit(importer_exit);

MODULE_AUTHOR("Leon He <343005384@qq.com>");
MODULE_DESCRIPTION("DMA-BUF Importer example for dma-access");
MODULE_LICENSE("GPL v2");

