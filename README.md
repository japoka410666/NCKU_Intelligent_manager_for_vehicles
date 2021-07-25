# Intelligent_manager_for_vehicles
(https://github.com/japoka410666/NCKU_Intelligent_manager_for_vehicles)
## 介紹
  在過去2020一年裡，疲勞駕駛發生事故比例約占總車禍事故約20%，居各類事故發生率第二名，我們希望能夠降低疲勞駕駛的事故發生。
因此在作品中，我們運用Himax WE-I Plus上G-Sensor和快速AI運算的功能，以CNN模型進行疲勞辨識，並偵測是否為使用者，透過WIFI連接的LINE app通知警示，達到無鑰匙且防盜的效果。
## 實現方法
* 用WE-I Plus上的加速度感測器監測震動，判斷是否有人上車。
* 使用Keras訓練一個CNN模型，轉成tflite_int8形式，部署至WE-I Plus，進行駕駛疲勞辨識，並偵測是否為使用者。
* 以`GPIO`做為Aduino ESP8266模組和Himax WE-I Plus的溝通橋樑。當有非使用者進入車輛，WE-I Plus會驅使ESP8266連接WIFI，並傳送訊息至Line notify，達到防盜警示。
## 硬體架構
![image](https://user-images.githubusercontent.com/48234255/126871588-c22f199f-6d63-41bb-889a-18ca2ae6a2c6.png)
## 軟硬體設置
* 利用`/model_trainnig/Training_eyes_model.ipynb`訓練出`model.tflite`。
* 將.tflite轉成.h形式加入`/Intelligent_manager_for_vehicles/inc/`(即目錄中eyes_model_data.h)。
* 將`/Intelligent_manager_for_vehicles/ouput_gnu.image`利用Tera Term的XMODEM燒錄至WEI Plus中。
* 將`/WIFI/arc_contest_esp8266withpwd.ino`燒錄至ESP8266 NODEMCU。
* 連接Himax WE-I Plus和ESP8266後即完成設置(接線圖如下)。

![image](https://user-images.githubusercontent.com/48234255/126736324-9d7d40e6-58cc-477b-a9ac-7a14d6ff3c4b.png)

## 操作方法
按下reset buttom後可開始操作。若偵測到震動大於一定數值，便判斷為有人上車。

![image](https://user-images.githubusercontent.com/48234255/119994911-a4f4bb80-bfff-11eb-9b96-754bb3ee03db.png)

而後開始進行疲勞駕駛判定，每0.5秒會回饋駕駛者當前狀態，括號內的數值為此次判定之可信度。
小於60%會出現`NotSure`，而當判定閉眼時間過長時便會使 WEI Plus亮起紅燈，代表發送警告訊號。

![image](https://user-images.githubusercontent.com/48234255/120002038-feacb400-c006-11eb-83df-b383f1ceb5fb.png)

在三秒後恢復，繼續監測駕駛。

在過程中亦會辨識是否為已登錄的使用者，若判定為`Stranger`，則會提出警示，並以Line notify通知車主。
![image](https://user-images.githubusercontent.com/48234255/126733247-4dea2340-4e31-4608-937a-83d7d5975cfe.png)
![image](https://user-images.githubusercontent.com/48234255/126733267-e9edee98-0284-4165-877b-c8a38a9b9be8.png)

