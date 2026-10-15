#include "EPuckGreenRobot.hpp"
#include <iostream>

EPuckGreenRobot:: EPuckGreenRobot()
   :mKeyboard{getKeyboard()}
   {
     mKeyboard->enable(TIME_STEP);
}

void EPuckGreenRobot::display() const{
  std::cout <<"[EPuckGreenRobot]: Press and hold t/T and I'll turn myself." << std::endl;
  std::cout <<"[EPuckGreenRobot]: Click b/B and I'll ask EPuckBlackRobot to move backward." <<std::endl;
  std::cout <<"[EPuckGreenRobot]: Click r/R and I'll ask EPuckBlackRobot to roam." << std::endl;
}

void EPuckGreenRobot::turn() {
  const double speedScale {0.5};
  mLeftSpeed  = speedScale * MAX_SPEED;
  mRightSpeed = -speedScale * MAX_SPEED;
  mLeftMotor->setVelocity(mLeftSpeed);
  mRightMotor->setVelocity(mRightSpeed);
}

void EPuckGreenRobot::run() {
  display();
  while(step(TIME_STEP) != -1){
    int k {mKeyboard->getKey()};
    if(k == 'T'){
      //turn right
      turn();
    }else{
      roam();
    }   
  }
}