#pragma once

//タイマークラス
class Timer {
public:
	static Timer* GetInstance();

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//IMGui
	void ImGuiDraw();
public:
	const bool GetEnd() { return m_End; }
private:
	int m_GameTimer = 1800;
	bool m_End = false;
};