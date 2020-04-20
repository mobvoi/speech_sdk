## 简介
出门问问目前提供了ASR、TTS、NLP、Search的HTTPS协议的接口。   
外网域名：` https://open.mobvoi.com `   
# 调用方式
## ASR接口
**URL:** api/asr/v1

**HTTP Method:** POST（表单提交方式即: content-type:multipart/form-data）

（**注意**：文件大小不大于512k,音频识别最大为15秒）

**调用参数及说明：**

| <div style="width: 70pt">名称 | <div style="width: 75pt">类型 | <div style="width: 45pt">是否必传 | 说明|
| :----: | :----: | :----: | :----|
| file | wav/mp3/pcm | 是 | 要解析的音频文件,最大为512k |
| Content-Type | String | 是 | 该请求头为音频的请求头类型见附表1.5格 |
| type | String | 是 | 如果不方便设置Content-Type,可以设置此参数,会覆盖Content-Type的值 |
| appkey | String | 是 | 开发者在AI开放平台上申请的appkey |
| signature | String | 是 | 授权码，通过“appkey+secret+timestamp”进行md5加密，再用16进制进行编码。（加号参与字符串拼接） |
| timestamp | Long | 是 | 当前时间戳，单位为秒 |
| user_id | String | 否 | 用户的唯一id,有则填写 |   

**文件对应Content-Type说明：** <br>  
| <div style="width:70pt">文件类型 | <div style="width:70pt">Rate |<div style="width: 300pt"> Content-Type 或者type|
| :-----:| :----: |  :---------:   |
| wav | 8000 | audio/x-wav;rate=8000 |
| wav | 16000 | audio/x-wav;rate=16000 |
| mp3 | 8000 | audio/x-mp3;rate=8000 |
| mp3 | 16000 | audio/x-mp3;rate=16000 |
| pcm | 8000 | audio/x-wav;codec=pcm;rate=8000 |
| pcm | 16000 | audio/x-wav;codec=pcm;rate=16000 |  

**调用Demo及响应:**   

{   
&nbsp;&nbsp;&nbsp;&nbsp;"content": "我是出门问问",   
&nbsp;&nbsp; &nbsp;"status": "200"  
}  

**响应说明：**   

| <div style="width: 150pt">名称 | <div style="width: 300pt">说明|
| :---: | :---: |
| content | 解析出文字内容 |
| message | 失败时的解释说明 |
| status | 状态：success, error |
| errorMessage | code: 错误码，desc: 错误描述 |

## TTS接口
**URL:** api/tts/v1

**HTTP Method:** 支持GET/POST请求

#### 调用参数及说明:   
| <div style="width: 56pt">字段名 | <div style="width: 50pt">类型 | <div style="width: 30pt">必填 | <div style="width: 40pt">默认值| <div style="width:  60pt">参考值 | 描述 |
| :----: | :----: | :----: | :----: | :----: | :--: |
| text | String | 是 |  |  | 要合成的文本内容，限制为500字节。支持ssml标记语言，使用说明见附录3。|
| appkey | String | 是 |  |  |开发者在AI开放平台上申请的appkey |
| signature | String | 是 |  |  | 授权码，通过“appkey+secret+timestamp”进行md5加密，再用16进制进行编码。（加号参与字符串拼接）|
| timestamp | Long | 是 |  |  |当前时间戳，单位为秒 |
| net_type | String | 否 | wifi | wifi/2g/3g  |网络类型,可能在服务器决定合成音频采样率和压缩算法 |
| platform | Enum | 否 | pc | android/ios/pc  |使用的平台，android,ios,pc三种类型，每种类型对应一个编码 |
| speaker | String | 否 | cissy | cissy/emily/billy/lucy/tina/dora/angela  |中文女声(成熟)/中文女声(温柔)/中文男声/中文童声/台湾女声/粤语女声/英语女声 |
| speed | Float | 否 | 1.15 | 0.5-2.0  |合成的语速 |
| convert | String | 否 |  |  robot | 是否转化为机器声 |
| rate | Enum | 否 | 16000 | 16000/8000  | 音频采样率 |
| volume | Float | 否 | 0.5 | 0-1.0  | 合成音量 |
| ignore_limit | Boolean | 否 | false | false/true  | 是否限制字数,如果设置true可以突破500字节限制 | 

**返回值说明：**   
如果成功转换，则没有返回值，直接返回语音流。   
如果转换失败，则返回json提示。  
```
{
  "status": "error",
  "errorMessage": {
    "code": 31002,
    "desc": "failed to convert text to speech"
  }
```

