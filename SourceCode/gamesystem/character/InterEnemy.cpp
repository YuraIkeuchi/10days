#include "InterEnemy.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"
#include "Slow.h"
#include "Random.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Timer.h"
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	//�s��
	Action();
}
void InterEnemy::SetResPos(PosSt pos,float otpos)
{
	constexpr float PosX_Min = -10.f;
	constexpr float PosX_Max = 10.f;

	constexpr float PosZ_Min = -10.f;
	constexpr float PosZ_Max = 10.f;

	if (pos == PosSt::UPRES)
	{
		m_Position.x = otpos;
	m_Position.z = PosZ_Max;
		StartState = CharaState::STATE_DOWN;
	}

	else if (pos == PosSt::BOTTOMRES) {
		m_Position.x = otpos;
		m_Position.z = PosZ_Min;
		StartState = CharaState::STATE_UP;
		
	}
	else if (pos == PosSt::RIGHTRES) {
		m_Position.z = otpos;
		StartState = CharaState::STATE_LEFT;
		m_Position.x = PosX_Max;
	}
	else if (pos == PosSt::LEFTRES) {
		m_Position.z = otpos;
		StartState = CharaState::STATE_RIGHT;
		m_Position.x = PosX_Min;
	}
}

//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::ImGuiDraw() {
	ImGui::Begin("--Pos--");
	ImGui::End();
	ImGui_Origin();
}

bool InterEnemy::CheckCollide() {
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_radius, Player::GetInstance()->GetAttackPos().x, Player::GetInstance()->GetAttackPos().z, m_radius)) {
		if (Player::GetInstance()->GetAttack() && !m_Death && !m_Miss && _charaState != STATE_INTER
			&& Timer::GetInstance()->getNowTime() < MovingTime) {
			Slow::GetInstance()->SetSlow(true);
			m_Slow = true;
			m_ViewEffect = true;

			return true;
		}
	}
	else {
		m_Slow = false;
		m_ViewEffect = false;
		return false;
	}

	return false;
}

void InterEnemy::AttackCollide() {
	const float l_DamageRadius = 0.5f;
	Input* input = Input::GetInstance();
	if (m_Slow) {
		if ((input->TriggerButton(input->B))) {		//Bボタンパターン
			if (m_EnemyType == RED_ENEMY) {
				Slow::GetInstance()->SetCheck(false);
				m_Death = true;
				int num = Random::GetRanNum(30, 40);
				float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
				ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
				BirthEffect();
				Slow::GetInstance()->SetSlow(false);
			}
			else {		//違ったボタンを押すとミス
				Slow::GetInstance()->SetCheck(false);
				m_Miss = true;
				m_HitCheck = false;
				Player::GetInstance()->SetDamage(true);
				Slow::GetInstance()->SetSlow(false);
				m_ViewEffect = false;
			}
		}
		else if ((input->TriggerButton(input->A))) {
			if (m_EnemyType == GREEN_ENEMY) {
				Slow::GetInstance()->SetCheck(false);
				m_Death = true;
				int num = Random::GetRanNum(30, 40);
				float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
				ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
				BirthEffect();
				Slow::GetInstance()->SetSlow(false);
			}
			else {		//違ったボタンを押すとミス
				Slow::GetInstance()->SetCheck(false);
				m_Miss = true;
				m_HitCheck = false;
				Player::GetInstance()->SetDamage(true);
				Slow::GetInstance()->SetSlow(false);
				m_ViewEffect = false;
			}
		}
		else if ((input->TriggerButton(input->X))) {
			if (m_EnemyType == BLUE_ENEMY) {
				Slow::GetInstance()->SetCheck(false);
				m_Death = true;
				int num = Random::GetRanNum(30, 40);
				float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
				ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
				BirthEffect();
				Slow::GetInstance()->SetSlow(false);
			}
			else {		//違ったボタンを押すとミス
				Slow::GetInstance()->SetCheck(false);
				m_Miss = true;
				m_HitCheck = false;
				Player::GetInstance()->SetDamage(true);
				Slow::GetInstance()->SetSlow(false);
				m_ViewEffect = false;
			}
		}
		if (Collision::CircleCollision(m_Position.x, m_Position.z, l_DamageRadius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, l_DamageRadius)
			&& !m_Death) {
			Slow::GetInstance()->SetCheck(false);
			m_Miss = true;
			Player::GetInstance()->SetDamage(true);
			Slow::GetInstance()->SetSlow(false);
			m_ViewEffect = false;
		}
	}
}

//エフェクトの生成
void InterEnemy::BirthEffect() {
	SlashEffect* effect;
	effect = new SlashEffect(m_Position);
	effect->Initialize();
	slash.push_back(effect);
}

//エフェクトが小さくなる
void InterEnemy::EffectCountDown() {
	if (m_ViewEffect && !m_Death) {
		gauge_up->SetScale(0.97f);
		gauge_down->SetScale(0.97f);
		effect_up->SetScale(0.97f);
		effect_down->SetScale(0.97f);
		m_MissTimer++;
	}

	if (m_MissTimer >= 80) {
		Slow::GetInstance()->SetCheck(false);
		m_Miss = true;
		m_HitCheck = false;
		Player::GetInstance()->SetDamage(true);
		Slow::GetInstance()->SetSlow(false);
		m_ViewEffect = false;

		gauge_up->SetSize({ 1632.0f,706 });
		gauge_down->SetSize({ 1632.0f,819 });
		effect_up->SetSize({ 1632.0f,703.0f });
		effect_down->SetSize({ 1632.0f,823.0f });
		gauge_up->SetScale(0.25f);
		gauge_down->SetScale(0.25f);
		effect_up->SetScale(0.25f);
		effect_down->SetScale(0.25f);
		m_MissTimer = {};
	}
}