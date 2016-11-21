 
#include "TEMP_HUM.h"
#include "MQT_CO7.h"
#include "GPS.h"
#include "SIM900.h"
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <TinyGPS.h>
  

GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate
TinyGPS gps;
SoftwareSerial ss(12, 13);

#define Updating_time 500000

unsigned short GPS_DTAT_found=0;

String  TEMP_S;
String  HUMI_S;
String  LONG_S;
String  LAT_S;
String  CO_S;

/*
 * Test REST endpoint URL: http://www.mocky.io/v2/5832da0a1200001207607078
 * 
 * 
 */

void setup()
{
  Serial.begin(9600);
 
  while(!gprsTest.init()) 
  {
      delay(1000);
      Serial.print("init error\r\n");
  }  
  Serial.println("gprs init success");
  Serial.println("start to send message ...");
  // ss.begin(9600);
}

void loop() 
{
  get_temp_hum ();

//  while(1)
//  {
//  ss.listen();
//  GPS_DATA ();
//   if(GPS_DTAT_found==1)
//    {
//      GPS_DTAT_found=0;
//      Serial.print("done");
//      break;
//    }
//  }
//  
  while(1)
  {
  MQ7_GET ();
  if(sensor_OK==1)
    {
      sensor_OK=0;
      Serial.print("done");
      break;
    }
  }

  data_convirsion();
  delay(Updating_time);
}

void data_convirsion()
{
  LONG_S="17.29140";
  LAT_S="78.23051";
  GPRS gprsTest1(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate
  char load_info[50];
  HUMI_S=String(HUMI);
  TEMP_S=String(TEMP);
  CO_S=String(CoData);
  MESSAGE="HUM:"+HUMI_S+'\r'+"TEMP:"+TEMP_S+'\r'+"CO:"+CO_S+'\r'+"LON:"+LONG_S+'\r'+"LAT:"+LAT_S;
  for(int x=0; x<=49 ; x++)
  load_info[x]= MESSAGE[x];
  gprsTest1.sendSMS(PHONE_NUMBER,load_info); //define phone number and text
  Serial.println("SMS SENT");
  while(1);
}

void GPS_DATA ()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
       Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
 if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print(flat);
    Serial.print(flon);
    LONG_S = String(flon);
    LAT_S =  String(flat);
    GPS_DTAT_found=1;
  }
}

