#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include "ImageManager.h"
#include "Helper.h"
#include <random>
#include "Random.h"

ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//テクスチャ全読み込み
	LoadTexture();
	//パーティクルマネージャー生成
	circleParticle.reset(ParticleManager::Create(ImageManager::Normal));
	bloodParticle.reset(ParticleManager::Create(ImageManager::Blood));
}

void ParticleEmitter::Update()
{
	//パーティクルマネージャー更新
	circleParticle->Update();
	bloodParticle->Update();
}

void ParticleEmitter::IntroDraw() {
}
void ParticleEmitter::FlontDrawAll() {
	circleParticle->Draw(AddBlendType);
	bloodParticle->Draw(AlphaBlendType);
}

void ParticleEmitter::DeathDrawAll() {
}
void ParticleEmitter::BackDrawAll() {
}

void ParticleEmitter::FireEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor)
{
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	circleParticle->Add(life, { pos.x,pos.y,pos.z }, vel, {}, startscale, endscale, startcolor, endcolor, 1.0f);
}

void ParticleEmitter::SplatterEffect(const int life, const int num, const XMFLOAT3& startpos, const XMFLOAT3& vec, const float startscale, const float endscale, const XMFLOAT4& bloodcolor)
{
	XMFLOAT3 pos = startpos;

	for (int i = 0; i < num; i++)
	{
		XMFLOAT3 vel = vec;
		float div = 10;
		const int rnd_vel = 5;
		if (vec.x == 0)
		{
			vel.x += static_cast<float>(Random::GetRanNum(0, rnd_vel) - rnd_vel / 2) / div;
		}
		else
		{
			vel.x += static_cast<float>(Random::GetRanNum(0, rnd_vel / 2)) / div;
		}
		vel.y += static_cast<float>(Random::GetRanNum(2, 4)) / div;
		if (vec.z == 0)
		{
			vel.z += static_cast<float>(Random::GetRanNum(0, rnd_vel) - rnd_vel / 2) / div;
		}
		else
		{
			vel.z += static_cast<float>(Random::GetRanNum(0, rnd_vel / 2)) / div;
		}

		bloodParticle->Add(life, { pos.x, pos.y, pos.z }, vel, { -vel.x / life, -0.04f, -vel.z / life }, startscale, endscale, bloodcolor, bloodcolor, 1.0f);
	}
}

void ParticleEmitter::AllDelete()
{
	//パーティクルマネージャー更新
	circleParticle->AllDelete();
	bloodParticle->AllDelete();
}

void ParticleEmitter::LoadTexture() {
	ImageManager::GetInstance()->LoadParticle();
}