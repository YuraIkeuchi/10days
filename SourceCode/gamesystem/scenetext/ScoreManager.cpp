#include "ScoreManager.h"
#include "imgui.h"
ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;

	return &instance;
}

//初期化
void ScoreManager::Initialize() {
	m_FirstNumber = 0;//一桁目
	m_SecondNumber = 0;//二桁め
	m_ThirdNumber = 0;//三桁め
	m_FourthNumber = 0;//四桁目
	m_Magnification = 0;//倍率
}
//更新
void ScoreManager::Update() {
	//繰り上げ処理
	//ひとけた目
	if (m_FirstNumber >= NUMBER_MAX) {
		m_FirstNumber = m_FirstNumber - NUMBER_MAX;
		m_SecondNumber++;
	}
	//二桁め
	if (m_SecondNumber >= NUMBER_MAX) {
		m_SecondNumber = m_SecondNumber - NUMBER_MAX;
		m_ThirdNumber++;
	}
	//三桁目
	if (m_ThirdNumber >= NUMBER_MAX) {
		m_ThirdNumber = m_FourthNumber - NUMBER_MAX;
		m_FourthNumber++;
	}
}
//ImGui
void ScoreManager::ImGuiDraw() {
	ImGui::Begin("Score");
	ImGui::Text("First:%d", m_FirstNumber);
	ImGui::Text("Second:%d", m_SecondNumber);
	ImGui::Text("Third:%d", m_ThirdNumber);
	ImGui::Text("Fourth:%d", m_FourthNumber);
	ImGui::Text("Magnification:%d", m_Magnification);
	ImGui::End();
}