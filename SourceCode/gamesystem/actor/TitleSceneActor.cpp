#include "TitleSceneActor.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"
#include "SceneChanger.h"
//初期化
void TitleSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	text_ = make_unique<TitleText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::FIRST);
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}
}
//更新
void TitleSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	camerawork->Update(camera);
	Input* input = Input::GetInstance();
	//シーンチェンジ
	if ((input->TriggerButton(input->B))) {
		_SceneType = PLAY;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	if ((input->TriggerButton(input->A))) {
		_SceneType = EDITOR;
		SceneChanger::GetInstance()->SetChangeStart(true);
		SceneManager::GetInstance()->SetEditF(true);
	}
	if ((input->TriggerButton(input->X))) {
		_SceneType = TUTORIAL;
		SceneChanger::GetInstance()->SetChangeStart(true);
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
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	text_->SpriteDraw(dxCommon);
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//背面描画
void TitleSceneActor::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui描画
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("TITLE");
	ImGui::Text("Title");
	ImGui::End();
}
//解放
void TitleSceneActor::Finalize() {
}
