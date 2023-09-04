#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "Slow.h"
//ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
NormalEnemy::NormalEnemy() {
	
}
//‰Šú‰»
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
	&NormalEnemy::Inter,//“®‚«‚Ì‡ŠÔ
	&NormalEnemy::RightMove,//‰E‚ÉˆÚ“®
	&NormalEnemy::LeftMove,//¶‚ÉˆÚ“®
};

//s“®
void NormalEnemy::Action() {
	(this->*stateTable[_charaState])();

	Obj_SetParam();
}
//•`‰æ
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui•`‰æ
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
//ŠJ•ú
void NormalEnemy::Finalize() {

}

void NormalEnemy::Inter() {

}

void NormalEnemy::RightMove() {
	const float l_MAX = 15.0f;
	m_velocity = 0.05f;
	m_Position.x += m_velocity * Slow::GetInstance()->GetSlowPower();

	if (Helper::GetInstance()->CheckMin(m_Position.x, l_MAX, m_velocity)) {
		m_Position.x = -15.0f;
	}
}

void NormalEnemy::LeftMove() {
	const float l_MIN = -15.0f;
	m_velocity = -0.05f;
	m_Position.x += m_velocity * Slow::GetInstance()->GetSlowPower();

	if (Helper::GetInstance()->CheckMax(m_Position.x, l_MIN, m_velocity)) {
		m_Position.x = 15.0f;
	}
}