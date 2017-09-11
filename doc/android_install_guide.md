# 下载SDK软件包

请于官网下载语音SDK，链接为：[SDK下载](http://ai.chumenwenwen.com/pages/document/intro?)。  

Android版SDK文件格式为：*speechsdk-{level}-{version}.zip*。  
* {level}： Android版根据功能强弱定制了3种版本：全量版(normal)，轻量版(lite)，超轻量版(mini)  
* {version}: Android版SDK发布版本号

所有版本均支持以下平台:
* x86_64
* MIPS
* ARMv7
* ARMv8

# 解压SDK软件包

可参考如下命令将SDK软件包解压到指定目录。此处假设所下载的软件包为：speechsdk-normal-1.04-1503567360357.zip（并假设其存放于HOME目录下），用户欲将之解压到HOME目录下的dev子目录：

```shell
$ mkdir -p ~/dev/
$ cd ~/dev/
$ unzip ~/speechsdk-normal-1.04-1503567360357.zip
$ cd SpeechSDKDemo
```

解压后的目录，有如下文件或子目录：

| 文件目录项      | 作用                                                |
|-----------------|-----------------------------------------------------|
| SpeechSDKLib    | Android版语音SDK(aar包)                    |
| demo             | 示例程序：涵盖热词唤醒，语音合成，在线与离线语音识别等|
| robot         | 示例程序：简单的聊天机器人程序     |
| ....       | 其他文件或目录均是标准gradle工程项目文件                         |

**注意：**
* 其他非全量版(normal)SDK解压后目录与上面类似，在此不再赘述

