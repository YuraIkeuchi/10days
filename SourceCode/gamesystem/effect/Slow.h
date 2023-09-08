#pragma once
//�q�b�g�X�g�b�v�悤�Ȃ̂Ń��\�[�X�͗v��Ȃ�
class Slow {
public:
	static Slow* GetInstance();
	void Initialize();
	void LoadCSV();
	//�X�V�����̂�
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
	bool m_Slow = false;//�q�b�g�X�g�b�v
	int m_SlowTimer = 0;//�q�b�g�X�g�b�v�̎���
	
	float m_SlowPower = 1.0f;
	float m_PlayerSlowPower = 1.0f;
	bool m_Tutorial = false;
	float m_MovePower = 1.0f;
};