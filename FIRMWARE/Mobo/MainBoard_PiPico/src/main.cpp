// MAIN BOARD CODE FOR GDIP7D OHM METER ARM PROJECT
// THIS CODE IS FOR RP2040 BASED SYSTEMS (PICO,PICO W)

#include <Arduino.h>
#define _USE_MATH_DEFINES
#define BASE_HGT 67.31  // base hight 2.65"
#define SHOULDER 146.05 // shoulder-to-elbow "bone" 5.75"
#define ELBOW 276.225   // elbow-to-wrist "bone" 7.375"
#define GRIPPER 85.72   // gripper length 3.94"
// float to long
#define ftl(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define boxHeight 200
// This is the the tolerance for the resistor readings
#define TOLERANCE 0.1
// Define constants for arm
float sh_sq = SHOULDER * SHOULDER;
float el_sq = ELBOW * ELBOW;
float robotX = 0, robotY = 0, robotZ = 0;
//variables for conveyor belt
const int ldrPin = A0;
const int laserPin = 7;
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;



// function prototypes
void runMotor();
char *getArmCode(float, float, float);
void goBackPickup();
void startAndCalibrateArm();
void moveStatusCheck();
void testValue();
void electroMagnet(int);
String listen_for_serial();
String listen_for_serial2();
// Init serial 2
UART Serial2(8, 9, NC, NC);
// Coordinates for boxes
long BoxCoordinatesXY[11][3] = {
    // Row 1
    {0, 260, 1000},  // 1k
    {0, 330, 4700},  // 4.7k
    {0, 390, 10000}, // 10k
    // Row 2
    {80, 270, 20000}, // 20k
    {80, 330, 47000}, // 47k
    {80, 390, 86000}, // 86k
    // Row 3
    {160, 270, 100000}, // 100k
    {160, 330, 180000}, // 180k
    // Row 4
    {240, 270, 470000},   // 470k
    {240, 330, 1000000}, // 1M
    {240,220,1}};        //for values not in range
