## 编译说明
 1. 拷贝 exporter-sg.c 和 importer-sg.c 文件到 linux-4.14.143/drivers/dma-buf/ 目录下。
 2. 修改 linux-4.14.143/drivers/dma-buf/Makefile，添加如下内容：
```bash
obj-m += exporter-sg.o importer-sg.o
```
 3. make

最终会在 linux-4.14.143/drivers/dma-buf 目录下生成 exporter-sg.ko 和 importer-sg.ko 文件。

