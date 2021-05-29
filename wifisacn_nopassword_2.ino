#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<TridentTD_LineNotify.h>
#define LINE_TOKEN "UElWIW6D9u2ULuvxVKUIRhuARE5Yb0DE7ahXbJUdqSq"

WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);

  // Disconnect from an AP if it was previously connected
  WiFi.disconnect();
  delay(100);
  
}


void loop() {
  // put your main code here, to run repeatedly:
   while(WiFi.status()!=WL_CONNECTED){
    Serial.println("start to scan wifi");
    int n=WiFi.scanNetworks();
    if(n==0){
      Serial.println("no founded wifi");
      }
      else{
        for(int i=0;i<n;i++){
          Serial.print(i + 1);     
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print("   need password?：");
          Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?"open":"lock");
          if(WiFi.encryptionType(i) == ENC_TYPE_NONE){
             Serial.printf("try to connect %s\n",WiFi.SSID(i));
             WiFi.begin(WiFi.SSID(i).c_str(),NULL);
             while(WiFi.status()!=WL_CONNECTED){
              delay(500);
              Serial.print(".");
              }
              if(WiFi.status()==WL_CONNECTED){
              Serial.print("connection successful!\n");
              Serial.print("Your device IP address is ");
              Serial.println(WiFi.localIP());
              Serial.println(LINE.getVersion());
              LINE.setToken(LINE_TOKEN);
              LINE.notify("\nhello i am nodemcu");
              delay(30000);
              }
            }
          }
        }
    }
     delay(30000);
}
