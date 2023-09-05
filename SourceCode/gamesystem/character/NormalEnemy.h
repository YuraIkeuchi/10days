#pragma once
#include"InterEnemy.h"
using namespace std;         //  名前空間指定
//普通の敵
class NormalEnemy :public InterEnemy {
public:
	NormalEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画

private:
	void SlowCollide();
private:
	
	//関数ポインタ
	static void(NormalEnemy::* stateTable[])();


private:
	void Inter();//待機
	void RightMove();//右向き
	void LeftMove();//左向き
	void UpMove();//右向き
	void BottomMove();//左向き
protected:

private:
	float m_velocity = 0.2f;
	bool m_Slow = false;
	float m_radius = 0.8f;
	int m_ResPornTimer = 0;

public:
	float m_BaseSpeed = {};
};

