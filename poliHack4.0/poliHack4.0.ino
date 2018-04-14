#include <Stepper.h>

#include <DHT22.h>
#include <LiquidCrystal.h>

#define MAX_LONG 9999999
#define DHT22_PIN 10

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);

const int trigPin1 = 12;
const int echoPin1 = 13;
const int trigPin2 = 8;
const int echoPin2 = 9;

long duration1, cm1 = MAX_LONG, duration2, cm2 = MAX_LONG, duration1Millis, duration2Millis, speedVehicle;
boolean firstVehicleDetected, secondVehicleDetected;

int rainArray[250] = {0};
int rainArrayIndex = -1;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

byte celsius[8] = {
                        B00111,
                        B00101,
                        B00111,
                        B00000,
                        B00000,
                        B00000,
                        B00000,
                        B00000
                        };

 byte arrowDown[8] = {
                        B00100,
                        B00100,
                        B00100,
                        B00100,
                        B00100,
                        B10101,
                        B01110,
                        B00100
                        };

float temperature;
int humidity;
int prevTimeReadTemp = 0;
int currentTimeReadTemp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(A2, OUTPUT);
  pinMode(11, OUTPUT);

  firstVehicleDetected = false;
  secondVehicleDetected = false;

  lcd.createChar(0, celsius);
  lcd.createChar(1, arrowDown);
  lcd.begin(16, 2);
  }

