 
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

String lift_brack = "{";
String right_brack = "}";

String ID_s = "deviceId";
String ID="15";

String s_virsion ="softwareVersion";
String s_v ="1.2";

String s_telemetry ="Telemetry";

String lift_squ_brack="[";
String right_squ_brack="]";

/*sensor ID Map :
  DHT11 -------> 0
  CO    -------> 1
*/

String s_sensorid="sensorId";
String s_sensorType="sensorType";
String s_sensorValue="sensorValue";

String s_CO="CO";
String s_TEMP="TEMP";
String s_HUMI="HUMI";
String s_location="location";
String s_long ="long";
String s_lat="Lat";

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
 
//  while(!gprsTest.init()) 
//  {
//      delay(1000);
//      Serial.print("init error\r\n");
//  }  
//  Serial.println("gprs init success");
//  Serial.println("start to send message ...");
   ss.begin(9600);
}

void loop() 
{
  get_temp_hum ();

  while(1)
  {
  ss.listen();
  GPS_DATA ();
   if(GPS_DTAT_found==1)
    {
      GPS_DTAT_found=0;
      Serial.print("done");
      break;
    }
  }
  
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
//  LONG_S="17.29140";
//  LAT_S="78.23051";
 // GPRS gprsTest1(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate
  char load_info[50];
  HUMI_S=String(DHT.humidity);
  TEMP_S=String(DHT.temperature);
  CO_S=String(CoData);
  MESSAGE="HUM:"+HUMI_S+'\r'+"TEMP:"+TEMP_S+'\r'+"CO:"+CO_S+'\r'+"LON:"+LONG_S+'\r'+"LAT:"+LAT_S;
  for(int x=0; x<=49 ; x++)
  load_info[x]= MESSAGE[x];
//  gprsTest1.sendSMS(PHONE_NUMBER,load_info); //define phone number and text
  Serial.println(HUMI_S);
   Serial.println(TEMP_S);
   Serial.println(CO_S);
  
  test_fun();
  test_fun();
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


void test_fun()
{
   //while(1)
 // {
      //Serial.println(lift_brack+"\""+ID_s+"\""+":"+"\""+ID+"\""+right_brack);
      //Serial.println(lift_brack+"\""+ID_s+"\""+":"+"\""+ID+"\""+","+"\""+s_virsion+"\""+":"+"\""+s_v+"\""+","+"\""+s_telemetry+"\""+":"+lift_squ_brack+lift_brack+"\""+s_sensorid+"\""+":"+"\""+"0"+"\""+","+"\""+s_sensorType+"\""+":"+"\""+"CO"+"\""+","+"\""+s_sensorValue+"\""+":"+"\""+CO_S+"\""+right_brack+right_squ_brack+right_brack);
     // Serial.println("{\"Surya\":\"ALAH\"}");
    //Serial.println(lift_brack+ID_s+ID+right_brack);
Serial.println(lift_brack+"\""+ID_s+"\""+":"+"\""+ID+"\""+","+"\""+s_virsion+"\""+":"+"\""+s_v+"\""+","+"\""+s_telemetry
                      +"\""+":"+lift_squ_brack+lift_brack+"\""+s_sensorid+"\""+":"+"\""+"0"+"\""+","+"\""+s_sensorType
                      +"\""+":"+"\""+"CO"+"\""+","+"\""+s_sensorValue+"\""+":"+"\""+CO_S+"\""+right_brack+","+lift_brack+"\""+s_sensorid
                      +"\""+":"+"\""+"1"+"\""+","+"\""+s_sensorType+"\""+":"+"\""+"TEMP"+"\""+","+"\""+s_sensorValue+"\""+":"+"\""+TEMP_S+"\""+right_brack
                      +","+lift_brack+"\""+s_sensorid+"\""+":"+"\""+"1"+"\""+","+"\""+s_sensorType+"\""+":"+"\""+"HUMI"+"\""+","+"\""+s_sensorValue
                      +"\""+":"+"\""+HUMI_S+"\""+right_brack+right_squ_brack+","+"\""+"Location"+"\""+":"+lift_squ_brack+lift_brack
                      +"\""+"Long"+"\""+":"+LONG_S+","+"\""+"Lat"+"\""+":"+LAT_S+right_brack+right_squ_brack+right_brack);
    delay(500);
 // }
}

