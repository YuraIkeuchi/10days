#include "ScoreManager.h"
#include "imgui.h"
ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;

	return &instance;
}

//������
void ScoreManager::Initialize() {
	m_FirstNumber = 0;//�ꌅ��
	m_SecondNumber = 0;//�񌅂�
	m_ThirdNumber = 0;//�O����
	m_FourthNumber = 0;//�l����
	m_Magnification = 0;//�{��
}
//�X�V
void ScoreManager::Update() {
	//�J��グ����
	//�ЂƂ�����
	if (m_FirstNumber >= NUMBER_MAX) {
		m_FirstNumber = m_FirstNumber - NUMBER_MAX;
		m_SecondNumber++;
	}
	//�񌅂�
	if (m_SecondNumber >= NUMBER_MAX) {
		m_SecondNumber = m_SecondNumber - NUMBER_MAX;
		m_ThirdNumber++;
	}
	//�O����
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