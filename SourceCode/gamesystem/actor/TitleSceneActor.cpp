#include "TitleSceneActor.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"
#include "SceneChanger.h"
#include "ScoreManager.h"
#include "Timer.h"
#include "BackObj.h"
//初期化
void TitleSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_BATTLE, 0.075f);

	text_ = make_unique<TitleText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::FIRST);
	text_->ChangeColor(0, { 1.0f,0.2f,0.0f,1.0f });
	text_->ChangeColor(1, { 0.0f,1.0f,0.2f,1.0f });
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		ScoreManager::GetInstance()->LoadResource();
		Timer::GetInstance()->LoadResource();
		s_GameLoop = true;
	}

	title = IKESprite::Create(ImageManager::TITLE, {});
	title->SetSize({ 1280.0f,720.0f });

	//背景
	BackObj::GetInstance()->Initialize();

	window = IKESprite::Create(ImageManager::TUTORIAL, {});
	window->SetAnchorPoint({ 0.5f,0.5f });
	window->SetPosition({ WinApp::window_width / 2.f,WinApp::window_height - 100 });
	window->SetSize({ 1300.0f, 220.0f });
}
//更新
void TitleSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	camerawork->TitleCamera(camera);
	Input* input = Input::GetInstance();
	//シーンチェンジ
	if ((input->TriggerButton(input->B))) {
		_SceneType = PLAY;
		SceneChanger::GetInstance()->SetChangeStart(true);
		Audio::GetInstance()->PlayWave("Resources/audio/botton.wav", 0.3f);
	}
	if ((input->TriggerButton(input->X))) {
		_SceneType = EDITOR;
		SceneChanger::GetInstance()->SetChangeStart(true);
		SceneManager::GetInstance()->SetEditF(true);
		Audio::GetInstance()->PlayWave("Resources/audio/botton.wav", 0.3f);
	}
	if ((input->TriggerButton(input->A))) {
		_SceneType = TUTORIAL;
		SceneChanger::GetInstance()->SetChangeStart(true);
		Audio::GetInstance()->PlayWave("Resources/audio/botton.wav", 0.3f);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		if (_SceneType == PLAY) {
			SceneManager::GetInstance()->ChangeScene("FIRSTSTAGE");
		}
		else if (_SceneType == EDITOR) {
			SceneManager::GetInstance()->ChangeScene("EDITOR");
		}
		else {
			SceneManager::GetInstance()->ChangeScene("TUTORIAL");
		}
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
	BackObj::GetInstance()->Update();
}
//描画
void TitleSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//前面描画
void TitleSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	window->Draw();
	IKESprite::PostDraw();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	text_->SpriteDraw(dxCommon);
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	title->Draw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//背面描画
void TitleSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ImGui描画
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//ImGui::Begin("TITLE");
	//ImGui::Text("Title");
	//ImGui::End();
}
//解放
void TitleSceneActor::Finalize() {
}
