#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<TridentTD_LineNotify.h>
#define LINE_TOKEN "UElWIW6D9u2ULuvxVKUIRhuARE5Yb0DE7ahXbJUdqSq"
const char* ssid = "xuan";
const char* password = "123789456";

WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
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
  
}

void loop() {
  // put your main code here, to run repeatedly:
  /*WiFiClient client = server.available();
  if(!client){
    return;
  }*/
 /* Serial.println("new client");
  while(!client.available()){
    delay(1);
  }*/
Serial.println(LINE.getVersion());
LINE.setToken(LINE_TOKEN);
LINE.notify("\nhello i am nodemcu");
delay(30000);
  
}
