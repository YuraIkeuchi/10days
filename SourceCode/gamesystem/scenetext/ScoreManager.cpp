#include "ScoreManager.h"
#include "imgui.h"
#include "ImageManager.h"
#include "Helper.h"
ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;

	return &instance;
}

//読み込み
void ScoreManager::LoadResource() {
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Score_First.size(); i++) {
		//ひとけた目
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
		//二桁め
		Score_Second[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Second[i]->SetPosition({ 1210.0f,40.0f });
		Score_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Second[i]->SetScale(0.8f);
		//三桁目
		Score_Third[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Third[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Third[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Third[i]->SetPosition({ 1170.0f,40.0f });
		Score_Third[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Third[i]->SetScale(0.8f);
		//四桁目
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
//初期化
void ScoreManager::Initialize() {
	m_FirstNumber = 0;//一桁目
	m_SecondNumber = 0;//二桁め
	m_ThirdNumber = 0;//三桁め
	m_FourthNumber = 0;//四桁目
	m_Magnification = 0;//倍率
	m_RealScore = 0;//スコア
}
//更新
void ScoreManager::Update() {
	////繰り上げ処理
	////ひとけた目
	//if (m_FirstNumber > NUMBER_MAX - 1) {
	//	m_FirstNumber = m_FirstNumber - NUMBER_MAX;
	//	m_SecondNumber++;
	//}
	////二桁め
	//if (m_SecondNumber > NUMBER_MAX - 1) {
	//	m_SecondNumber = m_SecondNumber - NUMBER_MAX;
	//	m_ThirdNumber++;
	//}
	////三桁目
	//if (m_ThirdNumber > NUMBER_MAX - 1) {
	//	m_ThirdNumber = m_FourthNumber - NUMBER_MAX;
	//	m_FourthNumber++;
	//}
	m_FirstNumber = getDigits(m_RealScore, 1, 1);
	m_SecondNumber = getDigits(m_RealScore, 2, 2);
	m_ThirdNumber = getDigits(m_RealScore, 3, 3);
	m_FourthNumber = getDigits(m_RealScore, 4, 4);

	//Helper::GetInstance()->Clamp(m_FirstNumber, 0, NUMBER_MAX - 1);
}
//ImGui
void ScoreManager::ImGuiDraw() {
	ImGui::Begin("Score");
	ImGui::Text("Score:%d", m_RealScore);
	ImGui::End();
}
//描画
void ScoreManager::SpriteDraw() {
	Score_First[0]->Draw();
	Score_Second[m_FirstNumber]->Draw();
	Score_Third[m_SecondNumber]->Draw();
	Score_Fourth[m_ThirdNumber]->Draw();
}
//割合を返す
int ScoreManager::getDigits(int value, int m, int n)
{
	int mod_value;
	int result;

	/* n桁目以下の桁を取得 */
	mod_value = value % (int)pow(10, n + 1);

	/* m桁目以上の桁を取得 */
	result = mod_value / (int)pow(10, m);

	return result;
}