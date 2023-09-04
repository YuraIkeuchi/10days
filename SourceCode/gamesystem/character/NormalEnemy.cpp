#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
//モデル読み込み
NormalEnemy::NormalEnemy() {
	
}
//初期化
bool NormalEnemy::Initialize() {

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetPosition({ 0.0f,0.0f,20.0f });
	m_Object->SetColor({ 1.0f,0.0f,0.0,1.0f });
	m_Position = { 15.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	m_Object->VertexCheck();

	_charaState = STATE_LEFT;
	return true;
}

void (NormalEnemy::* NormalEnemy::stateTable[])() = {
	&NormalEnemy::Inter,//動きの合間
	&NormalEnemy::RightMove,//右に移動
	&NormalEnemy::LeftMove,//左に移動
};

//行動
void NormalEnemy::Action() {
	(this->*stateTable[_charaState])();

	Obj_SetParam();
}
//描画
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	//if (ImGui::RadioButton("FOLLOW", &_charaState)) {
	//	_charaState = STATE_FOLLOW;
	//}
	//else if (ImGui::RadioButton("CIRCLE", &_charaState)) {
	//	m_StartPos = m_Position;
	//	m_CircleScale = 0.0f;
	//	m_CircleSpeed = 0.0f;
	//	_charaState = STATE_CIRCLE;
	//}
	//else if (ImGui::RadioButton("SIN", &_charaState)) {
	//	_charaState = STATE_SIN;
	//}
	//else if (ImGui::RadioButton("INTER", &_charaState)) {
	//	_charaState = STATE_INTER;
	//}
	ImGui::End();
}
//開放
void NormalEnemy::Finalize() {

}

void NormalEnemy::Inter() {

}

void NormalEnemy::RightMove() {
	const float l_MAX = 15.0f;
	m_velocity = 0.05f;
	m_Position.x += m_velocity;

	if (Helper::GetInstance()->CheckMin(m_Position.x, l_MAX, m_velocity)) {
		m_Position.x = -15.0f;
	}
}

void NormalEnemy::LeftMove() {
	const float l_MIN = -15.0f;
	m_velocity = -0.05f;
	m_Position.x += m_velocity;

	if (Helper::GetInstance()->CheckMax(m_Position.x, l_MIN, m_velocity)) {
		m_Position.x = 15.0f;
	}
}