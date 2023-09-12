#include "SceneChanger.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Audio.h"

SceneChanger* SceneChanger::GetInstance() {
	static SceneChanger instance;

	return &instance;
}

void SceneChanger::Initialize() {
	left_door = IKESprite::Create(ImageManager::LEFTDOOR, {});
	left_door->SetSize({ 640.0f,720.0f });

	right_door = IKESprite::Create(ImageManager::RIGHTDOOR, {});
	right_door->SetSize({ 640.0f,720.0f });

	m_ChangeStart = false;
	m_Position[DOOR_LEFT] = { -640.0f,0.0f };
	m_Position[DOOR_RIGHT] = { 1280.0f,0.0f };


	m_AfterPos[DOOR_LEFT] = { 0.0f,0.0f };
	m_AfterPos[DOOR_RIGHT] = { 640.0f,0.0f };
}
void SceneChanger::Update() {
	const float l_AddFrame = 0.01f;
	if (m_ChangeStart) {
		if (_doorState == OPEN_DOOR) {
			m_AfterPos[DOOR_LEFT] = { 0.0f,0.0f };
			m_AfterPos[DOOR_RIGHT] = { 640.0f,0.0f };
			if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				m_ChangeTimer++;
				if (m_ChangeTimer == 5) {
					m_Change = true;
					_doorState = CLOSE_DOOR;
					m_Frame = {};
					m_ChangeTimer = {};
					Audio::GetInstance()->PlayWave("Resources/audio/close.wav", 0.2f);
				}
			}
		}
		else {
			m_AfterPos[DOOR_LEFT] = { -640.0f,0.0f };
			m_AfterPos[DOOR_RIGHT] = { 1280.0f,0.0f };
			if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				m_Frame = {};
				m_ChangeStart = false;
				_doorState = OPEN_DOOR;
			}
		}

		m_Position[DOOR_LEFT].x = Ease(In, Cubic, m_Frame, m_Position[DOOR_LEFT].x, m_AfterPos[DOOR_LEFT].x);
		m_Position[DOOR_RIGHT].x = Ease(In, Cubic, m_Frame, m_Position[DOOR_RIGHT].x, m_AfterPos[DOOR_RIGHT].x);
	}

	left_door->SetPosition(m_Position[DOOR_LEFT]);
	right_door->SetPosition(m_Position[DOOR_RIGHT]);
}
void SceneChanger::InitializeOver() {
}
void SceneChanger::Draw() {
	left_door->Draw();
	right_door->Draw();
}

void SceneChanger::SetChangeStart(const bool ChangeStart)
{
	if (m_ChangeStart == false)
	{
		Audio::GetInstance()->PlayWave("Resources/audio/open.wav", 0.5f);
	}

	m_ChangeStart = ChangeStart;
}
