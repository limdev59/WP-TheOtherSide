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
    // ���⿡ �ʿ��� �߰� ������ ������ �߰��ϼ���.
}

void Shadow::Charge() {
    if (isCharging) {
        chargeAmount += 1.0f; // ���� �� ����
    }
}

void Shadow::Release(const POINT& mousePos, const Camera& camera) {
    // ������ �翡 ���� �̵� �Ÿ� ����
    float distance = min(chargeAmount * 1.0f, 8.0f);
    Vector3 currentMousePos3D = Unproject2DTo3D(camera, mousePos, 0.0f);
    direction = (currentMousePos3D - startMousePos3D).Normalized();
    targetPos = getPosition() + direction * distance;
    remainingDistance = distance;
    isMoving = true;
    chargeAmount = 0.0f; // ���� �� �ʱ�ȭ
}

void Shadow::Update(float deltaTime) {
    if (isMoving) {
        float moveDistance = min(remainingDistance, 5.0f * deltaTime); // �ӵ� ����
        move2DPosition(direction.x * moveDistance, direction.z * moveDistance);
        remainingDistance -= moveDistance;
        if (remainingDistance <= 0.0f) {
            isMoving = false;
        }
    }
}