#include "TutorialSceneActor.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "Player.h"
#include "Helper.h"
#include "Slow.h"
#include "Timer.h"
#include "SceneManager.h"
#include "TutorialTask.h"
#include "BackObj.h"
#include "ScoreManager.h"
#include "SceneChanger.h"
#include "Random.h"
//状態遷移
/*stateの並び順に合わせる*/
void (TutorialSceneActor::* TutorialSceneActor::TutorialTable[])() = {
	&TutorialSceneActor::MoveState,//移動
	&TutorialSceneActor::AttackState,//攻撃
	& TutorialSceneActor::EndState,//終わり
};

void TutorialSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	m_SceneState = SceneState::IntroState;

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ 0.0f,0.0f,8.0f });
	Player::GetInstance()->Initialize();

	//タイマー
	Timer::GetInstance()->Initialize();

	//テキスト
	text_ = make_unique<TutorialText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::INTRO);

	//チュートリアルタスク
	TutorialTask::GetInstance()->Initialize();

	//背景
	BackObj::GetInstance()->Initialize();

	window = IKESprite::Create(ImageManager::TUTORIAL, {});
	window->SetAnchorPoint({ 0.5f,0.5f });
	window->SetPosition({ WinApp::window_width / 2.f,WinApp::window_height - 100 });
	window->SetSize({1300.0f, 220.0f});
	window_size = { 0.f,0.f };

	window2 = IKESprite::Create(ImageManager::TUTORIAL, {});
	window2->SetPosition({ -20.0f, 0.0f });
	window2->SetSize({ 300.0f, 100.0f });


	const int TextCount = TEXT_MAX;
	const float l_Width_Cut = 256.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Skip_Text.size(); i++) {
		//ひとけた目
		Skip_Text[i] = IKESprite::Create(ImageManager::SKIP, { 0.0f,0.0f });
		int number_index_y = i / TextCount;
		int number_index_x = i % TextCount;
		Skip_Text[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Skip_Text[i]->SetPosition({ -256.0f,20.0f });
		m_Position[i] = { -256.0f,20.0f };
		Skip_Text[i]->SetSize({ l_Width_Cut,l_Height_Cut });
	}
	//スコア
	ScoreManager::GetInstance()->Initialize();
	//スロー
	Slow::GetInstance()->Initialize();

	//UI
	ui = make_unique<UI>();
	ui->Initialize();
}

void TutorialSceneActor::Finalize() {
	blood.clear();
}

void TutorialSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	//状態移行(stateに合わせる)
	(this->*TutorialTable[static_cast<size_t>(nowstate_)])();
	//各クラス更新
	camerawork->TutorialUpdate(camera);
	lightgroup->Update();
	window->SetSize(window_size);
	window->SetColor({ 1.0f,1.0f,1.0f,m_Alpha });
	BackObj::GetInstance()->Update();

	if (!TutorialTask::GetInstance()->GetStop()) {
		Player::GetInstance()->TutorialUpdate();
		Slow::GetInstance()->Update();
	}
	//スキップの更新
	SkipUpdate();

	//プレイヤーが手前に来たら消す
	if (Player::GetInstance()->GetPosition().z <= -7.0f) {
		m_Vanish = true;
	}
	else {
		m_Vanish = false;
	}

	if (m_Vanish) {
		m_Alpha = Ease(In, Cubic, 0.8f, m_Alpha, 0.0f);
	}
	else {
		m_Alpha = Ease(In, Cubic, 0.8f, m_Alpha, 1.0f);
	}

	//スキップテキスト
	if (nowstate_ != state::END) {
		if (m_EndCount == 0) {
			m_AfterPos[SKIP_Y].x = 10.0f;
			m_AfterPos[ONE_MORE].x = -256.0f;
			m_AfterPos[GAME_Y].x = -256.0f;
		}
		else if (m_EndCount >= 1) {
			m_AfterPos[SKIP_Y].x = -256.0f;
			m_AfterPos[ONE_MORE].x = 10.0f;
			m_AfterPos[GAME_Y].x = -256.0f;
		}
	}
	else {
		m_AfterPos[SKIP_Y].x = -256.0f;
		m_AfterPos[ONE_MORE].x = -256.0f;
		m_AfterPos[GAME_Y].x = 10.0f;
	}

	m_Position[SKIP_Y].x = Ease(In, Cubic, 0.5f, m_Position[SKIP_Y].x, m_AfterPos[SKIP_Y].x);
	m_Position[ONE_MORE].x = Ease(In, Cubic, 0.5f, m_Position[ONE_MORE].x, m_AfterPos[ONE_MORE].x);
	m_Position[GAME_Y].x = Ease(In, Cubic, 0.5f, m_Position[GAME_Y].x, m_AfterPos[GAME_Y].x);
	for (int i = 0; i < Skip_Text.size(); i++) {
		Skip_Text[i]->SetPosition(m_Position[i]);
	}
	//ゲーム終了
	if (m_EndCount == m_TargetCount) {
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	if (SceneChanger::GetInstance()->GetChange()) {
		SceneManager::GetInstance()->ChangeScene("FIRSTSTAGE");
		SceneChanger::GetInstance()->SetChange(false);
	}

	ParticleEmitter::GetInstance()->Update();

	//敵の削除
	for (int i = 0; i < enemys.size(); i++) {
		if (enemys[i] == nullptr) {
			continue;
		}

		if (enemys[i]->GetDeath() && !enemys[i]->GetDamage()) {
			ui->SetMag(true);
			for (auto& m : blood)
			{
				if (m->counter == 0)
				{
					m->object->SetPosition({ enemys[i]->GetPosition().x,0.3f,enemys[i]->GetPosition().z});
					m->counter = 1;
					break;
				}
			}
			m_EnemyCount--;
			if (ScoreManager::GetInstance()->GetMagnification() < 5) {
				ScoreManager::GetInstance()->SetMagnification(ScoreManager::GetInstance()->GetMagnification() + 1);
			}
			m_AddScore = (ScoreManager::GetInstance()->GetMagnification() * 1);
			BirthScoreText(1, ScoreManager::GetInstance()->GetMagnification());
			ScoreManager::GetInstance()->SetRealScore(ScoreManager::GetInstance()->GetRealScore() + (m_AddScore * 10));
			m_AddScore = 0;
			enemys[i]->SetDamage(true);
		}

		if (!enemys[i]->GetAlive()) {
			enemys.erase(cbegin(enemys) + i);
		}
	}

	for (auto i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}
	//血だまりのエフェクト
	for (int i = 0; i < blood.size(); i++)
	{
		//拡大
		if (0 < blood[i]->counter && blood[i]->counter < 60)
		{
			blood[i]->counter++;
			float scale = blood[i]->object->GetScale().x;
			scale += 0.05f;
			scale = min(0.4f, scale);
			blood[i]->object->SetScale({ scale, scale, scale });
		}
		//消える
		else
		{
			XMFLOAT4 color = blood[i]->object->GetColor();
			color.w -= 0.01f;
			color.w = max(0, color.w);
			blood[i]->object->SetColor(color);
			if (color.w <= 0)
			{
				blood.erase(cbegin(blood) + i);
			}
		}
	}
	//更新
	for (auto& m : blood)
	{
		m->object->Update();
	}

	//倍率UIの表示
	if (!Player::GetInstance()->GetAttack()) {
		ui->SetMag(false);
	}
	ui->Update();

	//倍率テキスト
	for (auto i = 0; i < magtext.size(); i++)
	{
		if (magtext[i] == nullptr)continue;
		magtext[i]->Update();
	}
	//テキストの削除
	for (int i = 0; i < magtext.size(); i++) {
		if (magtext[i] == nullptr) {
			continue;
		}

		if (!magtext[i]->GetAlive()) {
			magtext.erase(cbegin(magtext) + i);
		}
	}
	ScoreManager::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();

	//タイマーを図る
	if (!Slow::GetInstance()->GetSlow()) {
		//Timer::GetInstance()->Update();
		PlayPostEffect = false;
		radPower -= addPower;
		radPower = max(0, radPower);
		postEffect->SetRadPower(radPower);
	}
	else {
		PlayPostEffect = true;
		radPower += addPower;
		radPower = min(radPower, 10.0f);
		postEffect->SetRadPower(radPower);
	}
	ScoreManager::GetInstance()->Update();
}

void TutorialSceneActor::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
		ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw();
		dxCommon->PostDraw();
	}
}
//ポストエフェクトかからない
void TutorialSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	ui->FrontDraw();
	window2->Draw();
	for (int i = 0; i < Skip_Text.size(); i++) {
		Skip_Text[i]->Draw();
	}
	IKESprite::PostDraw();
	for (auto i = 0; i < enemys.size(); i++)
	{
		if (enemys[i] == nullptr)continue;
		enemys[i]->EffectDraw(dxCommon);
	}
	//倍率テキスト
	for (auto i = 0; i < magtext.size(); i++)
	{
		if (magtext[i] == nullptr)continue;
		magtext[i]->Draw();
	}
	IKESprite::PreDraw();
	ui->BackDraw();
	IKESprite::PostDraw();
	
	IKESprite::PreDraw();
	TutorialTask::GetInstance()->Draw();
	IKESprite::PostDraw();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	window->Draw();
	if (!m_Vanish && nowstate_ != state::END) {
		text_->SpriteDraw(dxCommon);
	}

	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//ポストエフェクトかかる
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	if (!TutorialTask::GetInstance()->GetStop()) {
		Player::GetInstance()->Draw(dxCommon);
	}
	for (auto i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw(dxCommon);
	}
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (auto& m : blood)
	{
		m->object->Draw();
	}
	IKETexture::PostDraw();
}
//導入しーんの更新
void TutorialSceneActor::IntroUpdate(DebugCamera* camera) {

}
//プレイ中の更新
void TutorialSceneActor::MainUpdate(DebugCamera* camera) {

}

void TutorialSceneActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}

void TutorialSceneActor::ImGuiDraw() {
	
}

//移動
void TutorialSceneActor::MoveState() {
	m_TexTimer++;
	if (m_TexTimer == 200) {
		text_->SelectText(TextManager::MOVE);
	}

	if (Player::GetInstance()->GetMoveTimer() >= 250 && !m_Vanish) {
		nowstate_ = state::ATTACK;
		text_->SelectText(TextManager::ATTACK);
		m_TexTimer = 0;
		TutorialTask::GetInstance()->SetStop(true);
	}

	window_size.x = Ease(In, Cubic, 0.8f, window_size.x, 1300);
	window_size.y = Ease(In, Cubic, 0.8f, window_size.y, 223);
	//window->SetSize(window_size);
}
//攻撃
void TutorialSceneActor::AttackState() {
	if (_AttackState == ATTACK_INTRO) {
		if (!m_Vanish) {
			m_TexTimer++;
		}
		if (m_TexTimer == 100) {
			m_TexTimer = {};
			_AttackState = ATTACK_EXPLAIN;
			text_->SelectText(TextManager::ATTACK2);//Aで攻撃ができるぞ
			TutorialTask::GetInstance()->SetStop(false);
		}
	}
	else if (_AttackState == ATTACK_EXPLAIN) {
		if (!m_Vanish) {
			m_TexTimer++;
		}
		if (m_TexTimer >= 150 && !Player::GetInstance()->GetAttack()) {
			BirthEnemy(false,false);
			m_TexTimer = {};
			_AttackState = ENEMY_BIRTH;//敵が出てきたぞ
			TutorialTask::GetInstance()->SetMission(true);
			camerawork->SetLookEnemy(true);
		}
	}
	else if (_AttackState == ENEMY_BIRTH) {
		if (!Player::GetInstance()->GetAttack()) {
			TutorialTask::GetInstance()->SetStop(true);
		}
		if (!m_Vanish) {
			m_TexTimer++;
		}
		if (m_TexTimer == 50) {
			text_->SelectText(TextManager::ATTACK3);
		}
		if (m_TexTimer == 200) {
			TutorialTask::GetInstance()->SetStop(false);
			m_TexTimer = {};
			_AttackState = ENEMY_NEAR;
			TutorialTask::GetInstance()->SetMission(false);
			text_->SelectText(TextManager::ATTACK4);
		}
	}
	else if (_AttackState == ENEMY_NEAR) {
		if (Slow::GetInstance()->GetSlow()) {
			TutorialTask::GetInstance()->SetStop(true);
			m_TexTimer++;
			Slow::GetInstance()->SetTutorial(true);
			if (m_TexTimer == 1) {
				text_->SelectText(TextManager::ATTACK5);
			}
			if (m_TexTimer == 100) {
				m_TexTimer = {};
				text_->SelectText(TextManager::ATTACK6);
				_AttackState = ENEMY_DEATH;
				TutorialTask::GetInstance()->SetStop(false);
			}
		}
	}
	else if (_AttackState == ENEMY_DEATH) {
		if (m_EnemyCount == 0) {	//敵を倒した瞬間
			Slow::GetInstance()->SetTutorial(false);
			m_TexTimer = {};
			_AttackState = ENEMY_INTERVAL;
			text_->SelectText(TextManager::ATTACK7);
			Slow::GetInstance()->SetSlow(false);
		}
	}
	else if (_AttackState == ENEMY_INTERVAL) {
		if (!m_Vanish) {
			m_TexTimer++;
		}

		if (m_TexTimer == 150) {
			text_->SelectText(TextManager::ATTACK8);
		}
		else if (m_TexTimer == 300) {
			text_->SelectText(TextManager::ATTACK9);
		}
		else if (m_TexTimer == 450) {
			text_->SelectText(TextManager::ATTACK10);
			text_->ChangeColor(0, { 1.0f,0.0f,0.0f });
			text_->ChangeColor(1, { 0.0f,1.0f,0.0f });
			text_->ChangeColor(2, { 0.0f,0.0f,1.0f });
		}
		else if (m_TexTimer == 600) {
			text_->SelectText(TextManager::ATTACK11);
		}
		else if (m_TexTimer == 750) {
			text_->SelectText(TextManager::ATTACK12);
		}
		else if (m_TexTimer == 900) {
			text_->SelectText(TextManager::ATTACK13);
		}
		else if (m_TexTimer == 1050) {
			text_->SelectText(TextManager::ATTACK14);
		}
		else if (m_TexTimer == 1200) {
			text_->SelectText(TextManager::ATTACK15);
		}
		else if (m_TexTimer == 1350) {
			text_->SelectText(TextManager::ATTACK16);
		}
		else if (m_TexTimer == 1500) {
			text_->SelectText(TextManager::END);
		}
		else if (m_TexTimer == 1600) {
			nowstate_ = state::END;
			m_TexTimer = {};
		}
		if (m_EnemyCount == 0) {
			Slow::GetInstance()->SetSlow(false);
			m_BirthTimer++;
			if (m_BirthTimer == 150) {
				BirthEnemy(false, false);
				m_BirthTimer = 0;
			}
		}
	}
}
//終わり
void TutorialSceneActor::EndState() {
	m_TargetCount = 1;
	window_size.x = Ease(In, Cubic, 0.8f, window_size.x, 0);
	window_size.y = Ease(In, Cubic, 0.8f, window_size.y, 0);
	if (m_EnemyCount == 0) {
		m_TexTimer++;
		if (m_TexTimer >= 100) {
			BirthEnemy(false, true);
			m_TexTimer = {};
		}
	}
}
//スキップの更新
void TutorialSceneActor::SkipUpdate() {
	Input* input = Input::GetInstance();
	//二回ボタンを押すとチュートリアル終了する
	if ((input->TriggerButton(input->Y))) {
		Audio::GetInstance()->PlayWave("Resources/audio/botton.wav", 0.2f);
		m_EndCount++;
	}
	//一定時間立つとスキップ状態リセットされる
	if (m_EndCount != 0) {
		m_ResetTimer++;
	}

	if (m_ResetTimer == 100) {
		m_EndCount = {};
		m_ResetTimer = {};
	}
}
//敵の生成
void TutorialSceneActor::BirthEnemy(bool Move,bool End) {
	if (!End) {
		InterEnemy* newEnemy;
		newEnemy = new TutorialEnemy();	
		newEnemy->SetPosition({ 0.0f,3.0f,0.0f });
		newEnemy->SetMove(Move);
		if (_AttackState == ATTACK_EXPLAIN) {
			newEnemy->SetEnemyType(0);
			newEnemy->SetCheckMiss(false);
		}
		else {
			int num = Random::GetRanNum(0, 2);
			newEnemy->SetEnemyType(num);
			newEnemy->SetCheckMiss(true);
		}
		newEnemy->Initialize();
		newEnemy->SetEffectMove(false);
		
		enemys.push_back(newEnemy);
		m_EnemyCount++;

		EnemyDeadEffect* newEffect = new EnemyDeadEffect(IKETexture::Create(ImageManager::BLOOD, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1, 1 }));
		newEffect->object->TextureCreate();
		newEffect->object->SetRotation({ 90.0f, 0.0f, 0.0f });
		blood.push_back(newEffect);
	}
	else {
		m_TutorialEnd = true;
		for (int i = 0; i < ENEMY_MAX; i++) {
			InterEnemy* newEnemy;
			newEnemy = new TutorialEnemy();
			if (i == 0) {
				newEnemy->SetPosition({ 0.0f,3.0f,0.0f });
				newEnemy->SetEnemyType(0);
			}else if (i == 1) {
				newEnemy->SetPosition({ 3.0f,3.0f,0.0f });
				newEnemy->SetEnemyType(1);
			}else if (i == 2) {
				newEnemy->SetPosition({ -3.0f,3.0f,0.0f });
				newEnemy->SetEnemyType(2);
			}
			newEnemy->Initialize();
			newEnemy->SetMove(Move);
			newEnemy->SetCheckMiss(true);
			enemys.push_back(newEnemy);
			m_EnemyCount++;

			EnemyDeadEffect* newEffect = new EnemyDeadEffect(IKETexture::Create(ImageManager::BLOOD, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1, 1 }));
			newEffect->object->TextureCreate();
			newEffect->object->SetRotation({ 90.0f, 0.0f, 0.0f });
			blood.push_back(newEffect);
		}
	}
}

//倍率スコアの生成
void TutorialSceneActor::BirthScoreText(const int EnemyCount, const int Magnification) {
	MagText* newtext;
	newtext = new MagText(EnemyCount, Magnification);
	newtext->Initialize();
	magtext.push_back(newtext);
}