## NLP接口   
**URL:** api/nlp/v1   
**HTTP Method:** GET   

**调用参数及说明：**    
| <div style="width: 70pt">名称 | <div style="width: 45pt">类型 | <div style="width: 55pt">是否必传 | 说明|
| :----: | :----: | :----: | :----|
| query | String | 是 | 查询语句 |
| appkey | String | 是 | 开发者在AI开放平台上申请的appkey |
| signature | String | 是 | 授权码，通过“appkey+secret+timestamp”进行md5加密，再用16进制进行编码。（加号参与字符串拼接） |
| timestamp | Long | 是 | 当前时间戳，单位为秒 |
| context | String | 否 | 强制指定domain |

**返回字段说明：**   
|<div style="width: 60pt">名称|<div style="width: 60pt">类型   |说明  |
| :----: | :----: | :----: |
| traits | JsonObject | 特征意图 | 
| task | String | 技能分类，public.weather、public.restaurant、public.music等 |
| norm_query | String | 归一化的query | 
| query | String | 原始query | 
| params | JsonObejct | 参数列表，time、location、ranking_condition、price、type等 |
| status | String | 状态：success, error | 
| errorMessage | JsonObejct | code: 错误码，desc: 错误描述 | 
**返回示例：**<br>

```
{
	"status": "success",
	"traits": {
		"eav": "other",
		"intent": "other"
	},
	"task": "public.weather",
	"norm_query": "今天的天气",
	"query": "今天的天气",
	"params": {
		"time": [{
			"prob": 0.9967646175503476,
			"index": {
				"0-2": "今天"
			},
			"raw_data": "今天",
			"norm_value": {
				"type": "period",
				"content": [
					[2018, 11, 27, 0, 0, 0],
					[2018, 11, 27, 23, 59, 59]
				],
				"status": "valid"
			},
			"status": "valid"
		}]
	}
}
```
## Search接口   
**URL:** api/search/v1<br>
**HTTP Method:** GET<br>

**调用参数及说明：**<br>
| <div style="width: 55pt">名称 | <div style="width: 45pt">类型 | <div style="width: 55pt">是否必传 | 说明|
| :----: | :----: | :----: | :----:|
| query | String | 是 | 查询语句 |
| appkey | String | 是 | 开发者在AI开放平台上申请的appkey |
| address | String | 是 | 用户地址（国家,省份,城市,区/镇,街道,街排号,纬度,经度），其中经纬度采用百度坐标系 |
| output | String | 是 | 默认传lite |
| debug | Bool | 否 | 是否显示debug信息 |
| task | String | 否 | 指定domain，比如: public.weather、public.restaurant、public.music等 |
| version | Long| 是 | 默认传43000 |
| signature | String | 是 | 授权码，通过“appkey+secret+timestamp”进行md5加密，再用16进制进行编码。（加号参与字符串拼接） |
| timestamp | long | 是 | 当前时间戳，单位为秒 |
| ignore_context | Bool | 否 | 是否忽略多轮会话，默认为false |
| user_id | String | 否 | 用户ID，可根据需求由开发者生成 |


**返回字段说明：**  

|<div style="width: 70pt">名称     |<div style="width: 70pt">类型   |<div style="width: 300pt">说明  |
| :----: | :----: | :----: |
| control | JsonObject | 多轮对话控制 | 
| clientAction | JsonObject | 前端执行逻辑 |
| contextHint | JsonObject | 基于上下文的提示 | 
| status| String | 服务状态 | 
| query | String | 用户原始query |
| domain | String | 垂直搜索分类 |  
| states| JsonObejct | semantic slots信息 | 
| queryTime | JsonObejct | query执行的时间 |
| messageId | String | query执行时的ID，同一个ID可能包含多个query |
| languageOutput | JsonObejct | 语言输出结果 | 
| clientData | JsonObejct | 垂直搜索查询结果 | 

lite协议详细输出结果说明请参考：https://ai.chumenwenwen.com/pages/document/search-output-intro

