#include "EditorSceneActor.h"

#include "CsvLoader.h"
#include"Easing.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "Player.h"
#include "Helper.h"
#include "Slow.h"
#include"NormalEnemy.h"
#include "BackObj.h"
#include "Timer.h"

void EditorSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
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

	int Quantity = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/enemy.csv", "Enemy_Quantity")));

	IKEModel* model=IKEModel::LoadFromOBJ("samurai2");

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
		enemy[i]->SetPosition(EPos[i]);
		enemy[i]->SetEnemyType(InitEnemyMoveType[i]);
		enemy[i]->EditPos(EPos[i]);
		enemy[i]->Initialize();
		enemys.emplace_back(enemy[i].get());
	}

	//敵
	Sample.reset(new NormalEnemy());
	Sample->Initialize();


	Timer::GetInstance()->Initialize();

	//背景
	BackObj::GetInstance()->Initialize();
}

void EditorSceneActor::Finalize() {
}

void EditorSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
		camerawork->Update(camera);
	lightgroup->Update();
	skydome->Update();
	Sample->SetColor({ 1,1,1,0.5f });
	Player::GetInstance()->Update();
	BackObj::GetInstance()->Update();
	Slow::GetInstance()->Update();
	
	if (!delf) {
		//各クラス更新
		
		if (checkPos[2] || checkPos[3])
			Sample->SetResPos(ini_enmeypos, PosY);
		//	Sample->SetPosX(PosX);

		if (checkPos[0] || checkPos[1])
			Sample->SetResPos(ini_enmeypos, PosX);
		//Sample->SetPosZ(PosY);

		if (ArgF)
		{
			//初期化
			InterEnemy* l_enemy = new NormalEnemy();
			if (checkPos[0] || checkPos[1])
				l_enemy->SetResPos(ini_enmeypos, PosX);

			if (checkPos[2] || checkPos[3])
				l_enemy->SetResPos(ini_enmeypos, PosY);

			l_enemy->SetEnemyType(m_EnemyType);
			l_enemy->Initialize();
			l_enemy->SetMovingTime(timer);
			l_enemy->EditPos(l_enemy->GetPosition());
		
			enemys.emplace_back(l_enemy);
			ArgF = false;
		}

		for (auto i = 0; i < enemys.size(); i++) {
			if (enemys[i] == nullptr)continue;
			enemys[i]->Update();
		}
	}
	else
	{
		enemys.pop_back();
		delf = false;
	}
	Sample->Update();
	Sample->SetStopF(true);
	if(timerstart)
	Timer::GetInstance()->Update();
}

void EditorSceneActor::Draw(DirectXCommon* dxCommon) {
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
void EditorSceneActor::FrontDraw(DirectXCommon* dxCommon) {

}
//ポストエフェクトかかる
void EditorSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();

	skydome->Draw();
	if (viewf)
	{
		for (auto i = 0; i < enemys.size(); i++) {
			if (enemys[i]->GetMovingT() == TimeRap)
			{
if (enemys[i] == nullptr)continue;

		enemys[i]->Draw(dxCommon);
			}
		}
	}

	else {
		for (auto i = 0; i < enemys.size(); i++) {
			if (enemys[i] == nullptr)continue;

			enemys[i]->Draw(dxCommon);
		}
	}

	Sample->Draw(dxCommon);
	BackObj::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//導入しーんの更新
void EditorSceneActor::IntroUpdate(DebugCamera* camera) {

}
//プレイ中の更新
void EditorSceneActor::MainUpdate(DebugCamera* camera) {

}

void EditorSceneActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}

