#include "UI.h"
#include "ImageManager.h"
#include "Easing.h"
#include "Helper.h"
#include "Timer.h"
#include "ScoreManager.h"
UI::UI() {
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;
	for (int i = 0; i < number.size(); i++) {
		//‚Ð‚Æ‚¯‚½–Ú
		number[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		number[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		number[i]->SetAnchorPoint({ 0.5f,0.5f });
		number[i]->SetPosition({ 700.0f,200.0f });
		number[i]->SetSize({ l_Width_Cut,l_Height_Cut });
	}

	TimeGage = IKESprite::Create(ImageManager::TIMEGAGE, {});
	TimeGage->SetPosition({ 880.0f,-10.0f });
	TimeGage->SetSize({ 200.0f,100.0f });
	ScoreGage = IKESprite::Create(ImageManager::SCOREGAGE, {});
	ScoreGage->SetPosition({ 1080.0f,-10.0f });
	ScoreGage->SetSize({ 200.0f,100.0f });
}
//‰Šú‰»
void UI::Initialize() {

}
void UI::Update() {

}
void UI::Draw() {
	TimeGage->Draw();
	ScoreGage->Draw();
	Timer::GetInstance()->SpriteDraw();
	ScoreManager::GetInstance()->SpriteDraw();
}