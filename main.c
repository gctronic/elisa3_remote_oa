#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "elisa3-lib.h"
#ifdef _WIN32
    #include "windows.h"
#endif

#define OBSTACLE_THR 50
#define NUM_ROBOTS 8

unsigned char updateRGB(char *red, char *green, char *blue) {
    static unsigned int i=0;
    unsigned int rndNum;

    i = (i+1)%65000;  // use to change the rgb leds
    if(i==0) {
        rndNum = rand()%400;
        if(rndNum < 100) {
            *red = rand()%100;
            *green = rand()%100;
            *blue = 0;
        } else if(rndNum < 200) {
            *red = rand()%100;
            *green = 0;
            *blue = rand()%100;
        } else if(rndNum < 300) {
            *red = 0;
            *green = rand()%100;
            *blue = rand()%100;
        } else {
            *red = rand()%100;
            *green = rand()%100;
            *blue = rand()%100;
        }
        return 1;
    }
    return 0;
}

void avoidObstacles(unsigned int *prox, char *left, char *right) {
    int rightProxSum=0, leftProxSum=0;	// sum of proximity values on right or left

    // obstacle avoidance using the 3 front proximity sensors
    rightProxSum = prox[0]/2 + prox[1];
    leftProxSum = prox[0]/2 + prox[7];

    rightProxSum /= 5;                 // scale the sum to have a moderate impact on the velocity
    leftProxSum /= 5;
    if(rightProxSum > 60) {             // velocity of the motors set to be from -30 to +30
        rightProxSum = 60;
    }
    if(leftProxSum > 60) {
        leftProxSum = 60;
    }
    *right = 30-leftProxSum;     // set the speed to the motors
    *left = 30-rightProxSum;

}

int main(void) {

    unsigned int k=0;
    int robotAddress[NUM_ROBOTS];
    // received from robot
    unsigned int robProx[NUM_ROBOTS][8];
    // sent to robot
    char robLSpeed[NUM_ROBOTS], robRSpeed[NUM_ROBOTS];
    char robRedLed, robGreenLed, robBlueLed;

    srand(time(NULL));

    for(k=0; k<NUM_ROBOTS; k++) {
        printf("\r\nInsert the robot address %d: ", k);
        scanf("%d", &robotAddress[k]);
    }

    // init the communication with the robots; set the addresses and number of the robots to control
    startCommunication(robotAddress, NUM_ROBOTS);

    while(1) {

        getAllProximityFromAll(robProx);

        for(k=0; k<NUM_ROBOTS; k++) {
            avoidObstacles(robProx[k], &robLSpeed[k], &robRSpeed[k]);
        }
        setLeftSpeedForAll(robLSpeed);
        setRightSpeedForAll(robRSpeed);

        if(updateRGB(&robRedLed, &robGreenLed, &robBlueLed)) {
            for(k=0; k<NUM_ROBOTS; k++) {
                setRed(robotAddress[k], robRedLed);
                setGreen(robotAddress[k], robGreenLed);
                setBlue(robotAddress[k], robBlueLed);
            }
        }

    }

    stopCommunication();

	return 0;

}

