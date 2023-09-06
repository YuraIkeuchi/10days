#include "TutorialSceneActor.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "Player.h"
#include "Helper.h"
#include "Slow.h"
#include "Timer.h"
#include "SceneManager.h"
#include "TutorialTask.h"
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

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	//地面
	ground.reset(new IKEObject3d());
	ground->Initialize();
	ground->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::GROUND));
	ground->SetScale({ 2.f,1.f,2.f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetAddOffset(0.5f);
	ground->VertexCheck();

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

	tex[0]->SetPosition({ 0.0f,2.0f,8.0f });
	tex[1]->SetPosition({ 0.0f,2.0f,-8.0f });
	tex[2]->SetPosition({ 9.3f,2.0f,0.0f });
	tex[3]->SetPosition({ -9.3f,2.0f,0.0f });
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
	if (!TutorialTask::GetInstance()->GetStop()) {
		Player::GetInstance()->TutorialUpdate();
		Slow::GetInstance()->Update();
	}
	//スキップの更新
	SkipUpdate();
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
	text_->SpriteDraw(dxCommon);

	IKESprite::PostDraw();
	IKESprite::PreDraw();
	TutorialTask::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//ポストエフェクトかかる
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	//skydome->Draw();
	Player::GetInstance()->Draw(dxCommon);
	enemy->Draw(dxCommon);
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
	if (Slow::GetInstance()->GetSlow()) {
		ImGui::Text("PUSH A!!!");
	}
	ImGui::Text("EndCount:%d", m_EndCount);
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
}
//攻撃
void TutorialSceneActor::AttackState() {
	if (_AttackState == ATTACK_INTRO) {
		m_TexTimer++;
		if (m_TexTimer == 200) {
			m_TexTimer = {};
			_AttackState = ATTACK_EXPLAIN;
			text_->SelectText(TextManager::ATTACK2);
			TutorialTask::GetInstance()->SetStop(false);
		}
	}
	else if (_AttackState == ATTACK_EXPLAIN) {
		m_TexTimer++;
		if (m_TexTimer == 200) {
			m_TexTimer = {};
			_AttackState = ENEMY_BIRTH;
			TutorialTask::GetInstance()->SetMission(true);
			camerawork->SetLookEnemy(true);
		}
	}
	else if (_AttackState == ENEMY_BIRTH) {
		m_TexTimer++;
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
		m_TexTimer++;
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