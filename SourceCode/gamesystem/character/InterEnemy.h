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
	enum PosSt
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
	int GetState() { return StartState; }

	void SetState(int state) { StartState=state; }
	bool CheckPos[4];
	void SetResPos(PosSt pos,float otpos);
	void SetPosX(float posx) { m_Position.x = posx; }

	void InitState() { _charaState = CharaState::STATE_INTER; }
	void SetPosZ(float posz) { m_Position.z= posz; }

	void SetNum(size_t num) { Num = num; }

	void SetMovingTime(int t) { MovingTime = t; }

	const int GetMovingT() { return MovingTime; }
protected:
	int MovingTime;
	bool StopF = false;
	XMFLOAT3 RespawnPos;
	size_t Num;
	XMFLOAT3 EditStartPos;;
public:

	inline void EditPos(XMFLOAT3 pos) { EditStartPos = pos; }
	const XMFLOAT3 GetEditPos() {return EditStartPos; }
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
	
	void SetStopF(bool f) { StopF = f; }
	void SetSlowMove(const bool SlowMove) { m_SlowMove = SlowMove; }
protected:
	
	bool m_SlowMove = false;
};