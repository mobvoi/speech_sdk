// Copyright 2017 Mobvoi Inc. All Rights Reserved.

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "http_util.h"
#include "player.h"
#include "recorder.h"
#include "speech_sdk.h"

static pthread_mutex_t mutex;
static pthread_cond_t cond;

static const int kBufferSize = 640;
static const int kLineWrapLength = 100;

static const int kPlaySampleRate = 16000;
static const int kPlayChannelNum = 2;

struct AppCmdOptions {
  mobvoi_recognizer_type recognzier_type;
  std::string record_dev;
  std::string play_dev;
  int channels;
  int sample_rate;

  AppCmdOptions()
      : recognzier_type(MOBVOI_RECOGNIZER_ONLINE_ONEBOX),
        record_dev("default"),
        play_dev("default"),
        channels(1),
        sample_rate(16000) {}
};
static AppCmdOptions g_cmd_opts = AppCmdOptions();

static volatile bool in_the_session = true;

bool ParseCmdLine(int argc, const char* argv[], AppCmdOptions* options) {
  if (argc < 2) return false;

  std::string type(argv[1]);
  if (type == "online") {
    options->recognzier_type = MOBVOI_RECOGNIZER_ONLINE_ONEBOX;
  } else if (type == "offline") {
    options->recognzier_type = MOBVOI_RECOGNIZER_OFFLINE;
  } else if (type == "mix") {
    options->recognzier_type = MOBVOI_RECOGNIZER_MIX;
  } else {
    std::cerr << "Unsupported recognizer type:" << type << std::endl;
    return false;
  }

  int arg_pos = 2;
  while (arg_pos < argc) {
    std::string opt(argv[arg_pos]);

    if (opt.length() != 2 || opt[0] != '-') {
      std::cerr << "Invalid option format:" << opt << std::endl;
      return false;
    }

    if (arg_pos + 1 >= argc) {
      std::cerr << "Expect a value after option:" << opt << std::endl;
      return false;
    }

    switch (opt[1]) {
      case 'D':
        options->record_dev = argv[arg_pos + 1];
        break;
      case 'd':
        options->play_dev = argv[arg_pos + 1];
        break;
      case 'r':
        options->sample_rate = atoi(argv[arg_pos + 1]);
        break;
      case 'c':
        options->channels = atoi(argv[arg_pos + 1]);
        if (options->channels != 1 && options->channels != 2) {
          std::cerr << "Bad channel number, only support 1 or 2" << std::endl;
          return false;
        }
        break;
      default:
        std::cerr << "Unrecognized option:" << opt << std::endl;
        return false;
    }

    arg_pos += 2;
  }

  return true;
}

void MonoToStereo(const char* in, int in_len, const char* out, int out_len) {
  assert(in_len * 2 == out_len);
  for (int i = 0; i < in_len / 2; i++) {
    *((short*)out + 2 * i) = *((short*)in + i);
    *((short*)out + 2 * i + 1) = *((short*)in + i);
  }
}

std::string GetTTS(const std::string& final_transcription) {
  static const std::string kPromptWords[] = {
    std::string("\"displayText\":\""),
    std::string("\"query\":\""),
  };

  size_t pos;
  size_t i;

  for (i = 0; i < sizeof(kPromptWords) / sizeof(kPromptWords[0]); ++i) {
    pos = final_transcription.find(kPromptWords[i], 0);
    if (pos != std::string::npos) {
      break;
    }
  }

  if (pos != std::string::npos) {
    size_t start = pos + kPromptWords[i].size();
    size_t end = final_transcription.find('"', start);
    if (end != std::string::npos) {
      std::string promt_content =
          final_transcription.substr(start, end - start);
      return promt_content;
    }
  }

  return "问问没有听清,您能再说一次吗";
}

