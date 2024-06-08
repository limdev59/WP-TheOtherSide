//AnimationController.cpp
#include "AnimationController.h"

AnimationController::AnimationController(const std::string& initialState)
    : currentState(initialState) {}

void AnimationController::addState(const std::string& state, const Animation& animation, const std::vector<Transition>& transitions) {
    AnimationState newState;
    newState.animation = animation;
    newState.transitions = transitions;
    states[state] = newState;
}

void AnimationController::update(float deltaTime) {
    if (states.find(currentState) != states.end()) {
        auto& state = states[currentState];
        state.animation.update(deltaTime);

        for (const auto& transition : state.transitions) {
            if (transition.condition()) {
                currentState = transition.targetState;
                break;
            }
        }
    }
}

const CImage* AnimationController::getCurrentFrame() const {
    if (states.find(currentState) != states.end()) {
        return states.at(currentState).animation.getCurrentFrame();
    }
    return nullptr;
}
