#pragma once
#include "IKESprite.h"
#include <array>
#include <memory>

using namespace std;
//タイマークラス
class Timer {
public:
	static Timer* GetInstance();

public:
	void LoadResource();
	//初期化
	void Initialize();
	//更新
	void Update();
	//IMGui
	void ImGuiDraw();
	//タイムの表示
	void SpriteDraw();
public:
	const bool GetEnd() { return m_End; }
	const bool GetStop() { return m_Stop; }
	const int getNowTime() { return m_GameTimer; }
private:
	int m_GameTimer = 1800;
	int m_SlowTimer = {};
	bool m_End = false;
	bool m_Stop = false;
	int m_FirstCount = 0;
	int m_SecondCount = 3;
private:
	static const int NUMBER_MAX = 10;
private://メンバ変数
	array<unique_ptr<IKESprite>, NUMBER_MAX> Time_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Time_Second;
};