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

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ 0.0f,0.0f,8.0f });
	Player::GetInstance()->Initialize();

	int Quantity = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/enemy.csv", "Enemy_Quantity")));

	enemy.resize(Quantity);
	EPos.resize(Quantity);
	EnemyMoveType.resize(Quantity);
	InitEnemyMoveType.resize(Quantity);
	ResCount.resize(Quantity);
	blood.resize(Quantity);
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

		blood[i].reset(new EnemyDeadEffect(IKETexture::Create(ImageManager::BLOOD, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1, 1 })));
		blood[i]->object->TextureCreate();
		blood[i]->object->SetRotation({ 90.0f, 0.0f, 0.0f });
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
	BackObj::GetInstance()->Update();

	Timer::GetInstance()->Update();
	if (!Timer::GetInstance()->GetStop()) {
		Player::GetInstance()->Update();
		Slow::GetInstance()->Update();
		for(auto i=0;i<enemy.size();i++)
		{
			if (enemy[i] == nullptr)continue;
			enemy[i]->Update();
		}

	}
	if (!Slow::GetInstance()->GetCheck()) {
		for (auto i = 0; i < enemy.size(); i++)
		{
			if (enemy[i] == nullptr)continue;
			if (enemy[i]->CheckCollide() && !enemy[i]->GetHitCheck()) {
				enemy[i]->SetHitCheck(true);
				Slow::GetInstance()->SetCheck(true);
				break;
			}
		}
	}

	//for (auto i = 0; i < enemy.size(); i++) {
	//	if (enemy[i] == nullptr)continue;
	//	if (enemy[i]->GetHitCheck()) {
	//		enemy[i]->AttackCollide();
	//	}
	//}
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

	ParticleEmitter::GetInstance()->Update();

	//敵の削除
	for (int i = 0; i < enemy.size(); i++) {
		if (enemy[i] == nullptr) {
			continue;
		}
		if (enemy[i]->GetDeath() && !enemy[i]->GetDamage()) {
			if (!enemy[i]->GetDestroy()) {
				for (auto& m : blood)
			{
				if (m->counter == 0)
				{
					m->object->SetPosition({ enemy[i]->GetPosition().x,0.3f,enemy[i]->GetPosition().z });
					m->counter = 1;
					break;
				}
			}
				//スコアを手に入れた瞬間
				ui->SetMag(true);
				if (ScoreManager::GetInstance()->GetMagnification() < 5) {
					ScoreManager::GetInstance()->SetMagnification(ScoreManager::GetInstance()->GetMagnification() + 1);
				}
				m_AddScore = (ScoreManager::GetInstance()->GetMagnification() * 1);
				BirthScoreText(1, ScoreManager::GetInstance()->GetMagnification());
				ScoreManager::GetInstance()->SetRealScore(ScoreManager::GetInstance()->GetRealScore() + (m_AddScore * 10));
				m_AddScore = 0;
			}
			enemy[i]->SetDamage(true);
		}

		if (!enemy[i]->GetAlive()) {
			enemy.erase(cbegin(enemy) + i);
		}
	}

	//コンボが途切れると倍率がもどる
	if (Player::GetInstance()->GetDamage() && Player::GetInstance()->GetDamageTimer() == 1) {
		ScoreManager::GetInstance()->SetMagnification(0);
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

	//血だまりのエフェクト
	for (int i = 0; i < blood.size(); i++)
	{
		if (blood[i]->counter == 0)
		{
			continue;
		}

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
	ScoreManager::GetInstance()->Update();
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
	BackObj::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (auto& m : blood)
	{
		m->object->Draw();
	}
	IKETexture::PostDraw();
	for (auto i = 0; i < enemy.size(); i++)
	{
		if (enemy[i] == nullptr)continue;
		enemy[i]->Draw(dxCommon);
	}
	//ground->Draw();
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();
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
	///*ImGui::Begin("FIRST");
	//if (Slow::GetInstance()->GetSlow()) {
	//	ImGui::Text("PUSH A!!!");
	//}
	//ImGui::End();
	//
	//}*/
	//Slow::GetInstance()->ImGuiDraw();
	//Player::GetInstance()->ImGuiDraw();
	//for (auto i = 0; i < enemy.size(); i++)
	//{
	//	if (enemy[i] == nullptr)continue;
	//	enemy[i]->ImGuiDraw();
	//}
	//ScoreManager::GetInstance()->ImGuiDraw();
}
//倍率スコアの生成
void FirstStageActor::BirthScoreText(const int EnemyCount, const int Magnification) {
	MagText* newtext;
	newtext = new MagText(EnemyCount, Magnification);
	newtext->Initialize();
	magtext.push_back(newtext);
}