#pragma once
#include "IKEObject3d.h"
#include "IKEModel.h"
#include <memory>
#include <array>

using namespace std;         //  名前空間指定
//背景の岩など共通背景のクラス
class BackObj{
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static BackObj* GetInstance();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

private:
	static const int WALL_NUM = 4;

private:
	IKEModel* Up_model = nullptr;
	IKEModel* Under_model = nullptr;

	array<unique_ptr<IKEObject3d>, WALL_NUM> Up_Obj;
	array<unique_ptr<IKEObject3d>, WALL_NUM> Under_Obj;
};