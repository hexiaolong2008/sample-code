1. Android版本：8.1

2. 请将mytest文件夹拷贝到frameworks/native/libs/gui/tests/目录下，并添加如下代码到frameworks/native/libs/gui/tests/Android.bp文件的最后一行：

subdirs = ["*"]

3. mmm frameworks/native/libs/gui/ -j8

4. 编译完成后会在out目录下对应的data/nativetests目录生成mytest文件夹

4. 有任何问题，请发邮件到343005384@qq.com