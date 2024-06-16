#pragma once
#include "Actor.h"
#include "Vector3.h"
#include "Camera.h"
#include <Windows.h>

class Shadow : public Actor {
public:
    Shadow(Vector3 position, Vector3 size);

    // ���콺 ���� ���� �޼���
    void OnLButtonDown(const POINT& mousePos, const Camera& camera);
    void OnLButtonUp(const POINT& mousePos, const Camera& camera);
    void OnMouseMove(const POINT& mousePos, const Camera& camera);

    // �̵� �� ���� ���� �޼���
    void Charge();
    void Release(const POINT& mousePos, const Camera& camera);
    void Update(float deltaTime);

private:
    bool isCharging;        // ������ ���� ����
    float chargeAmount;     // ������ ������ ��
    Vector3 startMousePos3D; // ���콺 ��ư�� ���� ���� ��ġ (3D)
    Vector3 direction;      // �̵� ����
    Vector3 targetPos;      // ��ǥ ��ġ
    bool isMoving;          // �̵� �� ����
    float remainingDistance; // ���� �Ÿ�
};
