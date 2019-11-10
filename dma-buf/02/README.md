## 编译说明
 1. 拷贝 exporter-kmap.c 和 importer-kmap.c 文件到 linux-4.14.143/drivers/dma-buf/ 目录下。
 2. 修改 linux-4.14.143/drivers/dma-buf/Makefile，添加如下内容：
```bash
obj-m += exporter-kmap.o importer-kmap.o
```
 3. make

最终会在 linux-4.14.143/drivers/dma-buf 目录下生成 exporter-kmap.ko 和 importer-kmap.ko 文件。

