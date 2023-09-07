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
}
