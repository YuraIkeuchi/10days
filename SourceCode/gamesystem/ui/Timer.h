#pragma once
#include "IKESprite.h"
#include <array>
#include <memory>
enum GameType {
	NO_MODE,
	PAD_MODE,
	KEYBOARD_MODE,
};
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
	const int getGameType() { return m_GameType; }

	void SetGameType(const int GameType) { m_GameType = GameType; }
private:
	int m_GameTimer = 5400;
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
	int m_GameType = NO_MODE;
};