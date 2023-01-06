// C program for testing kinematics for AL5D robot arm
// This program is for testing purposes only, it is not intended to be used in a production environment
// This code is part of the GDIP AL5D robot arm project
// 
// Author: J. O'Hare
// Student ID: 20023827
// GDIP GROUP: 7D
// Version 1.0;
// Date 10/12/2022
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define _USE_MATH_DEFINES
#define BASE_HGT 67.31      //base hight 2.65"
#define SHOULDER 146.05       //shoulder-to-elbow "bone" 5.75"
#define ELBOW 276.225        //elbow-to-wrist "bone" 7.375"
#define GRIPPER 85.72      //gripper (wrist rotate mechanism) length 3.94"
//float to long
#define ftl(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  
// Define constants
float sh_sq = SHOULDER*SHOULDER;
float el_sq = ELBOW*ELBOW;
float robotX = 0, robotY = 0, robotZ = 0;
//function prototypes
float degrees(float rad);
float radians(float deg);
char* getArmCode( float x, float y, float z, float grip_angle_d);
int main()
{
    float x, y, z;
    printf("\n\n");
    printf("Enter x");
    scanf("%f", &x);
    printf("Enter y");
    scanf("%f", &y);
    printf("Enter z");
    scanf("%f", &z);
    //gripper position set to 45 degrees, optimum for measuring when z is 0
    printf("%s", getArmCode(x, y, z, 45));
    return 0;
}

// function to convert radians to degrees
float degrees(float rad)
{
    return (rad * 180.0 / M_PI);
}

float radians(float deg)
{
    return (deg * M_PI / 180.0);
}

char* getArmCode( float x, float y, float z, float grip_angle_d )
{   
    //grip angle in radians for use in calculations
    float grip_angle_r = radians( grip_angle_d );    
    //Adjustment so that the gripper hits the floor at z=0;
    z += 170;
    //Base angle and radial distance from x,y coordinates
    float d = sqrt( x*x + y*y );    //distance from base center to x,y coordinates
    float phi=90.00;
    float bas_angle_r = atan2( x, y );
    float L = sqrt(( x * x ) + ( y * y ));
    //L is y coordinate for the arm
    L -= cos(phi)*GRIPPER;
    y = L;
    //Grip offsets calculated based on grip angle
    float grip_off_z = ( sin( grip_angle_r )) * GRIPPER;
    float grip_off_y = ( cos( grip_angle_r )) * GRIPPER;
    //Wrist position
    float wrist_z = ( z - grip_off_z ) - BASE_HGT;
    wrist_z -= sin(phi)*GRIPPER;
    float wrist_y = y - grip_off_y;
    //Shoulder to wrist distance ( AKA sw )
    float s_w = ( wrist_z * wrist_z ) + ( wrist_y * wrist_y );
    float s_w_sqrt = sqrt( s_w );
    //s_w angle to ground
    //float a1 = atan2( wrist_y, wrist_z );
    float a1 = atan2( wrist_z, wrist_y );
    //s_w angle to SHOULDER
    float a2 = acos((( sh_sq - el_sq ) + s_w ) / ( 2 * SHOULDER * s_w_sqrt ));
    //shoulder angle
    float shl_angle_r = a1 + a2;
    float shl_angle_d = degrees( shl_angle_r );
    //elbow angle
    float elb_angle_r = acos(( sh_sq + el_sq - s_w ) / ( 2 * SHOULDER * ELBOW ));
    float elb_angle_d = degrees( elb_angle_r );
    float elb_angle_dn = -( 180.0 - elb_angle_d );
    //wrist angle
    float wri_angle_d = ( grip_angle_d - elb_angle_dn ) - shl_angle_d;
    //Servo pulses
    float bas_servopulse = 1500.0 - (( degrees( bas_angle_r )) * 11.11 );
    float shl_servopulse = 1500.0 + (( shl_angle_d - 90.0 ) * 6.6 );
    float elb_servopulse = 1500.0 -  (( elb_angle_d - 90.0 ) * 6.6 );
    float wri_servopulse = 1500 - ( wri_angle_d  * 11.1 );
    //Set servo pulses
    //create string for output
    char *output;
    //initialise the string
    output = (char*)malloc(100);
    //Added some offsets here, not sure if they are correct, they seem to work though
    sprintf(output, "#0P%ld#1P%uld#2P%uld#3P%uldT1000\r", ftl(1500-bas_servopulse+500), ftl(shl_servopulse), ftl(elb_servopulse), ftl(wri_servopulse+400));
    return output;
}