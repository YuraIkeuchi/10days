#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>

using namespace std;

//倍率テキストクラス
class MagText {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	MagText(const int EnemyCount, const int Magnification);
	//初期化
	void Initialize();
	void Update();
	void Draw();
private:
	static const int NUMBER_MAX = 10;
public:
	const bool GetAlive() { return m_Alive; }
private:

	int m_EnemyCount = {};
	int m_Magnification = {};

	array<unique_ptr<IKESprite>, NUMBER_MAX> Count_Sprite;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Count_Sprite2;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Mag_Sprite;
	unique_ptr<IKESprite> Base_Sprite;
	XMFLOAT2 m_CountPos = {};
	XMFLOAT2 m_MagPos = {};

	int m_Timer = {};

	enum TextType {
		BIRTH_TEXT,
		VANISH_TEXT
	}_TextType = BIRTH_TEXT;

	bool m_Alive = false;
};