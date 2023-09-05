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
private:
	int m_GameTimer = 1800;
	bool m_End = false;
	bool m_Stop = false;
};