void EditorSceneActor::ImGuiDraw() {
	ImGui::Begin("EDIT");
	if (Slow::GetInstance()->GetSlow()) {
		//ImGui::Text("PUSH A!!!");
	}
	ImGui::SliderFloat("Interval", &IntervalRes, 0.f, 180.f);
	ImGui::SliderFloat("PosX", &PosX, 0, 400);
	ImGui::SliderFloat("PosY", &PosY, 0, 400);
	if(ImGui::Button("ArgmentRED",ImVec2(100,100)))
	{
		ArgF = true;
		m_EnemyType = 0;
	}
	if (ImGui::Button("ArgmentGREEN", ImVec2(100, 100)))
	{
		ArgF = true;
		m_EnemyType = 1;
	}
	if (ImGui::Button("ArgmentBLUE", ImVec2(100, 100)))
	{
		ArgF = true;
		m_EnemyType = 2;
	}
	//
	ImGui::Checkbox("Z_Max", &checkPos[0]);
	ImGui::Checkbox("Z_Min", &checkPos[1]);

	ImGui::Checkbox("X_Max", &checkPos[2]);
	ImGui::Checkbox("X_Min", &checkPos[3]);


	ImGui::SliderFloat("PosX", &PosX, -10, 10);
	ImGui::SliderFloat("PosZ", &PosY, -10, 10);

	ImGui::InputInt("Timer", &timer);
	if (checkPos[0]) {
		ini_enmeypos = InterEnemy::PosSt::UPRES;
		for(auto i=0;i<4;i++)
		{
			if (i == 0)continue;
			checkPos[i] = false;
		}
		//
	}
	else if (checkPos[1]) {
		ini_enmeypos = InterEnemy::PosSt::BOTTOMRES;
		for (auto i = 0; i < 4; i++)
		{
			if (i == 1)continue;
			checkPos[i] = false;
		}
		//
	}
	else if (checkPos[2]) {
		ini_enmeypos = InterEnemy::PosSt::RIGHTRES;
		for (auto i = 0; i < 4; i++)
		{
			if (i == 2)continue;
			checkPos[i] = false;
		}
		//
	}
	else if (checkPos[3]) {
		ini_enmeypos = InterEnemy::PosSt::LEFTRES;
		for (auto i = 0; i < 4; i++)
		{
			if (i ==3)continue;
			checkPos[i] = false;
		}
	}

	ImGui::Checkbox("Timerstart", &timerstart);

	if(enemys.size()>0&& ImGui::Button("deleteEnemy",ImVec2(50,20)))
	{
		delf = true;
	}
	if(ImGui::Button("Save",ImVec2(150,150)))
	{
		FileWriting();
	}
	ImGui::InputInt("viewEnemy TimeRap", &TimeRap);
	//ImGui::SameLine();
	ImGui::Checkbox("View", &viewf);

	if (ImGui::Button("ResetP", ImVec2(50, 50))) {
		ResetF = true;
	}
	ImGui::End();

	Timer::GetInstance()->ImGuiDraw();
	//enemys->ImGuiDraw();
	Player::GetInstance()->ImGuiDraw();
	//Slow::GetInstance()->ImGuiDraw();
}
void EditorSceneActor::FileWriting()
{

	file.open("esources/csv/enemy.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Resources/csv/enemy.csv"); // ファイルパスを指定する
	ofs << "Enemy_Quantity" << "," << enemys.size() << std::endl;

	for (int i = 0; i < enemys.size(); i++)
	{
		ofs << "Number" << "," <<i << std::endl;
		ofs << "POP" << "," <<enemys[i]->GetEditPos().x
			<< "," << enemys[i]->GetEditPos().y
			<< "," << enemys[i]->GetEditPos().z << std::endl;
		ofs << "ResCount" << "," << enemys[i]->GetMovingT() << endl;
		ofs << "MoveType" << "," << enemys[i]->GetState() << endl;
		ofs << "EnemyType" << "," << enemys[i]->GetEnemyType() << endl;
		ofs << "/*--------------------------------*/" << endl;
	}
	if (ResetF) {
		for (auto i = 0; i < enemys.size(); i++) {
			enemys[i]->SetPosition(enemys[i]->GetEditPos());
		}
}
}

