#pragma once
#include "Actor.h"
#include "Vector3.h"
#include "Camera.h"
#include <Windows.h>

class Shadow : public Actor {
public:
    Shadow(Vector3 position, Vector3 size);

    // 마우스 조작 관련 메서드
    void OnLButtonDown(const POINT& mousePos, const Camera& camera);
    void OnLButtonUp(const POINT& mousePos, const Camera& camera);
    void OnMouseMove(const POINT& mousePos, const Camera& camera);

    // 이동 및 충전 관련 메서드
    void Charge();
    void Release(const POINT& mousePos, const Camera& camera);
    void Update(float deltaTime);

private:
    bool isCharging;        // 게이지 충전 여부
    float chargeAmount;     // 충전된 게이지 양
    Vector3 startMousePos3D; // 마우스 버튼을 누른 시작 위치 (3D)
    Vector3 direction;      // 이동 방향
    Vector3 targetPos;      // 목표 위치
    bool isMoving;          // 이동 중 여부
    float remainingDistance; // 남은 거리
};
