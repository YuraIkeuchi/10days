#pragma once
#include "BaseActor.h"
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

private:
	static const int AREA_NUM = 2;
private:
	bool checkPos[4];
	float IntervalRes;
	float PosY, PosX;
	bool ArgF;
	unique_ptr<InterEnemy>Sample;
	InterEnemy::PosSt ini_enmeypos;
	unique_ptr<IKEObject3d> ground;
	IKEModel* model;
	unique_ptr<IKEObject3d> skydome;
	XMFLOAT2 m_AddOffset = {};
	std::vector<unique_ptr<InterEnemy>> enemys;
	unique_ptr<IKETexture> tex[AREA_NUM];
};