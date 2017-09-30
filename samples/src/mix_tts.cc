// Copyright 2017 Mobvoi Inc. All Rights Reserved.
// Author: wmzheng@mobvoi.com (Wenming Zheng)

#include <pthread.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include "linux/util.h"
#include "speech_sdk.h"

const static int kBufferSize = 640;
const static std::string kTtsFile = "mix_tts_";
const static char* kText[] = {
    "北京市海淀区今天天气晴朗",
    "华北对流天气减弱,东北雷雨天气增多,北京市海淀区今天天气晴朗,后天降雨,"
    "为您找到以下结果请你查看",
    "据悉，黄瓜主要分布在韩国的中南部地区，是耐高温、生命力强的品种。此外，西瓜作为夏季非常欢迎的水果，价格也呈上升趋势。"
    "据统计结果，1个西瓜平均的销售价格为1.79万元(约合人民币105元)，相比往年的1.57万元，上升14%。还有一些地方，1个西瓜的销售价格为2.16万元。",
    "16岁的时候他去做了泥水匠。一年以后没生意，转行去西藏收废轮胎，一收就是四年。四处漂泊的青春里他开始后悔没有继续读书了，而此后的人生他也将吃尽没有文化的苦头,"
    "甚至连收据上签字都让他难堪。他说家里一直没有余钱，只能搞些歪门邪道的生意。他去刮千年古树的皮子，六块钱一斤，拉到丽江去，几吨几吨地卖。他也挖树根，四米的树根，"
    "长了一千多年，挖去给别人做茶几。警察会查，要是真被抓住就完了，但每次塞一万块钱，总能过去,十多年后在车上，他跟我说起这些事的时候冷静、诚恳、事无巨细。"
    "他说：我这一辈子，好事做了不少。坏事也是",
};

static char buffer[kBufferSize];

static void* tts_read(void* arg) {
  int index = *(int*)arg;
  std::string file_name = kTtsFile;
  file_name += util::IntToString(index);
  file_name += ".pcm";
  std::ofstream output(file_name.c_str());
  int data_bytes = -1;
  memset(buffer, 0, sizeof(buffer));
  while ((data_bytes = mobvoi_tts_read_data(buffer, kBufferSize)) != -1) {
    output.write(buffer, data_bytes);
  }
  output.close();
  std::cout << "read mix tts successfully" << std::endl;
  return NULL;
}

int main(int argc, char* argv[]) {
  mobvoi_sdk_init(NULL);
  mobvoi_tts_init();
  std::cout << "sdk inited!" << std::endl;
  pthread_t tid;
  int len = sizeof(kText) / sizeof(kText[0]);
  for (int i = 0; i < len; ++i) {
    mobvoi_tts_start_synthesis(MOBVOI_TTS_MIX, kText[i]);
    pthread_create(&tid, NULL, tts_read, &i);
    mobvoi_tts_cancel_synthesis();
  }
  for (int i = 0; i < len; ++i) {
    int index = i + len;
    mobvoi_tts_start_synthesis(MOBVOI_TTS_MIX, kText[i]);
    tts_read(&index);
    mobvoi_tts_cancel_synthesis();
  }
  mobvoi_sdk_cleanup();
  std::cout << "sdk cleanup!" << std::endl;

  return 0;
}
