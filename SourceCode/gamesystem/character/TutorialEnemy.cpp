#include "TutorialEnemy.h"
#include <random>
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
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::ENEMY));
	effect_up = IKESprite::Create(ImageManager::CUT_UP, {});
	effect_down = IKESprite::Create(ImageManager::CUT_DOWN, {});
	gauge_up = IKESprite::Create(ImageManager::CUTGAGE_UP, {});
	gauge_down = IKESprite::Create(ImageManager::CUTGAGE_DOWN, {});
	_charaState = StartState;
	_EnemyType = m_EnemyType;

	if (_EnemyType == RED_ENEMY) {
		m_Color = { 1.0f,0.2f,0.0f,1.0f };
		m_UpPos = { 800.0f,200.0f };
		m_DownPos = { 800.0f,195.0f };
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
	gauge_up->SetScale(0.3f);
	gauge_down->SetScale(0.3f);
	effect_up->SetScale(0.3f);
	effect_down->SetScale(0.3f);
	gauge_up->SetColor(m_Color);
	gauge_down->SetColor(m_Color);

	gauge_up->SetPosition(m_UpPos);
	gauge_down->SetPosition(m_DownPos);
	effect_up->SetPosition(m_UpPos);
	effect_down->SetPosition(m_DownPos);
	m_BaseSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "speed")));
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
	if (!m_Death) {
		(this->*stateTable[_charaState])();
	}
	else {
		DeathMove();
	}
	

	//当たり判定
	if (!m_Death) {
		SlowCollide();
	}
	
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
	ImGui::Text("Alpha:%f", m_Alpha);
	ImGui::Text("UpPos:%f", m_UpPos.x);
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
		}
		else {
			if (m_EnemyType == RED_ENEMY) {
				if ((input->TriggerButton(input->B))) {
					m_Death = true;
					int num = Random::GetRanNum(30, 40);
					float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
					ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
					BirthEffect();
					Slow::GetInstance()->SetSlow(false);
					Slow::GetInstance()->SetSlowTimer(0);
				}
			}
			else if (m_EnemyType == GREEN_ENEMY) {
				if ((input->TriggerButton(input->A))) {
					m_Death = true;
					int num = Random::GetRanNum(30, 40);
					float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
					ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
					BirthEffect();
					Slow::GetInstance()->SetSlow(false);
					Slow::GetInstance()->SetSlowTimer(0);
				}
			}
			else {
				if ((input->TriggerButton(input->X))) {
					m_Death = true;
					int num = Random::GetRanNum(30, 40);
					float size = static_cast<float>(Random::GetRanNum(5, 15)) / 50;
					ParticleEmitter::GetInstance()->SplatterEffect(20, num, m_Position, Player::GetInstance()->GetPlayerVec(), size, size, { 1, 0, 0, 1 });
					BirthEffect();
					Slow::GetInstance()->SetSlow(false);
					Slow::GetInstance()->SetSlowTimer(0);
				}
			}
		}
	}
	else {
		m_ViewEffect = false;
		m_Slow = false;
	}
}
//死んだときの動き
void TutorialEnemy::DeathMove() {
	const float l_AddFrame = 0.05f;
	m_Slow = false;
	m_Rotation.y += 2.0f;
	m_Scale = { Ease(In,Cubic,0.5f * Slow::GetInstance()->GetSlowPower(),m_Scale.x,0.0f),
				Ease(In,Cubic,0.5f * Slow::GetInstance()->GetSlowPower(),m_Scale.y,0.0f),
				Ease(In,Cubic,0.5f * Slow::GetInstance()->GetSlowPower(),m_Scale.z,0.0f), };

	if (m_Scale.x <= 0.1f) {
		m_Alive = false;
	}

	if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
		m_ViewEffect = false;
	}
	m_UpPos.x = Ease(In, Cubic, m_Frame, m_UpPos.x, 700.0f);
	m_DownPos.x = Ease(In, Cubic, m_Frame, m_DownPos.x, 900.0f);
	m_Alpha = Ease(In, Cubic, m_Frame, m_Alpha, 0.0f);
}
//エフェクトの生成
void TutorialEnemy::BirthEffect() {
	SlashEffect* effect;
	effect = new SlashEffect(m_Position);
	effect->Initialize();
	slash.push_back(effect);
}