void loop() {
   DHT22_ERROR_t errorCode;
   // The sensor can only be read from every 1-2s, and requires a minimum
   // 2s warm-up after power-on.
   //delay(1000);
   
   errorCode = myDHT22.readData();
   temperatureAndHumidity(errorCode); 
 /* Serial.print("Temperature: ");
  Serial.println(temperature);
   Serial.print("humidity: ");
  Serial.println(humidity);*/
  
   analogWrite(A2, 0);//for lighting the RED LEDs
   digitalWrite(11, LOW);
   
   int analogPinForRain = analogRead(5);
   boolean checkRain = checkIfItIsRaining(analogPinForRain);
  
   blankLCD();//make the lcd blank

  
    // put your main code here, to run repeatedly:
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  triggeringHIGH(trigPin1);
  duration1 = pulseIn(echoPin1, HIGH);
  duration1Millis = millis();
  
  
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  triggeringHIGH(trigPin2);
  duration2 = pulseIn(echoPin2, HIGH);
  duration2Millis = millis();
  
  // convert the time into a distance
  cm1 = microsecondsToCentimeters(duration1);
  cm2 = microsecondsToCentimeters(duration2);
  
  Serial.print("cm1: ");
  Serial.println(cm1);
  Serial.print("cm2: ");
  Serial.println(cm2);
   
  int delay_time = 0;
  if(cm1 < 20)
  {
    Serial.println("Sonar1!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    delay_time = 0;
    
    while(true)
    {
      if(cm2 < 20)
      {
        Serial.println("Sonar2!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        break;
      }

      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(5);
      digitalWrite(trigPin2, LOW);

      duration2 = pulseIn(echoPin2, HIGH);
      duration2Millis = millis();
      cm2 = microsecondsToCentimeters(duration2);
      
     }

    float speedVehicle = 300. * 3.6/(duration2Millis - duration1Millis);//
    printDataLCD(speedVehicle, checkRain, temperature, humidity);
    //delay(1000);
  }
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return (microseconds / 29 / 2);
}

void blankLCD(){
  //blank lcd
   lcd.setCursor(0, 0);
   lcd.write("                "); //16 spaces for the whole first row
   lcd.setCursor(0, 1);
   lcd.write("                "); //16 spaces for the whole second row 
}

void printDataLCD(double speedVehicle, boolean raining, float temperature, int humidity){
   lcd.setCursor(0, 0);
   lcd.print("Speed: ");
   lcd.print(speedVehicle, 1);
   lcd.print(" km/h");

   if(speedVehicle >= 2. && speedVehicle < 3.)
   {
    lcd.setCursor(0,1);
    digitalWrite(11, HIGH);// rise of the speed bump
    
    if(raining && temperature < 0)
    {
      lcd.print("Limit: 1.3km/h!!");
    }
    else if(raining && temperature >= 0)
   {
    lcd.print("Limit: 1.5km/h!!");
   }
   else if(temperature <= 8 && humidity >= 80){//mazga
      lcd.print("Limit: 1.4km/h!!");
   }
   else if(temperature < 0 && (raining == true || humidity >= 80)){//polei
    lcd.print("Limit: 1.2km/h!!");
   }
   else{
    lcd.print("Limit: 2.0km/h!!");
    }
    lcd.write(byte(1));

    int ok = 0;
    for(int j = 0; j < 10; j++)
    {
      analogWrite(A2, 700);
      delay(100);
      analogWrite(A2, 0);
      delay(100);
      ok = 1;
    }
    if(ok == 0)
      delay(2500);
    else 
     delay(500);
  }
   
   else if(speedVehicle >= 3.)
   {
    digitalWrite(11, HIGH);// rise of the speed bump
    int ok = 0;
    lcd.setCursor(0,1); 
    lcd.print("SLOW DOWN!!!    ");
    for(int j = 0; j < 10; j++)
    {
      analogWrite(A2, 700);
      delay(100);
      analogWrite(A2, 0);
      delay(100);
      ok = 1;
    }
    if(ok == 0)
      delay(2500);
    else 
     delay(500);
   }
   else
   {
    //second row of the lcd is blank
    lcd.setCursor(0,1); 
    lcd.print("                ");
    delay(2500);
   }

   //page nr 2 of display
   
   lcd.setCursor(0, 0);

   //printing temperature
   lcd.print(temperature, 1);
   lcd.write(byte(0));
   lcd.print("C ");
   
   if(raining)
   {
     lcd.print("WET ROAD!");
   }

   else
   {
     lcd.print("          ");
   }

   //printing humidity
   lcd.setCursor(0, 1);
   lcd.print("Humidity: ");
   lcd.print(humidity);
   lcd.print("%   ");
   delay(2500);
}

void triggeringHIGH(const int trigPin){
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

boolean checkIfItIsRaining(int analogPinValue){
  if(rainArrayIndex == 249)
  {
    for(int j = 0; j < 250; j++)
      rainArray[j] = 0;
    rainArrayIndex = -1;
    return false;
  }

  if(rainArrayIndex < 7)
  {
    rainArray[++rainArrayIndex] = analogPinValue;
    return false;
  }

  //if rainArrayIndex > 6
  rainArray[++rainArrayIndex] = analogPinValue;
  
  for(int j = rainArrayIndex - 7; j <= rainArrayIndex; j++)
    if(rainArray[j] < 800)
      return false;
 
 return true;
}

 
 void temperatureAndHumidity(DHT22_ERROR_t errorCode){
  
  if(prevTimeReadTemp == 0 && currentTimeReadTemp == 0)
  {
    delay(2000);

    switch(errorCode)
  {
    case DHT_ERROR_NONE:
      temperature = myDHT22.getTemperatureC();
      humidity = myDHT22.getHumidity();
      break;
       
    case DHT_ERROR_CHECKSUM:
      temperature = myDHT22.getTemperatureC();
      humidity = myDHT22.getHumidity();
      break;
      
    default: break;
  }
  
    prevTimeReadTemp = millis();
    return;
  }

    currentTimeReadTemp = millis();

    if( (currentTimeReadTemp - prevTimeReadTemp) >= 1800){
      
      switch(errorCode)
      {
        case DHT_ERROR_NONE:
          temperature = myDHT22.getTemperatureC();
          humidity = myDHT22.getHumidity();
          break;
           
        case DHT_ERROR_CHECKSUM:
          temperature = myDHT22.getTemperatureC();
          humidity = myDHT22.getHumidity();
          break;
          
        default: break;
      }
      prevTimeReadTemp = currentTimeReadTemp;
       
    }
}


