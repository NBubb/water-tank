/*
*using initally a non contact sensor and if 1/0 == 0 then a ultrasonic sensor to determin the level of the water in the tank
*then calculates the volume of the water remaining in the tank
*/

void volume() {  //calculates the volume of water in the tank
  now = millis();
  if ((now - volumeTimer) >= volumePeriod) {  //timer for volume poll
    volumeTimer = now;                        //updates levelPrevMillis
    level = digitalRead(3);                   //updates fullLevel variable with sensor reading
    Serial.print("water level = ");
    Serial.println(level);

    if (level == 1) {
      Volume = 3.6;
    }

    if (level == 0) {                     //"level == 0" this section of code has delays as they are necessary in-order to mesure the US signal
      digitalWrite(trigPin, LOW);         //sets trigger pin is LOW double check
      delayMicroseconds(2);               //waits
      digitalWrite(trigPin, HIGH);        //sets trigger pin HIGH starts US ping
      delayMicroseconds(20);              //waits, there is some question as to how long
      digitalWrite(trigPin, LOW);         //sets trigger pin LOW
      duration = pulseIn(echoPin, HIGH);  //sets duration as TOF
      distance = duration * 0.34 / 2;     //calculates distance from TOF
      Serial.print("TOF = ");
      Serial.println(duration);
      Serial.print("Distance = ");
      Serial.print(distance);
      Serial.println(" mm");
      Volume = ((1700 - distance) * 2.27 / 1000);  //needs equation for volume
    }
    Serial.print("water remaining ");
    Serial.print(Volume);
    Serial.println(" m3");
  }
}