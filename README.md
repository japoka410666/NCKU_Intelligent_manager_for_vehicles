# Intelligent_manager_for_vehicles
## 介紹
在過去2020一年裡，疲勞駕駛發生事故比例約占總車禍事故約20%，居各類事故發生率第二名，我們希望能夠降低疲勞駕駛的事故發生。
因此在作品中，我們運用Himax WE-I Plus上G-Sensor和快速AI運算的功能，以CNN模型進行疲勞辨識，並偵測是否為使用者，透過WIFI連接使用者手機中的LINE app，達到無鑰匙且防盜的效果。
## 實現方法
* 用himax_wei_plus上的加速度感測器監測震動，判斷是否有人上車。
* 使用keras訓練一個CNN模型，轉成tflite_int8形式，部署至WE-I Plus，進行駕駛疲勞辨識，並偵測是否為使用者。
* 以`GPIO`做為Aduino ESP8266模組和Himax WE-I Plus的溝通橋樑。當有非使用者進入車輛，WE-I Plus會驅使ESP8266連接WIFI，並傳送訊息至Line notify，達到防盜警示。

## 操作方法
將`/Intelligent_manager_for_vehicles/ouput_gnu.image`利用Tera Term的XMODEM燒錄到WE-I Plus中，按下reset buttom後即可開始操作。
若偵測到震動大於一定數值，便判斷為有人上車。

![image](https://user-images.githubusercontent.com/48234255/119994911-a4f4bb80-bfff-11eb-9b96-754bb3ee03db.png)

而後開始進行疲勞駕駛判定，每0.5秒會回饋駕駛者當前狀態，括號內的數值為此次判定之可信度。
小於60%會出現`NotSure`，而當判定閉眼時間過長時便會使Himax-WEI-Plus亮起紅燈，代表發送警告訊號。

![image](https://user-images.githubusercontent.com/48234255/120002038-feacb400-c006-11eb-83df-b383f1ceb5fb.png)

並在三秒後恢復狀態，繼續監測駕駛。

在過程中亦會辨識是否為已登錄的使用者，若判定為`Stranger`，則會提出警示，並以Line notify通知車主。
![image](https://user-images.githubusercontent.com/48234255/126733247-4dea2340-4e31-4608-937a-83d7d5975cfe.png)
![image](https://user-images.githubusercontent.com/48234255/126733267-e9edee98-0284-4165-877b-c8a38a9b9be8.png)

