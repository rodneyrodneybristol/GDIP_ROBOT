#include <Arduino.h>
#include <SoftwareSerial.h>
#define _USE_MATH_DEFINES
#define BASE_HGT 67.31  // base hight 2.65"
#define SHOULDER 146.05 // shoulder-to-elbow "bone" 5.75"
#define ELBOW 276.225   // elbow-to-wrist "bone" 7.375"
#define GRIPPER 85.72   // gripper (wrist rotate mechanism) length 3.94"
// float to long
#define ftl(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
// DEBUG global delay used for debugging only
#define delayTime 5000
#define boxHeight 200
// This is the the tolerance for the resistor readings
#define TOLERANCE 0.1
// Define constants

float sh_sq = SHOULDER * SHOULDER;
float el_sq = ELBOW * ELBOW;
float robotX = 0, robotY = 0, robotZ = 0;
// function prototypes
char *getArmCode(float, float, float);
void goBackPickup();
void startAndCalibrateArm();
// initialise the string
String listen_for_serial();
long BoxCoordinatesXY[10][3] = {
    // Row 1
    {0, 260, 1000},       //1k
    {0, 330, 4700},       //4.7k
    {0, 390, 10000},      //10k
    // Row 2
    {80, 270, 20000},     //20k
    {80, 330, 47000},     //47k
    {80, 390, 86000},     //86k
    // Row 3
    {160, 270, 100000},   //100k
    {160, 330, 180000},   //180k
    // Row 4
    {240, 270, 470000},   //470k
    {240, 330, 1000000}}; //1M
char output[256] = "";

void setup()
{
  Serial.begin(9600);
  //Serial.begin(9600);
  startAndCalibrateArm();
}

void loop()
{
  // Even though the Arduino main loop is essentially the same as a while(1) loop,
  // I have noticed small bugs where certain functions such as serial will have
  // unpredictable results, such as serial printing twice on startup.
  // So I prefer to run a while(1) within the "loop", which avoids these issues
  goBackPickup();
  while (1)
  {

    // listens for a integer over serial, if that value is within the tolerance of a
    // specified value, then send the associated position command to SSC-32u
    String checker = listen_for_serial();
    if (checker != "")
    {
      for (int i = 0; i < 10; i++)
      {
        if (checker.toInt() >= (BoxCoordinatesXY[i][2] - (BoxCoordinatesXY[i][2] * TOLERANCE)) && checker.toInt() <= (BoxCoordinatesXY[i][2] + (BoxCoordinatesXY[i][2] * TOLERANCE)))
        {
          Serial.println(getArmCode(BoxCoordinatesXY[i][0], BoxCoordinatesXY[i][1], boxHeight));
          delay(3000);
          //Serial.print("MOF\n ");
          delay(1000);
          //Serial.println("MOVE");
          //Serial.println(getArmCode(BoxCoordinatesXY[i][0], BoxCoordinatesXY[i][1], boxHeight));
          // DEBUG
          //delay(delayTime);
          goBackPickup();
        }
      }
    }
  }
}

String listen_for_serial()
{
  String message;
  if (Serial.available() > 0)
  {
    message = Serial.readString();

    return message;
  }
  return "";
}

// ========================================================================
// ARM FUNCTIONS
// ========================================================================

// Startup calibration function
void startAndCalibrateArm()
{

  Serial.println("#0P1425#1P1500#2P1500#3P700T2000\r"); // starting position
  Serial.println("#2PO-100\r");                 // calibrate
  
  // DEBUG
  delay(2000);
}

// Function moves arm up back to start position, down to
// measure, then back up again
void goBackPickup()
{
  Serial.println("#0P1425#1P1500#2P1500#3P700T2000\r");
  delay(2000);
  Serial.println("#0P1425#1P1243#2P1616#3P940T3000\r");
  delay(3500);
  Serial.print("TEST\n");
  delay(3000);
  Serial.println("#0P1425#1P1500#2P1500#3P700T1000\r");
  delay(1100);
}

// ========================================================================
// KINEMATICS
// ========================================================================
char *getArmCode(float x, float y, float z)
{
  float grip_angle_d = 68;
  // grip angle in radians for use in calculations
  float grip_angle_r = radians(grip_angle_d);
  // Adjustment so that the gripper hits the floor at z=0;
  z += 70;
  // Base angle and radial distance from x,y coordinates
  // float d = sqrt( x*x + y*y );    //distance from base center to x,y coordinates
  float phi = 90.00;
  float bas_angle_r = atan2(x, y);
  float L = sqrt((x * x) + (y * y));
  // L is y coordinate for the arm
  L -= cos(phi) * GRIPPER;
  y = L;
  // Grip offsets calculated based on grip angle
  float grip_off_z = (sin(grip_angle_r)) * GRIPPER;
  float grip_off_y = (cos(grip_angle_r)) * GRIPPER;
  // Wrist position
  float wrist_z = (z - grip_off_z) - BASE_HGT;
  wrist_z -= sin(phi) * GRIPPER;
  float wrist_y = y - grip_off_y;
  // Shoulder to wrist distance ( AKA sw )
  float s_w = (wrist_z * wrist_z) + (wrist_y * wrist_y);
  float s_w_sqrt = sqrt(s_w);
  // s_w angle to ground
  // float a1 = atan2( wrist_y, wrist_z );
  float a1 = atan2(wrist_z, wrist_y);
  // s_w angle to SHOULDER
  float a2 = acos(((sh_sq - el_sq) + s_w) / (2 * SHOULDER * s_w_sqrt));
  // shoulder angle
  float shl_angle_r = a1 + a2;
  float shl_angle_d = degrees(shl_angle_r);
  // elbow angle
  float elb_angle_r = acos((sh_sq + el_sq - s_w) / (2 * SHOULDER * ELBOW));
  float elb_angle_d = degrees(elb_angle_r);
  float elb_angle_dn = -(180.0 - elb_angle_d);
  // wrist angle
  float wri_angle_d = (grip_angle_d - elb_angle_dn) - shl_angle_d;
  // Servo pulses
  float bas_servopulse = 1500.0 - ((degrees(bas_angle_r)) * 11.11);
  float shl_servopulse = 1500.0 + ((shl_angle_d - 90.0) * 6.6);
  float elb_servopulse = 1500.0 - ((elb_angle_d - 90.0) * 6.6);
  float wri_servopulse = 1500 - (wri_angle_d * 11.1);
  // Set servo pulses
  // create string for output

  // Added some offsets here, not sure if they are correct, they seem to work though
  sprintf(output, "#0P%ld#1P%ld#2P%ld#3P%ldT2000\r", ftl(1500 - bas_servopulse + 500), ftl(shl_servopulse), ftl(elb_servopulse), ftl(wri_servopulse + 400));
  return output;
}
