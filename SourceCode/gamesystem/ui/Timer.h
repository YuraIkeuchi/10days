#pragma once

//�^�C�}�[�N���X
class Timer {
public:
	static Timer* GetInstance();

public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//IMGui
	void ImGuiDraw();
public:
	const bool GetEnd() { return m_End; }
	const bool GetStop() { return m_Stop; }
	const int getNowTime() { return m_GameTimer; }
private:
	int m_GameTimer = 1800;
	int m_SlowTimer = {};
	bool m_End = false;
	bool m_Stop = false;
};