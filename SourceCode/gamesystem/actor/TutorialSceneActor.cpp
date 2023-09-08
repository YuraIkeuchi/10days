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
	//オーディオ
	//Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	m_SceneState = SceneState::IntroState;

	//スカイドーム
	skydome.reset(new IKEObject3d());
	skydome->Initialize();
	skydome->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::SKYDOME));
	skydome->SetScale({ 8.f,8.f,8.f });
	skydome->SetPosition({ 0.0f,0.0f,0.0f });
	skydome->VertexCheck();

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

	//スコア
	ScoreManager::GetInstance()->Initialize();
	//スロー
	Slow::GetInstance()->Initialize();

	//UI
	ui = make_unique<UI>();
	ui->Initialize();
}

void TutorialSceneActor::Finalize() {
}

void TutorialSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	//状態移行(stateに合わせる)
	(this->*TutorialTable[static_cast<size_t>(nowstate_)])();
	//各クラス更新
	camerawork->TutorialUpdate(camera);
	lightgroup->Update();
	skydome->Update();
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
	//ゲーム終了
	if (m_EndCount == 2) {
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
			m_EnemyCount--;
			if (ScoreManager::GetInstance()->GetMagnification() < 5) {
				ScoreManager::GetInstance()->SetMagnification(ScoreManager::GetInstance()->GetMagnification() + 1);
			}
			m_AddScore = (ScoreManager::GetInstance()->GetMagnification() * 1);
			BirthScoreText(1, ScoreManager::GetInstance()->GetMagnification());
			ScoreManager::GetInstance()->SetFirstNumber(ScoreManager::GetInstance()->GetFirstNumber() + m_AddScore);
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
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	window->Draw();
	if (!m_Vanish && nowstate_ != state::END) {
		text_->SpriteDraw(dxCommon);
	}
	
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	TutorialTask::GetInstance()->Draw();
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//ポストエフェクトかかる
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	for (auto i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw(dxCommon);
	}
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();

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
	
	Slow::GetInstance()->ImGuiDraw();
	for (auto i = 0; i < enemys.size(); i++) {
		enemys[i]->ImGuiDraw();
	}

	ImGui::Begin("Tuto");
	ImGui::Text("Timer:%d", m_TexTimer);
	ImGui::Text("Count:%d", m_EnemyCount);
	ImGui::Text("State:%d", _AttackState);
	ImGui::End();
}

//移動
void TutorialSceneActor::MoveState() {
	m_TexTimer++;
	if (m_TexTimer == 200) {
		text_->SelectText(TextManager::MOVE);
	}

	if (Player::GetInstance()->GetMoveTimer() >= 300 && !m_Vanish) {
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
		if (!m_Vanish) {
			m_TexTimer++;
		}
		if (m_TexTimer == 50) {
			text_->SelectText(TextManager::ATTACK3);
		}
		if (m_TexTimer == 200) {
			m_TexTimer = {};
			_AttackState = ENEMY_DEATH;
			TutorialTask::GetInstance()->SetMission(false);
			text_->SelectText(TextManager::ATTACK4);
		}
	}
	else if (_AttackState == ENEMY_DEATH) {
		if (!m_Vanish) {
			m_TexTimer++;
		}
		
		if (Slow::GetInstance()->GetSlow()) {
			Slow::GetInstance()->SetTutorial(true);
			text_->SelectText(TextManager::ATTACK5);
		}

		if (m_EnemyCount == 0) {	//敵を倒した瞬間
			Slow::GetInstance()->SetTutorial(false);
			m_TexTimer = {};
			_AttackState = ENEMY_INTERVAL;
			text_->SelectText(TextManager::ATTACK6);
		}
	}
	else if (_AttackState == ENEMY_INTERVAL) {
		if (!m_Vanish) {
			m_TexTimer++;
		}

		if (m_TexTimer == 150) {
			text_->SelectText(TextManager::ATTACK7);
		}
		else if (m_TexTimer == 300) {
			text_->SelectText(TextManager::ATTACK8);
		}
		else if (m_TexTimer == 450) {
			text_->SelectText(TextManager::ATTACK9);
		}
		else if (m_TexTimer == 600) {
			text_->SelectText(TextManager::END);
		}
		else if (m_TexTimer == 750) {
			nowstate_ = state::END;
			m_TexTimer = {};
		}
	}
}
//終わり
void TutorialSceneActor::EndState() {
	window_size.x = Ease(In, Cubic, 0.8f, window_size.x, 0);
	window_size.y = Ease(In, Cubic, 0.8f, window_size.y, 0);
	Helper::GetInstance()->CheckMin(m_TexTimer, 200, 1);
	if(m_TexTimer == 100){
		BirthEnemy(false, true);
	}

	if (m_EnemyCount == 0 && m_TutorialEnd) {
		m_TexTimer = {};
		m_TutorialEnd = false;
	}

}
//スキップの更新
void TutorialSceneActor::SkipUpdate() {
	Input* input = Input::GetInstance();
	//二回ボタンを押すとチュートリアル終了する
	if ((input->TriggerButton(input->X))) {
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
		newEnemy->Initialize();
		newEnemy->SetPosition({ 0.0f,0.0f,0.0f });
		newEnemy->SetMove(Move);
		newEnemy->SetEnemyType(0);
		enemys.push_back(newEnemy);
		m_EnemyCount++;
	}
	else {
		m_TutorialEnd = true;
		for (int i = 0; i < ENEMY_MAX; i++) {
			InterEnemy* newEnemy;
			newEnemy = new TutorialEnemy();
			if (i == 0) {
				newEnemy->SetPosition({ 0.0f,0.0f,0.0f });
				newEnemy->SetEnemyType(0);
			}else if (i == 1) {
				newEnemy->SetPosition({ 3.0f,0.0f,0.0f });
				newEnemy->SetEnemyType(1);
			}else if (i == 2) {
				newEnemy->SetPosition({ -3.0f,0.0f,0.0f });
				newEnemy->SetEnemyType(2);
			}
			newEnemy->Initialize();
			newEnemy->SetMove(Move);
			enemys.push_back(newEnemy);
			m_EnemyCount++;
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