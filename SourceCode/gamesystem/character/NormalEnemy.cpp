#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Helper.h"
#include "CsvLoader.h"
#include "Input.h"
#include "Easing.h"
#include "Slow.h"
#include "Collision.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"
#include"Timer.h"
#include "Random.h"
#include "ImageManager.h"

#define MapMinX -10
#define MapMaxX 10

#define MapMinZ -10
#define MapMaxZ 10

//モデル読み込み
NormalEnemy::NormalEnemy() {
	
}
//初期化
bool NormalEnemy::Initialize() {

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::ENEMY));
	effect = IKESprite::Create(ImageManager::CUTEFFECT, {});
	effect->SetPosition({ 700.0f,300.0f });
	_charaState =  StartState;
	_EnemyType = m_EnemyType;

	if (_EnemyType == RED_ENEMY) {
		m_Color = { 1.0f,0.2f,0.0f,1.0f };
	}
	else if (_EnemyType == GREEN_ENEMY) {
		m_Color = { 0.0f,1.0f,0.2f,1.0f };
	}
	else {
		m_Color = { 0.2f,0.0f,1.0f,1.0f };
	}

	effect->SetColor(m_Color);
	m_Alive = true;
	//_charaState =  StartState;
	m_Move = false;
	m_BaseSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "speed")));
	return true;
}

void (NormalEnemy::* NormalEnemy::stateTable[])() = {
	&NormalEnemy::Inter,//動きの合間
	&NormalEnemy::RightMove,//右に移動
	&NormalEnemy::LeftMove,//左に移動
	&NormalEnemy::UpMove,//上に移動
	&NormalEnemy::BottomMove,//下に移動

};

//行動
void NormalEnemy::Action() {
	if (!StopF&&Timer::GetInstance()->getNowTime()<MovingTime) {
		(this->*stateTable[_charaState])();

		//当たり判
		SlowCollide();
	}
	Obj_SetParam();
}
//描画
void NormalEnemy::Draw(DirectXCommon* dxCommon) {

	if (StopF||SceneManager::GetInstance()->GetEditF()||Timer::GetInstance()->getNowTime()<MovingTime && m_Alive) {
		Obj_Draw();
	}
}
//エフェクト描画
void NormalEnemy::EffectDraw(DirectXCommon* dxCommon) {
	if (m_Slow) {
		effect->Draw();
	}
}
//ImGui描画
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("Slow:%f", m_velocity);
	ImGui::End();
}
//開放
void NormalEnemy::Finalize() {

}

//リスポーン
void NormalEnemy::Inter() {
	m_ResPornTimer++;
	if (m_ResPornTimer == 100) {
		m_ResPornTimer = {};
		m_Slow = false;
		_charaState = CharaState::STATE_LEFT;
	}
}
//右に動く
void NormalEnemy::RightMove() {

	const float l_MAX = MapMaxX;
	
	if (m_SlowMove) {
		m_velocity = m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = m_BaseSpeed;
	}
	
	if (Helper::GetInstance()->CheckMin(m_Position.x, l_MAX, m_velocity)) {
		m_Position.x = MapMinX;
		m_Alive = false;
		m_Slow = false;
		m_Destroy = true;
	}
}
//左に動く
void NormalEnemy::LeftMove() {
	m_Rotation = { 0.0f,90.0f,0.0f };
	const float l_MIN = MapMinX;
	if (m_SlowMove) {
		m_velocity = -m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = -m_BaseSpeed;
	}

	if (Helper::GetInstance()->CheckMax(m_Position.x, l_MIN, m_velocity)) {
		m_Position.x = MapMaxX;
		m_Alive = false;
		m_Slow = false;
		m_Destroy = true;
	}
	
}
//下に動く
void NormalEnemy::BottomMove() {
	const float l_MIN = MapMinZ;
	if (m_SlowMove) {
		m_velocity = -m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = -m_BaseSpeed;
	}

	if (Helper::GetInstance()->CheckMax(m_Position.z, l_MIN, m_velocity)) {
		m_Position.z = MapMaxZ;
		m_Alive = false;
		m_Slow = false;
		m_Destroy = true;
	}
}
//上に動く
void NormalEnemy::UpMove() {
	const float l_MIN =MapMaxZ;
	if (m_SlowMove) {
		m_velocity = m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = m_BaseSpeed;
	}
	
	if (Helper::GetInstance()->CheckMin(m_Position.z, l_MIN, m_velocity)) {
		m_Position.z = MapMinZ;
		m_Alive = false;
		m_Slow = false;
		m_Destroy = true;
	}
}

void NormalEnemy::SlowCollide() {
	Input* input = Input::GetInstance();
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, m_radius)) {
		if (!m_Slow && Player::GetInstance()->GetAttack()) {
			m_Slow = true;
			Slow::GetInstance()->SetSlow(true);
		}
		else {
			if (m_EnemyType == RED_ENEMY) {
				if ((input->TriggerButton(input->B))) {
					m_Alive = false;
					_charaState = STATE_INTER;
					m_ResPornTimer = {};
					ParticleEmitter::GetInstance()->SplatterEffect(20, Random::GetRanNum(3, 6), m_Position, Player::GetInstance()->GetPlayerVec(), 1.0f, 1.0f, { 1, 0, 0, 1 });
				}
			}
			else if (m_EnemyType == GREEN_ENEMY) {
				if ((input->TriggerButton(input->A))) {
					m_Alive = false;
					_charaState = STATE_INTER;
					m_ResPornTimer = {};
					ParticleEmitter::GetInstance()->SplatterEffect(20, Random::GetRanNum(3, 6), m_Position, Player::GetInstance()->GetPlayerVec(), 1.0f, 1.0f, { 1, 0, 0, 1 });
				}
			}
			else {
				if ((input->TriggerButton(input->X))) {
					m_Alive = false;
					_charaState = STATE_INTER;
					m_ResPornTimer = {};
					ParticleEmitter::GetInstance()->SplatterEffect(20, Random::GetRanNum(3, 6), m_Position, Player::GetInstance()->GetPlayerVec(), 1.0f, 1.0f, { 1, 0, 0, 1 });
				}
			}
		}
	}
	else {
		m_Slow = false;
	}
}