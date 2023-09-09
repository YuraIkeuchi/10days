#pragma once
#include "IKEObject3d.h"
#include "IKEModel.h"
#include "IKETexture.h"
#include <memory>
#include <array>

using namespace std;         //  ���O��Ԏw��
//�w�i�̊�Ȃǋ��ʔw�i�̃N���X
class BackObj{
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static BackObj* GetInstance();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);

private:
	static const int WALL_NUM = 4;

private:
	IKEModel* Up_model = nullptr;
	IKEModel* Under_model = nullptr;

	array<unique_ptr<IKEObject3d>, WALL_NUM> Up_Obj;
	array<unique_ptr<IKEObject3d>, WALL_NUM> Under_Obj;
	unique_ptr<IKETexture> ground;
	unique_ptr<IKETexture> grass;
	unique_ptr<IKETexture> base_ground;
};