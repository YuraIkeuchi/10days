#include "MagText.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"

//読み込み
MagText::MagText(const int EnemyCount, const int Magnification) {
	m_EnemyCount = EnemyCount;
	m_Magnification = Magnification;

	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Count_Sprite.size(); i++) {
		//ひとけた目
		Count_Sprite[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		Count_Sprite[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Count_Sprite[i]->SetAnchorPoint({ 0.5f,0.5f });
		Count_Sprite[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Count_Sprite[i]->SetScale(0.7f);
		//二桁め
		Count_Sprite2[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Count_Sprite2[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Count_Sprite2[i]->SetAnchorPoint({ 0.5f,0.5f });
		Count_Sprite2[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Count_Sprite2[i]->SetScale(0.7f);
		//倍率
		Mag_Sprite[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Mag_Sprite[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Mag_Sprite[i]->SetAnchorPoint({ 0.5f,0.5f });
		Mag_Sprite[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Mag_Sprite[i]->SetScale(0.7f);
	}

	Base_Sprite = IKESprite::Create(ImageManager::MAGNIFICATION, {});
	Base_Sprite->SetSize({ 64.0f,64.0f });
	Base_Sprite->SetAnchorPoint({ 0.5f, 0.5f });
	Base_Sprite->SetScale(0.7f);
}
//初期化
void MagText::Initialize() {
	m_CountPos = { 1120.0f,-10.0f };
	m_MagPos = { 1240.0f,-10.0f };
	m_Alive = true;
}
//更新
void MagText::Update() {
	const int l_TimerMax = 30;//表示される時間
	//上から下に落ちてくる
	if (_TextType == BIRTH_TEXT) {
		if (Helper::GetInstance()->CheckMin(m_Timer, 50, 1)) {
			_TextType = VANISH_TEXT;
		}
		m_CountPos.y = Ease(In, Cubic, 0.5f, m_CountPos.y, 130.0f);
		m_MagPos.y = Ease(In, Cubic, 0.5f, m_MagPos.y, 130.0f);
	}
	else {		//上に戻る
		m_CountPos.y = Ease(In, Cubic, 0.5f, m_CountPos.y, -10.0f);
		m_MagPos.y = Ease(In, Cubic, 0.5f, m_MagPos.y, -10.0f);

		if (m_CountPos.y <= -8.0f) {
			m_Alive = false;
		}
	}

	Count_Sprite[m_EnemyCount]->SetPosition(m_CountPos);
	Count_Sprite2[0]->SetPosition({ m_CountPos.x + 30.0f,m_CountPos.y });
	Mag_Sprite[m_Magnification]->SetPosition(m_MagPos);
	Base_Sprite->SetPosition({ 1200.0f,m_CountPos.y });
}

//描画
void MagText::Draw() {
	Count_Sprite[m_EnemyCount]->Draw();
	Count_Sprite2[0]->Draw();
	Mag_Sprite[m_Magnification]->Draw();
	Base_Sprite->Draw();
}