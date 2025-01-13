/*
 * checks WiFi connection and reconnects if it has dropped 
 * maintains the mqtt broker connection
 * sends packets to the mqtt broker
 */

void Comms() {

  mqttClient.poll();  // call poll() regularly to allow the library to send MQTT 'keep alive' which avoids being disconnected by the broker

  now = millis();                         //takes millis() and stores it
  if ((now - mqttTimer) >= mqttPeriod) {  //time check for the mqtt message broadcast
    mqttTimer = now;                      //save the time a message is sent

    conMan.check();  //This runs a WiFi connection check reconnecting if connection is lost -Arduino_ConnectionHandler.h
    Serial.print("Checking WiFi connection ");

    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker);

    if (!mqttClient.connect(broker, port)) {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.connectError());

      //could add and LED alarm state here if the MQTT broker fails to connect
    }

    Serial.println("You're connected to the MQTT broker!");
    Serial.print("Sending message to topic: ");
    Serial.print("tank/flow ");
    Serial.println(Flow);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topicFlow);
    mqttClient.print(Flow);
    mqttClient.endMessage();

    Serial.print("Sending message to topic: ");
    Serial.print("tank/volume ");
    Serial.println(Volume);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topicVolume);
    mqttClient.print(Volume);
    mqttClient.endMessage();
  }
}

void onNetworkConnect() {
  Serial.println(">>>> CONNECTED to network");
}

void onNetworkDisconnect() {
  Serial.println(">>>> DISCONNECTED from network");
}

void onNetworkError() {
  Serial.println(">>>> ERROR");
}
