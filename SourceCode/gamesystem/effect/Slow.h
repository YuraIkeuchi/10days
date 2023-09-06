#pragma once
//ヒットストップようなのでリソースは要らない
class Slow {
public:
	static Slow* GetInstance();
	void LoadCSV();
	//更新処理のみ
	void Update();
	void ImGuiDraw();
public:
	//getter setter
	bool GetSlow() { return  m_Slow; }
	float GetSlowPower() { return  m_SlowPower; }
	void SetSlow(bool Slow) { this->m_Slow = Slow; }
	void SetTutorial(bool Tutorial) { this->m_Tutorial = Tutorial; }
private:
	bool m_Slow = false;//ヒットストップ
	int m_SlowTimer = 0;//ヒットストップの時間
	
	float m_SlowPower = 1.0f;
	bool m_Tutorial = false;
};