void PlaySound(const char* tts_url) {
  static const int kPlaybackBufSize = 1600;
  static const int kPlaySampleRate = 16000;
  static const int kPlayChannelNum = 2;

  std::string down_bytes;
  if (!HttpUtil::GetInMemory(tts_url, &down_bytes)) return;
  Player::Open(g_cmd_opts.play_dev, kPlaySampleRate, kPlayChannelNum);
  int read_pos = 0;
  char buf[kPlaybackBufSize] = {0};
  char stereoBuf[kPlaybackBufSize * 2] = {0};

  while (true) {
    int left_size = down_bytes.size() - read_pos;
    if (left_size <= 0) break;
    int actual_read = std::min(left_size, kPlaybackBufSize);
    memcpy(buf, down_bytes.c_str() + read_pos, actual_read);
    MonoToStereo(buf, actual_read, stereoBuf, actual_read * 2);
    Player::PlayAudioData(stereoBuf, actual_read * 2);
    read_pos += actual_read;
  }
  Player::Close();
}

static void* tts_read_thread(void* arg) {
  int bytes = 0;
  Player::Open(g_cmd_opts.play_dev, kPlaySampleRate, kPlayChannelNum);
  char buffer[kBufferSize];
  char stereoBuf[kBufferSize * 2];
  while ((bytes = mobvoi_tts_read_data(buffer, sizeof(buffer))) != -1) {
    MonoToStereo(buffer, bytes, stereoBuf, bytes * 2);
    Player::PlayAudioData(stereoBuf, bytes * 2);
  }
  Player::Close();
}

void on_remote_silence_detected() {
  std::cout << "--------> dummy on_remote_silence_detected" << std::endl;
}

void on_partial_transcription(const char* result) {
  std::cout << "--------> dummy on_partial_transcription: " << result
            << std::endl;
}

void on_final_transcription(const char* result) {
  std::cout << "--------> dummy on_final_transcription: " << result
            << std::endl;
}

