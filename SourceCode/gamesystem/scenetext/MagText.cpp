#include "MagText.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"

//�ǂݍ���
MagText::MagText(const int EnemyCount, const int Magnification) {
	m_EnemyCount = EnemyCount;
	m_Magnification = Magnification;

	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Count_Sprite.size(); i++) {
		//�ЂƂ�����
		Count_Sprite[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		Count_Sprite[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Count_Sprite[i]->SetAnchorPoint({ 0.5f,0.5f });
		Count_Sprite[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Count_Sprite[i]->SetScale(0.8f);
		//�񌅂�
		Mag_Sprite[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Mag_Sprite[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Mag_Sprite[i]->SetAnchorPoint({ 0.5f,0.5f });
		Mag_Sprite[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Mag_Sprite[i]->SetScale(0.8f);
	}

	Base_Sprite = IKESprite::Create(ImageManager::MAGNIFICATION, {});
	Base_Sprite->SetSize({ 64.0f,64.0f });
	Base_Sprite->SetAnchorPoint({ 0.5f, 0.5f });
	Base_Sprite->SetScale(0.8f);
}
//������
void MagText::Initialize() {
	m_CountPos = { 1140.0f,-10.0f };
	m_MagPos = { 1240.0f,-10.0f };
	m_Alive = true;
}
//�X�V
void MagText::Update() {
	const int l_TimerMax = 30;//�\������鎞��
	//�ォ�牺�ɗ����Ă���
	if (_TextType == BIRTH_TEXT) {
		if (Helper::GetInstance()->CheckMin(m_Timer, 50, 1)) {
			_TextType = VANISH_TEXT;
		}
		m_CountPos.y = Ease(In, Cubic, 0.5f, m_CountPos.y, 130.0f);
		m_MagPos.y = Ease(In, Cubic, 0.5f, m_MagPos.y, 130.0f);
	}
	else {		//��ɖ߂�
		m_CountPos.y = Ease(In, Cubic, 0.5f, m_CountPos.y, -10.0f);
		m_MagPos.y = Ease(In, Cubic, 0.5f, m_MagPos.y, -10.0f);

		if (m_CountPos.y <= -8.0f) {
			m_Alive = false;
		}
	}

	Count_Sprite[m_EnemyCount]->SetPosition(m_CountPos);
	Mag_Sprite[m_Magnification]->SetPosition(m_MagPos);
	Base_Sprite->SetPosition({ 1190.0f,m_CountPos.y });
}

//�`��
void MagText::Draw() {
	Count_Sprite[m_EnemyCount]->Draw();
	Mag_Sprite[m_Magnification]->Draw();
	Base_Sprite->Draw();
}