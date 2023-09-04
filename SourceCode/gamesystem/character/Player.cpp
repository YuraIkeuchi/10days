#include "Player.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Easing.h"
#include "Collision.h"
#include "Slow.h"
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
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed2")));
}
//ステータスの初期化
void Player::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	//移動処理用
	velocity /= 5.0f;
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
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::End();
}

//移動
void Player::Move() {
	XMFLOAT3 rot = m_Rotation;
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
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f) && (m_Position.x < 9.5f)) {
			m_AddSpeed = 0.2f;
		}

		//左入力
		if (input->TiltPushStick(Input::L_LEFT, 0.0f) && (m_Position.x > -9.5f)) {
			m_AddSpeed = -0.2f;
		}

		//移動量加算
		m_Position.x += m_AddSpeed;
	}

	if ((input->TriggerButton(input->B))) {
		_charaState = STATE_ATTACK;
		m_Frame = {};
		if (_AttackState == ATTACK_DOWN) {
			m_AfterPosZ = -8.0f;
			_AttackState = ATTACK_UP;
		}
		else {
			m_AfterPosZ = 8.0f;
			_AttackState = ATTACK_DOWN;
		}
	}
	Helper::GetInstance()->Clamp(m_Position.x, -9.5f, 9.5f);
}

void Player::Attack() {
	const float l_AddFrame = 0.01f;
	if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame * Slow::GetInstance()->GetSlowPower())) {
		m_Frame = {};
		_charaState = STATE_MOVE;
	}

	m_Position.z = Ease(In, Cubic, m_Frame, m_Position.z, m_AfterPosZ);
}
