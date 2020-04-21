# TTS ssml使用说明
**ssml标志: \<speak\>**<br>
所有ssml必须在<speak>标签之内，且需要注明version, xmlns和xml:lang。
其中version和xmlns分别固定为version="1.0"
xmlns="http://www.w3.org/2001/10/synthesis"
xml:lang="zh-CN"(默认中文)或"en-US"(默认英文)

以domain="public.weather"为例：
```html
<speak version="1.0" xml:lang="zh-CN" xmlns="http://www.w3.org/2001/10/synthesis" domain="public.weather">
    今天天气不错
</speak>
```

**标签定义说明**

![TTS_SSML](http://mobvoi-ai-public.ufile.ucloud.cn/img/TTS_SSML.PNG)

**例1:** <br>
**ssml标记前：**
```
今天天气不错，气温-12摄氏度适合穿长T恤
```
**ssml标记后：**
```html
<speak version="1.0" xml:lang="zh-CN" xmlns="http://www.w3.org/2001/10/synthesis" domain="public.weather">
    今天天气不错，气温<say-as interpret-as="value">-12</say-as>摄氏度<break strength="medium"/>适合穿<phoneme alphabet="ipa" ph="chang2">长</phoneme><w>T恤</w>
</speak>
```

**例2:** <br>
**ssml标记前：**
```
9月10日，庆祝2019年教师节暨全国教育系统先进集体和先进个人表彰大会在京举行。
习近平总书记在人民大会堂亲切会见受表彰代表，向受到表彰的先进集体和先进个人表示热烈祝贺，向全国广大教师和教育工作者致以节日的问候。
```

**ssml标记后：**
```html
<speak version="1.0" xml:lang="zh-CN"   <br> xmlns="http://www.w3.org/2001/10/synthesis" domain="public.news"> <br>
    9月10日，庆祝2019年教师节暨全国教育系统先进集体和先进个人表彰大会在京举行。<break time=”500ms”/>习近平总书记在人民大会堂亲切会见受表彰代表，<break time=”500ms”/>向受到表彰的先进集体和先进个人表示热烈祝贺，<break time=”500ms”/>向全国广大教师和教育工作者致以节日的问候。
</speak>
```
