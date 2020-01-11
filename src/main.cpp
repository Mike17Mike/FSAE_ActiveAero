#include "mbed.h"
#include "Servo.h"

//Can use PB_0 for led 
PwmOut led(PB_0);

Servo servo1A(PA_0);
Servo servo2A(PA_1);
Servo servo3A(PA_2);
Servo servo4A(PA_3);
 
CAN can1(PA_11, PA_12);
CANMessage msg;
int brakePercent;
int mode = 0;
int transmission = 0;


void moveAero(int x){
  x = 180 * (x/100);
  servo1A.write(x);
  servo2A.write(x);
  servo3A.write(x);
  servo4A.write(x);

  wait_ms(10);
}

int main() {
    can1.frequency(250000);
    while(1) {
        if(can1.read(msg)){         //Gets a message from the can bus
            if(msg.id == 0){          //Change ID later 
                brakePercent = msg.data[0]; //The brake pressure will be stored in msg.data[x]
                led.write(float(brakePercent/100.0));
            }
        }

        if(transmission == 0){       //If the car is in automatic it will run
            switch(mode){             //Check if the mode is in wet, dry, or semi-wet
                case 1:                   //Mode is Wet
                    if(brakePercent <= 50){
                    moveAero(50);
                    }
                    else{
                        moveAero(brakePercent);
                    }
                    break;
                case 2:                   //Mode is Semi-wet
                    if(brakePercent <= 25){
                        moveAero(25);
                    }
                    else{
                        moveAero(brakePercent);
                    }
                    break;
                case 3:                   //Mode is Dry
                    moveAero(brakePercent);
                    break;
            }
        }
        else{
            if(can1.read(msg)){       //Gathers message from can bus
                if(msg.id == 1){        //Checks if the message is from desired location
                    if(msg.data[0] == 0){ //If the data is true and it is turned off
                        moveAero(0);        //Moves the wing all the way down
                    }
                    else if(msg.data[0] ==1){//If the button is pressed and turned on
                        moveAero(180);      //Moves the wing all the way up
                    }
                }
            }
        }
    }
}