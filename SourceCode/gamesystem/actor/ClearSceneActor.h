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
	static const int NUMBER_MAX = 10;
private://メンバ変数
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_Fourth;

	unique_ptr<IKESprite> back;
};
