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
	//‰Šú‰»
	void Initialize();
	//XV
	void Update();
	//‘O–Ê•`‰æ
	void FrontDraw();
	//”w–Ê•`‰æ
	void BackDraw();
private:
	void MagMove();//”{—¦UI‚Ì“®‚«
private:
	static const int NUMBER_MAX = 10;
public:
	//gettersetter
	void SetMag(const bool Mag) { m_Mag = Mag; }
private:
	//ƒ^ƒCƒ€•\¦
	array<unique_ptr<IKESprite>, NUMBER_MAX> number;
	unique_ptr<IKESprite> TimeGage;
	unique_ptr<IKESprite> ScoreGage;
	unique_ptr<IKESprite> MagGage;
private:
	int m_MagTimer = 0;
	bool m_Mag = false;
	XMFLOAT2 m_MagPos = {};
};