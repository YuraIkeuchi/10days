#include "IKEFBXObject3d.h"
#include "IKEFBXModel.h"
#include "IKEFbxLoader.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ComPtr<ID3D12RootSignature> IKEFBXObject3d::rootsignature;
ComPtr<ID3D12PipelineState> IKEFBXObject3d::pipelinestate;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* IKEFBXObject3d::device = nullptr;
Camera* IKEFBXObject3d::camera = nullptr;

void IKEFBXObject3d::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/FBXVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
		            errorBlob->GetBufferSize(),
		            errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/FBXPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
		            errorBlob->GetBufferSize(),
		            errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			// xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			//�e�����󂯂�{�[���ԍ��i4�j
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			//�{�[���̃X�L���E�F�C�g�i4�jBONEWEIGHTS
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV�i�X�L�j���O�p�j
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	auto samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
	                           D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
	                                               &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
	                                     IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void IKEFBXObject3d::Initialize()
{
	HRESULT result;
	//�萔�o�b�t�@
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));
	// �萔�o�b�t�@�փf�[�^�]�� (�X�L���Ȃ�)
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);
	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void IKEFBXObject3d::Update(bool Loop, int Speed, bool& Stop)
{
	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
	matWorld *= matScale; //���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->ShadeSet = ShadeSet;
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constMap->color = color;
		constBuffTransform->Unmap(0, nullptr);
	}

	//�{�[���z��
	std::vector<IKEFBXModel::Bone>& bones = model->GetBones();

	//�A�j���[�V����
	if (isPlay)
	{
		//1�t���[���i�߂�
		currentTime += frameTime * Speed;
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (Loop)
		{
			if (currentTime > endTime)
			{
				currentTime = startTime;
			}
		}
		else
		{
			if (currentTime > endTime)
			{
				if (Stop)
				{
					Stop = false;
				}
				isPlay = false;
			
			}
		}
	}

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++) {
		//���̎p��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		IKEFbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		auto& bindMatrix = model->GetModelTransform();
		auto inverseBindMatrix = XMMatrixInverse(nullptr, bindMatrix);
		constMapSkin->bones[i] = bindMatrix * bones[i].invInitialPose * matCurrentPose * inverseBindMatrix;
	}
	constBuffSkin->Unmap(0, nullptr);
}


void IKEFBXObject3d::FollowUpdate(bool Loop, int Speed, bool& Stop)
{
	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
	matWorld *= matScale; //���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	matWorld = modelTransform * matWorld;

	if (SUCCEEDED(result))
	{
		constMap->ShadeSet = ShadeSet;
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = cameraPos;
		constMap->color = color;
		constBuffTransform->Unmap(0, nullptr);
	}

	//�{�[���z��
	std::vector<IKEFBXModel::Bone>& bones = model->GetBones();

	//�A�j���[�V����
	if (isPlay)
	{
		//�ʏ�Đ�
		if (!m_Reverse)
		{
			//1�t���[���i�߂�
			currentTime += frameTime * Speed;
			//�Ō�܂ōĐ�������擪�ɖ߂�
			if (Loop)
			{
				if (currentTime > endTime)
				{
					currentTime = startTime;
				}
			}
			else
			{
				if (currentTime > endTime)
				{
					isPlay = false;
					if (Stop)
					{
						Stop = false;
					}
				}
			}
		}
		//�t�Đ�
		else
		{
			//1�t���[���i�߂�
			currentTime -= frameTime * Speed;
			//�Ō�܂ōĐ�������擪�ɖ߂�
			if (Loop)
			{
				if (currentTime < startTime)
				{
					currentTime = endTime;
					m_Reverse = false;
				}
			}
			else
			{
				if (currentTime < startTime)
				{
					isPlay = false;
					m_Reverse = false;
					if (Stop)
					{
						Stop = false;
					}
				}
			}
		}
	}
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	BoneSize = bones.size();
	for (int i = 0; i < bones.size(); i++) {
		//���̎p��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		IKEFbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}

	IKEFbxLoader::ConvertMatrixFromFbx(
		&WorldMat, bones[BoneNumber].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime));

	WorldMat = WorldMat * matWorld;
	constBuffSkin->Unmap(0, nullptr);
}

void IKEFBXObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr)
	{
		return;
	}
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	//���f���`��
	model->Draw(cmdList);
}

void IKEFBXObject3d::LoadAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//�A�j���[�V�����J�E���g
	int AnimationCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < AnimationCount; i++)
	{
		//���f�[�^
		Animation tempData;
		//i�Ԃ̃A�j���[�V�����擾
		tempData.stack = fbxScene->GetSrcObject<FbxAnimStack>(i);
		//�A�j���[�V�����̖��O���擾
		const char* animstackname = tempData.stack->GetName();
		//�A�j���[�V�����̎��ԏ��
		tempData.info = fbxScene->GetTakeInfo(animstackname);
		//���f�[�^�����f�[�^�ɓ����
		animationData.push_back(tempData);
	}
}

void IKEFBXObject3d::PlayAnimation(int Number)
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0�Ԃ̃A�j���[�V�����擾
	//FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(2);
	//�A�j���[�V�����̕ύX
	fbxScene->SetCurrentAnimationStack(animationData[Number].stack);
	////�A�j���[�V�����̖��O�擾
	//const char* animstackname = animstack->GetName();
	////�A�j���[�V�����̎��Ԏ擾
	//FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);
	//�J�n���Ԏ擾
	startTime = animationData[Number].info->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = animationData[Number].info->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}

void IKEFBXObject3d::StopAnimation()
{
	isPlay = false;
}

XMMATRIX IKEFBXObject3d::ExtractRotationMat(XMMATRIX matworld)
{
	XMMATRIX mOffset = ExtractPositionMat(matworld);
	XMMATRIX mScaling = ExtractScaleMat(matworld);

	XMVECTOR det;
	// ������Scaling�A�E����Offset�̋t�s������ꂼ�ꂩ����B
	return XMMatrixInverse(&det, mScaling) * matworld * XMMatrixInverse(&det, mOffset);
}

XMMATRIX IKEFBXObject3d::ExtractScaleMat(XMMATRIX matworld)
{
	return XMMatrixScaling(
		XMVector3Length(XMVECTOR{
			matworld.r[0].m128_f32[0], matworld.r[0].m128_f32[1], matworld.r[0].m128_f32[2]
		}).m128_f32[0],
		XMVector3Length(XMVECTOR{
			matworld.r[1].m128_f32[0], matworld.r[1].m128_f32[1], matworld.r[1].m128_f32[2]
		}).m128_f32[0],
		XMVector3Length(XMVECTOR{
			matworld.r[2].m128_f32[0], matworld.r[2].m128_f32[1], matworld.r[2].m128_f32[2]
		}).m128_f32[0]
	);
}

XMMATRIX IKEFBXObject3d::ExtractPositionMat(XMMATRIX matworld)
{
	return XMMatrixTranslation(matworld.r[3].m128_f32[0], matworld.r[3].m128_f32[1], matworld.r[3].m128_f32[2]);
}

void IKEFBXObject3d::GetBoneIndexMat(int index, XMMATRIX& matworld)
{
	IKEFbxLoader::ConvertMatrixFromFbx(&matworld,
		model->GetBones()[index].fbxCluster->GetLink()->
		EvaluateGlobalTransform(currentTime));

	matworld = matworld * matWorld;
}