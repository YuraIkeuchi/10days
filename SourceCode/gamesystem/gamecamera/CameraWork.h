﻿#pragma once
#include "DebugCamera.h"
#include "Shake.h"
#include <memory>
#include "Feed.h"
#include "InterBoss.h"
#include"Spline.h"
using namespace std;         //  名前空間指定

//ゲームのカメラの動き
class CameraWork {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	CameraWork(XMFLOAT3 eye = { 2.0f, 30.0f, 2.0f }, XMFLOAT3 target = { 2.0f, 0.0f, 3.0f });
	void SplineSet();
	void Update(DebugCamera* camera);//更新
	void TutorialUpdate(DebugCamera* camera);//チュートリアルの更新
	void TitleCamera(DebugCamera* camera);//タイトルカメラ
	void ImGuiDraw();

private:
	void SetAfterCamera(const XMFLOAT3& pos);
public:
	const bool GetLookEnd() { return m_LookEnd; }

	void SetLookEnemy(const bool LookEnemy) { m_LookEnemy = LookEnemy; }
private:
	XMFLOAT3 m_eyePos = {};
	XMFLOAT3 m_targetPos = {};
	XMFLOAT3 m_AfterEye = {};
	XMFLOAT3 m_AfterTarget = {};

	//チュートリアルのカメラ
	bool m_LookEnemy = false;
	float m_Frame = {};
	int m_LookTimer = {};
	bool m_LookEnd = false;

	//タイトルのカメラの変数
	//円運動のため
	float m_PI = 3.14f;
	float m_TitleCameraRadius = 0.0f;
	float m_TitleCameraSpeed = 0.0f;
	float m_TitleCameraScale = 0.0f;
	float m_TitleCameraCircleX = 0.0f;
	float m_TitleCameraCircleZ = 0.0f;
};