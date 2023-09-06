#pragma once
#include"BaseActor.h"
#include "TutorialText.h"
#include "MessageWindow.h"
#include "NormalEnemy.h"
#include<windows.h>
#include<vector>

class TutorialSceneActor :public BaseActor
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
	enum class state {
		MOVE,
		ATTACK,
		END,
	}nowstate_ = state::MOVE;
private:
	static void (TutorialSceneActor::* TutorialTable[])();
	void MoveState();
	void AttackState();
	void EndState();
	void SkipUpdate();
private:
	static const int AREA_NUM = 4;
private:
	unique_ptr<TutorialText> text_;
	unique_ptr<IKEObject3d> ground;
	IKEModel* model;
	unique_ptr<IKEObject3d> skydome;
	XMFLOAT2 m_AddOffset = {};
	unique_ptr<InterEnemy> enemy;
	unique_ptr<IKETexture> tex[AREA_NUM];
	int m_TexTimer = {};
	int m_EndCount = {};
	int m_ResetTimer = {};
};

