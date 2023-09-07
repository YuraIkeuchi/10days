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
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
private:
	static const int NUMBER_MAX = 10;
private:
	//タイム表示
	array<unique_ptr<IKESprite>, NUMBER_MAX> number;
	unique_ptr<IKESprite> TimeGage;
	unique_ptr<IKESprite> ScoreGage;
private:


};