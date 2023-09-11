#pragma once
#include"BaseActor.h"
#include "TutorialText.h"
#include "MessageWindow.h"
#include "TutorialEnemy.h"
#include<windows.h>
#include<vector>
#include "UI.h"
#include "MagText.h"
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
	void BirthScoreText(const int EnemyCount, const int Magnification);
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
	static const int TEXT_MAX = 3;
	static const int ENEMY_MAX = 3;

private:
	//敵死亡時のエフェクト
	struct EnemyDeadEffect
	{
		IKETexture* object = nullptr;
		int counter;

		EnemyDeadEffect(IKETexture* effect)
		{
			object = effect;
			counter = 0;
		}
	};
private:
	array<unique_ptr<IKESprite>, TEXT_MAX> Skip_Text;
	std::vector<EnemyDeadEffect*> blood;
	unique_ptr<IKESprite> window;
	unique_ptr<IKESprite> window2;
	unique_ptr<TutorialText> text_;
	IKEModel* model;
	XMFLOAT2 m_AddOffset = {};
	std::vector<InterEnemy*> enemys;
	int m_TexTimer = {};
	int m_EndCount = {};
	int m_ResetTimer = {};

	enum AttackState {
		ATTACK_INTRO,
		ATTACK_EXPLAIN,
		ENEMY_BIRTH,
		ENEMY_NEAR,
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
	int m_AddScore = {};

	float radPower = 0;
	const float addPower = 0.5f;

	unique_ptr<UI> ui;
	std::vector<MagText*> magtext;

	array<XMFLOAT2, TEXT_MAX> m_Position;
	array<XMFLOAT2, TEXT_MAX> m_AfterPos;

	enum SkipType {
		SKIP_Y,
		ONE_MORE,
		GAME_Y
	};

	int m_BirthTimer = 0;
	bool m_PlayerDraw = false;
	int m_TargetCount = 2;
};

