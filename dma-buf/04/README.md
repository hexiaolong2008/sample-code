## 说明
* exporter-fd.c - exporter 内核驱动
* mmap_dmabuf.c - userspace应用程序

## 驱动编译
1. 拷贝 exporter-fd.c 文件到 linux-4.14.143/drivers/dma-buf/ 目录下。
2. 修改 linux-4.14.143/drivers/dma-buf/Makefile，添加如下内容：
```bash
obj-m += exporter-fd.o
```
3. make

最终会在 linux-4.14.143/drivers/dma-buf 目录下生成 exporter-fd.ko 文件。



## 应用程序编译
1. 拷贝整个 dmabuf-test 文件夹到 linux-4.14.143/samples/ 目录下。
2. make -C samples/dmabuf-test

最终会在 linux-4.14.143/samples/dmabuf-test/ 目录下生成 mmap_dmabuf 可执行程序。
