#include "Shadow.h"

Shadow::Shadow(Vector3 position, Vector3 size)
    : Actor(position, size), isCharging(false), chargeAmount(0.0f), isMoving(false), remainingDistance(0.0f) {
}

void Shadow::OnLButtonDown(const POINT& mousePos, const Camera& camera) {
    isCharging = true;
    chargeAmount = 0.0f;
    startMousePos3D = Unproject2DTo3D(camera, mousePos, 0.0f);
}

void Shadow::OnLButtonUp(const POINT& mousePos, const Camera& camera) {
    if (isCharging) {
        Release(mousePos, camera);
    }
    isCharging = false;
}

void Shadow::OnMouseMove(const POINT& mousePos, const Camera& camera) {
    // 여기에 필요한 추가 로직이 있으면 추가하세요.
}

void Shadow::Charge() {
    if (isCharging) {
        chargeAmount += 1.0f; // 충전 양 증가
    }
}

void Shadow::Release(const POINT& mousePos, const Camera& camera) {
    // 충전된 양에 따라 이동 거리 결정
    float distance = min(chargeAmount * 1.0f, 8.0f);
    Vector3 currentMousePos3D = Unproject2DTo3D(camera, mousePos, 0.0f);
    direction = (currentMousePos3D - startMousePos3D).Normalized();
    targetPos = getPosition() + direction * distance;
    remainingDistance = distance;
    isMoving = true;
    chargeAmount = 0.0f; // 충전 양 초기화
}

void Shadow::Update(float deltaTime) {
    if (isMoving) {
        float moveDistance = min(remainingDistance, 5.0f * deltaTime); // 속도 조절
        move2DPosition(direction.x * moveDistance, direction.z * moveDistance);
        remainingDistance -= moveDistance;
        if (remainingDistance <= 0.0f) {
            isMoving = false;
        }
    }
}