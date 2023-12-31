#pragma once
#include "ParticleManager.h"

using namespace std;         //  名前空間指定
/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //シングルトン化
	//コンストラクタを隠蔽
	ParticleEmitter() = default;
	//デストラクタを隠蔽
	~ParticleEmitter() = default;
public: //メンバ関数
	static ParticleEmitter* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();

	void IntroDraw();
	//描画
	void FlontDrawAll();

	void DeathDrawAll();
	//壁のパーティクルの描画
	void BackDrawAll();
	//炎エフェクト
	void FireEffect(const int life,const XMFLOAT3& pos,const float startscale, const float endscale,const XMFLOAT4& startcolor,const XMFLOAT4& endcolor);
	//血しぶきエフェクト
	void SplatterEffect(const int life, const int num, const XMFLOAT3& startpos, const XMFLOAT3& vec, const float startscale, const float endscale, const XMFLOAT4& bloodcolor);
	//砂煙エフェクト
	void DashEffect(const int life, const int num, const XMFLOAT3& startpos, const XMFLOAT3& vec, const float startscale, const float endscale, const XMFLOAT4& color);

	void AllDelete();
private: //メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

private: //メンバ変数
	//パーティクルマネージャー(丸)
	unique_ptr<ParticleManager> circleParticle;
	//パーティクルマネージャー(血)
	unique_ptr<ParticleManager> bloodParticle;
	//パーティクルマネージャー(砂煙)
	unique_ptr<ParticleManager> dashParticle;
private:
	int m_Timer = 0;
};
