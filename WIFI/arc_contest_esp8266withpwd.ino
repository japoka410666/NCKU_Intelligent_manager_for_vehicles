#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<TridentTD_LineNotify.h>
#define LINE_TOKEN "EpPZSNrGBiwObZGyuBMGtKy8bBgmSi63m3IQte0XdMQ"
const char* ssid = "Japoka";/****這行放wifi名*****/
const char* password = "shit1234";/*****這行放wifi密碼*****/
int inPin = 2;   // 與 pin 2 連接
WiFiServer server(80);
void setup() {
Serial.begin(115200);
delay(10);

Serial.println();
Serial.println();
Serial.print("Connecting to");
Serial.println(ssid);

WiFi.begin(ssid,password);
while(WiFi.status()!=WL_CONNECTED){
  delay(500);
  Serial.print(".");
  }
Serial.println("");
Serial.println("WiFi connected");
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
Serial.println(LINE.getVersion());
LINE.setToken(LINE_TOKEN);
pinMode(inPin, INPUT);      // 將數位 pin 腳 2 設定為輸入
}

void loop() {         
    if(digitalRead(inPin)){//如果是高電平
      LINE.notify("\n!Unregistered User!\nWe have detected a stranger in your car.");
      delay(10000);
    }else{//如果是低電平
      //do nothing
    }
  
}
