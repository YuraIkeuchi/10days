#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class TutorialEnemy :public InterEnemy {
public:
	TutorialEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
	void EffectDraw(DirectXCommon* dxCommon) override;//�G�t�F�N�g�`��
private:
	void SlowCollide();
	void DeathMove();
	void BirthEffect();
private:

	//�֐��|�C���^
	static void(TutorialEnemy::* stateTable[])();



private:
	void Inter();//�ҋ@
	void RightMove();//�E����
	void LeftMove();//������
	void UpMove();//�E����
	void BottomMove();//������
protected:

private:
	float m_velocity = 0.2f;
	bool m_Slow = false;
	float m_radius = 1.2f;

	int m_ResPornTimer = 0;

	float m_BaseSpeed = {};
	float m_Gravity = 0.01f;
	float m_AddPower = {};
};

