/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Levi                                                      */
/*    Created:      5/14/2025, 5:13:51 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

//This includes programs that help run the and code this program.
#include "vex.h"
#include "menu.h"

//These are the namespaces to program with: vex is the main one, and cchs is for the menu system
using namespace vex;
using namespace cchs;

//This helps define the physical terms, and these come with the premade vex outline
brain Brain;
competition Competition;
controller Controller1 = controller(primary);
menu Menu(Competition, Controller1);

//These are all of the motors we use, what ports they are in, and what gearbox is being used
motor leftMotorA = motor(PORT16, ratio6_1, true);
motor leftMotorB = motor(PORT14, ratio6_1, true);
motor leftMotorC = motor(PORT12, ratio6_1, true);
motor rightMotorA = motor(PORT15, ratio6_1, false);
motor rightMotorB = motor(PORT13, ratio6_1, false);
motor rightMotorC = motor(PORT11, ratio6_1, false);
//These are the groups, to help with programming the move and turn functions
motor_group LeftDrive = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor_group RightDrive = motor_group(rightMotorA, rightMotorB, rightMotorC);
//These motors are for our intake, and tell what way they spin
motor HighIntake = motor(PORT20, ratio18_1, false);
motor LowIntake = motor(PORT10, ratio6_1, false);

//These are what we use for pneumatics and sensors.
//Trapdoor helps us to put the blocks on either the middle or higher level
digital_out TrapDoor = digital_out(Brain.ThreeWirePort.H);
//This helps us take out blocks from the starting posts in the corners
digital_out Wings = digital_out(Brain.ThreeWirePort.G);
//This helps with autonomous, as it registers turning to be more realistic
inertial IMU = inertial(PORT21);
//This is a color sensor, to help detect different colored blocks
optical Eyes = optical(PORT3);

//These are booleans to help with programming
bool toggleTrapDoor = false;
bool toggleWings = false;
bool stopBlock = false;
int blockDelay = 0;

//This is an action for programming, so one button opens and closes the Trapdoor
void TrapDoorToggle (){
  if (Menu.isComplete) {
    toggleTrapDoor = !toggleTrapDoor;
    TrapDoor.set(toggleTrapDoor);
  }
}

//Brings the wings up and down
void wingsToggle (){
  if (Menu.isComplete) {
    toggleWings = !toggleWings;
    Wings.set(toggleWings);
  }
}

//These next voids ones register the events of releasing buttons, and when released, an action ends
void onevent_Controller1ButtonL1_released_0() {
  HighIntake.stop();
  LowIntake.stop();
}

void onevent_Controller1ButtonL2_released_0() {
  HighIntake.stop();
  LowIntake.stop();
}

void onevent_Controller1ButtonR1_released_0() {
  HighIntake.stop();
  LowIntake.stop();
}

void onevent_Controller1ButtonR2_released_0() {
  HighIntake.stop();
  LowIntake.stop();
}

void onevent_Controller1ButtonRight_released_0() {
  HighIntake.stop();
  LowIntake.stop();
}

void onevent_Controller1ButtonDown_released_0() {
  HighIntake.stop();
  LowIntake.stop();
}

//This is the wait, and shows how long something needs to happen before another action
void waitUntilOrTimeout(double t, double timeout = 30, bool greater = false) {
  // @TODO need to pass a condition 
  double start = Brain.Timer.value();
  printf("start move\n");

  if (greater) {
    while (leftMotorA.position(turns) <= t && Brain.Timer.value() - start < timeout) {
      wait(5, msec);
      //leftMotorA.position());
    }
  } else {
    while (leftMotorA.position(turns) >= t && Brain.Timer.value() - start < timeout) {
      wait(5, msec);
    }
  }

}

//This is the function for move, so in the code I can tell it to move a certain amount, and it does
void move(int distance, int speed = 40, int timeout = 5) {
  LeftDrive.setVelocity(speed, percent);
  RightDrive.setVelocity(speed, percent);
  LeftDrive.setTimeout(timeout, seconds);
  RightDrive.setTimeout(timeout, seconds);
  leftMotorA.resetPosition();
  //leftMotorA.setTimeout(3, seconds);
  // calculate turns for distance
  double t = distance / (3.25 * M_PI) * 0.75;
  printf("start move\n");
  if (distance < 0) {
    LeftDrive.spin(reverse);
    RightDrive.spin(reverse);
    // This should be a wait with a time limit to move on:
    waitUntil(leftMotorA.position(turns) <= t);
    //waitUntilOrTimeout(t, 3000, true);
    
  } else {
    LeftDrive.spin(forward);
    RightDrive.spin(forward);
    // This should be a wait with a time limit to move on:
    waitUntil(leftMotorA.position(turns) >= t);
    //waitUntilOrTimeout(t, 3000, false);
  }
  LeftDrive.stop(brake);
  RightDrive.stop(brake);
  printf("end move\n");
}

//This is the function for turn, so that I type in turn then the amount of degrees I need
void turn(int angle, int speed = 20, double timeout = 3) {
  IMU.resetRotation();
  double start = Brain.Timer.value();
  printf("start turn\n");
  while (fabs(IMU.rotation(degrees)) < fabs(angle) && Brain.Timer.value() - start < timeout) {
    if (angle < 0) {
      // turn counterclockwise
      LeftDrive.spin(reverse, speed, percent);
      RightDrive.spin(forward, speed, percent);
    } else {
      // turn clockwise
      LeftDrive.spin(forward, speed, percent);
      RightDrive.spin(reverse, speed, percent);
    }
  }
  LeftDrive.stop(brake);
  RightDrive.stop(brake);
  printf("end turn\n");
}

//Lines 172-193 define what a block is for the color sensor
enum block {
  NONE,
  RED,
  BLUE
};

block Block = block::NONE;

void blockDetect() {
  while (true) {
    Block = block::NONE;
    if (Eyes.isNearObject()) {
      color c = Eyes.color();
      if (c == color::red) {
        Block = block::RED;
      } else if (c == color::blue) {
        Block = block::BLUE;
      }
    }
    this_thread::sleep_for(50);
  }
}

//This is for the menu, so the menu loads before the autonomous
void auton() {
  Menu.currentAuton();
}

void autonomous1(void) {
 move(10);
 turn(90);
}

void autonomous2(void) {

}

void autonomous3(void) {

}

void autonomous4(void) {

}

void autonomous5(void) {

}

//This is for the menu, so we can register which autonomous to choose before a match begins
void pre_auton(void) {
  waitUntil(IMU.isCalibrating() == true);
  Menu.registerAuton("BlueLeft", autonomous1);
  Menu.registerAuton("BlueRight", autonomous2);
  Menu.registerAuton("RedLeft", autonomous3);
  Menu.registerAuton("RedRight", autonomous4);
  Menu.registerAuton("Skills", autonomous5);
}

//This is the controls that define which button presses do which functions
void usercontrol(void) {
  while (!Menu.isComplete) {
    wait(20, msec);
  }
  if (Menu.autonMode) {
    Menu.currentAuton();
    return;
  }
  while (1) {
    if (Controller1.ButtonL2.pressing()) {
      HighIntake.spin(forward);
      LowIntake.spin(forward);
    } else if (Controller1.ButtonL1.pressing()) {
      HighIntake.spin(reverse);
      LowIntake.spin(reverse);
      if ((Block == block::BLUE && (Menu.currentAuton == &autonomous3 || Menu.currentAuton == &autonomous4))
        || (Block == block::RED && (Menu.currentAuton == &autonomous1 || Menu.currentAuton == &autonomous2))) {
        wait(240, msec);
        HighIntake.stop();
        printf("fast!\n");
        wait(500, msec);
        printf("slow.\n");
      }
    }

    if (Controller1.ButtonRight.pressing()) {
      HighIntake.setVelocity(19, percent); //8
      HighIntake.spin(reverse);
      LowIntake.spin(forward);
    } else if (Controller1.ButtonDown.pressing()) {
      if ((Block == block::RED && (Menu.currentAuton == &autonomous3 || Menu.currentAuton == &autonomous4 || Menu.currentAuton == &autonomous5))
      || (Block == block::BLUE && (Menu.currentAuton == &autonomous1 || Menu.currentAuton == &autonomous2))) {
        stopBlock = true;
      } else {
        if (stopBlock) {
          blockDelay++;
          if (blockDelay > 1) {//4
            HighIntake.stop();
          }
        } else {
          HighIntake.setVelocity(19, percent);
          HighIntake.spin(forward);
          LowIntake.spin(forward);
          blockDelay = 0;
        }
      }
    }

    if (Block == block::RED) {
      Brain.Screen.clearScreen(color::red);
    } else if (Block == block::BLUE) {
      Brain.Screen.clearScreen(color::blue);
    } else {
      Brain.Screen.clearScreen();
    }
    // calculate the drivetrain motor velocities from the controller joystick axies
    // left = Axis3 + Axis1
    // right = Axis3 - Axis1
    //NOTE TO SELF IMPORTANT, the plus and minus signs are the side of turning, number is percent of turn speed
    int speedLeft = Controller1.Axis3.position() + Controller1.Axis1.position() * 0.5;
    int speedRight = Controller1.Axis3.position() - Controller1.Axis1.position() * 0.5;
    // check if the value is inside of the deadband range
    if (abs(speedLeft) < 3) speedLeft = 0;
    if (abs(speedRight) < 3) speedRight = 0;
    LeftDrive.spin(forward, speedLeft, percent);
    RightDrive.spin(forward, speedRight, percent);
    wait(20, msec);
  }
}

void setBlock() {
  if (Menu.isComplete) {
    LowIntake.spin(forward);
    HighIntake.spin(reverse, 30, percent);
    while (Block == block::NONE) {
    }
    if ((Block == block::BLUE && (Menu.currentAuton == &autonomous3 || Menu.currentAuton == &autonomous4))
      || (Block == block::RED && (Menu.currentAuton == &autonomous1 || Menu.currentAuton == &autonomous2))) {
      HighIntake.setVelocity(60, percent);
      wait(500, msec);
    }
    wait(290, msec);
    HighIntake.setVelocity(35, percent);
    HighIntake.stop();
  }
}

int main() {
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);
  pre_auton();

//These register either the single button presses for pneumatics, or releasing buttons for the intake
  Controller1.ButtonL1.released(onevent_Controller1ButtonL1_released_0);
  Controller1.ButtonL2.released(onevent_Controller1ButtonL2_released_0);
  Controller1.ButtonR1.released(onevent_Controller1ButtonR1_released_0);
  Controller1.ButtonR2.released(onevent_Controller1ButtonR2_released_0);
  Controller1.ButtonB.pressed(TrapDoorToggle);
  Controller1.ButtonRight.released(onevent_Controller1ButtonRight_released_0);
  Controller1.ButtonDown.released(onevent_Controller1ButtonDown_released_0);
  // Controller1.ButtonA.pressed(setBlock);
  HighIntake.setVelocity(100, percent);
  LowIntake.setVelocity(100, percent);
  Eyes.setLight(ledState::on);
  Eyes.integrationTime(50);
  Eyes.objectDetectThreshold(150);
  thread block(blockDetect);

  while (true) {
    wait(100, msec);
  }
}