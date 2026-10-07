# BennyBot 
[BennyBot Simulation](https://wokwi.com/projects/476278603594848257)
//the simulation has A4988 drivers used.

A self balancing bot that will evolve as time passes.


Bot overview:
Phase 1( make it balance);

  Parts used:
-Microcontroller: Esp -32 Wroom
-Motor: NEMA 17 stepper motor
-Motor drivers: TMC 2209
-IMU sensor: MPU 6050
-LiPo battery: ovionic air 2200 mAh 50C
-Chasis: 3d printed (inprogress)
-Wheels : 3d printed (in progress


Potential Applications used or to be used:
-Auto fusion 360: for chassis and wheels to be 3d printed.
-KiCad: for the schematic drawing of the circuit.
-Github: to store the code for the project( keep it open source)
-Arduino IDE:For coding the PID loop. Libraries include accelstepper library and MPU 6050 library.

What it is:
This phase we will be implementing a tilt-angle control system through a PID controller driving the error to zero in real time.The bot will have two wheels connected to the NEMA 17 motors which will be controlled by a PID control loop through the inputs of the MPU 6050 that gives the Processor the tilt angle the bot is currently at. WIth this info the code in the esp makes a calculation of the amount of steps the motor has to take in either direction to get the tilt angle to zero balancing the bot. 
