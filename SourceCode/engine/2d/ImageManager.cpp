#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{
	IKESprite::LoadTexture(NUMBER, L"Resources/2d/objicon/Num.png");
	IKESprite::LoadTexture(BLACKWINDOW, L"Resources/2d/objicon/BlackWindow.png");
	IKESprite::LoadTexture(TUTORIAL, L"Resources/2d/objicon/Tutorial.png");
	IKESprite::LoadTexture(RESULT, L"Resources/2d/sceneback/Result.png");
	IKESprite::LoadTexture(LEFTDOOR, L"Resources/2d/sceneback/Door.png");
	IKESprite::LoadTexture(RIGHTDOOR, L"Resources/2d/sceneback/Door2.png");
	IKESprite::LoadTexture(TIMEGAGE, L"Resources/2d/objicon/Emaki.png");
	IKESprite::LoadTexture(SCOREGAGE, L"Resources/2d/objicon/Emaki2.png");
	IKESprite::LoadTexture(MAGGAGE, L"Resources/2d/objicon/Emaki3.png");
	IKESprite::LoadTexture(MAGNIFICATION, L"Resources/2d/objicon/Mag.png");
	IKESprite::LoadTexture(CUT_UP, L"Resources/2d/effect/zanup.png");
	IKESprite::LoadTexture(CUT_DOWN, L"Resources/2d/effect/zanbot.png");
	IKESprite::LoadTexture(CUTGAGE_UP, L"Resources/2d/effect/gaugeup.png");
	IKESprite::LoadTexture(CUTGAGE_DOWN, L"Resources/2d/effect/gaugebot.png");
	IKESprite::LoadTexture(TITLE, L"Resources/2d/sceneback/Title.png");
	IKESprite::LoadTexture(SKIP, L"Resources/2d/sceneback/Skip.png");
	IKESprite::LoadTexture(MISS, L"Resources/2d/effect/Miss.png");
}

//非同期処理で読み込むスプライト
void ImageManager::SecondLoad2D()
{
}

//テクスチャの読み込み
void ImageManager::StartLoadTex2D()
{
	IKETexture::LoadTexture(MAGIC, L"Resources/2d/objicon/magic.png");
	IKETexture::LoadTexture(AREA, L"Resources/2d/effect/DArea.png");
	IKETexture::LoadTexture(BLOOD, L"Resources/2d/effect/blood.png");
	IKETexture::LoadTexture(GRASS, L"Resources/2d/objicon/grass.png");
	IKETexture::LoadTexture(GROUND, L"Resources/2d/objicon/ground.png");
	IKETexture::LoadTexture(BASE_GROUND, L"Resources/2d/objicon/baseground.png");
	IKETexture::LoadTexture(SLASH, L"Resources/2d/effect/Slash.png");
}

//非同期処理でのテクスチャの読み込み
void ImageManager::SecondLoadTex2D()
{
}

void ImageManager::LoadParticle()
{
	ParticleManager::LoadTexture(Normal, "effect1.png");
	ParticleManager::LoadTexture(Smoke, "hootEffect.png");
	ParticleManager::LoadTexture(Heal, "effect2.png");
	ParticleManager::LoadTexture(Blood, "blood.png");
	ParticleManager::LoadTexture(Dash, "hootEffect.png");
}
