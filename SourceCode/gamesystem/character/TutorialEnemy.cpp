#include "TutorialEnemy.h"
#include "Player.h"
#include "Helper.h"
#include "CsvLoader.h"
#include "Input.h"
#include "Easing.h"
#include "Slow.h"
#include "Collision.h"
#include "ParticleEmitter.h"
#include "Random.h"
#include "ImageManager.h"
#include "TutorialTask.h"
#include "Timer.h"
#define MapMinX -10
#define MapMaxX 10

#define MapMinZ -10
#define MapMaxZ 10

//モデル読み込み
TutorialEnemy::TutorialEnemy() {
	
}
//初期化
bool TutorialEnemy::Initialize() {

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	effect_up = IKESprite::Create(ImageManager::CUT_UP, {});
	effect_up->SetAnchorPoint({ 0.5f,1.0f });
	effect_down = IKESprite::Create(ImageManager::CUT_DOWN, {});
	effect_down->SetAnchorPoint({ 0.5f,0.0f });
	gauge_up = IKESprite::Create(ImageManager::CUTGAGE_UP, {});
	gauge_up->SetAnchorPoint({ 0.5f,1.0f });
	gauge_down = IKESprite::Create(ImageManager::CUTGAGE_DOWN, {});
	gauge_down->SetAnchorPoint({ 0.5f,0.0f });
	_charaState = StartState;
	_EnemyType = m_EnemyType;
	m_Scale = { 0.5f,0.5f,0.5f };
	if (_EnemyType == RED_ENEMY) {
		m_UpPos = { 1000.0f,200.0f };
		m_DownPos = { 1000.0f,195.0f };
		m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::ENEMYRED));
		m_EffectColor = { 1.0f,0.2f,0.0f,1.0f };
	}
	else if (_EnemyType == GREEN_ENEMY) {
		m_UpPos = { 1000.0f,280.0f };
		m_DownPos = { 1000.0f,275.0f };
		m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::ENEMYGREEN));
		m_EffectColor = { 0.0f,1.0f,0.2f,1.0f };
	}
	else {
		m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::ENEMYBLUE));
		m_UpPos = { 1000.0f,360.0f };
		m_DownPos = { 1000.0f,355.0f };
		m_EffectColor = { 0.2f,0.0f,1.0f,1.0f };
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
	gauge_up->SetColor(m_EffectColor);
	gauge_down->SetColor(m_EffectColor);

	gauge_up->SetPosition(m_UpPos);
	gauge_down->SetPosition(m_DownPos);
	effect_up->SetPosition(m_UpPos);
	effect_down->SetPosition(m_DownPos);
	m_BaseSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "speed")));
	m_Birth = false;
	m_AddPower = 0.2f;
	//m_Position.y = 2.0f;
	return true;
}

void (TutorialEnemy::* TutorialEnemy::stateTable[])() = {
	&TutorialEnemy::Inter,//動きの合間
	&TutorialEnemy::RightMove,//右に移動
	&TutorialEnemy::LeftMove,//左に移動
	&TutorialEnemy::UpMove,//上に移動
	&TutorialEnemy::BottomMove,//下に移動
};

//行動
void TutorialEnemy::Action() {
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
	

	//当たり判定
	if (!m_Death && !TutorialTask::GetInstance()->GetStop()) {
		SlowCollide();
	}
	
	//エフェクト関係
	gauge_up->SetPosition(m_UpPos);
	gauge_down->SetPosition(m_DownPos);
	effect_up->SetPosition(m_UpPos);
	effect_down->SetPosition(m_DownPos);
	gauge_up->SetColor({ m_EffectColor.x,m_EffectColor.y,m_EffectColor.z,m_Alpha });
	gauge_down->SetColor({ m_EffectColor.x,m_EffectColor.y,m_EffectColor.z,m_Alpha });
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

	if (m_EffectMove) {
		EffectCountDown();
	}

	if (Slow::GetInstance()->GetSlow()) {
		m_JumpPower = 0.1f;
	}
	else {
		m_JumpPower = 1.0f;
	}
	Obj_SetParam();
}
//描画
void TutorialEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//エフェクト描画
void TutorialEnemy::EffectDraw(DirectXCommon* dxCommon) {
	//斬撃エフェクト
	for (auto i = 0; i < slash.size(); i++)
	{
		if (slash[i] == nullptr)continue;
		slash[i]->Draw(dxCommon);
	}

	IKESprite::PreDraw();
	if (m_ViewEffect) {
		gauge_up->Draw();
		gauge_down->Draw();
		effect_up->Draw();
		effect_down->Draw();
	}
	IKESprite::PostDraw();
}
//ImGui描画
void TutorialEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("Scale.x:%f", m_Scale.x);
	ImGui::Text("AddPower:%f", m_AddPower);
	ImGui::Text("PosY:%f", m_Position.y);
	ImGui::Text("Birth:%d", m_Birth);
	ImGui::End();
}
//開放
void TutorialEnemy::Finalize() {

}

