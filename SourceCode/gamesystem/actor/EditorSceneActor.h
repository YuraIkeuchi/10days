#pragma once
#include "BaseActor.h"

#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
class LoadBox;

/// �^�C�g���V�[��
class EditorSceneActor : public BaseActor {
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
private:
	void IntroUpdate(DebugCamera* camera)override;		//�o��V�[��
	void MainUpdate(DebugCamera* camera)override;		//�o�g���V�[��
	void FinishUpdate(DebugCamera* camera)override;		//���j�V�[��

	void ImGuiDraw();
	void FileWriting();
private:
	static const int AREA_NUM = 4;
private:
	std::ifstream file;
	std::stringstream popcom;
	bool checkPos[4];
	float IntervalRes;
	float PosY, PosX;
	bool ArgF;
	bool timerstart;
	int timer;
	unique_ptr<InterEnemy>Sample;
	InterEnemy::PosSt ini_enmeypos;
	IKEModel* model;
	XMFLOAT2 m_AddOffset = {};
	std::vector<unique_ptr<InterEnemy>> enemys;
	bool delf;
	int TimeRap;
	bool viewf;

	bool ResetF;	std::vector <unique_ptr<InterEnemy>> enemy;
	std::vector<int>EnemyMoveType;
	std::vector<int>InitEnemyMoveType;
	std::vector<int>ResCount;
	//unique_ptr<IKETexture> tex[AREA_NUM];
	std::vector<XMFLOAT3>EPos;
	int m_EnemyType;
};