## 说明
exporter-page.c - exporter 内核驱动
importer-page.c - importer 内核驱动
dmabuf_sync.c - userspace应用程序，mmap、begin/end cpu_access 测试
share_fd.c - userspace应用程序，kmap、vmap、map attachment 测试

## 驱动编译
1. 拷贝 exporter-page.c & importer-page.c 文件到 linux-4.14.143/drivers/dma-buf/ 目录下。
2. 修改 linux-4.14.143/drivers/dma-buf/Makefile，添加如下内容：
```bash
obj-m += exporter-page.o importer-page.o
```
3. make

最终会在 linux-4.14.143/drivers/dma-buf 目录下生成 exporter-page.ko 和 importer-page.ko 文件。



## 应用程序编译
1. 拷贝整个 dmabuf-test 文件夹到 linux-4.14.143/samples/ 目录下。
2. make headers_install
3. make -C samples/dmabuf-test

最终会在 linux-4.14.143/samples/dmabuf-test/ 目录下生成 dmabuf_sync 和 share_fd 可执行程序。
