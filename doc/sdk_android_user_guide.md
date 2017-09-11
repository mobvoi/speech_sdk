# 核心类与接口
* *SpeechClient* 类:   
  单例类，语音服务对应的客户端，所有语音功能都应通过此对象交互
* *HotwordListener* 接口：  
  热词监听接口
* *SpeechClientListener* 接口：  
  语音服务回调接口
* *TTSListener* 接口：  
  语音合成回调接口

# SDK初始化
在使用SDK之初，应进行SDK的初始化。  

SDK的初始化，使用*SpeechClient*类的*init*方法来完成：

``` java 
    /**
     * 初始化SpeechClient。
     *
     * @param appContext    Android上下文。
     * @param appkey        平台对应的app key。
     * @param enableOnline  是否开启在线识别功能。
     * @param enableOffline 是否开启离线识别功能。
     */
    public void init(Context appContext, String appkey, boolean enableOnline, boolean enableOffline)
```
**注意**：
* 关于参数appKey，请参考如下链接，以获得一个有效的SDK授权键值：[http://ai.chumenwenwen.com/pages/document/get-started](http://ai.chumenwenwen.com/pages/document/get-started)
* *SpeechClient*类是单例。使用*SpeechClient.getInstance().XXXX* 来进行调用


# 语音识别

## 基本设置

SDK设置语音服务回调接口，使用*SpeechClient*类的*setClientListener*方法来完成：
```java
    /**
     * 设置语音识别的回调。
     *
     * @param deviceName     使用的设备名称。
     * @param clientListener 语音识别回调实例。
     */
    public void setClientListener(String deviceName, SpeechClientListener clientListener)
```
SpeechClientListener接口解释如下:
```java
/**
 * 语音服务回调。
 *
 */
public interface SpeechClientListener {
    /**
     * 开始提供录音数据给语音识别引擎时回调。
     */
    void onStartRecord();

    /**
     * 服务器端检测到静音（说话人停止说话）后回调。
     */
    void onRemoteSilenceDetected();

    /**
     * 输入语音数据实时的音量回调。
     *
     * @param volume 根据语音的能量算出，单位为db, 范围为[0, 60]。
     */
    void onVolume(double volume);

    /**
     * 语音识别部分结果返回，比如“今天天气怎么样”，会按顺序返回“今天”，“今天天气”。
     *
     * @param fixedContent 识别到的部分结果字符串。
     */
    void onPartialTranscription(String fixedContent);

    /**
     * 语音识别最终结果返回，比如“今天天气怎么样”，会按顺序返回“今天”，“今天天气”，“今天天气怎么样”，
     * 最后一个就是Final Transcription。
     *
     * @param result 最终认为用户说的完整字符串。
     */
    void onFinalTranscription(String result);

    /**
     * 语音搜索结果返回, 为JSON格式字符串。
     *
     * @param result 语音搜索结果json。
     */
    void onResult(String result);

    /**
     * 当执行出错时会回调。
     *
     * @param errorCode 错误码，See {@link ErrorCode}。
     */
    void onError(int errorCode);

    /**
     * 在检测到本地语音之后，又检测到本地静音时回调。
     */
    void onLocalSilenceDetected();

    /**
     * 一段时间未检测到本地语音时回调。
     */
    void onNoSpeechDetected();

    /**
     * 检测到本地语音时回调。
     */
    void onSpeechDetected();
}

```
## 开启语音识别
SDK支持多种语音识别结果
 * ASR, 在线语音识别，无语义分析，无搜索结果
 * Semantic, 在线语音识别，返回语义分析，无搜索结果
 * Onebox, 在线语音识别，并返回搜索结果
 * Offline, 离线语音识别，支持命令词识别
 * MultiKeywords, 多关键词识别，可长时间离线监听关键词
 * Mix, 混合语音识别，支持离线在线混合

针对以上不同的语音识别结果，*SpeechClient*类中拥有对应的*setXXXRecognizer*方法用来开启语音识别，例如：
```java
    /**
     * 开启一个语音任务，仅是online，返回语音识别结果，和搜索结果。
     *
     * @param clientName 使用的设备名称。
     */
    public synchronized void startOneboxRecognizer(String clientName);
```

**注意：**
* 对于离线识别和多关键词识别，在开启语音识别之前，还应进行进一步的设置。详见后文中**离线识别**与**多关键词识别**小节中的阐述。
* 调用*setXXXRecognizer*方法开启语音识别后，SDK会自动调用当前环境设备进行录音

## 停止/取消语音识别

使用*SpeechClient*类的stopRecognizer方法来停止语音识别：
```java
    /**
     * 停止目前正在进行的Recognizer任务，并且等待结果的返回，
     * 返回结果会调用{@link SpeechClientListener}中的相关方法。
     *
     * @param clientName 使用的设备名称。
     */
    public synchronized void stopRecognizer(final String clientName)
```
使用*SpeechClient*类的cancelRecognizer方法:
```java
    /**
     * 取消目前正在进行的Recognizer任务，不会有后续结果返回。
     *
     * @param clientName 使用的设备名称。
     */
    public synchronized void cancelReconizer(final String clientName) 
```

# 离线识别
使用*SpeechClient*类的*startOfflineRecognizer*方法开启:
```java
    /**
     * 开启一个语音任务，目前支持命令词识别，如“打电话给王路”，“打开支付宝”等。需要用户提供APP列表，通讯录列表或命令词列表。
     *
     * @param clientName 使用的设备名称。
     */
    public synchronized void startOfflineRecognizer(String clientName)
```
## 离线识别设置
使用离线识别之前，应先进行语音识别的相关配置。  
  参考*SpeechClient*类中*setApps*，*setVoiceAction*， *setContacts*方法  
  例如*setApps*方法：
  ```java
      /**
     * 设置应用名称列表，用于离线识别应用名。
     *
     * @param apps 设备上已安装的应用的名称列表,如：{"支付宝", "微信", "微博"}
     */
    public void setApps(String[] apps)
  ```
  
# 多关键词识别

多关键词识别，功能类似于热词唤醒，可以不开启在线语音识别的情况下，同时对多个关键词进行监听。此外，还可以支持联系人、应用、音乐及视频相关的语义理解。

使用*SpeechClient*类的*startMultiKeywordsRecognizer*方法开启:
```java
    /**
     * 开启一个语音任务，可以长时间离线监听关键词
     * 一次启动会无限监听，直到{@link #cancelReconizer(String)}被调用。
     *
     * @param clientName 使用的设备名称。
     * @param model      设置离线识别模型名称，只识别模型中的语音。
     */
    public synchronized void startMultiKeywordsRecognizer(String clientName, String model)
```

多关键词识别的使用方法，类似于离线语音识别，也应先进行语音识别的相关配置。

## 多关键词设置

多关键词的识别，使用*SpeechClient*类的*setKeywords*方法来完成:
```java
    /**
     * 设置离线识别关键词模型，配合Recognizer使用。
     *
     * @param name     关键词模型的名称，配合startXXXRecognizer使用。
     * @param keywords 希望模型离线时能够识别的一些关键词。
     */
    public void setKeywords(final String name, final String[] keywords)
```

# 热词唤醒

一般情况下，用户并不希望语音识别功能一直处于开启状态，而是希望以特定的词汇开启这一功能。这在SDK中，被称作“热词唤醒”。

在本SDK中，唤醒所使用的关键词是：“你好问问”。

下面介绍SDK热词唤醒相关的用法：

## 配置热词唤醒回调接口

在SDK检测到热词后，将会调用相应的回调接口。相关定义如下：
```java
public interface HotwordListener {
    /**
     * 当热词被触发时，回调该方法。
     */
    void onHotwordDetected();
```
使用*SpeechClient*类的*addHotwordListener*方法来进行设置：
```java
    /**
     * 设置HotwordListener回调。注意一个client只能有一个listener。
     *
     * @param listener HotwordListener实例。
     */
    public synchronized void addHotwordListener(HotwordListener listener)
```

**提示**

* 用户可于该回调接口方法中，再开启真正的语音识别任务。

## 开启热词唤醒功能
使用*SpeechClient*类的*startHotword*方法


```java
    /**
     * 开始监听热词。
     */
    public synchronized void startHotword() 
```

## 关闭热词唤醒功能

```java
    /**
     * 停止热词监听。
     */
    public synchronized void stopHotword()
```

## 取消热词唤醒回调

```java
    /**
     * 移除之前设置的HotwordListener回调
     *
     * @param listener HotwordListener实例。
     */
    public synchronized void removeHotwordListener(HotwordListener listener)
```

# 语音合成

语音合成用于为用户给定的文本生成对应的语音。

## 启动语音合成

使用*SpeechClient*类的*startTTS*方法

```java
    /**
     * 开始合成tts，并且播放。
     *
     * @param request     参数对象，包含了所有tts合成，播放所需参数。
     */
    @BasicApi(1)
    public void startTTS(final TTSRequest request)
```

## 停止语音合成
```java
    /**
     * 停止tts。
     */
    public void stopTTS() 
```

# 示例代码

如下是一份简单的Android Activity示例代码，该代码演示了如何使用SDK中的onebox语音识别API

```java
// Copyright 2017 Mobvoi Inc. All Rights Reserved.

package com.mobvoi.demomini;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.mobvoi.speech.SpeechClient;
import com.mobvoi.speech.SpeechClientListener;

public class MainActivity extends Activity {
  // This key is just a testing key for demo use.
  // Please apply for a standard key from http://ai.chumenwenwen.com/
  private static final String sAppKey = "com.mobvoi.test";
  private static final String sLocation = "中国,北京市,北京市,中关村SOHO,,,39.989602,116.316568";
  private static final String sDeviceOne = "Huawei";
  private TextView mTextView;

  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    init(this);
    SpeechClient.getInstance().setClientListener(sDeviceOne, new SpeechClientListenerImpl());
    mTextView = (TextView) findViewById(R.id.text_view);

    //  UI entry for speech Onebox recognizer
    (findViewById(R.id.btn_start_onebox))
        .setOnClickListener(
            new View.OnClickListener() {
              public void onClick(View view) {
                startOnebox(sDeviceOne);
              }
            });
    (findViewById(R.id.btn_stop_onebox))
        .setOnClickListener(
            new View.OnClickListener() {
              public void onClick(View v) {
                stop(sDeviceOne);
              }
            });
    (findViewById(R.id.btn_cancel_onebox))
        .setOnClickListener(
            new View.OnClickListener() {
              public void onClick(View v) {
                cancel(sDeviceOne);
              }
            });
  }

  protected void onResume() {
    super.onResume();
    runOnUiThread(
        new Runnable() {
          public void run() {
            mTextView.setText("Welcome to Mobvoi's SpeechSDK!");
          }
        });
  }

  protected void onPause() {
    super.onPause();
  }

  public void init(Context context) {
    SpeechClient.getInstance().init(context, sAppKey);
  }

  public void startOnebox(String deviceName) {
    SpeechClient.getInstance().setLocationString(deviceName, sLocation);
    SpeechClient.getInstance().startOneboxRecognizer(deviceName);
  }

  public void stop(String deviceName) {
    runOnUiThread(
        new Runnable() {
          public void run() {
            mTextView.setText("stopped");
          }
        });
    SpeechClient.getInstance().stopRecognizer(deviceName);
  }

  public void cancel(String deviceName) {
    runOnUiThread(
        new Runnable() {
          public void run() {
            mTextView.setText("cancelled");
          }
        });
    SpeechClient.getInstance().cancelReconizer(deviceName);
  }

  private class SpeechClientListenerImpl implements SpeechClientListener {

    public void onStartRecord() {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText("start recording ...");
            }
          });
    }

    @Override
    public void onRemoteSilenceDetected() {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText("remote silence ...");
            }
          });
    }

    public void onVolume(final double volume) {}

    public void onPartialTranscription(final String result) {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText(result);
            }
          });
    }

    public void onFinalTranscription(final String result) {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText(result);
            }
          });
    }

    public void onResult(final String result) {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText(result);
            }
          });
    }

    public void onError(final int errorCode) {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText("return error " + errorCode);
            }
          });
    }

    @Override
    public void onLocalSilenceDetected() {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText("local silence ...");
            }
          });
    }

    @Override
    public void onNoSpeechDetected() {
      runOnUiThread(
          new Runnable() {
            public void run() {
              mTextView.setText("local no speech ...");
            }
          });
    }

    @Override
    public void onSpeechDetected() {}
  }
}

```
