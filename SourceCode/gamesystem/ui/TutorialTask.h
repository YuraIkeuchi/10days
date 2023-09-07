#pragma once
#include"IKESprite.h"
#include <memory>
#include <DirectXMath.h>
using namespace std;         //  名前空間指定
//タスククラス
class TutorialTask {
public:
	static TutorialTask* GetInstance();

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
public:
	const bool GetStop() { return m_Stop; }
	const bool GetMission() { return m_Mission; }

	void SetStop(const bool Stop) { m_Stop = Stop; }
	void SetMission(const bool Mission) { m_Mission = Mission; }
private:
	bool m_Stop = false;
	bool m_Mission = false;
	unique_ptr<IKESprite> window;
};