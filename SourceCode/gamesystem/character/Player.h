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
public:
	const int GetMoveState() { return (int)_MoveState; }
	const bool GetCameraZoom() { return m_CameraZoom; }
private:
	void LoadCSV();

private:
	//歩きまたは走り状態
	float velocity = {};
	//移動加算値
	float m_AddSpeed = 0.0f;

	//キャラの状態
	enum CharaState
	{
		STATE_MOVE,
		STATE_ATTACK,
	}_charaState;

	float m_Frame = {};
	float m_AfterPosZ = {};
	float m_AfterPosX = {};
	enum MoveState {
		MOVE_UP,
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_LEFT,
	}_MoveState;

	bool m_ChangeLine = false;
	bool m_CameraZoom = false;
};