**返回示例：**   
```
{
    "clientAction":{

    },
    "control":{
        "execType":"show_onebox",
        "voiceControl":"end"
    },
    "contextHint":{

    },
    "status":"success",
    "query":"刘德华的百科",
    "domain":"public.baike",
    "states":{
        "inform":{
            "keyword":"刘德华::刘德华",
            "intent":"other::other"
        },
        "request":[

        ],
        "semantic":{
            "slots":{
                "keyword":[
                    {
                        "index":{
                            "0-3":"刘德华"
                        },
                        "isNegligible":true,
                        "task_turn_id":1,
                        "norm_value":"刘德华",
                        "raw_data":"刘德华",
                        "prob":0.9997296922645181
                    }
                ]
            },
            "domain":"public.baike",
            "confidence":1,
            "intent":"other"
        }
    },
    "queryTime":1548657613427,
    "messageId":"e8ad4981-2998-4c3e-9e75-5db8db5be995",
    "languageOutput":{
        "tts":"<speak version="1.0" xml:lang="zh-cn" 
xmlns="http://www.w3.org/2001/10/synthesis"><tts::prompt 
domain="public.baike"/>为你查到以下关于刘德华的资料</speak>",
        "displayText":"为你查到以下关于刘德华的资料"
    },
    "intent":"other",
    "clientData":{
        "resolveDetail":{
            "select":{
            }
        },
        "status":"success",
        "clientDataPageId":"assistant+baike-BaikeEntity-Resolved",
        "pageId":0,
        "data":{
            "sourceHost":"baike.baidu.com",
            "source":"百度百科",
            "params":{
                "backgroundUrl":"http://mobvoi-search-
					public.ufile.ucloud.cn/mobvoi-one-
					box/baike/5168941475097331364?
iopcmd=thumbnail&type=6&width=120&height=200",
                "information":"中文名：刘德华
主要成就：三届香港电影金像奖最佳男主角
两届台湾电影金马奖最佳男主角
1985-2005年全港最高累积票房香港男演员奖
中国电影百年形象大使
釜山电影节亚洲最有贡献电影人奖
六届十大劲歌金曲最受欢迎男歌星
两届ChannelV亚洲最受欢迎歌手
世界十大杰出青年
香港特别行政区荣誉勋章佩戴者
吉尼斯世界纪录中获奖最多的香港男歌手
收起
代表作品：无间道、天若有情、旺角卡门、桃姐、天下无贼、忘情水、谢谢你的爱、爱你一万年、冰雨、今天
体重：63kg
信仰：佛教",
                "tts":"为您查到以下资料",
    "linkUrl":"https://wapbaike.baidu.com/item/%E5%88%98%E5%B
	E%B7%E5%8D%8E/114923",
                "nameZh":"刘德华",
                "namecard":[
                    {
                        "value":"演员，歌手，填词人，制片人",
                        "key":"职业"
                    },
                    {
                        "value":"中国",
                        "key":"国籍"
                    },
                    {
                        "value":"1961年9月27日",
                        "key":"出生日期"
                    },
                    {
                        "value":"无间道、天若有情、旺角卡门、桃姐、天下无贼、
					忘情水、谢谢你的爱、爱你一万年、冰雨、今天",
                        "key":"代表作品"
                    },
                    {
                        "value":"三届香港电影金像奖最佳男主角
两届台湾电影金马奖最佳男主角
1985-2005年全港最高累积票房香港男演员奖
中国电影百年形象大使
釜山电影节亚洲最有贡献电影人奖

六届十大劲歌金曲最受欢迎男歌星
两届ChannelV亚洲最受欢迎歌手
世界十大杰出青年
香港特别行政区荣誉勋章佩戴者
吉尼斯世界纪录中获奖最多的香港男歌手
收起",
                        "key":"主要成就"
                    }
                ],
                "nameEn":"Andy Lau，Lau Tak Wah",
                "background":"",
                "moreInfo":"更多资料 >>",
                "introduction":"刘德华（Andy Lau），1961年9月27日出生于
					中国香港，中国香港男演员、歌手、作词人、制片
					人。1981年出演电影处女作《彩云曲》"
            },
            "type":"baike_one"
        },
        "pageSize":3,
        "type":"baike_one",
        "dataSummary":{
            "title":"百科",
            "hint":"找到 1 条结果",
            "tips":"刘德华的百科",
            "type":"baike_one"
        }
    }
}
```
# 示例与说明   
## 单轮对话开发示例   
以onebox接口为例：<br>
 
