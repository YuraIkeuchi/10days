#include "FirstStageActor.h"
#include"Easing.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "Player.h"
#include "Helper.h"
#include "Slow.h"
void FirstStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
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

	//敵
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
}

void FirstStageActor::Finalize() {
}

void FirstStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);

	//各クラス更新
	camerawork->Update(camera);
	lightgroup->Update();
	ground->Update();
	skydome->Update();
	m_AddOffset.x = 0.001f;
	ground->SetAddOffset(m_AddOffset.x);
	Player::GetInstance()->Update();
	Slow::GetInstance()->Update();
	enemy->Update();
	for (int i = 0; i < AREA_NUM; i++) {
		tex[i]->Update();
	}
}

void FirstStageActor::Draw(DirectXCommon* dxCommon) {
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
	} else {
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
void FirstStageActor::FrontDraw(DirectXCommon* dxCommon) {

}
//ポストエフェクトかかる
void FirstStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	ground->Draw();
	skydome->Draw();
	Player::GetInstance()->Draw(dxCommon);
	enemy->Draw(dxCommon);
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (int i = 0; i < AREA_NUM; i++) {
		tex[i]->Draw();
	}
	IKETexture::PostDraw();
}
//導入しーんの更新
void FirstStageActor::IntroUpdate(DebugCamera* camera) {

}
//プレイ中の更新
void FirstStageActor::MainUpdate(DebugCamera* camera) {

}

void FirstStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}

void FirstStageActor::ImGuiDraw() {
	ImGui::Begin("FIRST");
	if (Slow::GetInstance()->GetSlow()) {
		ImGui::Text("PUSH A!!!");
	}
	ImGui::End();

	enemy->ImGuiDraw();
	Player::GetInstance()->ImGuiDraw();
	//Slow::GetInstance()->ImGuiDraw();
}
