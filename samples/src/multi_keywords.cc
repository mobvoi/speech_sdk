// Copyright 2017 Mobvoi Inc. All Rights Reserved.

#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

#include "player.h"
#include "recorder.h"
#include "speech_sdk.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
volatile bool in_the_session = true;

static const std::string kOfflineModel = "offline_model";
static const std::string kOfflineModelName = "multi_keywords_test";
static const char
    * kMultiKeywords[] = {"打开地图", "你好大众", "问众智能", "明天天气如何", "把音量降低"};
static const int kBufferSize = 640;
static const int kChannelNum = 2;
static const int kSampleRate = 16000;

static void
MonoToStereo(const char* in, int in_len, const char* out, int out_len) {
  assert(in_len * 2 == out_len);
  for (int i = 0; i < in_len / 2; i++) {
    *((short*) out + 2 * i) = *((short*) in + i);
    *((short*) out + 2 * i + 1) = *((short*) in + i);
  }
}

static void* tts_play_thread(void* arg) {
  int bytes = 0;

  Player::Open("default", kSampleRate, kChannelNum);
  char buffer[kBufferSize];
  char stereoBuf[kBufferSize * 2];
  while ((bytes = mobvoi_tts_read_data(buffer, sizeof(buffer))) != -1) {
    MonoToStereo(buffer, bytes, stereoBuf, bytes * 2);
    Player::PlayAudioData(stereoBuf, bytes * 2);
  }
  Player::Close();

  mobvoi_tts_cancel_synthesis();
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
}

void on_remote_silence_detected() {
  std::cout << "--------> dummy on_remote_silence_detected" << std::endl;
}

void on_partial_transcription(const char* result) {
  std::cout << "--------> dummy on_partial_transcription: " << result
            << std::endl;

  std::string transcription(result);
  pthread_t tid;
  if (transcription.compare(" GARBAGE") == 0) {
    mobvoi_tts_start_synthesis(MOBVOI_TTS_ONLINE, "不能识别的关键词");
  } else {
    mobvoi_tts_start_synthesis(MOBVOI_TTS_ONLINE,
                               std::string("识别结果，").append(result).c_str());
  }
  int res = pthread_create(&tid, NULL, tts_play_thread, NULL);
  assert(0 == res);
}

void on_final_transcription(const char* result) {
  std::cout << "--------> dummy on_final_transcription: " << result
            << std::endl;
}

void on_result(const char* result) {
  std::cout << "--------> dummy on_result: result: " << result << std::endl;
}

void on_volume(float spl) {
  // the sound press level is here, do whatever you want.
  // std::cout << "--------> dummy on_speech_spl_generated: spl = "
  //           << std::fixed << std::setprecision(6) << spl
  //           << std::endl;
}

int main(int argc, const char* argv[]) {
  if (argc != 1) {
    std::cout << "usage:" << argv[0] << std::endl;
    return 1;
  }
  // SDK initilalize including callback functions
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  mobvoi_sdk_init(NULL);
  mobvoi_recognizer_init_offline();

  mobvoi_recognizer_handler_vtable* speech_handlers =
      new mobvoi_recognizer_handler_vtable;
  assert(speech_handlers != NULL);
  speech_handlers->on_error = &on_error;
  speech_handlers->on_final_transcription = &on_final_transcription;
  speech_handlers->on_local_silence_detected = &on_local_silence_detected;
  speech_handlers->on_remote_silence_detected = &on_remote_silence_detected;
  speech_handlers->on_partial_transcription = &on_partial_transcription;
  speech_handlers->on_result = &on_result;
  speech_handlers->on_volume = &on_volume;
  mobvoi_recognizer_set_handler(speech_handlers);

  mobvoi_recognizer_set_keywords(
      sizeof(kMultiKeywords) / sizeof(kMultiKeywords[0]),
      kMultiKeywords, kOfflineModelName.c_str());

  std::cout << "--------> start set multi-keywords parameter" << std::endl;
  mobvoi_recognizer_set_params(kOfflineModel.c_str(),
                               kOfflineModelName.c_str());

  mobvoi_recognizer_build_keywords(kOfflineModel.c_str());

  std::cout << "--------> start multi-keywords" << std::endl;
  mobvoi_recognizer_start(MOBVOI_RECOGNIZER_KEYWORDS);

  //Start record
  do {
    Recorder::Open("default", 16000, 1);
    pthread_mutex_lock(&mutex);
    in_the_session = true;
    while (in_the_session) {
      pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    Recorder::Close();
    // Prompt if we need to continue SDK test.
    char type;
    while (1) {
      std::cout << "Continue or not?[y/n] ";
      std::cin >> type;

      if (type == 'y' || type == 'n') {
        break;
      }
    }

    if (type == 'n') {
      break;
    }
  } while (1);

  std::cout << "--------> cancel multi-keywords" << std::endl;
  mobvoi_recognizer_cancel();

  // SDK Clean up
  std::cout << "start sdk cleanup" << std::endl;
  mobvoi_sdk_cleanup();
  std::cout << "end sdk cleanup" << std::endl;
  delete speech_handlers;
  std::cout << "delete speech_handlers" << std::endl;
  pthread_mutex_destroy(&mutex);
  std::cout << "pthread_mutex_destroy" << std::endl;
  pthread_cond_destroy(&cond);
  std::cout << "pthread_cond_destroy" << std::endl;
  return 0;
}
