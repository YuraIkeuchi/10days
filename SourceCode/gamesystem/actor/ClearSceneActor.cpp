#include "ClearSceneActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"
#include "ScoreManager.h"
//������
void ClearSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);

	//�I�[�f�B�I
	//Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	//Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());
	//�V�[���`�F���W���[
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < Sprite_First.size(); i++) {
		//�ЂƂ�����
		Sprite_First[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		Sprite_First[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Sprite_First[i]->SetAnchorPoint({ 0.5f,0.5f });
		Sprite_First[i]->SetPosition({ 700.0f,200.0f });
		Sprite_First[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		//�񌅂�
		Sprite_Second[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Sprite_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Sprite_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		Sprite_Second[i]->SetPosition({ 600.0f,200.0f });
		Sprite_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		//�O����
		Sprite_Third[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Sprite_Third[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Sprite_Third[i]->SetAnchorPoint({ 0.5f,0.5f });
		Sprite_Third[i]->SetPosition({ 500.0f,200.0f });
		Sprite_Third[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		//�l����
		Sprite_Fourth[i] = IKESprite::Create(ImageManager::NUMBER, { 0.0f,0.0f });
		Sprite_Fourth[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		Sprite_Fourth[i]->SetAnchorPoint({ 0.5f,0.5f });
		Sprite_Fourth[i]->SetPosition({ 400.0f,200.0f });
		Sprite_Fourth[i]->SetSize({ l_Width_Cut,l_Height_Cut });
	}

	back = IKESprite::Create(ImageManager::RESULT, { 0.0f,0.0f });
	back->SetSize({ 1280.0f,720.0f });
}
//�X�V
void ClearSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	lightgroup->Update();
	
}
//�`��
void ClearSceneActor::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
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
//�O�ʕ`��
void ClearSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	back->Draw();
	Sprite_First[ScoreManager::GetInstance()->GetFirstNumber()]->Draw();
	Sprite_Second[ScoreManager::GetInstance()->GetSecondNumber()]->Draw();
	Sprite_Third[ScoreManager::GetInstance()->GetThirdNumber()]->Draw();
	Sprite_Fourth[ScoreManager::GetInstance()->GetFourthNumber()]->Draw();
	IKESprite::PostDraw();
}
//�w��
void ClearSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ImGui�`��
void ClearSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ScoreManager::GetInstance()->ImGuiDraw();
}
//���
void ClearSceneActor::Finalize() {
}
