#include "TutorialTask.h"
#include "ImageManager.h"
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

//初期化
void TutorialTask::Initialize() {
	m_Stop = false;
	window = IKESprite::Create(ImageManager::BLACKWINDOW, {});
	window->SetSize({ 1280.0f,720.0f });
}
//更新
void TutorialTask::Update() {
	
}
//描画
void TutorialTask::Draw() {
	if (m_Stop) {
		window->Draw();
	}
}