#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>

using namespace std;
//�X�R�A�֌W
class ScoreManager {
public:
	static ScoreManager* GetInstance();
	//������
	void LoadResource();
	void Initialize();
	void Update();
	void ImGuiDraw();
	void SpriteDraw();

private:
	//������Ԃ�
	int getDigits(int value, int m, int n);
private:

private:
	static const int NUMBER_MAX = 10;

public:
	//getter setter
	const int GetFirstNumber() { return m_FirstNumber; }
	const int GetSecondNumber() { return m_SecondNumber; }
	const int GetThirdNumber() { return m_ThirdNumber; }
	const int GetFourthNumber() { return m_FourthNumber; }
	const int GetMagnification() { return m_Magnification; }
	const int GetRealScore() { return m_RealScore; }

	//
	void SetFirstNumber(const int FirstNumber) { m_FirstNumber = FirstNumber; }
	void SetSecondNumber(const int SecondNumber) { m_SecondNumber = SecondNumber; }
	void SetThirdNumber(const int ThirdNumber) { m_ThirdNumber = ThirdNumber; }
	void SetFourthNumber(const int FourthNumber) { m_FourthNumber = FourthNumber; }
	void SetMagnification(const int Magnification) { m_Magnification = Magnification; }
	void SetRealScore(const int RealScore) { m_RealScore = RealScore; }
private:
	int m_FirstNumber = 0;//�ꌅ��
	int m_SecondNumber = 0;//�񌅂�
	int m_ThirdNumber = 0;//�O����
	int m_FourthNumber = 0;//�l����
	int m_Magnification = 0;//�{��
	int m_RealScore = {};
private:
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Fourth;
};
