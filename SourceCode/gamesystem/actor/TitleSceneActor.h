#pragma once
#include "BaseActor.h"
#include "Helper.h"
#include "TitleText.h"
#include <memory>
#include"Font.h"
#include <array>
/// タイトルシーン
class TitleSceneActor : public BaseActor {
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
	void FrontDraw(DirectXCommon* dxCommon);


private:
	static const int NUMBER_MAX = 10;
private:
	unique_ptr<TitleText> text_;
	array<unique_ptr<IKESprite>,NUMBER_MAX> Sprite_First;
	array<unique_ptr<IKESprite>,NUMBER_MAX> Sprite_Second;
	array<unique_ptr<IKESprite>,NUMBER_MAX> Sprite_Third;
	array<unique_ptr<IKESprite>,NUMBER_MAX> Sprite_Fourth;

	enum SceneType {
		PLAY,
		TUTORIAL,
		EDITOR,
	}_SceneType = PLAY;
};