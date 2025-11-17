/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Levi                                                      */
/*    Created:      5/14/2025, 5:13:51 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// This is the main (main.cpp) file, where all of the main programming is done (ie: programming for autonomous and driver control, defining motors and sensors, etc.)

// This includes programs that help run and code this program.
#include "vex.h"
#include "menu.h"

// These are the namespaces to program with: vex is the main one, and cchs is for the menu system
using namespace vex;
using namespace cchs;

// This helps define the physical terms, and these come with the premade vex outline
brain Brain;
competition Competition;
controller Controller1 = controller(primary);
menu Menu(Competition, Controller1);

// These are all of the motors for the drivetrain, what ports they are in, and what gearbox is being used
motor leftMotorA = motor(PORT7, ratio6_1, true);
motor leftMotorB = motor(PORT4, ratio6_1, true);
motor leftMotorC = motor(PORT6, ratio6_1, true);
motor rightMotorA = motor(PORT1, ratio6_1, false);
motor rightMotorB = motor(PORT3, ratio6_1, false);
motor rightMotorC = motor(PORT5, ratio6_1, false);

// These are the groups, to help with programming the move and turn functions
motor_group LeftDrive = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor_group RightDrive = motor_group(rightMotorA, rightMotorB, rightMotorC);
// These motors are for our intake, and tell what way they spin
motor IntakeA = motor(PORT8, ratio6_1, true);
motor IntakeB = motor(PORT9, ratio18_1, false);
motor IntakeC = motor(PORT10, ratio18_1, true);

// These are what we use for pneumatics and sensors.
// Stopper helps us to either hold th blocks in the intake, or release them
digital_out Stopper = digital_out(Brain.ThreeWirePort.H);
// This helps us take out blocks from the starting posts in the corners
digital_out Scraper = digital_out(Brain.ThreeWirePort.A);
// This helps us to descore the long goals that have opponent blocks on them
digital_out Descore = digital_out(Brain.ThreeWirePort.G);
// This helps with autonomous, as it registers turning to be more realistic
inertial IMU1 = inertial(PORT19);
// This is a color sensor, to help detect different colored blocks
optical Eyes = optical(PORT3);

// These are booleans to help with programming
bool toggleStopper = false;
bool toggleScraper = false;
bool toggleDescore = false;
bool stopBlock = false;
int blockDelay = 0;

// This is an action for programming, so one button opens and closes the Stopper
void StopperToggle()
{
  if (Menu.isComplete)
  {
    toggleStopper = !toggleStopper;
    Stopper.set(toggleStopper);
  }
}

// Brings the Scraper up and down
void ScraperToggle()
{
  if (Menu.isComplete)
  {
    toggleScraper = !toggleScraper;
    Scraper.set(toggleScraper);
  }
}

// Brings the Scraper up and down
void DescoreToggle()
{
  if (Menu.isComplete)
  {
    toggleDescore = !toggleDescore;
    Descore.set(toggleDescore);
  }
}

// These next voids ones register the events of releasing buttons, and when released, an action ends
void onevent_Controller1ButtonL1_released_0()
{
  IntakeA.stop();
  IntakeB.stop();
  IntakeC.stop();
}

void onevent_Controller1ButtonL2_released_0()
{
  IntakeA.stop();
  IntakeB.stop();
  IntakeC.stop();
}

void onevent_Controller1ButtonR1_released_0()
{
  IntakeA.stop();
  IntakeB.stop();
  IntakeC.stop();
}

void onevent_Controller1ButtonR2_released_0()
{
  IntakeA.stop();
  IntakeB.stop();
  IntakeC.stop();
}

void onevent_Controller1ButtonRight_released_0()
{
  IntakeA.stop();
  IntakeB.stop();
  IntakeC.stop();
}

void onevent_Controller1ButtonDown_released_0()
{
  IntakeA.stop();
  IntakeB.stop();
  IntakeC.stop();
}

// This is the wait, and shows how long something needs to happen before another action
void waitUntilOrTimeout(double t, double timeout = 30, bool greater = false)
{
  // @TODO need to pass a condition
  double start = Brain.Timer.value();
  printf("start move\n");

  if (greater)
  {
    while (leftMotorA.position(turns) <= t && Brain.Timer.value() - start < timeout)
    {
      wait(5, msec);
      // leftMotorA.position());
    }
  }
  else
  {
    while (leftMotorA.position(turns) >= t && Brain.Timer.value() - start < timeout)
    {
      wait(5, msec);
    }
  }
}

// This is the function for move, so in the code I can tell it to move a certain amount, and it does
void move(int distance, int speed = 40, int timeout = 5)
{
  LeftDrive.setVelocity(speed, percent);
  RightDrive.setVelocity(speed, percent);
  LeftDrive.setTimeout(timeout, seconds);
  RightDrive.setTimeout(timeout, seconds);
  leftMotorA.resetPosition();
  // leftMotorA.setTimeout(3, seconds);
  //  calculate turns for distance
  double t = distance / (3.25 * M_PI) * 0.75;
  printf("start move\n");
  if (distance < 0)
  {
    LeftDrive.spin(reverse);
    RightDrive.spin(reverse);
    // This should be a wait with a time limit to move on:
    waitUntil(leftMotorA.position(turns) <= t);
    // waitUntilOrTimeout(t, 3000, true);
  }
  else
  {
    LeftDrive.spin(forward);
    RightDrive.spin(forward);
    // This should be a wait with a time limit to move on:
    waitUntil(leftMotorA.position(turns) >= t);
    // waitUntilOrTimeout(t, 3000, false);
  }
  LeftDrive.stop(brake);
  RightDrive.stop(brake);
  printf("end move\n");
}

// This is the function for turn, so that I type in turn then the amount of degrees I need
void turn(int angle, int speed = 20, double timeout = 3)
{
  {
    IMU1.resetRotation();
    double start = Brain.Timer.value();
    printf("start turn\n");
    while (fabs(IMU1.rotation(degrees)) < fabs(angle) && Brain.Timer.value() - start < timeout)
    {
      if (angle < 0)
      {
        // turn counterclockwise
        LeftDrive.spin(reverse, speed, percent);
        RightDrive.spin(forward, speed, percent);
      }
      else
      {
        // turn clockwise
        LeftDrive.spin(forward, speed, percent);
        RightDrive.spin(reverse, speed, percent);
      }
    }
    LeftDrive.stop(brake);
    RightDrive.stop(brake);
    printf("end turn\n");
  }
}

// Lines 172-193 define what a block is for the color sensor
enum block
{
  NONE,
  RED,
  BLUE
};

block Block = block::NONE;

void blockDetect()
{
  while (true)
  {
    Block = block::NONE;
    if (Eyes.isNearObject())
    {
      color c = Eyes.color();
      if (c == color::red)
      {
        Block = block::RED;
      }
      else if (c == color::blue)
      {
        Block = block::BLUE;
      }
    }
    this_thread::sleep_for(50);
  }
}

// This is for the menu, so the menu loads before the autonomous
void auton()
{
  Menu.currentAuton();
}

void autonomous1(void)
{
  // This is the code for our autonomous in the left corner of the blue alliance
  // It already has the matchload, and spins to score in the middle top goal
  IntakeA.spin(forward, 70, percent);
  IntakeB.spin(reverse, 70, percent);
  IntakeC.spin(forward, 60, percent);
  // It moves toward the three blocks, and intakes them
  move(29.25);
  wait(.25, sec);
  // The scraper comes down to get the third block in the intake
  ScraperToggle();
  move(6.3); 
  // It turns to face the middle top goal
  turn(-92);
  // It moves backward to the middle top goal, and scores the blocks
  move(-27.1, 15);
  // The Stopper releases so the blocks can fall into the goal
  StopperToggle();
  wait(1.75, sec);
  // The intake now spins in the direction to score in the long goal
  move(76.3);
  // The Stopper comes back so the blocks do not fall out
  IntakeC.spin(reverse, 100, percent);
  // It moves to the area between the matchloading part and the long goal
  StopperToggle();
  // It turns to face the matchloading part
  turn(-17.3);
  // It moves to the matchloading part, and takes out three more blocks
  move(20.8, 15);
  wait(1, sec);
  // It moves backwards to the long goal
  move(-10);
  //turn(-0.35);
  move(-36);
  // The Stopper releases so the blocks can fall into the goal
  StopperToggle();
  if (Block == block::RED)
  {
    StopperToggle();
  }
  // IMPORTANT, NEGATIVE TURNS ARE LEFT, POSITIVE TURNS ARE RIGHT
}

void autonomous2(void)
{
  // This is the code for our autonomous in the right corner of the blue alliance
  // It already has the matchload, and spins to score in the top goal
  IntakeA.spin(forward);
  IntakeB.spin(reverse);
  IntakeC.spin(reverse);
  // It moves toward the three blocks, and intakes them
  move(28.5); //29
  wait(.25, sec);
  // The scraper comes down to get the third block in the intake
  ScraperToggle();
  move(4.2); 
  // It turns to face the middle bottom goal
  turn(-42);
  // The scraper lifts up to be ready to score
  ScraperToggle();
  // It moves forward to the middle bottom goal, and scores the blocks
  move(23, 15); 
  // It reverses the intake direction to outtake the blocks into the goal
  IntakeA.spin(reverse, 70, percent);
  IntakeB.spin(forward, 70, percent);
  wait(1, sec);
  // It moves backward to between the matchloading part and the long goal
  IntakeA.spin(forward, 100, percent);
  IntakeB.spin(reverse, 100, percent);
  move(-77.5);
  ScraperToggle();
  turn(-115);
  move(15.8, 20);
  wait(0.8, sec);
  // It moves backwards to the long goal
  move(-16);
  turn(0.2);
  move(-30);
  // The Stopper releases so the blocks can fall into the goal
  StopperToggle();
  if (Block == block::RED)
  {
    StopperToggle();
  }
}

void autonomous3(void)
{
  // This is the code for our autonomous in the left corner of the blue alliance
  // It already has the matchload, and spins to score in the middle top goal
  IntakeA.spin(forward, 70, percent);
  IntakeB.spin(reverse, 70, percent);
  IntakeC.spin(forward, 60, percent);
  // It moves toward the three blocks, and intakes them
  move(29.25);
  wait(.25, sec);
  // The scraper comes down to get the third block in the intake
  ScraperToggle();
  move(6.3); 
  // It turns to face the middle top goal
  turn(-92);
  // It moves backward to the middle top goal, and scores the blocks
  move(-27.1, 15);
  // The Stopper releases so the blocks can fall into the goal
  StopperToggle();
  wait(1.75, sec);
  // The intake now spins in the direction to score in the long goal
  move(76.3);
  // The Stopper comes back so the blocks do not fall out
  IntakeC.spin(reverse, 100, percent);
  // It moves to the area between the matchloading part and the long goal
  StopperToggle();
  // It turns to face the matchloading part
  turn(-17.3);
  // It moves to the matchloading part, and takes out three more blocks
  move(20.8, 15);
  wait(1, sec);
  // It moves backwards to the long goal
  move(-10);
  //turn(-0.35);
  move(-36);
  // The Stopper releases so the blocks can fall into the goal
  StopperToggle();
  if (Block == block::BLUE)
  {
    StopperToggle();
  }
  // IMPORTANT, NEGATIVE TURNS ARE LEFT, POSITIVE TURNS ARE RIGHT
}

void autonomous4(void)
{
  // This is the code for our autonomous in the right corner of the blue alliance
  // It already has the matchload, and spins to score in the top goal
  IntakeA.spin(forward);
  IntakeB.spin(reverse);
  IntakeC.spin(reverse);
  // It moves toward the three blocks, and intakes them
  move(28.5); //29
  wait(.25, sec);
  // The scraper comes down to get the third block in the intake
  ScraperToggle();
  move(4.2); 
  // It turns to face the middle bottom goal
  turn(-42);
  // The scraper lifts up to be ready to score
  ScraperToggle();
  // It moves forward to the middle bottom goal, and scores the blocks
  move(23, 15); 
  // It reverses the intake direction to outtake the blocks into the goal
  IntakeA.spin(reverse, 70, percent);
  IntakeB.spin(forward, 70, percent);
  wait(1, sec);
  // It moves backward to between the matchloading part and the long goal
  IntakeA.spin(forward, 100, percent);
  IntakeB.spin(reverse, 100, percent);
  move(-77.5);
  ScraperToggle();
  turn(-115);
  move(15.8, 20);
  wait(0.8, sec);
  // It moves backwards to the long goal
  move(-16);
  turn(0.2);
  move(-30);
  // The Stopper releases so the blocks can fall into the goal
  StopperToggle();
  if (Block == block::BLUE)
  {
    StopperToggle();
  }
}

void autonomous5(void)
{
  // This is the code for our skills autonomous
  IntakeA.spin(forward);
  IntakeB.spin(reverse);
  IntakeC.spin(reverse);
  ScraperToggle();
  move(48);
  turn(69);
  move(18, 15);
  wait(1.4, sec);
  move(-46);
  StopperToggle();
  wait(2, sec);
  move(5);
  ScraperToggle();
  turn(69);
  move(45);
  turn(78);
  StopperToggle();
}


// This is for the menu, so we can register which autonomous to choose before a match begins
// It also shows on the screen which autonomous we want to select in easier terms to understand
void pre_auton(void)
{
  waitUntil(IMU1.isCalibrating() == true);
  Menu.registerAuton("Blue Left Corner", autonomous1);
  Menu.registerAuton("Blue Right Corner", autonomous2);
  Menu.registerAuton("Red Left Corner", autonomous3);
  Menu.registerAuton("Red Right Corner", autonomous4);
  Menu.registerAuton("Skills", autonomous5);
}

// This is the controls that define which button presses do which functions
void usercontrol(void)
{
  while (!Menu.isComplete)
  {
    wait(20, msec);
  }
  if (Menu.autonMode)
  {
    Menu.currentAuton();
    return;
  }
  while (1)
  {
    if (Controller1.ButtonL2.pressing())
    {
      IntakeA.spin(reverse, 100, percent);
      IntakeB.spin(forward, 100, percent);
      IntakeC.spin(forward, 100, percent);
    }
    else if (Controller1.ButtonL1.pressing())
    {
      IntakeA.spin(forward, 100, percent);
      IntakeB.spin(reverse, 100, percent);
      IntakeC.spin(reverse, 100, percent);
      // if ((Block == block::BLUE && (Menu.currentAuton == &autonomous3 || Menu.currentAuton == &autonomous4))
      //   || (Block == block::RED && (Menu.currentAuton == &autonomous1 || Menu.currentAuton == &autonomous2))) {
      //   wait(240, msec);
      //   IntakeA.stop();
      //   printf("fast!\n");
      //   wait(500, msec);
      //   printf("slow.\n");
      // }
    }
    if (Controller1.ButtonR1.pressing())
    {
      IntakeA.spin(forward, 60, percent);
      IntakeB.spin(reverse, 60, percent);
      IntakeC.spin(forward, 60, percent);
    }
    else if (Controller1.ButtonR2.pressing())
    {
    }
    // if (Controller1.ButtonDown.pressing()) {

    // } else if (Controller1.ButtonDown.pressing()) {
    //   if ((Block == block::RED && (Menu.currentAuton == &autonomous3 || Menu.currentAuton == &autonomous4 || Menu.currentAuton == &autonomous5))
    //   || (Block == block::BLUE && (Menu.currentAuton == &autonomous1 || Menu.currentAuton == &autonomous2))) {
    //     stopBlock = true;
    //   } else {
    //     if (stopBlock) {
    //       blockDelay++;
    //       if (blockDelay > 1) {//4
    //         HighIntake.stop();
    //       }
    //     } else {
    //       HighIntake.setVelocity(19, percent);
    //       HighIntake.spin(forward);
    //       LowIntake.spin(forward);
    //       blockDelay = 0;
    //     }
    //   }
    // }

    // if (Block == block::RED) {
    //   Brain.Screen.clearScreen(color::red);
    // } else if (Block == block::BLUE) {
    //   Brain.Screen.clearScreen(color::blue);
    // } else {
    //   Brain.Screen.clearScreen();
    // }

    // calculate the drivetrain motor velocities from the controller joystick axies
    // left = Axis3 + Axis1;
    // right = Axis3 - Axis1;
    // NOTE TO SELF IMPORTANT, the plus and minus signs are the side of turning, number is percent of turn speed
    int speedLeft = Controller1.Axis3.position() + Controller1.Axis1.position() * 0.4;
    int speedRight = Controller1.Axis3.position() - Controller1.Axis1.position() * 0.4;
    // check if the value is inside of the deadband range
    if (abs(speedLeft) < 3)
      speedLeft = 0;
    if (abs(speedRight) < 3)
      speedRight = 0;
    LeftDrive.spin(forward, speedLeft, percent);
    RightDrive.spin(forward, speedRight, percent);
    wait(20, msec);
  }
}

// void setBlock() {
//   if (Menu.isComplete) {
//     LowIntake.spin(forward);
//     HighIntake.spin(reverse, 30, percent);
//     while (Block == block::NONE) {
//     }
//     if ((Block == block::BLUE && (Menu.currentAuton == &autonomous3 || Menu.currentAuton == &autonomous4))
//       || (Block == block::RED && (Menu.currentAuton == &autonomous1 || Menu.currentAuton == &autonomous2))) {
//       HighIntake.setVelocity(60, percent);
//       wait(500, msec);
//     }
//     wait(290, msec);
//     HighIntake.setVelocity(35, percent);
//     HighIntake.stop();
//   }
// }

int main()
{
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  // These register either the single button presses for pneumatics, or releasing buttons for other motors
  Controller1.ButtonL1.released(onevent_Controller1ButtonL1_released_0);
  Controller1.ButtonL2.released(onevent_Controller1ButtonL2_released_0);
  Controller1.ButtonR1.released(onevent_Controller1ButtonR1_released_0);
  Controller1.ButtonR2.released(onevent_Controller1ButtonR2_released_0);
  Controller1.ButtonR2.pressed(StopperToggle);
  Controller1.ButtonB.pressed(ScraperToggle);
  Controller1.ButtonDown.pressed(DescoreToggle);

  // Controller1.ButtonA.pressed(setBlock);

  // These set the speeds of the motors, and the settings of the color sensor
  IntakeA.setVelocity(100, percent);
  IntakeB.setVelocity(100, percent);
  IntakeC.setVelocity(100, percent);
  leftMotorA.setVelocity(96, percent); //96
  leftMotorB.setVelocity(96, percent); //96
  leftMotorC.setVelocity(96, percent); //96
  rightMotorA.setVelocity(100, percent);
  rightMotorB.setVelocity(100, percent);
  rightMotorC.setVelocity(100, percent);
  Eyes.setLight(ledState::on);
  Eyes.integrationTime(50);
  Eyes.objectDetectThreshold(150);
  thread block(blockDetect);

  while (true)
  {
    wait(100, msec);
  }
}