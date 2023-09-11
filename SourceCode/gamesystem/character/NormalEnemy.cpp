#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Helper.h"
#include "CsvLoader.h"
#include "Input.h"
#include "Easing.h"
#include "Slow.h"
#include "Collision.h"

#include "SceneManager.h"
#include"Timer.h"

#include "ImageManager.h"

#define MapMinX -9
#define MapMaxX 9

#define MapMinZ -7.5
#define MapMaxZ 7.5

//モデル読み込み
NormalEnemy::NormalEnemy() {
	
}
//初期化
bool NormalEnemy::Initialize() {

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::ENEMY));
	effect_up = IKESprite::Create(ImageManager::CUT_UP, {});
	effect_up->SetAnchorPoint({ 0.5f,1.0f });
	effect_down = IKESprite::Create(ImageManager::CUT_DOWN, {});
	effect_down->SetAnchorPoint({ 0.5f,0.0f });
	gauge_up = IKESprite::Create(ImageManager::CUTGAGE_UP, {});
	gauge_up->SetAnchorPoint({ 0.5f,1.0f });
	gauge_down = IKESprite::Create(ImageManager::CUTGAGE_DOWN, {});
	gauge_down->SetAnchorPoint({ 0.5f,0.0f });
	_charaState =  StartState;
	_EnemyType = m_EnemyType;
	m_Scale = { 0.5f,0.5f,0.5f };
	if (_EnemyType == RED_ENEMY) {
		m_Color = { 1.0f,0.2f,0.0f,1.0f };
		m_UpPos = { 1000.0f,200.0f };
		m_DownPos = { 1000.0f,195.0f };
	}
	else if (_EnemyType == GREEN_ENEMY) {
		m_Color = { 0.0f,1.0f,0.2f,1.0f };
		m_UpPos = { 800.0f,280.0f };
		m_DownPos = { 800.0f,275.0f };
	}
	else {
		m_Color = { 0.2f,0.0f,1.0f,1.0f };
		m_UpPos = { 800.0f,360.0f };
		m_DownPos = { 800.0f,355.0f };
	}

	if (_charaState == STATE_RIGHT) {
		m_Rotation.y = 0.0f;
	}
	else if (_charaState == STATE_LEFT) {
		m_Rotation.y = 180.0f;
	}
	else if (_charaState == STATE_UP) {
		m_Rotation.y = 270.0f;
	}
	else if (_charaState == STATE_DOWN) {
		m_Rotation.y = 90.0f;
	}
	
	gauge_up->SetScale(0.25f);
	gauge_down->SetScale(0.25f);
	effect_up->SetScale(0.25f);
	effect_down->SetScale(0.25f);
	gauge_up->SetColor(m_Color);
	gauge_down->SetColor(m_Color);

	gauge_up->SetPosition(m_UpPos);
	gauge_down->SetPosition(m_DownPos);
	effect_up->SetPosition(m_UpPos);
	effect_down->SetPosition(m_DownPos);
	m_Alive = true;
	//_charaState =  StartState;
	m_Move = false;
	m_BaseSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "speed")));
	m_SlowSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "slow")));
	m_Birth = false;
	m_AddPower = 0.2f;
	return true;
}
//状態遷移
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
		if (m_Birth) {
			if (!m_Death) {
				(this->*stateTable[_charaState])();
			}
			else {
				DeathMove();
			}
		}
		else {
			BirthMove();
		}
	}
	Obj_SetParam();

	//エフェクト関係
	gauge_up->SetPosition(m_UpPos);
	gauge_down->SetPosition(m_DownPos);
	effect_up->SetPosition(m_UpPos);
	effect_down->SetPosition(m_DownPos);
	gauge_up->SetColor({ m_Color.x,m_Color.y,m_Color.z,m_Alpha });
	gauge_down->SetColor({ m_Color.x,m_Color.y,m_Color.z,m_Alpha });
	effect_up->SetColor({ 1.0f,1.0f,1.0f,m_Alpha });
	effect_down->SetColor({ 1.0f,1.0f,1.0f,m_Alpha });

	//斬撃エフェクト
	for (auto i = 0; i < slash.size(); i++)
	{
		if (slash[i] == nullptr)continue;
		slash[i]->Update();
	}
	//斬撃エフェクトの削除
	for (int i = 0; i < slash.size(); i++) {
		if (slash[i] == nullptr) {
			continue;
		}

		if (!slash[i]->GetAlive()) {
			slash.erase(cbegin(slash) + i);
		}
	}

	if (m_HitCheck) {
		AttackCollide();
	}
	if (!Player::GetInstance()->GetAttack()) {
		m_Slow = false;
		m_Miss = false;
		m_ViewEffect = false;
		m_MissTimer = {};
		gauge_up->SetSize({ 1632.0f,706 });
		gauge_down->SetSize({ 1632.0f,819 });
		effect_up->SetSize({ 1632.0f,703.0f });
		effect_down->SetSize({ 1632.0f,823.0f });
		gauge_up->SetScale(0.25f);
		gauge_down->SetScale(0.25f);
		effect_up->SetScale(0.25f);
		effect_down->SetScale(0.25f);
	}

	EffectCountDown();
}
//描画
void NormalEnemy::Draw(DirectXCommon* dxCommon) {

	if (StopF||SceneManager::GetInstance()->GetEditF()||Timer::GetInstance()->getNowTime()<MovingTime && m_Alive) {
		Obj_Draw();
	}

	//斬撃エフェクト
	for (auto i = 0; i < slash.size(); i++)
	{
		if (slash[i] == nullptr)continue;
		slash[i]->Draw(dxCommon);
	}
}
//エフェクト描画
void NormalEnemy::EffectDraw(DirectXCommon* dxCommon) {
	if (m_ViewEffect) {
		gauge_up->Draw();
		gauge_down->Draw();
		effect_up->Draw();
		effect_down->Draw();
	}
}
//ImGui描画
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("Hit:%d", m_MissTimer);
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
		m_velocity = m_SlowSpeed * Slow::GetInstance()->GetSlowPower() * Slow::GetInstance()->GetMovePower();
	}
	else {
		m_velocity = m_BaseSpeed;
	}

	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_AddPower = 0.2f;
	}
	
	if (Helper::GetInstance()->CheckMin(m_Position.x, l_MAX, m_velocity)) {
		m_Death = true;
		m_Slow = false;
		m_Destroy = true;
	}
}
//左に動く
void NormalEnemy::LeftMove() {
	const float l_MIN = MapMinX;
	if (m_SlowMove) {
		m_velocity = -m_SlowSpeed * Slow::GetInstance()->GetSlowPower() * Slow::GetInstance()->GetMovePower();
	}
	else {
		m_velocity = -m_BaseSpeed;
	}

	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_AddPower = 0.2f;
	}

	if (Helper::GetInstance()->CheckMax(m_Position.x, l_MIN, m_velocity)) {
		m_Death = true;
		m_Slow = false;
		m_Destroy = true;
	}
	
}
//下に動く
void NormalEnemy::BottomMove() {

	const float l_MIN = MapMinZ;
	if (m_SlowMove) {
		m_velocity = -m_SlowSpeed * Slow::GetInstance()->GetSlowPower() * Slow::GetInstance()->GetMovePower();
	}
	else {
		m_velocity = -m_BaseSpeed;
	}

	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_AddPower = 0.2f;
	}

	if (Helper::GetInstance()->CheckMax(m_Position.z, l_MIN, m_velocity)) {
		m_Death = true;
		m_Slow = false;
		m_Destroy = true;
	}
}
//上に動く
void NormalEnemy::UpMove() {
	const float l_MIN =MapMaxZ;
	
	if (m_SlowMove) {
		m_velocity = m_SlowSpeed * Slow::GetInstance()->GetSlowPower() * Slow::GetInstance()->GetMovePower();
	}
	else {
		m_velocity = m_BaseSpeed;
	}

	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_AddPower = 0.2f;
	}
	
	if (Helper::GetInstance()->CheckMin(m_Position.z, l_MIN, m_velocity)) {
		m_Death = true;
		m_Slow = false;
		m_Destroy = true;
	}
}

