# Intelligent_manager_for_vehicles
## 介紹

## 操作方法
將`/Intelligent_manager_for_vehicles/ouput_gnu.image`利用Tera Term的XMODEM燒錄到himax_wei_plus中，
按下reset buttom後即可開始操作。

若偵測到震動大於一定數值，便判斷為有人上車。
![image](https://user-images.githubusercontent.com/48234255/119983894-5a6d4200-bff3-11eb-9b05-f588e759bbce.png)
而後開始進行疲勞駕駛判定，每0.5秒會回饋駕駛者當前狀態，括號內的數值為此次判定之可信度。
小於60%會出現`NotSure`，而當判定閉眼時間過長時便會使Himax-WEI-Plus亮起紅燈，代表發送警告訊號。
![image](https://user-images.githubusercontent.com/48234255/119983953-6d801200-bff3-11eb-95bd-7804cda27af4.png)
三秒後會回恢復狀態，並重新開始監測駕駛。
