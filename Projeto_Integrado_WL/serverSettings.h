/*
  Configurações de operação:
*/


//Tempo que cada canal pode ser trasmitido
int tempo2Mux = 4;  

//Atulização do oled
bool OLEDenable = false;

void atulizarOperacao(String newFreq){
  float floatNewFreq = float(newFreq.toFloat());
  int intNewFreq = int(floatNewFreq*10);
  digitalWrite(FREQCONTROLD0, 1);
  digitalWrite(FREQCONTROLD1, 1);
  digitalWrite(FREQCONTROLD2, 1);
  digitalWrite(FREQCONTROLD3, 1);
    switch (intNewFreq) {
      case 1067:
        digitalWrite(FREQCONTROLD0, 1);
        digitalWrite(FREQCONTROLD1, 1);
        digitalWrite(FREQCONTROLD2, 1);
        digitalWrite(FREQCONTROLD3, 0);
        Serial.println("Configuração do rádio: 0001");
        break;
      case 1069:
        digitalWrite(FREQCONTROLD0, 0);
        digitalWrite(FREQCONTROLD1, 1);
        digitalWrite(FREQCONTROLD2, 1);
        digitalWrite(FREQCONTROLD3, 0);
        Serial.println("Configuração do rádio: 1001");
        break;
      case 1071:
        digitalWrite(FREQCONTROLD0, 1);
        digitalWrite(FREQCONTROLD1, 0);
        digitalWrite(FREQCONTROLD2, 1);
        digitalWrite(FREQCONTROLD3, 0);
        Serial.println("Configuração do rádio: 0101");
        break;
      default:
      break;
  }
  OLEDenable = true;
}

void interromperOperacao(){
    digitalWrite(FREQCONTROLD0, 0);
    digitalWrite(FREQCONTROLD1, 0);
    digitalWrite(FREQCONTROLD2, 0);
    digitalWrite(FREQCONTROLD3, 0);
    Serial.println("Parando trasmissão: 1111");
    OLEDenable = false;
    //drawOperacaoParada();

}

//*********************************************
/*
  Utilização do SDcard:
*/
//Bibliotecas para o cartão SD
#include "FS.h"
#include "SD.h"
#include "SPI.h"

void initmicroSDCard(){
  if(!SD.begin()){
    Serial.println("Initialization SD Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("SD card is not present!");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}


//*******************************************//
/*
  Utilização do Servidor:
*/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Set web server port number to 80
AsyncWebServer server(80);

//Strings de trabalho
String tempo;
String frequencia;

void createServer(){
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("--------------------------");
    Serial.println();
    Serial.println("Recebendo requisição [/]");
    int paramsNr = request->params();
    Serial.print("Numero de parametros recebidos: ");
    Serial.println(paramsNr);

    for(int i=0;i<paramsNr;i++){
      AsyncWebParameter* p = request->getParam(i);
      String value = p->name();

      if(i == 0){
        tempo = value;
      }
      if(i == 1){
        frequencia = value;
        Serial.print("Frequencia do Tx: ");
        Serial.print(frequencia);
        Serial.println("MHz");
      }
    }
    request->send(SD, "/selecao.html", "text/html");
    if(paramsNr>0){
      atulizarOperacao(frequencia);
      tempo2Mux = int(tempo.toInt());
      Serial.print("Tempo por canal definido para: ");
      Serial.print(tempo2Mux);
      Serial.println(" Segundos");
      Serial.println("--------------------------");
      Serial.println();      
    }
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("--------------------------");
    Serial.println();
    Serial.println("Recebendo requisição [/stop]");
    request->send(SD, "/selecao.html", "text/html");
    interromperOperacao();
    Serial.println("--------------------------");
    Serial.println();
  });

  server.serveStatic("/", SD, "/");
  server.begin();

}
