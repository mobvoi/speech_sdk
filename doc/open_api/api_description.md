earch接口
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