#pragma once
//ヒットストップようなのでリソースは要らない
class Slow {
public:
	static Slow* GetInstance();
	void Initialize();
	void LoadCSV();
	//更新処理のみ
	void Update();
	void ImGuiDraw();
public:
	//getter setter
	bool GetSlow() { return  m_Slow; }
	float GetSlowPower() { return  m_SlowPower; }
	float GetPlayerSlowPower() { return  m_PlayerSlowPower; }
	float GetMovePower() { return  m_MovePower; }
	void SetSlow(bool Slow) { this->m_Slow = Slow; }
	void SetSlowTimer(int SlowTimer) { this->m_SlowTimer = SlowTimer; }
	void SetTutorial(bool Tutorial) { this->m_Tutorial = Tutorial; }
private:
	bool m_Slow = false;//ヒットストップ
	int m_SlowTimer = 0;//ヒットストップの時間
	
	float m_SlowPower = 1.0f;
	float m_PlayerSlowPower = 1.0f;
	bool m_Tutorial = false;
	float m_MovePower = 1.0f;
};