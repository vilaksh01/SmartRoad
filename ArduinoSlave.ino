// Arduino code for serial Read
#include <AFMotor.h>
// Connect a stepper motor with 48 steps per revolution (7.5 degree)
// to motor port #2 (M3 and M4)
AF_Stepper motor(48, 2);
void setup()
{
 Serial.begin(115200);
 Serial.println("Listening to ESP32 requests");
 motor.setSpeed(10);  // 10 rpm
while (!Serial) 
{
    ; // wait for serial port to connect. Needed for native USB port only
}
 
}
void loop()
{
  if (Serial.available()>0)
  {
    char c=Serial.read();
    if(c=='D') 
    {
      Serial.write("Yes there's a request");
      // There's a request from user for infinity exit, the car is standing on base so time to bring it down     
      // you can use ultrasonic sensor instead for more better performance   
      motor.step(100, BACKWARD, SINGLE);
      // bring up after 5 minutes
      delay(1000*60*5);
      motor.step(100, FORWARD, SINGLE);
    }
    else
    {
    Serial.write("no");

    }
   
  }
}
