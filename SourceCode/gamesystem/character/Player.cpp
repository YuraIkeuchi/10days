#include "Player.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Easing.h"
#include "Collision.h"
#include "Slow.h"
#include "ParticleEmitter.h"
#include "TutorialTask.h"
#include "ScoreManager.h"
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}
//リソース読み込み
void Player::LoadResource() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetScale({ 2.f,2.f,2.f });
	m_Object->SetPosition({ 0.0f,0.0f,0.0f });
	m_Object->VertexCheck();
}
//初期化
bool Player::Initialize()
{
	LoadCSV();
	//CSV読み込み
	return true;
}
//CSV読み込み
void Player::LoadCSV() {
	m_BaseSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed")));
}
//ステータスの初期化
void Player::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//移動処理用
	velocity /= 5.0f;

	_MoveState = MOVE_UP;

	//移動加算値
	m_AddSpeed = 0.0f;
	
	m_Frame = {};
	m_AfterPosZ = {};
	m_AfterPosX = {};

	m_ChangeLine = false;
	m_CameraZoom = false;
	m_MoveTimer = {};

	m_Attack = false;

	_charaState = STATE_MOVE;
}
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Move,//移動
	&Player::Attack,//攻撃
};
//更新処理
void Player::Update()
{
	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();

	Obj_SetParam();
	m_MoveTimer = {};
}
//VECTOR
XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}
//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	Obj_Draw();
}

//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("POSZ%f", m_Position.z);
	ImGui::Text("Timer:%d", m_MoveTimer);
	ImGui::End();
}

//移動
void Player::Move() {
	Input* input = Input::GetInstance();
	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float STICK_MAX = 32768.0f;

	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		//右入力
		if (_MoveState == MOVE_UP || _MoveState == MOVE_DOWN) {
			//右入力
			if (input->TiltPushStick(Input::L_RIGHT, 0.0f) && (m_Position.x < 9.5f)) {
				m_AddSpeed = m_BaseSpeed;
				m_MoveTimer++;
			}
			//左入力
			else if (input->TiltPushStick(Input::L_LEFT, 0.0f) && (m_Position.x > -9.5f)) {
				m_AddSpeed = -m_BaseSpeed;
				m_MoveTimer++;
			}
			//入力なし
			else {
				m_AddSpeed = {};
			}

			if (Helper::GetInstance()->CheckMax(m_Position.x, -9.5f, m_AddSpeed)) {
				m_ChangeLine = true;
				_MoveState = MOVE_LEFT;
			}
			else if (Helper::GetInstance()->CheckMin(m_Position.x, 9.5f, m_AddSpeed)) {
				m_ChangeLine = true;
				_MoveState = MOVE_RIGHT;
			}
		}
		else {
			//上入力
			if (input->TiltPushStick(Input::L_UP, 0.0f) && (m_Position.z < 8.0f)) {
				m_AddSpeed = m_BaseSpeed;
				m_MoveTimer++;
			}
			//下入力
			else if (input->TiltPushStick(Input::L_DOWN, 0.0f) && (m_Position.z > -8.0f)) {
				m_AddSpeed = -m_BaseSpeed;
				m_MoveTimer++;
			}
			//入力なし
			else {
				m_AddSpeed = {};
			}

			//チュートリアルのタスクのときだけ移動できない
			if (!TutorialTask::GetInstance()->GetMission()) {
				if (Helper::GetInstance()->CheckMax(m_Position.z, -8.0f, m_AddSpeed)) {
					m_ChangeLine = true;
					_MoveState = MOVE_DOWN;
				}
				else if (Helper::GetInstance()->CheckMin(m_Position.z, 8.0f, m_AddSpeed)) {
					m_ChangeLine = true;
					_MoveState = MOVE_UP;
				}
			}
		}
	}

	if ((input->TriggerButton(input->A))) {
		_charaState = STATE_ATTACK;
		m_Attack = true;
		m_Frame = {};
		m_CameraZoom = true;
		if (_MoveState == MOVE_UP || _MoveState == MOVE_DOWN) {
			m_AfterPosZ = m_Position.z * -1.0f;
			if (_MoveState == MOVE_UP) {
				_MoveState = MOVE_DOWN;
			}
			else {
				_MoveState = MOVE_UP;
			}
		}
		else {
			m_AfterPosX = m_Position.x * -1.0f;
			if (_MoveState == MOVE_RIGHT) {
				_MoveState = MOVE_LEFT;
			}
			else {
				_MoveState = MOVE_RIGHT;
			}
		}
	}
	if (_MoveState == MOVE_LEFT) {
		m_Rotation = { 0.0f,90.0f,0.0f };
	}
	else if (_MoveState == MOVE_RIGHT) {
		m_Rotation = { 0.0f,270.0f,0.0f };
	}
	else if (_MoveState == MOVE_UP) {
		m_Rotation = { 0.0f, 180.0f, 0.0f };
	}
	else {
		m_Rotation = { 0.0f, 0.0f, 0.0f };
	}
	//Helper::GetInstance()->Clamp(m_Position.x, -9.5f, 9.5f);
}
//攻撃
void Player::Attack() {
	const float l_AddFrame = 0.01f;
	m_AddFrame = l_AddFrame;
	if (!Slow::GetInstance()->GetSlow()) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, m_AddFrame)) {
			m_Frame = {};
			_charaState = STATE_MOVE;
			m_Attack = false;
			ScoreManager::GetInstance()->SetMagnification(0);
		}
	}
	
	if (_MoveState == MOVE_UP || _MoveState == MOVE_DOWN) {
		float oldZ = m_Position.z;
		m_Position.z = Ease(In, Cubic, m_Frame * Slow::GetInstance()->GetPlayerSlowPower(), m_Position.z, m_AfterPosZ);
		m_playerVec = {0, 0, m_Position.z - oldZ};
	}
	else {
		float oldX = m_Position.x;
		m_Position.x = Ease(In, Cubic, m_Frame * Slow::GetInstance()->GetPlayerSlowPower(), m_Position.x, m_AfterPosX);
		m_playerVec = { m_Position.x - oldX, 0, 0 };
	}

	if (m_Frame > 0.9f) {
		m_CameraZoom = false;
	}
	
}
//チュートリアルの更新
void Player::TutorialUpdate() {
	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();

	Obj_SetParam();
}