#include <mbed.h>
#include <Servo.h>

//Can use PB_0 for led 
PwmOut led(PB_0);


//Create Servos
Servo servo1A(PA_0);
Servo servo2A(PA_1);
Servo servo3A(PA_2);
Servo servo4A(PA_3);
 
//Create CAN message
CAN can1(PA_11, PA_12);
CANMessage msg;

//Limits
int aeroMax = 100;
int minBrakePercentage;
int minHeight;
int aeroOnManual;                //Aero is off with 0, Aero is on with 1

//Needed Variables 
float brakePercent=0;              //Variable for brake percentage
int mode = 0;                  //Variable for the current mode of the car
int automaticAero = 0;         //0 for automatic active aero, 1 for manual active aero

//Function prototypes
void moveAero(float x);
void setTrackCondition(int x);


int main() {
    can1.frequency(250000);
    while(1) {
        wait(.1);
        if(can1.read(msg)){         //Gets a message from the can bus
            if(msg.id == 0){          //Change ID later 
                brakePercent =(msg.data[0]/100.0); //The brake pressure will be stored in msg.data[x]
                led.write(float(brakePercent/100.0)); //This is just testing to see if the code was working
            }

            if(msg.id == 1){                                //Check to see if you need to use button or if aero is automatic
                automaticAero = msg.data[2];                //The data to know if the active aero is manual or auto
            }

            if(msg.id == 2 && automaticAero == 1){          //Check button for manual aero
                aeroOnManual = msg.data[1];                 //The button for manual aero will be stored in msg.data[x]
            }            
        }

        /**
         * Need this space to read CAN message to get mode for the car
         * and set the track conditions
        **/


        automaticAero =0;
        mode = 3;

        if(automaticAero == 0){                      //If the car is in automatic it will run
            setTrackCondition(mode);    
            if(brakePercent < minBrakePercentage){
                moveAero(minBrakePercentage);                //Keeps the wing at minimum height
            }
            else{
               moveAero(brakePercent);                       //Keeps wing at the height at the height of the brake percentage
            }
            
        }
        else{                                                //Allows Drive to use button to put wing up and wing down
            if(aeroOnManual == 0){
                moveAero(minHeight);
            }
            else{
                moveAero(aeroMax);
            }
        }
    }
}

void moveAero(float x){                                        //Moves aero wing to given input
    //x = 180 * (x/100);                                       //Converts the value so it is valid for the wing
    servo1A.write(x);
    servo2A.write(x);
    servo3A.write(x);
    servo4A.write(x);
}

void setTrackCondition(int x){                                //Sets track condition so you can change limitations 
    if(x == 1){                   //Wet mode
        minBrakePercentage = .50;
    }
    else if(x == 2){              //Semi-wet mode
        minBrakePercentage = .3;
    }
    else if(x == 3){              //Dry mode
        minBrakePercentage = 0;
    }
}