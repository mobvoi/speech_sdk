# 下载SDK软件包

请于官网下载语音SDK，链接为：[SDK下载](http://ai.chumenwenwen.com/pages/document/intro?)。

SDK文件名格式为：*speechsdk-{version}.tar.gz*。现在所支持的平台有：
* x86_64
* ARMv7
* ARMv8


# 解压SDK软件包

可参考如下命令将SDK软件包解压到指定目录。此处假设所下载的软件包为：speechsdk-1.1.0.tar.gz（并假设其存放于HOME目录下），用户欲将之解压到HOME目录下的dev子目录：

```shell
$ mkdir -p ~/dev/
$ cd ~/dev/
$ tar xvfz ~/speechsdk-1.1.0.tar.gz
```

解压后的目录，有如下文件或子目录：

| 文件目录项      | 作用                                                |
|-----------------|-----------------------------------------------------|
| speech_sdk.h    | 使用语音SDK所需要包含的头文件                       |
| lib             | 使用语音SDK所需要动态链接的共享库（libmobvoisdk.so）|
| .mobvoi         | SDK运行时所使用的配置文件。注意它是一个隐藏目录     |
| samples         | 样例代码及相应的可执行程序                          |
| README          | SDK的简要说明                                       |

**注意：**
* 解压SDK后所得到的.mobvoi隐藏目录，是SDK运行时所需要的。因此，在发布基于本SDK的语音应用时，应将.mobvoi目录一同部署。
* 在编写基于本SDK的语音应用时，请使用mobvoi_recognizer_set_params()函数来指定.mobvoi所在的位置。注意该目录应当具有可写权限。比如若用户选择将.mobvoi目录部署在/var下，则需要在sdk初始化后调用如下函数进行设置：
  *  mobvoi_recognizer_set_params("mobvoi_folder", "/var/.mobvoi/")
