#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
#include <array>

using namespace DirectX;

class UI {
public:
	UI() {};
	~UI();
private:
	//���t�H���g
	struct SpriteData {
		//�摜
		std::unique_ptr<IKESprite> Tex;
		//��b�p�����[�^
		XMFLOAT2 Position = { 0,0 };
		XMFLOAT2 Size = { 100,100 };
		float Rotation = 0.f;
		XMFLOAT4 Color = { 1,1,1,1 };
		//�\���ȃt���O
		bool IsVisible = true;
		bool IsContinue = false;
	};
	enum  SpriteName {
		Time = 0,
		UiMax,
	};

	//�X�v���C�g
	std::array<SpriteData, UiMax> sprites = {};

public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
private:
	//
	SpriteData CreateUi(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color);
private:
	//�^�C���\��
private:

};