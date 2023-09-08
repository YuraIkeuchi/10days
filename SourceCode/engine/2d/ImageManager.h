#pragma once
#include <map>
#include <string>
#include "IKESprite.h"
#include "IKETexture.h"
#include "ParticleManager.h"

//2Dの読み込み処理
class ImageManager final
{
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& r) = default;
	ImageManager& operator=(const ImageManager& r) = default;

public:
	//スプライトの名前
	enum texName
	{
		FEED,
		NUMBER,
		BLACKWINDOW,
		TUTORIAL,
		RESULT,
		LEFTDOOR,
		RIGHTDOOR,
		TIMEGAGE,
		SCOREGAGE,
		MAGGAGE,
		MAGNIFICATION,
		CUT_UP,
		CUT_DOWN,
		CUTGAGE_UP,
		CUTGAGE_DOWN,
	};
	//テクスチャの名前
	enum tex2dName
	{
		MAGIC,
		AREA,
		BLOOD,
		GRASS,
		GROUND,
		BASE_GROUND,
	};
	enum particleName
	{
		Normal,
		Smoke,
		Heal,
		Blood,
	};

public:
	static ImageManager* GetInstance(); //取得用
	void StartLoad2D();
	void SecondLoad2D();
	void StartLoadTex2D();
	void SecondLoadTex2D();
	void LoadParticle();
};
