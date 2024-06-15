#pragma once
#include "Actor.h"
class Shadow : public Actor
{
public:
	Shadow();
    
    void setLeftDown(bool leftInput);
    bool getLeftDown();
    void setRightDown(bool rightInput);
    bool getRightDown();

private:
    bool leftDown;
    bool rightDown;
};