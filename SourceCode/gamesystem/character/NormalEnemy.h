#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class NormalEnemy :public InterEnemy {
public:
	NormalEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��

private:
	void SlowCollide();
private:
	
	//�֐��|�C���^
	static void(NormalEnemy::* stateTable[])();


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
	float m_radius = 0.8f;
	int m_ResPornTimer = 0;

public:
	float m_BaseSpeed = {};
};

