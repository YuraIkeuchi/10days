#pragma once
#include "TextManager.h"
#include <memory>
#include "IKESprite.h"
//�^�C�g���̃e�L�X�g
class TutorialText {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	TutorialText();
	//������
	void Initialize(DirectXCommon* dxCommon);
	//�`��
	void SpriteDraw(DirectXCommon* dxCommon);
	//�F�ύX
	void ChangeColor(int row, const XMVECTOR& color);
	void ChangeAllColor(const XMVECTOR& color);
public:
	//���b�Z�[�W�I��
	void SelectText(TextManager::TUTORIAL name);
private:
	unique_ptr<TextManager> text_;
};