#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "elisa3-lib.h"

#ifdef _WIN32
    #include "windows.h"
#endif

#define OBSTACLE_THR 50 //170

// received from robot
int robotAddress;
unsigned int robProx[8] = {0};

// sent to robot
char robLSpeed=0, robRSpeed=0;
char robRedLed=0, robGreenLed=0, robBlueLed=0;

// various
int rightProxSum=0, leftProxSum=0;	// sum of proximity values on right or left
unsigned int i;

int main(void) {

    srand ( time(NULL) );

    printf("\r\nInsert the robot address: ");
    scanf("%d", &robotAddress);

    // init the communication with the RF module and thus with the robot
    openRobotComm();

    // set the address of the robot to control; if more robots (max of 4 per packet) need to be controlled
    // the function need to be called more times with different ids (0..3) and addresses
    setRobotAddress(0, robotAddress);

    resetFlagTX(robotAddress);  // no onboard obstacle avoidance

    i=0;

    while(1) {

        i = (i+1)%100;  // use to change the rgb leds
        if(i==0) {
            robRedLed = rand()%100;
            robGreenLed = rand()%100;
            robBlueLed = rand()%100;
        }

        getAllProximity(robotAddress, robProx);

        // obstacle avoidance using the 3 front proximity sensors
        rightProxSum = robProx[0]/2 + robProx[1];
        leftProxSum = robProx[0]/2 + robProx[7];

        rightProxSum /= 10;                 // scale the sum to have a moderate impact on the velocity
        leftProxSum /= 10;
        if(rightProxSum > 60) {             // velocity of the motors goes from -30 to +30
            rightProxSum = 60;
        }
        if(leftProxSum > 60) {
            leftProxSum = 60;
        }
        robRSpeed = 30-leftProxSum;     // set the speed to the motors
        robLSpeed = 30-rightProxSum;

        setLeftSpeed(robotAddress, robLSpeed);
        setRightSpeed(robotAddress, robRSpeed);
        setRed(robotAddress, robRedLed);
        setGreen(robotAddress, robGreenLed);
        setBlue(robotAddress, robBlueLed);

    }

    closeRobotComm();

	return 0;

}

