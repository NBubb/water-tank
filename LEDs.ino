/*
*uses LEDs (RAG) to indicate:
*remaining volume, above and below two thresholds
*if water flow has fallen below a threshold
*/

void LEDs() {
  now = millis();
  if ((now - ledTimer) >= ledPeriod) {
    //Serial.print("led update time ");
    //Serial.println(ledTimer);
    //green
    if ((level == 1) && (Flow >= flowThreshold)) {
      ledIndicators = green;
    }
    //amber
    if ((level == 0) && (Volume >= volumeThreshold) && (Flow >= flowThreshold)) {
      ledIndicators = amber;
    }
    //red
    if ((level == 0) && (Volume < volumeThreshold) && (Flow >= flowThreshold)) {
      ledIndicators = red;
    }
    //green blink
    if ((level == 1) && (Flow < flowThreshold)) {
      ledIndicators = greenBlink;
    }
    //amber blink
    if ((level == 0) && (Volume >= volumeThreshold) && (Flow < flowThreshold)) {
      ledIndicators = amberBlink;
    }
    //red blink
    if ((level == 0) && (Volume < volumeThreshold) && (Flow < flowThreshold)) {
      ledIndicators = redBlink;
    }
    //test
    if ((now - testTimer) >= testPeriod) {
      testTimer = now;  //resets the test timer
      ledIndicators = test;
    }
    //start
    if (now < startPeriod) {
      ledIndicators = test;
    }

    switch (ledIndicators) {

      case test:  //turns all LEDs on for a test period x after a period y of displaying status
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledAmber, HIGH);
        digitalWrite(ledRed, HIGH);
        break;

      case green:  //full with flow
        gr();
        break;

      case amber:  //half full with flow
        am();
        break;

      case red:  //empty with flow
        re();
        break;

      case greenBlink:  //full with no flow
        gr();
        bl();
        break;

      case amberBlink:  //half full with no flow
        am();
        bl();
        break;

      case redBlink:  //half full with no flow
        re();
        bl();
        break;
    }
    ledTimer = now;
  }
}

void gr() {
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledAmber, LOW);
  digitalWrite(ledRed, LOW);
}
void am() {
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledAmber, HIGH);
  digitalWrite(ledRed, LOW);
}
void re() {
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledAmber, LOW);
  digitalWrite(ledRed, HIGH);
}
void bl() {
  if (blink == 0) {
    digitalWrite(ledRed, HIGH);
    blink = 1;
    //Serial.println("High");
  } else {
    if (blink == 1) {
      digitalWrite(ledRed, LOW);
      blink = 0;
      //Serial.println("Low");
    }
  }
}
