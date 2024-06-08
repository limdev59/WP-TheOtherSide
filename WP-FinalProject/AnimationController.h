//AnimationController.h
#pragma once

#include "Animation.h"
#include <map>
#include <string>
#include <vector>
#include <functional>

class AnimationController {
public:
    struct Transition {
        std::string targetState;
        std::function<bool()> condition;
    };

    AnimationController(const std::string& initialState);

    void addState(const std::string& state, const Animation& animation, const std::vector<Transition>& transitions);
    void update(float deltaTime);
    const CImage* getCurrentFrame() const;

private:
    struct AnimationState {
        Animation animation;
        std::vector<Transition> transitions;
        AnimationState() {}
    };

    std::map<std::string, AnimationState> states;
    std::string currentState;
};