//スローの当たり判定
void NormalEnemy::SlowCollide() {
	const float l_DamageRadius = 0.5f;
	Input* input = Input::GetInstance();
	/*
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_radius, Player::GetInstance()->GetAttackPos().x, Player::GetInstance()->GetAttackPos().z, m_radius)) {
		if (!m_Slow && Player::GetInstance()->GetAttack()) {
			Slow::GetInstance()->SetSlow(true);
			Slow::GetInstance()->SetSlowTimer(60);
			m_Slow = true;
			m_ViewEffect = true;
		}
		else {
			if (m_EnemyType == RED_ENEMY) {
				if ((input->TriggerButton(input->B))) {
					m_Death = true;
					_charaState = STATE_INTER;
					int num = Random::GetRanNum(30, 40);
					float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
					ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
					BirthEffect();
				}
			}
			else if (m_EnemyType == GREEN_ENEMY) {
				if ((input->TriggerButton(input->A))) {
					m_Death = true;
					_charaState = STATE_INTER;
					int num = Random::GetRanNum(30, 40);
					float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
					ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
					BirthEffect();
				}
			}
			else {
				if ((input->TriggerButton(input->X))) {
					m_Death = true;
					_charaState = STATE_INTER;
					int num = Random::GetRanNum(30, 40);
					float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
					ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
					BirthEffect();
				}
			}
		}
	}
	else {
		m_ViewEffect = false;
		m_Slow = false;
		Player::GetInstance()->SetDamage(true);
	}*/
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_radius, Player::GetInstance()->GetAttackPos().x, Player::GetInstance()->GetAttackPos().z, m_radius)
		) {
		if (Player::GetInstance()->GetAttack()) {
			Slow::GetInstance()->SetSlow(true);
			m_Slow = true;
			m_ViewEffect = true;
		}
	}
	else {
		m_Slow = false;
		m_ViewEffect = false;
	}

}
