#include "Timer.h"
#include "Helper.h"

Timer* Timer::GetInstance() {
	static Timer instance;

	return &instance;
}

//初期化
void Timer::Initialize() {
	m_GameTimer = 1800;
	m_End = false;
}
//更新
void Timer::Update() {
	if (Helper::GetInstance()->CheckMax(m_GameTimer, 0, -1)) {
		m_End = true;
	}
}
//ImGui
void Timer::ImGuiDraw() {
	ImGui::Begin("Timer");
	ImGui::Text("Timerr:%d", m_GameTimer);
	ImGui::End();
}