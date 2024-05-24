#pragma once
#include "Actor.h"

class Player : public Actor {
public:
    Player(int x, int y);
    virtual ~Player();

    void handleInput(WPARAM wParam);
    virtual void update(int deltaTime) override;
    virtual void render(HDC hdc) override;

private:
    int speed;
};
