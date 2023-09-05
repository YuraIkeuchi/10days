#pragma once
#include "BaseActor.h"
class LoadBox;

/// タイトルシーン
class EditorSceneActor : public BaseActor {
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
	static const int AREA_NUM = 2;
private:
	bool checkPos[4];
	float IntervalRes;
	float PosY, PosX;
	bool ArgF;
	unique_ptr<InterEnemy>Sample;
	InterEnemy::PosSt ini_enmeypos;
	unique_ptr<IKEObject3d> ground;
	IKEModel* model;
	unique_ptr<IKEObject3d> skydome;
	XMFLOAT2 m_AddOffset = {};
	std::vector<unique_ptr<InterEnemy>> enemys;
	unique_ptr<IKETexture> tex[AREA_NUM];
};