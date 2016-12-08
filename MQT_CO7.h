#include <CS_MQ7.h>
CS_MQ7 MQ7(22, 13);  // 12 = digital Pin connected to "tog" from sensor board
                     // 13 = digital Pin connected to LED Power Indicator
int CoSensorOutput = 0; //analog Pin connected to "out" from sensor board
float CoData = 0;         //analog sensor data
unsigned short sensor_OK =0;
void MQ7_GET ()
{
   MQ7.CoPwrCycler();  
  if(MQ7.CurrentState() == LOW)
  {   //we are at 1.4v, read sensor data!
    CoData = analogRead(CoSensorOutput);
    Serial.println(CoData);
    sensor_OK=1;
  }
  else
  {                            //sensor is at 5v, heating time
    Serial.println("sensor heating!");
  }      
}

