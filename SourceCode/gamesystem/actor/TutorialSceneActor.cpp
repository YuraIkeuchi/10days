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

	
	//地面
	ground.reset(new IKEObject3d());
	ground->Initialize();
	ground->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::GROUND));
	ground->SetScale({ 1.f,1.f,1.f });
	ground->SetPosition({ 0.0f,5.0f,0.0f });
	ground->SetTiling(10.0f);
	
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

	enemy.reset(new NormalEnemy());
	enemy->Initialize();

	//テクスチャ
	for (int i = 0; i < AREA_NUM; i++) {
		tex[i].reset(IKETexture::Create(ImageManager::AREA, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		tex[i]->TextureCreate();

		tex[i]->SetRotation({ 90.0f,0.0f,0.0f });
		tex[i]->SetColor({ 1.0f,0.0,0.0f,0.5f });
	}

	tex[0]->SetPosition({ 0.0f,0.0f,8.0f });
	tex[1]->SetPosition({ 0.0f,0.0f,-8.0f });
	tex[2]->SetPosition({ 9.3f,0.0f,0.0f });
	tex[3]->SetPosition({ -9.3f,0.0f,0.0f });
	tex[0]->SetScale({ 2.0f,0.1f,0.1f });
	tex[1]->SetScale({ 2.0f,0.1f,0.1f });
	tex[2]->SetScale({ 0.1f,1.6f,0.1f });
	tex[3]->SetScale({ 0.1f,1.6f,0.1f });

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
	ground->Update();
	ground->UpdateWorldMatrix();
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
		SceneManager::GetInstance()->ChangeScene("FIRSTSTAGE");
	}

	for (int i = 0; i < AREA_NUM; i++) {
		tex[i]->Update();
	}
	ParticleEmitter::GetInstance()->Update();
}

void TutorialSceneActor::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
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
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	window->Draw();
	if (!m_Vanish) {
		text_->SpriteDraw(dxCommon);
	}
	
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	TutorialTask::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//ポストエフェクトかかる
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw();
	Player::GetInstance()->Draw(dxCommon);
	enemy->Draw(dxCommon);
	ground->Draw();
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (int i = 0; i < AREA_NUM; i++) {
		tex[i]->Draw();
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
	ImGui::Begin("TUTORIAL");
	ImGui::Text("SizeX:%f", window_size.x);
	ImGui::Text("SizeY:%f", window_size.y);
	ImGui::Text("Timer:%d", m_TexTimer);
	ImGui::End();
	Player::GetInstance()->ImGuiDraw();
	camerawork->ImGuiDraw();
}

//移動
void TutorialSceneActor::MoveState() {
	m_TexTimer++;
	if (m_TexTimer == 100) {
		text_->SelectText(TextManager::MOVE);
	}

	if (Player::GetInstance()->GetMoveTimer() == 300) {
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
		if (m_TexTimer == 200) {
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
		if (m_TexTimer >= 200 && !Player::GetInstance()->GetAttack()) {
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
		if (m_TexTimer == 150) {
			text_->SelectText(TextManager::ATTACK3);
		}
		if (m_TexTimer == 250) {
			m_TexTimer = {};
			_AttackState = ENEMY_DEATH;
			TutorialTask::GetInstance()->SetMission(false);
		}
	}
	else if (_AttackState == ENEMY_DEATH) {
		if (!m_Vanish) {
			m_TexTimer++;
		}
		if (m_TexTimer == 100) {
			text_->SelectText(TextManager::ATTACK4);
		}
	}
}
//終わり
void TutorialSceneActor::EndState() {

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