1.  脚本接收一个输入文件地址（可以是相对路径，也可以是绝对路径），文件内容是一些样例query，每一句query占一行；
2.	脚本结果输出到当前执行目录的result文件里，也可以在执行脚本时手动执行 -o 参数，把结果输出到指定文件；
3.  执行脚本前，请先自行修改appkey值和secret值；
4.	该脚本只在mac系统下测试过，使用python 2.7版本。
```
# coding=utf-8
import argparse
import hashlib
import requests
import os
import sys
import time

def func(input,out):
  appkey = '<app key，开放平台获取>'
  secret = ‘<app secret，开发平台获取>’
  address = '北京市,北京市,,,,,39.91488908,116.40387397'

  with open(input, 'rb') as fo:
    with open(output, 'wb') as fw:
      for line in fo.readlines():
        # ------ 生成加密字符串 ------
        timestamp = str(int(time.time()))

        message = '+'.join([appkey, secret, timestamp])
        m = hashlib.md5()
        m.update(message)
        signature = m.hexdigest()
        url = 'https://open.mobvoi.com/api/onebox/get?query={line}&appkey={appkey}&signature={signature}&timestamp={timestamp}&address={address}&output=lite&version=43000&ignore_context=true'
        url = url.format(line=line, appkey=appkey, signature=signature, timestamp=timestamp, address=address)

        print url
        # ----------------------------

        rv = requests.get(url)
        if not rv.status_code == 200:
          print 'interupted. rv.text'
          break
        fw.write(rv.text.encode('utf-8'))
        fw.write('\n')
        time.sleep(0.1)

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('input', help='specify the input file')
  parser.add_argument('-o', '--output', default='result', help='specify the output file, default is ./result')
  args = parser.parse_args()
  input,output = args.input,args.output
  if not os.path.isabs(input):
    curr_dir = os.path.dirname(os.path.realpath(__file__))
    input = os.path.join(curr_dir,input)
  if not os.path.exists(input) or not os.path.isfile(input):
    print 'there is no such input file.'
    sys.exit(1)
  func(input, output)
```
## 多轮对话开发示例   
系统默认多轮对话是开启的，DM会记录用户之前询问的历史信息，来判断对当前用户状态作出反应。DM中不会一直记录用户的状态信息，默认45秒后就会失效。如果想跳出多轮对话有三种方式：
1.	等待45秒，DM中记录的用户历史信息自动失效；
2.	通过url中添加ignore_context=true强制关闭多轮；
3.	更换user_id；

如果想测试多个多轮的效果，建议采用第三种方式，这样可以快速切换到新的多轮，避免之前多轮的影响。以onebox接口为例：

1.	脚本接收一个输入文件地址（可以是相对路径，也可以是绝对路径），文件内容是一些样例query，每一句query占一行；
2.	脚本结果输出到当前执行目录的result文件里，也可以在执行脚本时手动执行 -o 参数，把结果输出到指定文件；
3.	执行脚本前，请先自行修改appkey值和secret值；
4.	该脚本只在mac系统下测试过，使用python 2.7版本。
```
# coding=utf-8
import argparse
import hashlib
import requests
import os
import sys
import time

def func(input,out):
  appkey = '<app key，开放平台获取>'
  secret = ‘<app secret，开发平台获取>’
  address = '北京市,北京市,,,,,39.91488908,116.40387397'
  with open(input, 'rb') as fo:
    with open(output, 'wb') as fw:
      for line in fo.readlines():
        # ------ 生成加密字符串 ------
        timestamp = str(int(time.time()))

        message = '+'.join([appkey, secret, timestamp])
        m = hashlib.md5()
        m.update(message)
        signature = m.hexdigest()
        url = 'https://open.mobvoi.com/api/onebox/get?query={line}&appkey={appkey}&signature={signature}&timestamp={timestamp}&address={address}&output=lite&version=43000&user_id=001'
        url = url.format(line=line, appkey=appkey, signature=signature, timestamp=timestamp, address=address)

        print url
        # ----------------------------

        rv = requests.get(url)
        if not rv.status_code == 200:
          print 'interupted. rv.text'
          break
        fw.write(rv.text.encode('utf-8'))
        fw.write('\n')
        time.sleep(0.1)

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('input', help='specify the input file')
  parser.add_argument('-o', '--output', default='result', help='specify the output file, default is ./result')
  args = parser.parse_args()
  input,output = args.input,args.output
  if not os.path.isabs(input):
    curr_dir = os.path.dirname(os.path.realpath(__file__))
    input = os.path.join(curr_dir,input)
  if not os.path.exists(input) or not os.path.isfile(input):
    print 'there is no such input file.'
    sys.exit(1)
  func(input, output)
```
## TTS ssml使用说明
**ssml标志: <speak>**<br>
所有ssml必须在<speak>标签之内，且需要注明version, xmlns和xml:lang。  
其中version和xmlns分别固定为version="1.0"和 xmlns="http://www.w3.org/2001/10/synthesis"，<br>
xml:lang="zh-CN"(默认中文)或"en-US"(默认英文)<br>

以domain="public.weather"为例：<br>
<speak version="1.0" <br>  xml:lang="zh-CN"   <br> xmlns="http://www.w3.org/2001/10/synthesis" <br> 
domain="public.weather">
    今天天气不错  
