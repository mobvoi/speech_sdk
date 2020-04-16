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
         <td >w</td>  
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