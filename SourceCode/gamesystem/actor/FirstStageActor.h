#pragma once
#include"BaseActor.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "NormalEnemy.h"
#include<windows.h>
#include<vector>

class FirstStageActor :public BaseActor
{
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
private:
	void IntroUpdate(DebugCamera* camera)override;		//登場シーン
	void MainUpdate(DebugCamera* camera)override;		//バトルシーン
	void FinishUpdate(DebugCamera* camera)override;		//撃破シーン

	void ImGuiDraw();

private:
	static const int AREA_NUM = 4;
private:
	unique_ptr<IKEObject3d> ground;
	IKEModel* model;
	unique_ptr<IKEObject3d> skydome;
	XMFLOAT2 m_AddOffset = {};
	std::vector <unique_ptr<InterEnemy>> enemy;
	std::vector<int>EnemyMoveType;
	std::vector<int>ResCount;
	unique_ptr<IKETexture> tex[AREA_NUM];
private:
	float radPower = 0;
	const float addPower = 0.5f;
	std::vector<XMFLOAT3>EPos;
};