void on_result(const char* result) {
  std::cout << "--------> dummy on_result:" << std::endl;
  std::string s(result);
  for (int i = 0; i < s.size(); i += kLineWrapLength) {
    int len = std::min((int)(s.size() - i), kLineWrapLength);
    std::string temp = s.substr(i, len);
    std::cout << temp << std::endl;
  }
  std::string tts = GetTTS(result);
  if (!tts.empty()) {
    pthread_t tid;
    std::cout << "tts = " << tts << std::endl;
    std::cout << "--------> play the tts sound..." << std::endl;
    mobvoi_tts_start_synthesis(MOBVOI_TTS_OFFLINE, tts.c_str());
    int result = pthread_create(&tid, NULL, tts_read_thread, NULL);
    assert(result);
  }

  pthread_mutex_lock(&mutex);
  in_the_session = false;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

void on_error(int error_code) {
  std::cout << "--------> dummy on_error with error code: " << error_code
            << std::endl;
  pthread_mutex_lock(&mutex);
  in_the_session = false;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

void on_local_silence_detected() {
  std::cout << "--------> dummy on_local_silence_detected" << std::endl;
  mobvoi_recognizer_stop();
}

void on_hotword_detected() {
  std::cout << "--------> dummy on_hotword_detected" << std::endl;
  mobvoi_hotword_stop();
  mobvoi_recognizer_start(g_cmd_opts.recognzier_type);
}

void ShowUsage() {
  std::cerr << "Usage: asr <online/offline/mix> options" << std::endl;
  std::cerr << "-D  specify capture device('default'). " << std::endl;
  std::cerr << "-d  specify playback device('default')." << std::endl;
  std::cerr << "-c  channel numbers(1)." << std::endl;
  std::cerr << "-r  sample rate(16000)." << std::endl;
  std::cerr << std::endl;
  std::cerr << "Example 1: asr online" << std::endl;
  std::cerr << "Example 2: asr online -D hw:0,0 -d plughw:1,0" << std::endl;
}

void on_speech_spl_generated(float spl) {
  // the sound press level is here, do whatever you want.
  // std::cout << "--------> dummy on_speech_spl_generated: spl = "
  //           << std::fixed << std::setprecision(6) << spl
  //           << std::endl;
}

int main(int argc, const char* argv[]) {
  if (!ParseCmdLine(argc, argv, &g_cmd_opts)) {
    ShowUsage();
    return 1;
  }

  // SDK initilalize including callback functions
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  mobvoi_sdk_init(NULL);
  mobvoi_tts_init();
  mobvoi_recognizer_set_params(
      "location", "中国,北京市,北京市,海淀区,苏州街,3号,39.989602,116.316568");

  mobvoi_recognizer_handler_vtable* speech_handlers =
      new mobvoi_recognizer_handler_vtable;
  assert(speech_handlers != NULL);
  speech_handlers->on_error = &on_error;
  speech_handlers->on_final_transcription = &on_final_transcription;
  speech_handlers->on_partial_transcription = &on_partial_transcription;
  speech_handlers->on_local_silence_detected = &on_local_silence_detected;
  speech_handlers->on_remote_silence_detected = &on_remote_silence_detected;
  speech_handlers->on_result = &on_result;
  speech_handlers->on_volume = &on_speech_spl_generated;
  mobvoi_recognizer_set_handler(speech_handlers);

  mobvoi_hotword_handler_vtable* hotword_handler =
      new mobvoi_hotword_handler_vtable;
  hotword_handler->on_hotword_detected = &on_hotword_detected;
  mobvoi_hotword_add_handler(hotword_handler);

  if (g_cmd_opts.recognzier_type == MOBVOI_RECOGNIZER_OFFLINE ||
      g_cmd_opts.recognzier_type == MOBVOI_RECOGNIZER_MIX) {
    mobvoi_recognizer_init_offline();
    const char* contacts[3];
    contacts[0] = "焦莹璀";
    contacts[1] = "李勤";
    contacts[2] = "王斌";
    mobvoi_recognizer_set_data(MOBVOI_RECOGNIZER_OFFLINE_DATA_CONTACTS, 3,
                               contacts);

    const char* apps[3];
    apps[0] = "支付宝";
    apps[1] = "微信";
    apps[2] = "京东";
    mobvoi_recognizer_set_data(MOBVOI_RECOGNIZER_OFFLINE_DATA_APPS, 3, apps);

    const char* artists[4];
    artists[0] = "刘德华";
    artists[1] = "周华健";
    artists[2] = "毛宁";
    artists[3] = "乌兰图雅";
    mobvoi_recognizer_set_data(MOBVOI_RECOGNIZER_OFFLINE_DATA_ARTISTS, 4,
                               artists);

    const char* songs[3];
    songs[0] = "冰雨";
    songs[1] = "最真的梦";
    songs[2] = "涛声依旧";
    mobvoi_recognizer_set_data(MOBVOI_RECOGNIZER_OFFLINE_DATA_SONGS, 3, songs);

    const char* albums[3];
    albums[0] = "冰雨";
    albums[1] = "最真的梦";
    albums[2] = "涛声依旧";
    mobvoi_recognizer_set_data(MOBVOI_RECOGNIZER_OFFLINE_DATA_ALBUMS, 3,
                               albums);

    const char* videos[3];
    videos[0] = "水浒传";
    videos[1] = "三国演义";
    videos[2] = "红楼梦";
    mobvoi_recognizer_set_data(MOBVOI_RECOGNIZER_OFFLINE_DATA_VIDEOS, 3,
                               videos);

    mobvoi_recognizer_build_data();
  }

  Recorder::Open(g_cmd_opts.record_dev, g_cmd_opts.sample_rate,
                 g_cmd_opts.channels);

  // Try hotword
  do {
    std::cout << "--------> start hotword" << std::endl;
    mobvoi_hotword_start();

    pthread_mutex_lock(&mutex);
    in_the_session = true;
    while (in_the_session) {
      pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Prompt if we need to continue SDK test.
    char type;
    while (1) {
      std::cout << std::endl;
      std::cout << "###################### Continue or not?[y/n] "
                   "######################"
                << std::endl;
      std::cout << std::endl;
      std::cin >> type;

      if (type == 'y' || type == 'n') {
        mobvoi_tts_cancel_synthesis();
        break;
      }
    }

    if (type == 'n') {
      break;
    }
  } while (1);

  Recorder::Close();

  // SDK Clean up
  std::cout << "start sdk cleanup" << std::endl;
  mobvoi_sdk_cleanup();
  std::cout << "end sdk cleanup" << std::endl;
  delete speech_handlers;
  std::cout << "end dummy sender" << std::endl;
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
