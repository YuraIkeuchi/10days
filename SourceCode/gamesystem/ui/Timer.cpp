#include "Timer.h"
#include "Helper.h"
#include "Slow.h"

Timer* Timer::GetInstance() {
	static Timer instance;

	return &instance;
}

//‰Šú‰»
void Timer::Initialize() {
	m_GameTimer = 1800;
	m_SlowTimer = {};
	m_End = false;
}
//XV
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