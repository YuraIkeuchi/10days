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
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_RIGHT,
		STATE_LEFT,
	};

	//�֐��|�C���^
	static void(NormalEnemy::* stateTable[])();

	int _charaState = STATE_INTER;

private:
	void Inter();//�ҋ@
	void RightMove();//�E����
	void LeftMove();//������
protected:

private:
	float m_velocity = 0.2f;
};