// Kinematics output string
char output[256] = "";
// ========================================================================
//PROGRAM SETUP AND LOOP
void setup()
{
  Serial.begin(9600); // usb serial for debug
  Serial1.begin(9600);// UART to main board
  Serial2.begin(9600);// UART to ohm meter board
  pinMode(2, OUTPUT);// these pins are for the L298n/Electromagnet
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2, LOW); 
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  //small delay to allow ohm meter processor to boot fully
  delay(3000);
  // send arm to startng position and calibrate
  startAndCalibrateArm();
}
void loop()
{
  // Even though the Arduino main loop is essentially the same as a while(1) loop,
  // I have noticed small bugs where certain functions such as serial will have
  // unpredictable results, such as serial printing twice on startup.
  // So I prefer to run a while(1) within the "loop", which avoids these issues
  goBackPickup();
  moveStatusCheck();
  while (1)
  {
    testValue();
    moveStatusCheck();
    electroMagnet(0);
    delay(500);
    goBackPickup();
    moveStatusCheck();
  }
}
// ========================================================================
// SERIAL FUNCTIONS:
String listen_for_serial()
{
  String message;
  if (Serial1.available() > 0)
  {
    message = Serial1.readString();

    return message;
  }
  return "";
}
String listen_for_serial2()
{
  String message;
  if (Serial2.available() > 0)
  {
    message = Serial2.readString();

    return message;
  }
  return "";
}
// ========================================================================
// ARM FUNCTIONS:
// Startup calibration function
void startAndCalibrateArm()
{

  Serial.println("#0P1425#1P1500#2P1500#3P700T2000\r");  // starting position
  Serial1.println("#0P1425#1P1500#2P1500#3P700T2000\r"); // starting position
  Serial.println("#2PO-100\r");                          // calibrate
  Serial1.println("#2PO-100\r");                         // calibrate
  Serial.println("#3PO-10\r");                          // calibrate
  Serial1.println("#3PO-10\r");                         // calibrate
  // The following delay gives time for the ohm meter board to boot to a state
  // where it is ready to receive serial commands (~2 seconds, but we do 5
  //  just to be sure)
  delay(5000);
}
// Function moves arm up back to start position, down to
// measure, then back up again
void goBackPickup()
{
  Serial.println("#0P1425#1P1500#2P1500#3P700T2000\r"); //starting pos
  Serial1.println("#0P1425#1P1500#2P1500#3P700T2000\r");
  moveStatusCheck();
  Serial.println("#0P1425#1P1243#2P1616#3P940T3000\r"); //testing pos
  Serial1.println("#0P1425#1P1243#2P1616#3P940T3000\r");
}
// checks to see if previous move is complete, if not it waits until it is
void moveStatusCheck()
{
  bool continueLoop = true;
  while (continueLoop)
  {
    Serial.println("Q\r"); // query if movement is complete
    Serial1.print("Q\r"); // query if movement is complete
    delay(50);
    String statusCheck = listen_for_serial();
    if (statusCheck == ".")
    {
      Serial.println("."); // query if movement is complete
      continueLoop = false;
      break;
    }
    else if (statusCheck == "+")
    {
      Serial.println("+"); // query if movement is complete
      continueLoop = true;
      //continue;
    }
  }
}
//Turns on/off electromagnet
void electroMagnet(int onOff)
{
  if (onOff==1){
    digitalWrite(2, LOW); 
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    Serial.println("MAGNET ON");
  }
  if (onOff==0){
    digitalWrite(2, LOW); 
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    Serial.println("MAGNET OFF");
  }
}
// ========================================================================
// OHM METER COMMUNICATION:
void testValue()
{
  bool continueLoop = true;
  Serial.print("TEST\n");
  Serial2.print("TEST\n");
  while (continueLoop)
  {
    //delay(50);
    String checker = listen_for_serial2();

    if (checker != "")
    { 
      Serial.println(checker.toInt());
      bool valueFound = false;
      for (int i=0;i<10;i++){
        if (checker.toInt() >= (BoxCoordinatesXY[i][2] - (BoxCoordinatesXY[i][2] * TOLERANCE)) && checker.toInt() <= (BoxCoordinatesXY[i][2] + (BoxCoordinatesXY[i][2] * TOLERANCE)))
        {
          // Set valueFound to true because the value was found
          valueFound = true;
          electroMagnet(1);

          Serial.println("#0P1425#1P1500#2P1500#3P700T1000\r"); // move arm back up
          Serial1.println("#0P1425#1P1500#2P1500#3P700T1000\r");
          moveStatusCheck();
          Serial.println(getArmCode(BoxCoordinatesXY[i][0], BoxCoordinatesXY[i][1], boxHeight));
          Serial1.println(getArmCode(BoxCoordinatesXY[i][0], BoxCoordinatesXY[i][1], boxHeight));

          // Set continueLoop to false to break the loop
          continueLoop = false;
          break;
        }
      }

      // If the value was not found, skip the rest of the code in the loop and go to the next iteration
      if (!valueFound)
      { 
          electroMagnet(1);
          Serial.println("#0P1425#1P1500#2P1500#3P700T1000\r"); // move arm back up
          Serial1.println("#0P1425#1P1500#2P1500#3P700T1000\r");
          moveStatusCheck();
          Serial.println(getArmCode(BoxCoordinatesXY[10][0], BoxCoordinatesXY[10][1], boxHeight));
          Serial1.println(getArmCode(BoxCoordinatesXY[10][0], BoxCoordinatesXY[10][1], boxHeight));
        continueLoop = false;
      }
    }
  }
}
// ========================================================================
// KINEMATICS:
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


void runMotor(){
    // set direction
    digitalWrite(dirPin, LOW);
  // Begin a loop
  
  bool continueLoop = true;
  while(continueLoop){
    //rotate motor
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(6000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(6000);
    int ldrStatus = analogRead(ldrPin);
    if (ldrStatus < 700)
      { 
        // keep going with the rotation
        continueLoop=true;
      }
      else
      {
        //Serial.println(" ALARM ACTIVATED ");
        // break the loop and ultimately wait for new serial command
        continueLoop = false;
      }
    }
}