#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
#include <array>

using namespace DirectX;
using namespace std;

class UI {
public:
	UI();

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//前面描画
	void FrontDraw();
	//背面描画
	void BackDraw();
private:
	void MagMove();//倍率UIの動き
private:
	static const int NUMBER_MAX = 10;
public:
	//gettersetter
	void SetMag(const bool Mag) { m_Mag = Mag; }
private:
	//タイム表示
	array<unique_ptr<IKESprite>, NUMBER_MAX> number;
	unique_ptr<IKESprite> TimeGage;
	unique_ptr<IKESprite> ScoreGage;
	unique_ptr<IKESprite> MagGage;
	unique_ptr<IKESprite> Miss;
	XMFLOAT4 m_MissColor = { 1.0f,1.0f,1.0f,0.0f };
private:
	int m_MagTimer = 0;
	bool m_Mag = false;
	XMFLOAT2 m_MagPos = {};
};