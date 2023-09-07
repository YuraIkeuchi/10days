#pragma once
//�X�R�A�֌W

class ScoreManager {
public:
	static ScoreManager* GetInstance();
	//������
	void Initialize();
	void Update();
	void ImGuiDraw();
private:
	static const int NUMBER_MAX = 10;

public:
	//getter setter
	const int GetFirstNumber() { return m_FirstNumber; }
	const int GetSecondNumber() { return m_SecondNumber; }
	const int GetThirdNumber() { return m_ThirdNumber; }
	const int GetFourthNumber() { return m_FourthNumber; }
	const int GetMagnification() { return m_Magnification; }

	//
	void SetFirstNumber(const int FirstNumber) { m_FirstNumber = FirstNumber; }
	void SetSecondNumber(const int SecondNumber) { m_SecondNumber = SecondNumber; }
	void SetThirdNumber(const int ThirdNumber) { m_ThirdNumber = ThirdNumber; }
	void SetFourthNumber(const int FourthNumber) { m_FourthNumber = FourthNumber; }
	void SetMagnification(const int Magnification) { m_Magnification = Magnification; }
private:
	int m_FirstNumber = 0;//�ꌅ��
	int m_SecondNumber = 0;//�񌅂�
	int m_ThirdNumber = 0;//�O����
	int m_FourthNumber = 0;//�l����
	int m_Magnification = 0;//�{��
};
