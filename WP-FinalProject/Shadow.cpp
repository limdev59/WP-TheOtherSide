#include "Shadow.h"
#include <cmath>

Shadow::Shadow(Vector3 position, Vector3 size)
    : Actor(position, size), isCharging(false), chargeAmount(0.0f), isMoving(false), remainingDistance(0.0f) {
}

void Shadow::OnLButtonDown(const POINT& mousePos, const Camera& camera) {
    isCharging = true;
    chargeAmount = 0.0f;
    startMousePos = mousePos;
    startMousePos3D = Unproject2DTo3D(camera, mousePos, 0.0f);
}

void Shadow::OnLButtonUp(const POINT& mousePos, const Camera& camera) {
    if (isCharging) {
        Release(mousePos, camera);
    }
    isCharging = false;
}

void Shadow::OnMouseMove(const POINT& mousePos, const Camera& camera) {

}

void Shadow::Charge(float deltaTime) {
    if (isCharging) {
        float chargePerFrame = 12.0f / 1.1f; // Max charge amount corresponds to a maximum distance of 12, fully charged in 3 frames
        chargeAmount += chargePerFrame * deltaTime;
        if (chargeAmount > 12.0f) {
            chargeAmount = 12.0f; // Clamp to max charge amount
        }
    }
}

void Shadow::Release(const POINT& mousePos, const Camera& camera) {
    // Determine the travel distance based on the charge amount
    float distance = min(chargeAmount * 1.0f, 9.0f);
    Vector3 cPos3D = Unproject2DTo3D(camera, mousePos, 0.0f);
    direction = (cPos3D - startMousePos3D).Normalized();
    targetPos = getPosition() + direction * distance;
    remainingDistance = distance;
    isMoving = true;
    chargeAmount = 0.0f; // Reset charge amount
}

void Shadow::Update(float deltaTime) {
    Charge(deltaTime); // Ensure charging is updated each frame

    if (isMoving) {
        // Increase movement speed by using a higher factor (e.g., 10.0f instead of 5.0f)
        float moveDistance = max(min(remainingDistance, 20.0f * deltaTime), 0.0f);
        move2DPosition(direction.x * moveDistance, direction.z * moveDistance);
        remainingDistance -= moveDistance;
        if (remainingDistance <= 0.0f) {
            isMoving = false;
        }
    }
}