#include "EPuckRobot.hpp"
#include <string>
#include <iostream>
#include <iomanip>

EPuckRobot::EPuckRobot()
  :mLeftMotor{getMotor("left wheel motor")},
   mRightMotor{getMotor("right wheel motor")}
{
    // initialise distance sensors
    const std::array<std::string, N_DIST_SENSORS> psNames {
    "ps0", "ps1", "ps2", "ps3",
    "ps4", "ps5", "ps6", "ps7"
    };
    for (int i {0}; i < N_DIST_SENSORS; ++i) {
      mPs[i] = getDistanceSensor(psNames[i]);
      mPs[i]->enable(TIME_STEP);
    }
    
    //intialise motors
    mLeftMotor->setPosition(INFINITY);
    mRightMotor->setPosition(INFINITY);
    mLeftMotor->setVelocity(0.0);
    mRightMotor->setVelocity(0.0);   
}

void EPuckRobot::roam (){
   //contains all actions
   // read sensors outputs 
   for(int i {0}; i < N_DIST_SENSORS; ++i) {
     mPsValues[i] = mPs[i]->getValue();
   }
   // detect obstacles
    const double obstacleThreshold {80.0};
    mLeftObstacle = {
      mPsValues[0] > obstacleThreshold ||
      mPsValues[1] > obstacleThreshold ||
      mPsValues[2] > obstacleThreshold
    };
    mRightObstacle = {
      mPsValues[5] > obstacleThreshold ||
      mPsValues[6] > obstacleThreshold ||
      mPsValues[7] > obstacleThreshold
    };
    
    // initialize motor speeds at 50% of MAX_SPEED.
    const double speedScale {0.5};
    mLeftSpeed = {speedScale * MAX_SPEED};
    mRightSpeed = {speedScale * MAX_SPEED};
    
    // modify speeds according to obstacles
    if(mLeftObstacle) {
      // turn right
      mLeftSpeed  = speedScale * MAX_SPEED;
      mRightSpeed = -speedScale * MAX_SPEED;
    } else if(mRightObstacle) {
      // turn left
      mLeftSpeed  = -speedScale * MAX_SPEED;
      mRightSpeed = speedScale * MAX_SPEED;
    }
    
    // write actuators inputs
    mLeftMotor->setVelocity(mLeftSpeed);
    mRightMotor->setVelocity(mRightSpeed);
}

void EPuckRobot::report() const{
  
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Time: " << getTime() << ' ';
  std::cout << "DistanceSensors: ";
  for(const auto elem : mPsValues) {
     std::cout << elem << ' ';
  }

  // for debugging, you can print the detection results to the console
  std::cout << "leftObstacle: " << mLeftObstacle << ' '
  << "rightObstacle: " << mRightObstacle << ' ';
      
  // for debugging, you can print the speeds to the console
  std::cout << "leftSpeed: " << mLeftSpeed << ' '
  << "rightSpeed: " << mRightSpeed << std::endl;
}

void EPuckRobot::run(){
  while(step(TIME_STEP) != -1) {
    roam();
    report();
  }
}
