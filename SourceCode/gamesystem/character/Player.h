#pragma once
#include "ObjCommon.h"
#include <any>
using namespace DirectX;
class Player :public ObjCommon
{
public:
	static Player* GetInstance();

private:
public:
	void InitState(const XMFLOAT3& pos);
	void LoadResource();
	//初期化
	bool Initialize()override;
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
private:
	static void (Player::* stateTable[])();

private:
	void Move();
	void Attack();
private:
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);
private:
	void LoadCSV();

private:
	//移動方向指定用
	float angle = 0.0f;
	//歩きまたは走り状態
	float velocity;
	//移動加算値
	float m_AddSpeed;

	//キャラの状態
	enum CharaState
	{
		STATE_MOVE,
		STATE_ATTACK,
	}_charaState;

	enum AttackState {
		ATTACK_DOWN,
		ATTACK_UP,
	}_AttackState;

	float m_Frame = {};
	float m_AfterPosZ = {};
};