</speak>  

**标签定义说明**

<table>
    <tr>
        <td><div style="width: 40pt">Key</td> 
        <td><div style="width: 60pt">Function</td> 
        <td>Tag</td> 
   </tr>
    <tr>
        <td rowspan="5">say-as</td>    
        <td >整数读法</td>  
         <td ><say-as interpret-as=”value”>123</say-as></td>
    </tr>
    <tr>
        <td >数字串读</td>  
         <td ><say-as interpret-as=”digits”>123</say-as></td>
    </tr>
   <tr> 
         <td >电话号码式读法</td>  
         <td ><say-as interpret-as=”telephone”>13362734323</say-as></td>
    </tr>
 <tr> 
         <td >地址门牌读法</td>  
         <td ><say-as interpret-as=”building”>233</say-as></td>
    </tr>
 <tr> 
         <td >节日读法</td>  
         <td ><say-as interpret-as=”festival”>51</say-as></td>
    </tr>
<tr>
        <td rowspan="5">break</td>    
        <td >无停顿（default）</td>  
         <td ><break strength="weak"/></td>
    </tr>
    <tr>
        <td >逗号停顿</td>  
         <td ><break strength="medium"/></td>
    </tr>
   <tr> 
         <td >句子停顿</td>  
         <td ><break strength="strong"/></td>
    </tr>
 <tr> 
         <td >段落停顿</td>  
         <td ><break strength="x-strong”/></td>
    </tr>
    
  <tr> 
         <td >停顿x秒钟</td>  
         <td ><break time="300ms"/></td>
    </tr>
    <tr> 
         <td rowspan="1">say-as</td> 
         <td >指定分词不拆分并可指定词发音</td>  
         <td ><w>成都</w>是<br>
<w phoneme=”cheng2 du1”>成都</w>是<br>
我的指甲<w phoneme=”zhang3 chang2”>长长</w>了</td>
    </tr> 
</table>

**例1:** <br>
**ssml标记前：** 今天天气不错，气温-12摄氏度适合穿长T恤  
**ssml标记后：**  
<speak version="1.0" xml:lang="zh-CN" xmlns="http://www.w3.org/2001/10/synthesis" domain="public.weather">
    今天天气不错，气温<say-as interpret-as="value">-12</say-as>摄氏度<break strength="medium"/>适合穿<phoneme alphabet="ipa" ph="chang2">长</phoneme><w>T恤</w>  
</speak>

**例2:**  
**ssml标记前：**  
9月10日，庆祝2019年教师节暨全国教育系统先进集体和先进个人表彰大会在京举行。  
习近平总书记在人民大会堂亲切会见受表彰代表，向受到表彰的先进集体和先进个人表示热烈祝贺，向全国广大教师和教育工作者致以节日的问候。  

**ssml标记后：**  
<speak version="1.0" xml:lang="zh-CN"   <br> xmlns="http://www.w3.org/2001/10/synthesis" domain="public.news"> <br>
    9月10日，庆祝2019年教师节暨全国教育系统先进集体和先进个人表彰大会在京举行。<break time=”500ms”/>习近平总书记在人民大会堂亲切会见受表彰代表，<break time=”500ms”/>向受到表彰的先进集体和先进个人表示热烈祝贺，<break time=”500ms”/>向全国广大教师和教育工作者致以节日的问候。  
</speak>

## 错误码说明

| 错误码 | 备注 |
| :---- | :---- |
| 10002 | 上传方式错误，需使用form表单方式 |
| 10003 | 文件超出512K限制 |
| 10004 | appkey、signature、timestamp参数不全 |
| 10005 | 没找到上传的音频文件 |
| 10006 | 鉴权失败（原因可能是appkey非法或者signature计算错误） |
| 31000 | 参数错误，缺少必填参数或者参数取值范围错误，具体可参考errorMessage中描述|
| 31002 | 服务器内部错误 |
| 31006 | 服务器并发量超出限制 |
## 常见问题
 1、Q：调用时发现某个通用场景NLU分类错误。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A：请先检查是否在AI开放平台勾选了该通用场景并成功上线。

   2、Q：address用户地址经纬度格式？  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A：address格式为：（国家,省份,城市,区/镇,街道,街排号,纬度,经度），其中经纬度采用百度坐标系，如果经纬度存在，那么部分信息可缺省。如：北京市,北京市,,,,,39.91488908,116.40387397

   3、Q：timestamp和signature的拼接问题。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A：采用HTTP传参的方式，直接字符串拼接即可，可参考上面的示例。
