#pragma once
#include "EPuckRobot.hpp"

class EPuckBlackRobot : public EPuckRobot{
public:
  enum class State{roam, backward};
  EPuckBlackRobot();
  void run();
protected:
  void backward();
private:
  State mState{};
};
