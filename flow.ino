/*
*measures flow of water into the tank
*/

void FlowCount() {
  count++;  //increments count, called in the attachInterrupt()
}

void flow() {  //calculates the flow of water to the tank
  now = millis();
  if (now - flowTimer >= flowPeriod) {
    int localCount = 0;  //local counter for this function only used in calculation
    noInterrupts();      //disable interrupts
    localCount = count;  //takes the volatile count and stored it in the localcount
    count = 0;           //resets the counter
    interrupts();        //enable interrupts
    //calculation
    Serial.print("number of puleses = ");
    Serial.println(localCount);
    flowTime = ((now - flowTimer) / 1000);  //calculated period pulses were measured
    Serial.print("time over which flow is measured = ");
    Serial.print(flowTime);
    Serial.println(" s");
    Flow = ((localCount * 3600) / (flowTime * 23));  //l/h, the odd structure of the equation maintains integer values whilst processing
    Serial.print("flow = ");
    Serial.print(Flow);
    Serial.println(" l/h");
    flowTimer = now;
  }
}