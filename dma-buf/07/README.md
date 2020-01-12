## 说明
* exporter-sync.c - exporter 内核驱动
* importer-sync.c - importer 内核驱动
* dmabuf_sync.c - userspace应用程序

## 驱动编译
1. 拷贝 exporter-sync.c & importer-sync.c 文件到 linux-4.14.143/drivers/dma-buf/ 目录下。
2. 修改 linux-4.14.143/drivers/dma-buf/Makefile，添加如下内容：
```bash
obj-m += exporter-sync.o importer-sync.o
```
3. make

最终会在 linux-4.14.143/drivers/dma-buf 目录下生成 exporter-sync.ko 和 importer-sync.ko 文件。



## 应用程序编译
1. 拷贝整个 dmabuf-test 文件夹到 linux-4.14.143/samples/ 目录下。
2. make headers_install
3. make -C samples/dmabuf-test

最终会在 linux-4.14.143/samples/dmabuf-test/ 目录下生成 dmabuf_sync 可执行程序。
