#pragma once
#include "IKETexture.h"
#include <memory>

using namespace std;

//切ったときのエフェクト
class SlashEffect {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	SlashEffect(const XMFLOAT3& StartPos);
	//初期化
	void Initialize();
	void Update();
	void Draw(DirectXCommon* dxCommon);
	void ImGuiDraw();
public:
	const bool GetAlive() { return m_Alive; }
private:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = {};
	XMFLOAT4 m_Color = {1.0f,1.0f,1.0f,1.0f};

	bool m_Alive = true;
	float m_Frame = {};

	unique_ptr<IKETexture> slash;
};