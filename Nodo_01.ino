#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <EEPROM.h>

RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);// Include the radio in the network

const uint16_t node00 = 00;  // Address of our node in Octal format (04, 031, etc)
const uint16_t node01 = 01;

float evalor;
int valor;

struct payload_t {              // Structure of our payload
  int nodo;
  float estado;
};



void setup() {
  
  Serial.begin(9600);
  
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(90,node01);
  
}


void loop() {
  network.update();
  evalor= medidor_campo();
  Serial.print(evalor);
  DataRead();
  DataWrite(node00,evalor);
  delay(5000);
}

void DataRead() {

if (network.available()) {
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
 
    Serial.print("Del nodo: ");
    Serial.print(payload.nodo);
    Serial.print(" Llego: ");
    Serial.println(payload.estado);
  }
}


void DataWrite(const uint16_t node, float evalor) {

  network.update();
  payload_t payload_r= {01, evalor};
  RF24NetworkHeader header1(node);
  bool ok= network.write(header1, &payload_r, sizeof(payload_r));
  Serial.print("Envio: ");
  Serial.println(ok ? F("ok"):F("Failed"));
  delay(1000);
  }

 
 float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }


 float medidor_campo() {
    valor = analogRead(A0);
    evalor = mapfloat(valor, 0, 1023, 0, 5); //mapeamos a voltios *10 para mayor resolucion
    return evalor;
  }
