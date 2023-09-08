#pragma once
#include "IKEObject3d.h"
#include "IKEModel.h"
#include "IKETexture.h"
#include <memory>
#include <array>

using namespace std;         //  –¼‘O‹óŠÔw’è
//”wŒi‚ÌŠâ‚È‚Ç‹¤’Ê”wŒi‚ÌƒNƒ‰ƒX
class BackObj{
protected:
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static BackObj* GetInstance();
	//‰Šú‰»
	void Initialize();
	//XV
	void Update();
	//•`‰æ
	void Draw(DirectXCommon* dxCommon);

private:
	static const int WALL_NUM = 4;

private:
	IKEModel* Up_model = nullptr;
	IKEModel* Under_model = nullptr;

	array<unique_ptr<IKEObject3d>, WALL_NUM> Up_Obj;
	array<unique_ptr<IKEObject3d>, WALL_NUM> Under_Obj;
	unique_ptr<IKETexture> ground;
	unique_ptr<IKETexture> grass;
	unique_ptr<IKETexture> base_ground;
};