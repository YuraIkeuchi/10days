#include "Slow.h"
#include "imgui.h"
#include "CsvLoader.h"
Slow* Slow::GetInstance()
{
	static Slow instance;

	return &instance;
}
//CSV
void Slow::LoadCSV() {
	//m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/effect/Slow.csv", "Timer")));
}
//初期化
void Slow::Initialize() {
	m_Slow = false;//ヒットストップ
	m_SlowTimer = 0;//ヒットストップの時間

	m_SlowPower = 1.0f;
	m_PlayerSlowPower = 1.0f;
	m_Tutorial = false;
}
//更新
void Slow::Update() {
	if (m_Slow) {
		if (!m_Tutorial) {
			m_SlowPower = 0.2f;
			m_PlayerSlowPower = 0.5f;
			m_SlowTimer--;
		}
		else {
			m_SlowPower = 0.0f;
			m_PlayerSlowPower = 0.0f;
		}

		if (m_SlowTimer <= 0) {
			m_SlowTimer = {};
			m_Slow = false;
		}
	}
	else {
		m_SlowTimer = {};
		m_SlowPower = 1.0f;
		m_PlayerSlowPower = 1.0f;
	}
}
void Slow::ImGuiDraw() {
	ImGui::Begin("Slow");
	ImGui::Text("Power:%f", m_SlowPower);
	ImGui::Text("Timer:%d", m_SlowTimer);
	
	ImGui::End();
}