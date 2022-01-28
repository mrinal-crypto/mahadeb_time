#include<DS3231.h>
#define signalPin 13
#define speaker 3
#define pump 4
#define error 7

unsigned long prevSerialMillis = 0;
unsigned long prevSignalMillis = 0;
const long serialInterval = 1000;
const long signalOnTime = 100;
const long signalOffTime = 1000;

int ledState = LOW;

DS3231  rtc(SDA, SCL);

int Hor;
int Min;
int Sec;
int Year;
unsigned int workTime;

Time t;
//////////////////////////////////////////////////////

void setup()
{
    Serial.begin(9600);
    rtc.begin();
    pinMode(speaker,OUTPUT);
    pinMode(pump,OUTPUT);
    pinMode(error,OUTPUT);
    pinMode(signalPin,OUTPUT);
    void serial();
    void Signal();
    void autoSpeaker();
    void autoPump();

    delay(1000);

    

      
//rtc.setDOW(FRIDAY);///set day by uncomment
//rtc.setTime(23,03,10);///set time
//rtc.setDate(28,1,2022); //set date

}

//////////////////////////////////////////////////////

void loop()
{
  t = rtc.getTime();
  Hor=t.hour;
  Min=t.min;
  Year = t.year;
  workTime = (Hor*60)+Min;

  if(Year != 2000)
  {
    autoSpeaker();
    autoPump();
    Signal();
    serial();  
  }
  if(Year == 2000)
  {
    digitalWrite(error, HIGH);
    Serial.println("--- time not set ---");
    //serial();
    delay(150);
    digitalWrite(error, LOW);
    delay(150);
  }
  

}

/////////////////////////////////////////////////////

void autoSpeaker()
{
  if(workTime>=270 && workTime<720 || workTime>=900 && workTime<1380)
  {
    digitalWrite(speaker, HIGH);
  }
  else
  {
    digitalWrite(speaker, LOW);
  }
}
/////////////////////////////////////////////////
void autoPump()
{
  if(workTime>=300 && workTime<480 || workTime>=900 && workTime<960)
  {
    digitalWrite(pump, HIGH);
  }
  else
  {
    digitalWrite(pump, LOW);
  }
}

////////////////////////////////////////////////////

void serial()
{
  unsigned long currentMillis = millis();
  
  if (currentMillis - prevSerialMillis >= serialInterval)
  {
    prevSerialMillis = currentMillis;    
    Serial.print("Time: ");
    Serial.print(rtc.getTimeStr());
    Serial.print(" || Date: ");
    Serial.print(rtc.getDateStr(FORMAT_SHORT));
    Serial.print(" ");
    Serial.print(rtc.getDOWStr());
    Serial.print(" || temperature: ");
    Serial.print(rtc.getTemp());
    Serial.print(" C\n");
  }


}

////////////////////////////////////////////////////

void Signal()
{
  
  unsigned long currentMillis = millis();
  
  if((ledState == HIGH) && (currentMillis - prevSignalMillis >= signalOnTime))
  {
    ledState = LOW;
    prevSignalMillis = currentMillis;
    digitalWrite(signalPin, ledState);
  }
  else if ((ledState == LOW) && (currentMillis - prevSignalMillis >= signalOffTime))
  {
    ledState = HIGH;
    prevSignalMillis = currentMillis;
    digitalWrite(signalPin, ledState);
  }
  
}

///////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
