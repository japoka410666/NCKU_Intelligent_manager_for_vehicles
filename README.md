# Intelligent_manager_for_vehicles
## 介紹
在過去2020一年裡，疲勞駕駛發生事故比例約占總車禍事故約20%，居各類事故發生率第二名，我們希望能夠降低疲勞駕駛的事故發生，因此在此提案之中，我們希望運用到Himax WE-I Plus上的功能進行疲勞辨識，除此之外我們也希望能加入辨識車輛晃動，是否為使用者等感測功能，達到無鑰匙且防盜的效果，使功能更廣泛。
## 實現方法
* 用himax_wei_plus上的加速度感測器監測震動，完成第一步驟。
* 使用keras訓練一個簡易的CNN模型，轉成tflite_int8的形式，並部署在himax_wei_plus上，判斷駕駛的疲勞情形。
* 以I2C做為Aduino ESP8266模組和himax_wei_plus的溝通橋樑，當有非使用者進入車輛，himax_wei_plus會傳送訊息驅使ESP8266連接WIFI，並傳送訊息至使用者手機(Line notify)，來達到防盜警示功能。

## 操作方法
將`/Intelligent_manager_for_vehicles/ouput_gnu.image`利用Tera Term的XMODEM燒錄到himax_wei_plus中，
按下reset buttom後即可開始操作。

若偵測到震動大於一定數值，便判斷為有人上車。

![image](https://user-images.githubusercontent.com/48234255/119994911-a4f4bb80-bfff-11eb-9b96-754bb3ee03db.png)

而後開始進行疲勞駕駛判定，每0.5秒會回饋駕駛者當前狀態，括號內的數值為此次判定之可信度。
小於60%會出現`NotSure`，而當判定閉眼時間過長時便會使Himax-WEI-Plus亮起紅燈，代表發送警告訊號。

![image](https://user-images.githubusercontent.com/48234255/120002038-feacb400-c006-11eb-83df-b383f1ceb5fb.png)

並在三秒後恢復狀態，繼續監測駕駛。
