# TrafficControlArduino

## Description
####
The hardware components needed are: two Arduino Mega or Uno boards, two sonar, an LCD, a stepper motor, a stepper driver, a 9V battery, a led and a resistor.

We want to calculate the speed of a car and display its speed on the LCD, and if the car moves faster than a set speed limit, a light flashes and the stepper raises a bumper ahead of her to slow it down. The first Arduino is linked to sonar, led and LCD, and the second Arduino is linked to the first Arduino and the stepper driver, which in turn is connected to the stepper and powered by a 9V battery.

Speed calculation principle: when the first sonar receive a signal, a time variable begin counting the time until the second sonar receive a signal. So, is calculated the time the car needs to run the distance between the two sonar. The distance between the 2 sonar is 30 cm. The final speed formula we want is the distance / time between sonar multiplied by 3.6 to translate the result from m/s to km/h (float speedVehicle = 300. * 3.6 / (duration2Millis - duration1Millis);).
