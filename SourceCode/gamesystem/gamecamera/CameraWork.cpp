#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include "Player.h"
#include "Helper.h"
CameraWork::CameraWork(XMFLOAT3 eye, XMFLOAT3 target) {
	m_eyePos = { 0.0f,20.0f,-1.0f };
	m_targetPos = { 0.0f,0.0f,0.0f };
}
void CameraWork::SplineSet() {

}
//XV
void CameraWork::Update(DebugCamera* camera) {
	if (Player::GetInstance()->GetCameraZoom()) {
		m_AfterEye.x = Player::GetInstance()->GetPosition().x - 5.0f;
		m_AfterEye.y = Player::GetInstance()->GetPosition().y + 5.0f;
		m_AfterEye.z = Player::GetInstance()->GetPosition().z - 5.0f;

		m_AfterTarget = Player::GetInstance()->GetPosition();
	}
	else {
		m_AfterEye = { 0.0f,20.0f,-1.0f };
		m_AfterTarget = { 0.0f,0.0f,0.0f };
	}

	m_eyePos = { Ease(In,Cubic,0.5f,m_eyePos.x,m_AfterEye.x),
		Ease(In,Cubic,0.5f,m_eyePos.y,m_AfterEye.y),
		Ease(In,Cubic,0.5f,m_eyePos.z,m_AfterEye.z),
	};

	m_targetPos = { Ease(In,Cubic,0.5f,m_targetPos.x,m_AfterTarget.x),
	Ease(In,Cubic,0.5f,m_targetPos.y,m_AfterTarget.y),
	Ease(In,Cubic,0.5f,m_targetPos.z,m_AfterTarget.z),
	};
	camera->SetEye(m_eyePos);
	camera->SetTarget(m_targetPos);
	camera->Update();
}
void CameraWork::ImGuiDraw() {

}