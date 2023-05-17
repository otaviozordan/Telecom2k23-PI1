//*******************************************//
/*
  Utilização do OLED:
*/

//Bibliotecas para uso do OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void iniciarOLED(){
// initialize OLED display with address 0x3C for 128x64
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
  Serial.println(F("SSD1306 allocation failed"));
  while (true);
}
}

void drawOperacao(int canal, String tempo, String frequencia){
    display.clearDisplay();
    display.println("Trasmitindo em: " + frequencia);
    display.println("Mux de " + tempo + "S");
    display.println("Canal " + canal);
    display.display();
}
void drawAcessHome(){
    display.clearDisplay();
    display.println("Acesse o IP");
    display.println(String(WiFi.localIP()));
    display.println("Para configurar");
    display.display();
}

void drawOperacaoParada(){
    display.clearDisplay();
    display.println("Acesse o IP");
    display.println(String(WiFi.localIP()));
    display.println("Tx desabilitado");
    display.display();
}