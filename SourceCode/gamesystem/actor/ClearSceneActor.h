#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include <array>
/// クリアシーン
class ClearSceneActor : public BaseActor {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw();
private:
	void ResultMove();
	//割合を返す
	int getDigits(int value, int m, int n);
private:
	static const int NUMBER_MAX = 10;
	static const int NEXT_MAX = 2;
	static const int HYOUKA_MAX = 5;
private://メンバ変数
	//スコア
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Fourth;
	//次の評価
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_Fourth;
	array<unique_ptr<IKESprite>, HYOUKA_MAX> Hyouka_;
	array<unique_ptr<IKESprite>, NEXT_MAX> Next_Hyouka;

	unique_ptr<IKESprite> back;
	int m_TargetScore = {};//最終スコア
	int m_TargetNowScore = {};//最終スコア
	int m_FirstNumber = 0;//一桁目
	int m_SecondNumber = 0;//二桁め
	int m_ThirdNumber = 0;//三桁め
	int m_FourthNumber = 0;//四桁目

	int m_NextFirstNumber = 0;//一桁目
	int m_NextSecondNumber = 0;//二桁め
	int m_NextThirdNumber = 0;//三桁め
	int m_NextFourthNumber = 0;//四桁目

	int m_counterTimer = 0;//スコアのオーディオタイマー

	enum HyoukaType {
		BUT,//一番下
		NORMAL,//普通
		GOOD,//良い
		VERYGOOD,//とても良い
		SUPER,//最高
	};
	int _HyoukaType = BUT;

	array<int, HYOUKA_MAX> m_ScoreLine;

	int m_NextTarget = {};
	int m_NextScore = {};

	enum ResultType {
		SCORE,//スコアが出る
		HYOUKA,//評価が出る
		NEXT_SCORE,//次までの評価が出る
		END,//終わり
	};
	int _ResultType = SCORE;

	float m_Alpha = 0.0f;
	XMFLOAT2 m_HyoukaSize = { 400.0f,400.0f };
	float m_Frame = 0.0f;
};
