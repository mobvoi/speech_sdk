// Copyright 2016 Mobvoi Inc. All Rights Reserved.

#ifndef SPEECH_SDK_H_
#define SPEECH_SDK_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mobvoi_hotword_handler_vtable {
  /**
   * 当热词被触发时，回调该方法。
   */
  void (*on_hotword_detected)();
} mobvoi_hotword_handler_vtable;

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

enum mobvoi_recognizer_error_code {
  /**
   * 无错误。不会回调on error。
   */
  MOBVOI_RECOGNIZER_NO_ERROR = 0,
  /**
   * 服务器错误。
   */
  MOBVOI_RECOGNIZER_SERVER_ERROR = 1,
  /**
   * 网络错误。
   */
  MOBVOI_RECOGNIZER_NETWORK_ERROR = 2,
  /**
   * 识别语音内容为空。即认为没有人说话。
   */
  MOBVOI_RECOGNIZER_NO_SPEECH = 4,
  /**
   * 内部错误。
   */
  MOBVOI_RECOGNIZER_INTERNAL_ERROR = 5,
};

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

enum mobvoi_tts_type {
  /**
   * NO TTS。
   */
   MOBVOI_NO_TTS = 0,
  /**
   * 在线TTS。
   */
  MOBVOI_TTS_ONLINE = 1,
  /**
   * 离线TTS。
   */
  MOBVOI_TTS_OFFLINE = 2,
  /**
   * 混合TTS。
   */
  MOBVOI_TTS_MIX = 3,
};

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

/******************* global *********************/
/**
 * SDK初始化。
 *
 * @param path SDK的 .mobvoi 配置目录所在的路径，即 .mobvoi 的父目录。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_sdk_init(const char* path);

/**
 * 释放SDK资源。
 */
void mobvoi_sdk_cleanup();

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

/**
 * 设置verbose log级别。
 *
 * @param level 按照log详细程度在[1,3]中取值, 若level=3, 对录音数据进行dump。
 */
void mobvoi_set_vlog_level(int level);

/******************* hotword *********************/

/**
 * 添加热词回调。
 *
 * @param handlers 热词回调实例。
 */
void mobvoi_hotword_add_handler(mobvoi_hotword_handler_vtable* handlers);

/**
 * 移除之前设置的热词回调。
 *
 * @param handlers 热词回调实例。
 */
void mobvoi_hotword_remove_handler(mobvoi_hotword_handler_vtable* handlers);

/**
 * 开始监听热词。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_hotword_start();

/**
 * 停止监听热词。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_hotword_stop();

/***************** recognizer ********************/
/**
 * 初始化离线语音识别模型。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_recognizer_init_offline();

/**
 * 设置语音识别的回调。
 *
 * @param handlers 语音识别回调。
 */
void mobvoi_recognizer_set_handler(mobvoi_recognizer_handler_vtable* handlers);

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
 * "offline_model"        : "general"
 * 
 * @param value 参考识别参数表。
 */
void mobvoi_recognizer_set_params(const char* key, const char* value);

/**
 * 设置应用名称列表，用于离线识别应用名。
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

/**
 * 离线语音识别模型数据编译。
 * 数据设置请参考mobvoi_recognizer_set_data。
 */
void mobvoi_recognizer_build_data();

/**
 * 离线语音识别模型关键词编译。
 * 关键词设置请参考mobvoi_recognizer_set_keywords。
 *
 * @param model_name　语音快词模型的名称。
 */
void mobvoi_recognizer_build_keywords(const char* model_name);

/**
 * 开启一个语音识别任务。
 *
 * @param recognizer_type 参照mobvoi_recognizer_type。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_recognizer_start(mobvoi_recognizer_type recognizer_type);

/**
 * 停止目前正在进行的语音识别任务，并且等待结果的返回。
 * @return　0表示成功, -1表示失败。
 */
int mobvoi_recognizer_stop();

/**
 * 取消目前正在进行的语音识别任务，不会有后续结果返回。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_recognizer_cancel();

/***************** tts ********************/
/**
 * 初始化离线TTS模型。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_tts_init();

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

/**
 * 开始TTS合成。
 *
 * @param type 参考mobvoi_tts_type。
 * @param text 需要合成的字符串。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_tts_start_synthesis(mobvoi_tts_type type, const char* text);

/**
 * 取消上次的TTS合成。
 *
 * @return 0表示成功, -1表示失败。
 */
int mobvoi_tts_cancel_synthesis();

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

#ifdef __cplusplus
}
#endif

#endif  // SPEECH_SDK_H_