//リスポーン
void TutorialEnemy::Inter() {
	m_ResPornTimer++;
	if (m_ResPornTimer == 100) {
		m_ResPornTimer = {};
		m_Slow = false;
		_charaState = CharaState::STATE_LEFT;
	}
}
//右に動く
void TutorialEnemy::RightMove() {

	const float l_MAX = MapMaxX;
	m_velocity = m_BaseSpeed;

	if (m_SlowMove) {
		m_velocity = m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = m_BaseSpeed;
	}
	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * m_JumpPower)) {
		m_AddPower = 0.2f;
	}
	if (m_Move) {
		if (Helper::GetInstance()->CheckMin(m_Position.x, l_MAX, m_velocity)) {
			m_Position.x = MapMinX;
			m_Slow = false;
		}
	}
}
//左に動く
void TutorialEnemy::LeftMove() {
	m_Rotation = { 0.0f,180.0f,0.0f };
	const float l_MIN =MapMinX;
	m_velocity = -m_BaseSpeed;
	if (m_SlowMove) {
		m_velocity = -m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = -m_BaseSpeed;
	}
	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * m_JumpPower)) {
		m_AddPower = 0.2f;
	}
	if (m_Move) {
		if (Helper::GetInstance()->CheckMax(m_Position.x, l_MIN, m_velocity)) {
			m_Position.x = MapMaxX;
			m_Slow = false;
		}
	}
}
//左に動く
void TutorialEnemy::BottomMove() {
	const float l_MIN = MapMinZ;
	m_velocity = -m_BaseSpeed;
	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_AddPower = 0.2f;
	}
	if (m_Move) {
		if (Helper::GetInstance()->CheckMax(m_Position.z, l_MIN, m_velocity)) {
			m_Position.z = MapMaxZ;
			m_Slow = false;
		}
	}
}
//左に動く
void TutorialEnemy::UpMove() {
	const float l_MIN =MapMaxZ;
	m_velocity = m_BaseSpeed;
	m_AddPower -= m_Gravity;
	if (Helper::GetInstance()->CheckMax(m_Position.y, {}, m_AddPower * Slow::GetInstance()->GetSlowPower())) {
		m_AddPower = 0.2f;
	}
	if (m_Move) {
		if (Helper::GetInstance()->CheckMin(m_Position.z, l_MIN, m_velocity)) {
			m_Position.z = MapMinZ;
			m_Slow = false;
		}
	}
}

void TutorialEnemy::SlowCollide() {
	Input* input = Input::GetInstance();
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_radius, Player::GetInstance()->GetAttackPos().x, Player::GetInstance()->GetAttackPos().z, m_radius)) {
		if (!m_Slow) {
			Slow::GetInstance()->SetSlow(true);
			Slow::GetInstance()->SetSlowTimer(60);
			m_Slow = true;
			m_ViewEffect = true;
			Audio::GetInstance()->PlayWave("Resources/audio/attack.wav", 0.1f);
		}
		else {
			if (Timer::GetInstance()->getGameType() == PAD_MODE) {
				if ((input->TriggerButton(input->B))) {		//Bボタンパターン
					if (m_EnemyType == RED_ENEMY) {
						TutoSuccessAttack();
					}
					else {		//違ったボタンを押すとミス
						TutoMissAttack();
					}
				}
				else if ((input->TriggerButton(input->A))) {
					if (m_EnemyType == GREEN_ENEMY) {
						TutoSuccessAttack();
					}
					else {		//違ったボタンを押すとミス
						TutoMissAttack();
					}
				}
				else if ((input->TriggerButton(input->X))) {
					if (m_EnemyType == BLUE_ENEMY) {
						TutoSuccessAttack();
					}
					else {		//違ったボタンを押すとミス
						TutoMissAttack();
					}
				}
			}
			else {
				if ((input->TriggerKey(DIK_D))) {		//Ｄキーパターン
					if (m_EnemyType == RED_ENEMY) {
						TutoSuccessAttack();
					}
					else {		//違ったボタンを押すとミス
						TutoMissAttack();
					}
				}
				else if ((input->TriggerKey(DIK_S))) {
					if (m_EnemyType == GREEN_ENEMY) {
						TutoSuccessAttack();
					}
					else {		//違ったボタンを押すとミス
						TutoMissAttack();
					}
				}
				else if ((input->TriggerKey(DIK_A))) {
					if (m_EnemyType == BLUE_ENEMY) {
						TutoSuccessAttack();
					}
					else {		//違ったボタンを押すとミス
						TutoMissAttack();
					}
				}
			}
		}
	}
	else {
		m_ViewEffect = false;
		m_Slow = false;
	}
}

//エフェクトの生成
void TutorialEnemy::TutoBirthEffect() {
	SlashEffect* effect;
	effect = new SlashEffect(m_Position);
	effect->Initialize();
	slash.push_back(effect);
}
//攻撃成功
void TutorialEnemy::TutoSuccessAttack() {
	Slow::GetInstance()->SetCheck(false);
	m_Death = true;
	int num = Random::GetRanNum(30, 40);
	float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
	ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
	BirthEffect();
	Slow::GetInstance()->SetSlow(false);
	Player::GetInstance()->SetSlash(true);
	Player::GetInstance()->SetSlashTimer(0);
	Audio::GetInstance()->PlayWave("Resources/audio/kill.wav", 0.1f);
}
//攻撃失敗
void TutorialEnemy::TutoMissAttack() {
	if (m_CheckMiss) {
		Slow::GetInstance()->SetCheck(false);
		m_Miss = true;
		m_HitCheck = false;
		Player::GetInstance()->SetDamage(true);
		Slow::GetInstance()->SetSlow(false);
		m_ViewEffect = false;
		Audio::GetInstance()->PlayWave("Resources/audio/miss.wav", 0.1f);
	}
}