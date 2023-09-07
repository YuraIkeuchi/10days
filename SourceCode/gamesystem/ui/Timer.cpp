#include "Timer.h"
#include "Helper.h"
#include "Slow.h"
#include "ImageManager.h"
Timer* Timer::GetInstance() {
	static Timer instance;

	return &instance;
}
void Timer::LoadResource() {

	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Time_First.size(); i++) {
		//ひとけた目
		Time_First[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		Time_First[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Time_First[i]->SetAnchorPoint({ 0.5f,0.5f });
		Time_First[i]->SetPosition({ 1040.0f,40.0f });
		Time_First[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Time_First[i]->SetScale(0.8f);
		//二桁め
		Time_Second[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Time_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Time_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		Time_Second[i]->SetPosition({ 1000.0f,40.0f });
		Time_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Time_Second[i]->SetScale(0.8f);
	}
}
//初期化
void Timer::Initialize() {
	m_GameTimer = 1800;
	m_SlowTimer = {};
	m_End = false;
	m_FirstCount = 0;
	m_SecondCount = 3;
}
//更新
void Timer::Update() {
	if (!m_Stop) {
		if (!Slow::GetInstance()->GetSlow()) {
			if (Helper::GetInstance()->CheckMax(m_GameTimer, 0, -1)) {
				m_End = true;
			}
			m_SlowTimer = {};
		}
		else {
			m_SlowTimer++;
			if (m_SlowTimer % 5 == 0 && m_SlowTimer != 0) {
				if (Helper::GetInstance()->CheckMax(m_GameTimer, 0, -1)) {
					m_End = true;
				}
			}
		}
	}

	//タイマーに合わせてUIも減らす
	if (m_GameTimer % 60 == 0) {
		m_FirstCount--;
		if (m_FirstCount < 0) {
			m_FirstCount += 10;
			m_SecondCount--;
		}
	}
}
void Timer::SpriteDraw() {
	if (m_GameTimer > 0) {
		Time_First[m_FirstCount]->Draw();
		Time_Second[m_SecondCount]->Draw();
	}
}
//ImGui
void Timer::ImGuiDraw() {
	ImGui::Begin("Timer");
	ImGui::Text("Timer:%d", m_GameTimer);
	ImGui::Text("SlowTimer:%d", m_SlowTimer);
	if (ImGui::RadioButton("Stop", &m_Stop)) {
		m_Stop = true;
	}
	else if (ImGui::RadioButton("NoStop", &m_Stop)) {
		m_Stop = false;
	}
	if (m_Stop) {
		ImGui::Text("Stop");
	}
	else {
		ImGui::Text("NoStop");
	}
	ImGui::InputInt("Timer", &m_GameTimer);
	ImGui::End();
}