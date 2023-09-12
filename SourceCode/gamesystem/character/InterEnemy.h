#pragma once
#include "ObjCommon.h"
#include "IKESprite.h"
#include "SlashEffect.h"
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
	int GetEnemyType() { return m_EnemyType; }

	void SetState(int state) { StartState=state; }
	void SetEnemyType(int Type) { m_EnemyType = Type; }
	bool CheckPos[4];
	void SetResPos(PosSt pos,float otpos);
	void SetPosX(float posx) { m_Position.x = posx; }

	void InitState() { _charaState = CharaState::STATE_INTER; }
	void SetPosZ(float posz) { m_Position.z= posz; }

	void SetNum(size_t num) { Num = num; }

	void SetMovingTime(int t) { MovingTime = t; }

	const int GetMovingT() { return MovingTime; }
	void SetEffectMove(bool EffectMove) { m_EffectMove = EffectMove; }

private:
	void SuccessAttack();
	void MissAttack();
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

	virtual void EffectDraw(DirectXCommon* dxCommon) = 0;

	void ImGuiDraw();

	virtual void ImGui_Origin() = 0;

	void EffectCountDown();//��������
public:
	bool CheckCollide();
	void AttackCollide();
	void BirthEffect();
	void BirthMove();
	void DeathMove();
public://gettersetter
	
	void SetHitCheck(bool HitCheck) { m_HitCheck = HitCheck; }
	void SetStopF(bool f) { StopF = f; }
	void SetSlowMove(const bool SlowMove) { m_SlowMove = SlowMove; }
	void SetMove(const bool Move) { m_Move = Move; }
	void SetDamage(const bool Damage) { m_Damage = Damage; }
	void SetCheckMiss(const bool CheckMiss) { m_CheckMiss = CheckMiss; }
	const bool GetAlive() { return m_Alive; }
	const bool GetHitCheck() { return m_HitCheck; }
	const bool GetDeath() { return m_Death; }
	const bool GetDamage() { return m_Damage; }
	const bool GetDestroy() { return m_Destroy; }
protected:
	
	unique_ptr<IKESprite> effect_up;
	unique_ptr<IKESprite> effect_down;
	unique_ptr<IKESprite> gauge_up;
	unique_ptr<IKESprite> gauge_down;
	std::vector<SlashEffect*> slash;
	XMFLOAT4 m_EffectColor = {};
	bool m_SlowMove = false;
	bool m_Move = true;
	bool m_Alive = true;
	bool m_Destroy = false;
	bool m_HitCheck = false;
	bool m_EffectMove = true;
	bool m_Birth = false;
	enum EnemyType {
		RED_ENEMY,
		GREEN_ENEMY,
		BLUE_ENEMY,
	};
	int _EnemyType = RED_ENEMY;

	int m_EnemyType = {};

	XMFLOAT2 m_UpPos = {};
	XMFLOAT2 m_DownPos = {};
	bool m_Death = false;
	bool m_Damage = false;

	bool m_ViewEffect = false;
	float m_Frame = {};
	float m_Alpha = 1.0f;

	bool m_Miss = false;
	int SlowStopTimer = {};
	bool m_Slow = false;
	float m_radius = 0.8f;
	bool m_CheckMiss = false;

	//�G�t�F�N�g�̑傫��
	XMFLOAT2 EffectSize = {};
	float m_EffectFrame = {};
	int m_MissTimer = {};

	float m_Gravity = 0.01f;
	float m_AddPower = {};

	enum BirthType {
		BIRTH_SET,
		BIRTH_START
	}_BirthType = BIRTH_SET;
};
