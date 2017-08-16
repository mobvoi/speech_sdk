# SDK初始化
-----------

在使用SDK之初，应进行SDK的初始化。

SDK的初始化，使用mobvoi_sdk_init(）来完成：

``` cpp
/**
 * SDK初始化。
 *
 * @param appkey SDK授权键值。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_sdk_init(const char* appkey);
```

# 语音识别
----------

## 相关设置

进行语音识别之前，应进行相关的设置。这些设置，由mobvoi_recognizer_set_params()和mobvoi_recognizer_set_handler()来完成。

其中，mobvoi_recognizer_set_params()用于设置语音识别的相关参数：

```cpp
/**
 * 设置识别任务参数。
 *
 * @param key 参考识别参数表。
 *
 * ------------------------------------------------------------------------------
 * parameter name         : example value
 * ------------------------------------------------------------------------------
 * "user_id"              : "12345678"
 * "location"             : "中国,北京市,北京市,海淀区,苏州街,3号,39.989602,116.316568"
 * "remote_start_silence" : "5000"(ms)
 * "remote_end_silence"   : "1000"(ms)
 * "local_start_silence"  : "60"(ms)
 * "local_end_silence"    : "1000"(ms)
 * "offline_model"        : "genernal"
 * "mobvoi_folder"        : "/var/.mobvoi/"
 * 
 * @param value 参考识别参数表。
 */
void mobvoi_recognizer_set_params(const char* key, const char* value);
```
**注意**：
* 参数 mobvoi_folder 所指定的目录，应当是一个具备可写权限的目录。在 QNX 系统中，许多文件系统默认都只具有只读权限，因此这一点需要特别注意。

而mobvoi_recognizer_set_handler()用于设置语音识别过程中相关事件的处理函数（即回调函数），当相应事件发生时，用户所提供的回调函数将被调用。

```cpp
/**
 * 设置语音识别的回调。
 *
 * @param handlers 语音识别回调。
 */
void mobvoi_recognizer_set_handler(mobvoi_recognizer_handler_vtable* handlers);

```

其中，mobvoi_recognizer_handler_vtable的定义如下：

```cpp
typedef struct _mobvoi_recognizer_handler_vtable {
  /**
 　* 在检测到本地语音之后，又检测到本地静音时回调。
 　*/
  void (*on_local_silence_detected)();
  /**
   * 服务器端检测到静音（说话人停止说话）后回调。
   */
  void (*on_remote_silence_detected)();

  /**
   * 语音识别部分结果返回，比如“今天天气怎么样”，会按顺序返回“今天”，“今天天气”。
   *
   * @param result 识别到的部分结果字符串。
   */
  void (*on_partial_transcription)(const char* result);

  /**
   * 语音识别最终结果返回，比如“今天天气怎么样”，会按顺序返回“今天”，“今天天气”，
   * “今天天气怎么样”，最后一个就是Final Transcription。
   *
   * @param result 最终认为用户说的完整字符串。
   */
  void (*on_final_transcription)(const char* result);

  /**
   * 语音搜索结果返回, 为JSON格式字符串。
   *
   * @param result 语音搜索结果json。
   */
  void (*on_result)(const char* result);

  /**
   * 计算得到输入语音片断的声压（SPL）值时，回调该方法。
   *
   * @param volume 根据语音的能量算出，取值范围：[0.0, 1.0]。
   */
  void (*on_volume)(float volume);

  /**
   * 当执行出错时会回调。
   *
   * @param error_code 错误码。
   */
  void (*on_error)(int error_code);

} mobvoi_recognizer_handler_vtable;
```

## 开启语音识别

使用如下函数开启语音识别：

```cpp
/**
 * 开启一个语音识别任务。
 *
 * @param recognizer_type 参照mobvoi_recognizer_type。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_recognizer_start(mobvoi_recognizer_type recognizer_type);

```

其中的mobvoi_recognizer_type定义如下：

```cpp
enum mobvoi_recognizer_type {
  /**
   * 在线语音识别，返回语音识别结果。
   */
  MOBVOI_RECOGNIZER_ONLINE_ASR = 0,
  /**
   * 在线语音识别和语义理解，返回语音识别和语义理解结果。
   */
  MOBVOI_RECOGNIZER_ONLINE_SEMANTIC = 1,
  /**
   * 在线语音识别和垂直搜索，返回语音识别和垂直搜索结果。
   */
  MOBVOI_RECOGNIZER_ONLINE_ONEBOX = 2,
  /**
   * 离线语音识别。
   */
  MOBVOI_RECOGNIZER_OFFLINE = 3,
  /**
   * 混合在线和离线的语音识别。
   */
  MOBVOI_RECOGNIZER_MIX = 4,
  /**
   * 多关键词识别。
   */
  MOBVOI_RECOGNIZER_KEYWORDS = 5,
};

```

**注意：**
* 对于离线识别（MOBVOI_RECOGNIZER_OFFLINE），在开启语音识别之前，还应进行进一步的设置。详见后文中**离线识别**小节中的阐述。
* 开启语音识别之后，用户还需要向SDK提供语音数据，SDK才可以真正的进行语音识别。向SDK提供语音数据，由mobvoi_send_speech_frame()函数来进行。下一小节，将对此进行介绍。

## 向SDK提供语音数据

向SDK提供语音数据，由mobvoi_send_speech_frame()函数来进行。

```cpp
/**
 * 发送语音数据给SDK，要求格式为16k采样，16bit位深的小端(Little-Endian)
 * 单声道PCM数据。
 *
 * @param frame 发送数据的缓冲区。
 * @param size 发送数据缓冲区长度。
 *
 * @return SDK真正收到的数据长度，返回-1表示参数有误。
 */
int mobvoi_send_speech_frame(const char* frame, int size);
```

## 停止/取消语音识别

开启语音识别任务后，可使用mobvoi_recognizer_stop()及mobvoi_recognizer_cancel()来停止或取消语音识别。

使用mobvoi_recognizer_stop()来停止语音识别：

```cpp
/**
 * 停止目前正在进行的语音识别任务，并且等待结果的返回。
 * @return　0表示成功, -1表示失败。
 */
int mobvoi_recognizer_stop();
```

使用mobvoi_recognizer_cancel()来取消语音识别：

```cpp
/**
 * 取消目前正在进行的语音识别任务，不会有后续结果返回。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_recognizer_cancel();
```

# 离线识别
----------

开启离线识别类型的识别任务之前，应使用mobvoi_recognizer_init_offline()进行相关的设置。

相关函数定义如下：

```cpp
/**
 * 初始化离线语音识别模型。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_recognizer_init_offline();
```
**注意**
* 使用离线识别之前，应先进行语音识别的相关配置。
* 离线识别作为语音识别的一种，具体用法，应遵从**语音识别**一节中的相关介绍。

# 热词唤醒
----------

一般情况下，用户并不希望语音识别功能一直处于开启状态，而是希望以特定的词汇开启这一功能。这在SDK中，被称作“热词唤醒”。

在本SDK中，唤醒所使用的关键词是：“你好问问”。

下面介绍SDK热词唤醒相关的用法：

## 配置热词唤醒回调函数

在SDK检测到热词后，将会调用相应的回调函数。相关定义如下：

```cpp
/**
 * 添加热词回调。
 *
 * @param handlers 热词回调实例。
 */
void mobvoi_hotword_add_handler(mobvoi_hotword_handler_vtable* handlers);
```

其中，mobvoi_hotword_handler_vtable定义如下：

```cpp
typedef struct _mobvoi_hotword_handler_vtable {
  /**
   * 当热词被触发时，回调该方法。
   */
  void (*on_hotword_detected)();
} mobvoi_hotword_handler_vtable;
```
** 提示**

* 用户可于该回调函数中，再开启真正的语音识别任务。

## 开启热词唤醒功能

```cpp
/**
 * 开始监听热词。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_hotword_start();
```

## 关闭热词唤醒功能

```cpp
/**
 * 停止监听热词。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_hotword_stop();
```

## 取消热词唤醒回调函数

```cpp
/**
 * 移除之前设置的热词回调。
 *
 * @param handlers 热词回调实例。
 */
void mobvoi_hotword_remove_handler(mobvoi_hotword_handler_vtable* handlers);
```
# 多关键词识别
--------------

多关键词识别，功能类似于热词唤醒，可以不开启在线语音识别的情况下，同时对多个关键词进行监听。此外，还可以支持联系人、应用、音乐及视频相关的语义理解。

多关键词识别的使用方法，类似于离线语音识别，也应先进行语音识别的相关配置。具体用法，应遵从**语音识别**一节中的相关介绍。

## 语义理解设置

多关键词识别中的语义理解设置，使用mobvoi_recognizer_set_data()及mobvoi_recognizer_build_data()来完成。如下是这两个函数的相关定义：

```cpp
/**
 * 设置多关键词识别所需支持的语义理解内容。
 *
 * @param type 需要设置的数据类型，参考mobvoi_recognizer_offline_data_type。
 * @param num 数据字符串的数量。
 * @param data 某种类型对应的字符串列表，如APPS对应的是["支付宝", "微信",
 * "微博"]。
 *
 * 注意：数据设置不会立即生效，需调用mobvoi_recogizer_build_data。
 */
void mobvoi_recognizer_set_data(mobvoi_recognizer_offline_data_type type,
                                int num, const char* data[]);
```

其中，mobvoi_recognizer_offline_data_type定义如下：

```cpp
enum mobvoi_recognizer_offline_data_type {
  /**
   * 需要离线识别的应用名称。
   */
  MOBVOI_RECOGNIZER_OFFLINE_DATA_APPS = 0,
  /**
   * 需要离线识别的联系人名称。
   */
  MOBVOI_RECOGNIZER_OFFLINE_DATA_CONTACTS = 1,
  /**
   * 需要离线识别的艺术家。
   */
  MOBVOI_RECOGNIZER_OFFLINE_DATA_ARTISTS = 2,
  /**
   * 需要离线识别的歌曲名。
   */
  MOBVOI_RECOGNIZER_OFFLINE_DATA_SONGS = 3,
  /**
   * 需要离线识别的唱片名。
   */
  MOBVOI_RECOGNIZER_OFFLINE_DATA_ALBUMS = 4,
  /**
   * 需要离线识别的视频名。
   */
  MOBVOI_RECOGNIZER_OFFLINE_DATA_VIDEOS = 5,
};
```
**注意**：
* mobvoi_recognizer_set_data()的设置不会立即生效，需调用mobvoi_recognizer_build_data()来使之生效。

```cpp
/**
 * 离线语音识别模型数据编译。
 * 数据设置请参考mobvoi_recognizer_set_data。
 */
void mobvoi_recognizer_build_data();
```
## 多关键词设置

多关键词的识别，使用mobvoi_recognizer_set_keywords()及mobvoi_recognizer_build_keywords()来完成。如下是这两个函数的相关定义：

```cpp
/**
 * 设置语音快词列表。
 *
 * @param num　语音快词数量。
 * @param keywords　语音快词列表。
 * @param model_name　语音快词模型的名称。
 *
 * 注意：快词设置不会立即生效，需调用mobvoi_recogizer_build_keywords。
 */
void mobvoi_recognizer_set_keywords(int num, const char* keywords[],
                                    const char* model_name);
```
**注意**：
* mobvoi_recognizer_set_keywords()的设置不会立即生效，需调用mobvoi_recogizer_build_keywords()来使之生效。
* mobvoi_recognizer_set_keywords()要求用户定义相应设置的model_name，这样，用户就可以定义多组关键词，并在不同场景下切换应当处于活跃状态的那一组关键词。

```cpp
/**
 * 离线语音识别模型关键词编译。
 * 关键词设置请参考mobvoi_recognizer_set_keywords。
 *
 * @param model_name　语音快词模型的名称。
 */
void mobvoi_recognizer_build_keywords(const char* model_name);
```

# 语音合成
----------

语音识别用于为用户给定的文本生成对应的语音。

本SDK生成的语音为16K采样率、16位位深、小端（Little-Endian）的单声道 PCM 数据流。

## 相关设置

语音合成相关的设置，由mobvoi_tts_init()和mobvoi_tts_set_params()来完成。如下是这两个函数的原型：

```cpp
/**
 * 初始化离线TTS模型。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_tts_init();
```

```cpp
/**
 * 设置TTS参数。
 *
 * @param key 参考TTS参数表。
 * ---------------------------------------------------------
 * |"gender"               |"female"                       |
 * --------------------------------------------------------|
 * |"language"             |"chinese"                      |
 * ---------------------------------------------------------
 * @param value 参考TTS参数表。
 */
void mobvoi_tts_set_params(const char* key, const char* value);
```
其中，参数的取值范围如下（注意：参数及对应的取值，均为字符串，故使用时，都应置于""中）：

| 参数      | 取值范围         |  
|-----------|------------------|   
| gender 	| male, female   　| 
| language  | chinese, english |

## 启动语音识别

使用mobvoi_tts_start_synthesis()来合成给定文本的语音，该函数为非阻塞方式。待语音合成后，用户调用mobvoi_tts_read_data()来读取合成的语音流。

相关函数声明如下：

```cpp
/**
 * 开始TTS合成。
 *
 * @param type 参考mobvoi_tts_type。
 * @param text 需要合成的字符串。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_tts_start_synthesis(mobvoi_tts_type type, const char* text);
```

```cpp
/**
 * 读取合成后的音频数据，格式为16k采样，16bit位深的小端(Little-Endian)
 * 单声道PCM数据。
 *
 * @param data 输出缓冲区。
 * @param length 缓冲区大小。
 *
 * @return 若>0表示真正读到的字节数, 0表示读完;-1表示出错。
 */
int mobvoi_tts_read_data(char* data, int length);
```
## 取消语音合成

使用mobvoi_tts_start_synthesis()启动语音合成后，可以使用mobvoi_tts_cancel_synthesis()来取消。

相关函数声明如下：

```cpp
/**
 * 取消上次的TTS合成。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_tts_cancel_synthesis();
```

# SDK结束清理
-------------

在使用SDK结束之后，使用mobvoi_sdk_cleanup()相应相应资源的回收清理。

如下是相应的函数声明：

```cpp
/**
 * 释放SDK资源。
 */
void mobvoi_sdk_cleanup();
```

# SDK调试
---------

SDK运行时，可以产生一些调试信息。用户可以使用mobvoi_set_vlog_level()设置SDK调试/日志信息的丰富程度。

* 在Linux系统上，这些调试信息会输出在标准输出及标准错误输出上。
* 在QNX系统上，这些调试信息会以slog方式输出。

如下是相应的函数声明：

```cpp
/**
 * 设置verbose log级别。
 *
 * @param level 按照log详细程度在[1,3]中取值, 若level=3, 对录音数据进行dump。
 */
void mobvoi_set_vlog_level(int level);
```

**备注**：
* 声明 level 为 3 后，SDK除了会打印相应的级别的调试信息，而且还会将发送给 SDK 的语音数据保存在 *\<dir\>/audio_dump/record.pcm* 文件中。用户如感觉语音识别准确率不够高时，可以先检查对应的语音数据是否清晰。
  * 其中，\<dir\> 是用户调用 mobvoi_recognizer_set_params() 函数时，mobvoi_folder 参数的取值。
  * 如前文所述，record.pcm 文件中保存的语音格式，为16K采样率、16位位深、小端（Little-Endian）的PCM数据流。
