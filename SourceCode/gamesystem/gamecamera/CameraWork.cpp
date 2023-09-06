#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include "Player.h"
#include "Helper.h"
#include "TutorialTask.h"
CameraWork::CameraWork(XMFLOAT3 eye, XMFLOAT3 target) {
	m_eyePos = { 0.0f,20.0f,-1.0f };
	m_targetPos = { 0.0f,0.0f,0.0f };
}
void CameraWork::SplineSet() {

}
//XV
void CameraWork::Update(DebugCamera* camera) {
	
	camera->SetEye(m_eyePos);
	camera->SetTarget(m_targetPos);
	camera->Update();
}
void CameraWork::ImGuiDraw() {
	ImGui::Begin("Camera");
	ImGui::Text("Look:%d", m_LookEnemy);
	ImGui::Text("Timer:%d", m_LookTimer);
	ImGui::Text("End:%d", m_LookEnd);
	ImGui::End();
}
//カメラのセット
void CameraWork::SetAfterCamera(const XMFLOAT3& pos) {
	m_AfterEye = pos;
}
//チュートリアルのカメラ
void CameraWork::TutorialUpdate(DebugCamera* camera) {
	const float l_AddFrame = 0.05f;
	if (TutorialTask::GetInstance()->GetMission()) {
		//敵を見る
		if (m_LookEnemy) {
			m_AfterEye = { 0.0f,15.0f,-1.0f };
			m_AfterTarget = { 0.0f,0.0f,0.0f };
			if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				m_LookTimer++;
				if (m_LookTimer == 150) {
					m_LookEnemy = false;
					m_LookTimer = {};
					m_LookEnd = false;
					m_Frame = {};
				}
			}
			m_eyePos = { Ease(In,Cubic,0.5f,m_eyePos.x,m_AfterEye.x),
		Ease(In,Cubic,0.5f,m_eyePos.y,m_AfterEye.y),
		Ease(In,Cubic,0.5f,m_eyePos.z,m_AfterEye.z),
			};

			m_targetPos = { Ease(In,Cubic,0.5f,m_targetPos.x,m_AfterTarget.x),
			Ease(In,Cubic,0.5f,m_targetPos.y,m_AfterTarget.y),
			Ease(In,Cubic,0.5f,m_targetPos.z,m_AfterTarget.z),
			};
		}
		else {		//カメラを戻す
			if (m_LookEnd) {
				m_AfterEye = { 0.0f,20.0f,-1.0f };
				m_AfterTarget = { 0.0f,0.0f,0.0f };
				if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
					m_LookEnd = false;
					m_Frame = {};
				}
			}
			m_eyePos = { Ease(In,Cubic,0.5f,m_eyePos.x,m_AfterEye.x),
		Ease(In,Cubic,0.5f,m_eyePos.y,m_AfterEye.y),
		Ease(In,Cubic,0.5f,m_eyePos.z,m_AfterEye.z),
			};

			m_targetPos = { Ease(In,Cubic,0.5f,m_targetPos.x,m_AfterTarget.x),
			Ease(In,Cubic,0.5f,m_targetPos.y,m_AfterTarget.y),
			Ease(In,Cubic,0.5f,m_targetPos.z,m_AfterTarget.z),
			};
		}
	}
	else {
		if (Player::GetInstance()->GetCameraZoom()) {
			if (Player::GetInstance()->GetMoveState() == MOVE_DOWN) {
				SetAfterCamera({ Player::GetInstance()->GetPosition().x - 1.0f,Player::GetInstance()->GetPosition().y + 5.0f,
					Player::GetInstance()->GetPosition().z + 5.0f });
			}
			else if (Player::GetInstance()->GetMoveState() == MOVE_UP) {
				SetAfterCamera({ Player::GetInstance()->GetPosition().x + 1.0f,Player::GetInstance()->GetPosition().y + 5.0f,
					Player::GetInstance()->GetPosition().z - 5.0f });
			}
			else if (Player::GetInstance()->GetMoveState() == MOVE_RIGHT) {
				SetAfterCamera({ Player::GetInstance()->GetPosition().x - 5.0f,Player::GetInstance()->GetPosition().y + 5.0f,
					Player::GetInstance()->GetPosition().z - 1.0f });
			}
			else if (Player::GetInstance()->GetMoveState() == MOVE_LEFT) {
				SetAfterCamera({ Player::GetInstance()->GetPosition().x + 5.0f,Player::GetInstance()->GetPosition().y + 5.0f,
					Player::GetInstance()->GetPosition().z + 1.0f });
			}
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
	}
	camera->SetEye(m_eyePos);
	camera->SetTarget(m_targetPos);
	camera->Update();
}