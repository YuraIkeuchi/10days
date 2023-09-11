#include "Slow.h"
#include "imgui.h"
#include "CsvLoader.h"
#include "Audio.h"
Slow* Slow::GetInstance()
{
	static Slow instance;

	return &instance;
}
//CSV
void Slow::LoadCSV() {
	//m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/effect/Slow.csv", "Timer")));
}
//������
void Slow::Initialize() {
	m_Slow = false;//�q�b�g�X�g�b�v
	m_SlowTimer = 0;//�q�b�g�X�g�b�v�̎���

	m_SlowPower = 1.0f;
	m_PlayerSlowPower = 1.0f;
	m_Tutorial = false;
}
//�X�V
void Slow::Update() {
	if (m_Slow) {
		if (m_SlowTimer == 0)
		{
			Audio::GetInstance()->PlayWave("Resources/audio/result.wav", 0.1f);
		}
		if (!m_Tutorial) {
			m_SlowPower = 0.1f;
			m_PlayerSlowPower = 0.2f;
			m_MovePower = 0.1f;
		}
		else {
			m_SlowPower = 0.0f;
			m_PlayerSlowPower = 0.0f;
		}
		m_SlowTimer++;
	}
	else {
		m_SlowTimer = 0;
		m_SlowPower = 1.0f;
		m_PlayerSlowPower = 1.0f;
		m_MovePower = 1.0f;
	}
}
void Slow::ImGuiDraw() {
	ImGui::Begin("Slow");
	ImGui::Text("Power:%f", m_SlowPower);
	ImGui::Text("Timer:%d", m_SlowTimer);
	ImGui::Text("SlowFrag:%d", m_Slow);
	ImGui::End();
}