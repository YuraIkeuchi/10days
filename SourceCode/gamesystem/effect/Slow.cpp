#include "Slow.h"
#include "imgui.h"
#include "CsvLoader.h"
Slow* Slow::GetInstance()
{
	static Slow instance;

	return &instance;
}

void Slow::LoadCSV() {
	//m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/effect/Slow.csv", "Timer")));
}
void Slow::Update() {
	if (m_Slow) {
		m_SlowPower = 0.001f;
		m_SlowTimer++;
		if (m_SlowTimer == 20) {
			m_Slow = false;
		}
	}
	else {
		m_SlowTimer = {};
		m_SlowPower = 1.0f;
	}
}
void Slow::ImGuiDraw() {
	ImGui::Begin("Hit");
	ImGui::Text("Hit:%f", m_SlowPower);
	
	ImGui::End();
}