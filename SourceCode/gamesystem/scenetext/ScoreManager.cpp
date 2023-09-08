#include "ScoreManager.h"
#include "imgui.h"
#include "ImageManager.h"
#include "Helper.h"
ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;

	return &instance;
}

//�ǂݍ���
void ScoreManager::LoadResource() {
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Score_First.size(); i++) {
		//�ЂƂ�����
		Score_First[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		Score_First[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_First[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_First[i]->SetPosition({ 1250.0f,40.0f });
		Score_First[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_First[i]->SetScale(0.8f);
		//�񌅂�
		Score_Second[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Second[i]->SetPosition({ 1210.0f,40.0f });
		Score_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Second[i]->SetScale(0.8f);
		//�O����
		Score_Third[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Third[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Third[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Third[i]->SetPosition({ 1170.0f,40.0f });
		Score_Third[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Third[i]->SetScale(0.8f);
		//�l����
		Score_Fourth[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Fourth[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Fourth[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Fourth[i]->SetPosition({ 1130.0f,40.0f });
		Score_Fourth[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Fourth[i]->SetScale(0.8f);
	}
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
	if (m_FirstNumber > NUMBER_MAX - 1) {
		m_FirstNumber = m_FirstNumber - NUMBER_MAX;
		m_SecondNumber++;
	}
	//�񌅂�
	if (m_SecondNumber > NUMBER_MAX - 1) {
		m_SecondNumber = m_SecondNumber - NUMBER_MAX;
		m_ThirdNumber++;
	}
	//�O����
	if (m_ThirdNumber > NUMBER_MAX - 1) {
		m_ThirdNumber = m_FourthNumber - NUMBER_MAX;
		m_FourthNumber++;
	}

	//Helper::GetInstance()->Clamp(m_FirstNumber, 0, NUMBER_MAX - 1);
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
//�`��
void ScoreManager::SpriteDraw() {
	Score_First[m_FirstNumber]->Draw();
	Score_Second[m_SecondNumber]->Draw();
	Score_Third[m_ThirdNumber]->Draw();
	Score_Fourth[m_FourthNumber]->Draw();
}