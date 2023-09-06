#pragma once
#include "ObjCommon.h"
//�G���
class InterEnemy :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://getter setter
	enum class PosSt
	{
		UPRES,
		BOTTOMRES,
		LEFTRES,
		RIGHTRES
	};
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_RIGHT,
		STATE_LEFT,
		STATE_UP,
		STATE_DOWN
	};
	int _charaState = STATE_INTER;
	int StartState;
	void SetState(int state) { StartState=state; }
	bool CheckPos[4];
	void SetResPos(PosSt pos,float otpos);
	void SetPosX(float posx) { m_Position.x = posx; }

	void SetPosZ(float posz) { m_Position.z= posz; }

	void SetNum(size_t num) { Num = num; }
protected:
	XMFLOAT3 RespawnPos;
	size_t Num;
public:
	//virtual ~InterEnemy() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual bool Initialize()override;
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	virtual void Action() = 0;//�G�̍s��

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();

	virtual void ImGui_Origin() = 0;

public://gettersetter
	void SetSlowMove(const bool SlowMove) { m_SlowMove = SlowMove; }
	void SetMove(const bool Move) { m_Move = Move; }
	const bool GetAlive() { return m_Alive; }
protected:
	bool m_SlowMove = false;
	bool m_Move = true;
	bool m_Alive = true;
};