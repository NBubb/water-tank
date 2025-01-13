# water-tank
Detect the flow of into and the level of water in a tank.  Calculate the remaing volume of water in the tank.  Indicatevia LEDs if water flow is above or below a threshold, red LED blink if below.  Indicate level (volume) of remain water in the tank via RAG LEDs.  Connect to MQTT broker and update flow (l/h) and remaing volume of water (l).

Hardawre

Arduino nano 33 IOT, 
FL308T, 
XKC-Y25-T12V, 
AJ-SRO-4M, 
RAG LEDs, 
Resistors for LED voltage protection and voltage divider for signals from 5 V sensors, 
protoboard.

Introduction

This project was born out of a practical issue.  A hydralic ram pump lifts water from a spring seveal tens of meters below a holding tank of 3600 l.  The holding tank is large enough to ensure that there is at least one weeks worth of water avalable.  The hydralic ram pump ocasionaly stops pumping for a number of reasons if the operator fails to notice the tank becomes depleated of water.  This project is intended to inform when the water flow has fallen / stopped and the remainng volume of water.

Details

A flow meter using the Hall effect is fitted to the system to detect flow.  There are mechanical float methods avalable, however, this project is centred on a diffrent aproch using an ultrasonic (US) detector (I have an intrest in NDT and this apeals to me).  The selected US detected is water proof and has a operating range of something like 25 - 4000 mm when it cn detect the suface of the water in the tank.  When the tank is full there is less that 25 mm between the water surface and the detector, the output is nonsencical.  A non contact detector is stuck on the side of the water tank and detects the 'full state' of the tank over-riding the reliance on the US sensor at sub 25 mm ranges.

An array of three LDEs, red, amber & green is mounted to protoboard and is visible forom outside the enclosure.  The code checks for the non contact sensor 1/0 and if 0 then checks US sensor and then calculates remaing volume of water.  Using two thresholds this is visual reported via the RAG LEDs.  The code that further checks for a flow above a certain threshold and if it is below this the red LED is blinked.

The calculated flow and volume are then reported to a MQTT broker.

Further Development

inclusion of a thermistor to detect the water temprature.
