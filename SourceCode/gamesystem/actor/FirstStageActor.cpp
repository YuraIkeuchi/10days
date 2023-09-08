﻿#include "FirstStageActor.h"
#include"Easing.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "Player.h"
#include "Helper.h"
#include "Slow.h"
#include "Timer.h"
#include "SceneManager.h"
#include"CsvLoader.h"
#include "BackObj.h"
#include "ScoreManager.h"
#include "SceneChanger.h"

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
	ground->SetScale({ 1.f,1.f,1.f });
	ground->SetPosition({ 0.0f,5.0f,0.0f });
	ground->SetTiling(25.0f);

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

//	enemy.reset(new NormalEnemy());
//	enemy->Initialize();


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

	int Quantity = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/enemy.csv", "Enemy_Quantity")));

	enemy.resize(Quantity);
	EPos.resize(Quantity);
	EnemyMoveType.resize(Quantity);
	InitEnemyMoveType.resize(Quantity);
	ResCount.resize(Quantity);
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/enemy.csv", EPos, "POP");
		LoadCSV::LoadCsvParam_Int("Resources/csv/enemy.csv", EnemyMoveType, "MoveType");
		LoadCSV::LoadCsvParam_Int("Resources/csv/enemy.csv", InitEnemyMoveType, "EnemyType");
		LoadCSV::LoadCsvParam_Int("Resources/csv/enemy.csv", ResCount, "ResCount");

	for (auto i = 0; i < Quantity; i++) {
		
		enemy[i].reset(new NormalEnemy());
		enemy[i]->SetMovingTime(ResCount[i]);
		enemy[i]->SetState(EnemyMoveType[i]);
		enemy[i]->SetEnemyType(InitEnemyMoveType[i]);
		enemy[i]->SetPosition(EPos[i]);
		enemy[i]->Initialize();
	}

	Timer::GetInstance()->Initialize();

	//背景
	BackObj::GetInstance()->Initialize();

	//スコア
	ScoreManager::GetInstance()->Initialize();
	//スロー
	Slow::GetInstance()->Initialize();

	//UI
	ui = make_unique<UI>();
	ui->Initialize();
}

void FirstStageActor::Finalize() {
}

void FirstStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);

	//各クラス更新
	camerawork->Update(camera);
	lightgroup->Update();
	skydome->Update();
	ground->Update();
	ground->UpdateWorldMatrix();
	BackObj::GetInstance()->Update();
	ScoreManager::GetInstance()->Update();
	Timer::GetInstance()->Update();
	if (!Timer::GetInstance()->GetStop()) {
		Player::GetInstance()->Update();
		Slow::GetInstance()->Update();
		for(auto i=0;i<enemy.size();i++)
		{
			if (enemy[i] == nullptr)continue;
			enemy[i]->Update();
		}
	//	enemy->Update();
	}
	//タイマーを図る
	if (!Slow::GetInstance()->GetSlow()) {
		
		for (auto i = 0; i < enemy.size(); i++)
		{
			if (enemy[i] == nullptr)continue;
			enemy[i]->SetSlowMove(false);//Update();
		}
		PlayPostEffect = false;
		radPower -= addPower;
		radPower = max(0, radPower);
		postEffect->SetRadPower(radPower);
	}
	else {
		for (auto i = 0; i < enemy.size(); i++)
		{
			if (enemy[i] == nullptr)continue;
			enemy[i]->SetSlowMove(true);//Update();
		}
		PlayPostEffect = true;
		radPower += addPower;
		radPower = min(radPower, 10.0f);
		postEffect->SetRadPower(radPower);
	}

	//ゲーム終了
	if (Timer::GetInstance()->GetEnd()) {
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	
	if (SceneChanger::GetInstance()->GetChange() && Timer::GetInstance()->GetEnd()) {
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		SceneChanger::GetInstance()->SetChange(false);
	}

	for (int i = 0; i < AREA_NUM; i++) {
		tex[i]->Update();
	}
	ParticleEmitter::GetInstance()->Update();

	//敵の削除
	for (int i = 0; i < enemy.size(); i++) {
		if (enemy[i] == nullptr) {
			continue;
		}

		if (!enemy[i]->GetAlive()) {
			if (!enemy[i]->GetDestroy()) {
				ui->SetMag(true);
				if (ScoreManager::GetInstance()->GetMagnification() < 5) {
					ScoreManager::GetInstance()->SetMagnification(ScoreManager::GetInstance()->GetMagnification() + 1);
				}
				m_AddScore = (ScoreManager::GetInstance()->GetMagnification() * 1);
				BirthScoreText(1, ScoreManager::GetInstance()->GetMagnification());
				ScoreManager::GetInstance()->SetFirstNumber(ScoreManager::GetInstance()->GetFirstNumber() + m_AddScore);
				m_AddScore = 0;
			}
			enemy.erase(cbegin(enemy) + i);
		}
	}

	SceneChanger::GetInstance()->Update();

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
}

void FirstStageActor::Draw(DirectXCommon* dxCommon) {
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
	IKESprite::PreDraw();
	ui->FrontDraw();
	for (auto i = 0; i < enemy.size(); i++)
	{
		if (enemy[i] == nullptr)continue;
		enemy[i]->EffectDraw(dxCommon);
	}
	//倍率テキスト
	for (auto i = 0; i < magtext.size(); i++)
	{
		if (magtext[i] == nullptr)continue;
		magtext[i]->Draw();
	}
	ui->BackDraw();
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//ポストエフェクトかかる
void FirstStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw();
	Player::GetInstance()->Draw(dxCommon);

	for (auto i = 0; i < enemy.size(); i++)
	{
		if (enemy[i] == nullptr)continue;
		enemy[i]->Draw(dxCommon);
	}
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
	////enemy->ImGuiDraw();
	//Player::GetInstance()->ImGuiDraw();
	Slow::GetInstance()->ImGuiDraw();

	//Timer::GetInstance()->ImGuiDraw();
	//ScoreManager::GetInstance()->ImGuiDraw();
}
//倍率スコアの生成
void FirstStageActor::BirthScoreText(const int EnemyCount, const int Magnification) {
	MagText* newtext;
	newtext = new MagText(EnemyCount, Magnification);
	newtext->Initialize();
	magtext.push_back(newtext);
}