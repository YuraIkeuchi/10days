#include "InterEnemy.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"
#include "Slow.h"
#include "Random.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Timer.h"
#include "Easing.h"
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
	constexpr float PosX_Min = -9.0f;
	constexpr float PosX_Max = 9.0f;

	constexpr float PosZ_Min = -7.5f;
	constexpr float PosZ_Max = 7.5f;

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
		if (Player::GetInstance()->GetAttack() && !m_Death && !m_Miss && _charaState != STATE_INTER && m_Birth
			&& Timer::GetInstance()->getNowTime() < MovingTime) {
			Slow::GetInstance()->SetSlow(true);
			m_Slow = true;
			m_ViewEffect = true;

			return true;
		}
	}
	else {
		m_Slow = false;
		//m_ViewEffect = false;
		return false;
	}

	return false;
}

void InterEnemy::AttackCollide() {
	const float l_DamageRadius = 0.5f;
	Input* input = Input::GetInstance();
	if (m_Slow && m_MissTimer > 1) {
		if (Timer::GetInstance()->getGameType() == PAD_MODE) {			//ゲームパッド
			if ((input->TriggerButton(input->B))) {		//Bボタンパターン
				if (m_EnemyType == RED_ENEMY) {
					SuccessAttack();
				}
				else {		//違ったボタンを押すとミス
					MissAttack();
				}
			}
			else if ((input->TriggerButton(input->A))) {
				if (m_EnemyType == GREEN_ENEMY) {
					SuccessAttack();
				}
				else {		//違ったボタンを押すとミス
					MissAttack();
				}
			}
			else if ((input->TriggerButton(input->X))) {
				if (m_EnemyType == BLUE_ENEMY) {
					SuccessAttack();
				}
				else {		//違ったボタンを押すとミス
					MissAttack();
				}
			}
		}
		else {			//キーボード
			if ((input->TriggerKey(DIK_D))) {		//Dキーパターン
				if (m_EnemyType == RED_ENEMY) {
					SuccessAttack();
				}
				else {		//違ったボタンを押すとミス
					MissAttack();
				}
			}
			else if ((input->TriggerKey(DIK_S))) {
				if (m_EnemyType == GREEN_ENEMY) {
					SuccessAttack();
				}
				else {		//違ったボタンを押すとミス
					MissAttack();
				}
			}
			else if ((input->TriggerKey(DIK_A))) {
				if (m_EnemyType == BLUE_ENEMY) {
					SuccessAttack();
				}
				else {		//違ったボタンを押すとミス
					MissAttack();
				}
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

	if (m_MissTimer >= 100) {
		Audio::GetInstance()->PlayWave("Resources/audio/miss.wav", 0.1f);
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
		gauge_up->SetScale(0.23f);
		gauge_down->SetScale(0.3f);
		effect_up->SetScale(0.3f);
		effect_down->SetScale(0.3f);
		m_MissTimer = {};
	}
}
void InterEnemy::BirthMove() {
	if (_BirthType == BIRTH_SET) {
		m_Scale = { 0.0f,0.0f,0.0f };
		_BirthType = BIRTH_START;
	}
	else {
		m_AddPower -= m_Gravity;
		if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower()) && m_AddPower < 0.0f) {
			m_Birth = true;
		}

		m_Scale = { Ease(In,Cubic,0.5f,m_Scale.x,0.5f),
		Ease(In,Cubic,0.5f,m_Scale.y,0.5f),
		Ease(In,Cubic,0.5f,m_Scale.z,0.5f), };
	}
}
//死んだときの動き
void InterEnemy::DeathMove() {
	const float l_AddFrame = 0.05f;
	m_Slow = false;
	m_HitCheck = false;
	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_Scale = { Ease(In,Cubic,0.5f * Slow::GetInstance()->GetSlowPower(),m_Scale.x,0.0f),
					Ease(In,Cubic,0.5f * Slow::GetInstance()->GetSlowPower(),m_Scale.y,0.0f),
					Ease(In,Cubic,0.5f * Slow::GetInstance()->GetSlowPower(),m_Scale.z,0.0f), };

		m_Rotation.y += 2.0f;

		if (m_Scale.x <= 0.1f) {
			m_Alive = false;
		}
	}

	if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
		m_ViewEffect = false;
	}
	m_UpPos.x = Ease(In, Cubic, m_Frame, m_UpPos.x, 600.0f);
	m_DownPos.x = Ease(In, Cubic, m_Frame, m_DownPos.x, 1000.0f);
	m_Alpha = Ease(In, Cubic, m_Frame, m_Alpha, 0.0f);
}
//攻撃成功
void InterEnemy::SuccessAttack() {
	Slow::GetInstance()->SetCheck(false);
	m_Death = true;
	int num = Random::GetRanNum(30, 40);
	float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
	ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
	BirthEffect();
	Slow::GetInstance()->SetSlow(false);
	Audio::GetInstance()->PlayWave("Resources/audio/kill.wav", 0.1f);
	Player::GetInstance()->SetSlash(true);
	Player::GetInstance()->SetSlashTimer(0);
}
//攻撃失敗
void InterEnemy::MissAttack() {
	Slow::GetInstance()->SetCheck(false);
	m_Miss = true;
	m_HitCheck = false;
	Player::GetInstance()->SetDamage(true);
	Slow::GetInstance()->SetSlow(false);
	m_ViewEffect = false;
	Audio::GetInstance()->PlayWave("Resources/audio/miss.wav", 0.1f);
}
