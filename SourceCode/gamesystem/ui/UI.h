#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
#include <array>

using namespace DirectX;
using namespace std;

class UI {
public:
	UI();

public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
private:
	static const int NUMBER_MAX = 10;
private:
	//�^�C���\��
	array<unique_ptr<IKESprite>, NUMBER_MAX> number;
	unique_ptr<IKESprite> TimeGage;
	unique_ptr<IKESprite> ScoreGage;
private:


};