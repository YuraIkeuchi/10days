#include "TutorialTask.h"
#include "ImageManager.h"
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

//������
void TutorialTask::Initialize() {
	m_Stop = false;
	window = IKESprite::Create(ImageManager::BLACKWINDOW, {});
	window->SetSize({ 1280.0f,720.0f });
}
//�X�V
void TutorialTask::Update() {
	
}
//�`��
void TutorialTask::Draw() {
	if (m_Stop) {
		window->Draw();
	}
}