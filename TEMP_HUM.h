#include "dht.h"
dht DHT;
#define DHT11_PIN 5

float TEMP=0.0;
float HUMI=0.0;

void get_temp_hum ()
{
 
 while(1)
 {
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.println("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error,\t"); 
    break;
    case DHTLIB_ERROR_CONNECT:
        Serial.println("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.println("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.println("Ack High error,\t");
        break;
    default: 
    Serial.println("Unknown error,\t"); 
    break;
  }
  if(chk==0)
  {
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  break;
  }
  
  delay(2000);
}
}

