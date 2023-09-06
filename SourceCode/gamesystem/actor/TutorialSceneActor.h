#pragma once
#include"BaseActor.h"
#include "TutorialText.h"
#include "MessageWindow.h"
#include "TutorialEnemy.h"
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
	void BirthEnemy(bool Move,bool End);
private:
	static const int AREA_NUM = 4;
	static const int ENEMY_MAX = 3;
private:
	unique_ptr<IKESprite> window;
	unique_ptr<TutorialText> text_;
	unique_ptr<IKEObject3d> ground;
	IKEModel* model;
	unique_ptr<IKEObject3d> skydome;
	XMFLOAT2 m_AddOffset = {};
	std::vector<InterEnemy*> enemys;
	unique_ptr<IKETexture> tex[AREA_NUM];
	int m_TexTimer = {};
	int m_EndCount = {};
	int m_ResetTimer = {};

	enum AttackState {
		ATTACK_INTRO,
		ATTACK_EXPLAIN,
		ENEMY_BIRTH,
		ENEMY_DEATH,
		ENEMY_SLOW,
		ENEMY_INTERVAL,
		ENEMY_END,
	}_AttackState = ATTACK_INTRO;

	XMFLOAT2 window_size = { 0.f,0.f };
	float m_Alpha = 1.0f;

	bool m_Vanish = false;

	int m_EnemyCount = {};

	bool m_TutorialEnd = false;
};

