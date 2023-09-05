#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
#include <array>

using namespace DirectX;

class UI {
public:
	UI() {};
	~UI();
private:
	//仮フォント
	struct SpriteData {
		//画像
		std::unique_ptr<IKESprite> Tex;
		//基礎パラメータ
		XMFLOAT2 Position = { 0,0 };
		XMFLOAT2 Size = { 100,100 };
		float Rotation = 0.f;
		XMFLOAT4 Color = { 1,1,1,1 };
		//表示なフラグ
		bool IsVisible = true;
		bool IsContinue = false;
	};
	enum  SpriteName {
		Time = 0,
		UiMax,
	};

	//スプライト
	std::array<SpriteData, UiMax> sprites = {};

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
private:
	//
	SpriteData CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color);
private:
	//タイム表示
private:

};