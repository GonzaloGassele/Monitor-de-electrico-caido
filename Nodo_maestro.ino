#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <EEPROM.h>

RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);// Include the radio in the network 

const uint16_t node00 = 00;      
const uint16_t node01 = 01;
const uint16_t node011 = 011;


struct payload_t {
  int nodo;
  float estado;
};




void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(90, node00); //(channel, node address)
  
    
}

void loop() {

   //DataWrite(node01); //Pide informacion al nodo 01
   //DataWrite(node011); //Pide informacion al nodo 01
   DataRead();
   delay(1000);
}


void DataWrite(const uint16_t node)
{
  
  network.update();
  payload_t payload = {00,1};
  RF24NetworkHeader header1(node);
  bool ok= network.write(header1, &payload, sizeof(payload));
  Serial.print("Envio: ");
  Serial.println (ok ? F("ok"):F("Failed"));

}

void DataRead()
{
   network.update();
  if(network.available()){
    payload_t payload_r;
    RF24NetworkHeader header;
    bool ok= network.read(header, &payload_r, sizeof(payload_r));
    Serial.print("Del nodo: ");
    Serial.print(payload_r.nodo);
    Serial.print(" Llego: ");
    Serial.println(payload_r.estado);
  
  }
}
