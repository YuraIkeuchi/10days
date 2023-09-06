#pragma once
#include "TextManager.h"
#include <memory>
#include "IKESprite.h"
//タイトルのテキスト
class TutorialText {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	TutorialText();
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//描画
	void SpriteDraw(DirectXCommon* dxCommon);
	//色変更
	void ChangeColor(int row, const XMVECTOR& color);
	void ChangeAllColor(const XMVECTOR& color);
public:
	//メッセージ選択
	void SelectText(TextManager::TUTORIAL name);
private:
	unique_ptr<TextManager> text_;
};