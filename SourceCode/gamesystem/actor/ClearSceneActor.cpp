#include "ClearSceneActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"
#include "ScoreManager.h"
#include "SceneChanger.h"
#include "Helper.h"
//初期化
void ClearSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);

	//オーディオ
	Audio::GetInstance()->StopWave(AUDIO_BATTLE);
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

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
		Score_First[i]->SetPosition({ 800.0f,300.0f });
		Score_First[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_First[i]->SetScale(1.4f);
		//二桁め
		Score_Second[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Second[i]->SetPosition({ 700.0f,300.0f });
		Score_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Second[i]->SetScale(1.4f);
		//三桁目
		Score_Third[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Third[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Third[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Third[i]->SetPosition({ 600.0f,300.0f });
		Score_Third[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Third[i]->SetScale(1.4f);
		//四桁目
		Score_Fourth[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Score_Fourth[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Score_Fourth[i]->SetAnchorPoint({ 0.5f,0.5f });
		Score_Fourth[i]->SetPosition({ 500.0f,300.0f });
		Score_Fourth[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Score_Fourth[i]->SetScale(1.4f);

		//次の点数まで
		//ひとけた目
		Next_First[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Next_First[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Next_First[i]->SetAnchorPoint({ 0.5f,0.5f });
		Next_First[i]->SetPosition({ 600.0f,500.0f });
		Next_First[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Next_First[i]->SetScale(0.9f);
		//二桁め
		Next_Second[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Next_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Next_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		Next_Second[i]->SetPosition({ 550.0f,500.0f });
		Next_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Next_Second[i]->SetScale(0.9f);
		//三桁目
		Next_Third[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Next_Third[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Next_Third[i]->SetAnchorPoint({ 0.5f,0.5f });
		Next_Third[i]->SetPosition({ 500.0f,500.0f });
		Next_Third[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Next_Third[i]->SetScale(0.9f);
		//四桁目
		Next_Fourth[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Next_Fourth[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Next_Fourth[i]->SetAnchorPoint({ 0.5f,0.5f });
		Next_Fourth[i]->SetPosition({ 450.0f,500.0f });
		Next_Fourth[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		Next_Fourth[i]->SetScale(0.9f);
	}

	//次の評価
	const int NextCount = NEXT_MAX;
	const float l_NextWidth_Cut = 512.0f;
	const float l_NextHeight_Cut = 128.0f;
	for (int i = 0; i < Next_Hyouka.size(); i++) {
		//ひとけた目
		Next_Hyouka[i] = IKESprite::Create(ImageManager::NEXTHYOUKA, { 0.0f,0.0f });
		int number_index_y = i / NextCount;
		int number_index_x = i % NextCount;
		Next_Hyouka[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_NextWidth_Cut, static_cast<float>(number_index_y) * l_NextHeight_Cut },
			{ static_cast<float>(l_NextWidth_Cut), static_cast<float>(l_NextHeight_Cut) });
		Next_Hyouka[i]->SetAnchorPoint({ 0.0f,0.5f });
		Next_Hyouka[i]->SetPosition({ 100.0f,500.0f });
		Next_Hyouka[i]->SetSize({ l_NextWidth_Cut,l_NextHeight_Cut });
		Next_Hyouka[i]->SetScale(0.5f);
	}

	//評価
	const int HyoukaCount = HYOUKA_MAX;
	const float l_HyoukaWidth_Cut = 256.0f;
	const float l_HyoukaHeight_Cut = 256.0f;
	for (int i = 0; i < Hyouka_.size(); i++) {
		//ひとけた目
		Hyouka_[i] = IKESprite::Create(ImageManager::HYOUKA, { 0.0f,0.0f });
		int number_index_y = i / HyoukaCount;
		int number_index_x = i % HyoukaCount;
		Hyouka_[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_HyoukaWidth_Cut, static_cast<float>(number_index_y) * l_HyoukaHeight_Cut },
			{ static_cast<float>(l_HyoukaWidth_Cut), static_cast<float>(l_HyoukaHeight_Cut) });
		Hyouka_[i]->SetAnchorPoint({ 0.5f,0.5f });
		Hyouka_[i]->SetPosition({ 300.0f,300.0f });
		Hyouka_[i]->SetSize({ l_HyoukaWidth_Cut,l_HyoukaHeight_Cut });
	}
	
	back = IKESprite::Create(ImageManager::RESULT, { 0.0f,0.0f });
	back->SetSize({ 1280.0f,720.0f });

	//各評価の点数
	m_ScoreLine[BUT] = 0;
	m_ScoreLine[NORMAL] = 400;
	m_ScoreLine[GOOD] = 800;
	m_ScoreLine[VERYGOOD] = 1200;
	m_ScoreLine[SUPER] = 1600;

	m_TargetNowScore = ScoreManager::GetInstance()->GetRealScore();

	if (m_TargetNowScore < m_ScoreLine[NORMAL]) {
		_HyoukaType = BUT;
	}
	else if (m_TargetNowScore >= m_ScoreLine[NORMAL] && m_TargetNowScore < m_ScoreLine[GOOD]) {
		_HyoukaType = NORMAL;
	}
	else if (m_TargetNowScore >= m_ScoreLine[GOOD] && m_TargetNowScore < m_ScoreLine[VERYGOOD]) {
		_HyoukaType = GOOD;
	}
	else if (m_TargetNowScore >= m_ScoreLine[VERYGOOD] && m_TargetNowScore < m_ScoreLine[SUPER]) {
		_HyoukaType = VERYGOOD;
	}
	else if (m_TargetNowScore >= m_ScoreLine[SUPER]) {
		_HyoukaType = SUPER;
	}
	if (_HyoukaType != SUPER) {
		m_NextTarget = m_ScoreLine[_HyoukaType + 1] - m_TargetNowScore;
	}
	else {
		m_NextTarget = {};
	}
	PlayPostEffect = false;
}
//更新
void ClearSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {
		SceneChanger::GetInstance()->SetChangeStart(true);
		Audio::GetInstance()->PlayWave("Resources/audio/botton.wav", 0.1f);
	}
	if (SceneChanger::GetInstance()->GetChange()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneChanger::GetInstance()->SetChange(false);
	}
	lightgroup->Update();

	SceneChanger::GetInstance()->Update();

	//リザルトの動き
	if (!SceneChanger::GetInstance()->GetChangeStart()) {
		ResultMove();
	}
	for (int i = 0; i < Hyouka_.size(); i++) {
		Hyouka_[i]->SetColor({ 1.0f,1.0f,1.0f,m_Alpha });
		Hyouka_[i]->SetSize(m_HyoukaSize);
	}
}
//描画
void ClearSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw();
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//前面描画
void ClearSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	back->Draw();
	Hyouka_[_HyoukaType]->Draw();
	Score_First[0]->Draw();
	Score_Second[m_FirstNumber]->Draw();
	Score_Third[m_SecondNumber]->Draw();
	Score_Fourth[m_ThirdNumber]->Draw();

	if (_HyoukaType != SUPER) {
		Next_First[0]->Draw();
		Next_Second[m_NextFirstNumber]->Draw();
		Next_Third[m_NextSecondNumber]->Draw();
		Next_Fourth[m_NextThirdNumber]->Draw();
		Next_Hyouka[0]->Draw();
	}
	else {
		Next_Hyouka[1]->Draw();
	}
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//背面
void ClearSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ImGui描画
void ClearSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void ClearSceneActor::Finalize() {
}
//リザルトの動き
void ClearSceneActor::ResultMove() {
	const float l_AddFrame = 0.05f;
	if (_ResultType == SCORE) {
		for (int i = 0; i < 10; i++) {
			if (Helper::GetInstance()->CheckMin(m_TargetScore, m_TargetNowScore, 1)) {
				_ResultType = HYOUKA;
			}
		}
		//スコアに応じて数字が変わる
		m_FirstNumber = getDigits(m_TargetScore, 1, 1);
		m_SecondNumber = getDigits(m_TargetScore, 2, 2);
		m_ThirdNumber = getDigits(m_TargetScore, 3, 3);
		m_FourthNumber = getDigits(m_TargetScore, 4, 4);
	}
	else if (_ResultType == HYOUKA) {		//評価のスタンプは押される
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			_ResultType = NEXT_SCORE;
			Audio::GetInstance()->PlayWave("Resources/audio/result.wav", 0.1f);
		}

		m_Alpha = Ease(In, Cubic, m_Frame, m_Alpha, 1.0f);
		m_HyoukaSize = { Ease(In,Cubic,m_Frame,m_HyoukaSize.x,256.0f),
		Ease(In,Cubic,m_Frame,m_HyoukaSize.y,256.0f) };
	}
	else if (_ResultType == NEXT_SCORE) {		//次の評価まで
		for (int i = 0; i < 10; i++) {
			if (Helper::GetInstance()->CheckMin(m_NextScore, m_NextTarget, 1)) {
				_ResultType = END;
			}
		}

		//スコアに応じて数字が変わる
		m_NextFirstNumber = getDigits(m_NextScore, 1, 1);
		m_NextSecondNumber = getDigits(m_NextScore, 2, 2);
		m_NextThirdNumber = getDigits(m_NextScore, 3, 3);
		m_NextFourthNumber = getDigits(m_NextScore, 4, 4);
	}
	else {

	}
}
//割合を返す
int ClearSceneActor::getDigits(int value, int m, int n)
{
	int mod_value;
	int result;

	/* n桁目以下の桁を取得 */
	mod_value = value % (int)pow(10, n + 1);

	/* m桁目以上の桁を取得 */
	result = mod_value / (int)pow(10, m);

	return result;
}