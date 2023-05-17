/*
PINOS EM USO:
-> SD 
    * CS -> D5
    * SCK -> D18 
    * MOSI -> D23
    * MISO -> D19
-> OLED 
    * SDA -> D21
    * SCL -> D22
*/

#define CHAN1 12
#define CHAN2 14
#define CHAN3 27
#define CHAN4 26

#define FREQCONTROLD0 15
#define FREQCONTROLD1 4
#define FREQCONTROLD2 2
#define FREQCONTROLD3 25

#define DEFULT_MODE 33

#define LED_BUTIN 2

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "serverSettings.h"
#include "OLEDdrawers.h"

bool flag;

WiFiManager wm;

void buttonInterrupt();

void setup() {
  pinMode(LED_BUTIN, OUTPUT);
  digitalWrite(LED_BUTIN, LOW);

  pinMode(CHAN1, OUTPUT);
  pinMode(CHAN2, OUTPUT);
  pinMode(CHAN3, OUTPUT);
  pinMode(CHAN4, OUTPUT);

  pinMode(FREQCONTROLD0, OUTPUT);
  pinMode(FREQCONTROLD1, OUTPUT);
  pinMode(FREQCONTROLD2, OUTPUT);
  pinMode(FREQCONTROLD3, OUTPUT);
  
  Serial.begin(115200);
  initmicroSDCard();
  iniciarOLED();
  
  pinMode(DEFULT_MODE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEFULT_MODE), buttonInterrupt, CHANGE);

  // Define o endereço IP estático
  IPAddress staticIP(192, 168, 0, 100);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  
  // Configura o endereço IP estático
  wm.setSTAStaticIPConfig(staticIP, gateway, subnet);
  //wm.resetSettings();
  if (!wm.autoConnect("Duck Mux", "12345678")) {
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
  }

  digitalWrite(LED_BUTIN, HIGH);
  Serial.println();
  Serial.println("--------------------------");
  Serial.println("WiFi connected");
    // Verifica se o endereço IP estático é válido
  if (WiFi.localIP()) {
    Serial.println("Endereço IP estático definido: " + WiFi.localIP().toString());
  } else {
    Serial.println("Erro ao definir endereço IP estático");    
    wm.resetSettings();
    ESP.restart();
  }
  createServer();
}

void loop() {
  digitalWrite(CHAN4, LOW);
  digitalWrite(CHAN1, HIGH);
  if (OLEDenable){drawOperacao(1,tempo,frequencia);}
  delay(tempo2Mux*1000);

  digitalWrite(CHAN1, LOW);
  digitalWrite(CHAN2, HIGH);
  if (OLEDenable){drawOperacao(2,tempo,frequencia);}
  delay(tempo2Mux*1000);

  digitalWrite(CHAN2, LOW);
  digitalWrite(CHAN3, HIGH);
  if (OLEDenable){drawOperacao(3,tempo,frequencia);}
  delay(tempo2Mux*1000);

  digitalWrite(CHAN3, LOW);
  digitalWrite(CHAN4, HIGH);
  if (OLEDenable){drawOperacao(4,tempo,frequencia);}
  delay(tempo2Mux*1000);

  if(flag){
    digitalWrite(LED_BUTIN, LOW);
    wm.resetSettings();
    ESP.restart();
    delay(1000);
  }
}

void buttonInterrupt() {
  flag = true;
  tempo2Mux = 0;
}