[***常见问题***](#%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98)
* [SDK支持什么平台架构？](#sdk%E6%94%AF%E6%8C%81%E4%BB%80%E4%B9%88%E5%B9%B3%E5%8F%B0%E6%9E%B6%E6%9E%84)
* [SDK对输入音频格式有要求吗？](#sdk%E5%AF%B9%E8%BE%93%E5%85%A5%E9%9F%B3%E9%A2%91%E6%A0%BC%E5%BC%8F%E6%9C%89%E8%A6%81%E6%B1%82%E5%90%97)
* [有没有类似hello,world的demo？](#%E6%9C%89%E6%B2%A1%E6%9C%89%E7%B1%BB%E4%BC%BChelloworld%E7%9A%84demo)
* [有没有开发者论坛啊？](#%E6%9C%89%E6%B2%A1%E6%9C%89%E5%BC%80%E5%8F%91%E8%80%85%E8%AE%BA%E5%9D%9B%E5%95%8A)


[***功能相关***](#%E5%8A%9F%E8%83%BD%E7%9B%B8%E5%85%B3)
* [离线识别可以识别所有语音吗？](#%E7%A6%BB%E7%BA%BF%E8%AF%86%E5%88%AB%E5%8F%AF%E4%BB%A5%E8%AF%86%E5%88%AB%E6%89%80%E6%9C%89%E8%AF%AD%E9%9F%B3%E5%90%97)
* [唤醒热词如何更改？](#%E5%94%A4%E9%86%92%E7%83%AD%E8%AF%8D%E5%A6%82%E4%BD%95%E6%9B%B4%E6%94%B9)
* [关键词识别和热词唤醒一样吗？](#%E5%85%B3%E9%94%AE%E8%AF%8D%E8%AF%86%E5%88%AB%E5%92%8C%E7%83%AD%E8%AF%8D%E5%94%A4%E9%86%92%E4%B8%80%E6%A0%B7%E5%90%97)
* [支持中英文混合识别吗？](#%E6%94%AF%E6%8C%81%E4%B8%AD%E8%8B%B1%E6%96%87%E6%B7%B7%E5%90%88%E8%AF%86%E5%88%AB%E5%90%97)
* [Linux:如何屏蔽SDK本身日志输出？](#linux%E5%A6%82%E4%BD%95%E5%B1%8F%E8%94%BDsdk%E6%9C%AC%E8%BA%AB%E6%97%A5%E5%BF%97%E8%BE%93%E5%87%BA)
* [离线设置联系人数量有限制吗？](#%E7%A6%BB%E7%BA%BF%E8%AE%BE%E7%BD%AE%E8%81%94%E7%B3%BB%E4%BA%BA%E6%95%B0%E9%87%8F%E6%9C%89%E9%99%90%E5%88%B6%E5%90%97)
* [TTS的长度有限制吗？](#tts%E7%9A%84%E9%95%BF%E5%BA%A6%E6%9C%89%E9%99%90%E5%88%B6%E5%90%97)
* [TTS能切换人声吗？](#tts%E8%83%BD%E5%88%87%E6%8D%A2%E4%BA%BA%E5%A3%B0%E5%90%97)
* [TTS能够读出除了中文之外的语言吗？](#tts%E8%83%BD%E5%A4%9F%E8%AF%BB%E5%87%BA%E9%99%A4%E4%BA%86%E4%B8%AD%E6%96%87%E4%B9%8B%E5%A4%96%E7%9A%84%E8%AF%AD%E8%A8%80%E5%90%97)
* [ASR的最长识别时间是多久？](#asr%E7%9A%84%E6%9C%80%E9%95%BF%E8%AF%86%E5%88%AB%E6%97%B6%E9%97%B4%E6%98%AF%E5%A4%9A%E4%B9%85)
* [语音识别返回JSON字符串，能直接返回对应语音的文本吗？](#%E8%AF%AD%E9%9F%B3%E8%AF%86%E5%88%AB%E8%BF%94%E5%9B%9Ejson%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%83%BD%E7%9B%B4%E6%8E%A5%E8%BF%94%E5%9B%9E%E5%AF%B9%E5%BA%94%E8%AF%AD%E9%9F%B3%E7%9A%84%E6%96%87%E6%9C%AC%E5%90%97)


[***技术疑难***](#%E6%8A%80%E6%9C%AF%E7%96%91%E9%9A%BE)
* [Android:集成SDK，说话没有任何反应？](#android%E9%9B%86%E6%88%90sdk%E8%AF%B4%E8%AF%9D%E6%B2%A1%E6%9C%89%E4%BB%BB%E4%BD%95%E5%8F%8D%E5%BA%94)
* [Android:自定义场景为什么会训练失败？](#android%E8%87%AA%E5%AE%9A%E4%B9%89%E5%9C%BA%E6%99%AF%E4%B8%BA%E4%BB%80%E4%B9%88%E4%BC%9A%E8%AE%AD%E7%BB%83%E5%A4%B1%E8%B4%A5)
* [Linux:离线TTS占用内存过大？](#linux%E7%A6%BB%E7%BA%BFtts%E5%8D%A0%E7%94%A8%E5%86%85%E5%AD%98%E8%BF%87%E5%A4%A7)


### 常见问题

#### SDK支持什么平台架构？

> Android版SDK支持x86，mips，armv7，armv8平台。 Linux版SDK目前支持x86，armv7，armv8平台。

#### SDK对输入音频格式有要求吗？
> Android和Linux SDK音频均支持16k采样，16bit位深，pcm格式。

#### 有没有类似hello,world的demo？
> 请参考官网开发者文档：[Android版开发示例](http://ai.chumenwenwen.com/pages/document/android/example)，[Linux版开发示例](http://ai.chumenwenwen.com/pages/document/linux/example)

#### 有没有开发者论坛啊？
> 有的。请点击[出门问问开发者论坛](https://bbs.ticwear.com/forum.php?mod=forumdisplay&fid=262)


### 功能相关

#### 离线识别可以识别所有语音吗？
> 不可以。如果要识别*所有*语音，需要一个Large Vocabulary，从性能，资源和通用性考虑，离线识别版SDK目前不会做到识别所有语音。
更佳的方案是，用户可以选择调用在线语音识别。

#### 唤醒热词如何更改？
> 目前有两种方式提供唤醒热词，一种是深度定制，一种是动态生成（仅限Android SDK全量版）。深度定制版从功耗，性能，敏感度和误触率上均优于动态生成版，但需要商务联系出门问问进行定制。
动态生成方法可参考Android版SDK代码示例热词部分。

#### 关键词识别和热词唤醒一样吗？
> 首先问题中的关键词识别，我们也可以理解为快词，在Android和Linux版的SDK中是针对离线recognizer的设置，在recognizer不关闭的情况下，快词可以一直被识别，而热词是用于更广义的唤醒。
另外二者在性能上是有差距的，请参考[唤醒热词如何更改？](#%E5%94%A4%E9%86%92%E7%83%AD%E8%AF%8D%E5%A6%82%E4%BD%95%E6%9B%B4%E6%94%B9)

#### 支持中英文混合识别吗？
> 支持

#### Linux:如何屏蔽SDK本身日志输出？
> Linux版SDK暂不支持。

#### 离线设置联系人数量有限制吗？
> 受限于机器内存大小限制。但如果设置数量过多，可能会影响性能。

#### TTS的长度有限制吗？
> 中文单次输入最长500字

#### TTS能切换人声吗？
> 暂不支持切换人声。

#### TTS能够读出除了中文之外的语言吗？
> 目前能支持中文，英文和中英文混合，中文效果最好。

#### ASR的最长识别时间是多久？
> 目前在线识别是12秒超时，但使用Voice Input方式的话可以延长至30秒。

#### 语音识别返回JSON字符串，能直接返回对应语音的文本吗？
> 如果语音识别返回JSON字符串，意味着你可能在调用onebox类型的recognizer。如果直接调用asr类型的recognizer，将不会有JSON结果返回，具体可以参考官网文档:  
> [Andriod版](http://ai.chumenwenwen.com/pages/document/android/example) [Linux版](http://ai.chumenwenwen.com/pages/document/linux/example)


#### 返回场景中的action是什么意思，如何定义？
> action就是前端执行的唯一标示。比如问天气就是com.mobvoi.semantic.action.WEATHER，查股票就是com.mobvoi.semantic.action.STOCK。前端请按照这个字段来规划执行逻辑。

### 技术疑难

#### Android:集成SDK，说话没有任何反应？
> 请检查onError回调中有没有任何错误码返回。如果没有的话，请联系我们。方便的话抓取一下log，发送至ai-developer@mobvoi.com，谢谢。

#### Android:自定义场景为什么会训练失败？
> 请保证每个语义标签，都至少有一条训练数据。再重新训练以及发布。

#### Linux:离线TTS占用内存过大？
> 离线TTS会加载相关模型，所以占用内存较多，如果对内存使用有限制，可以切换为在线TTS。




