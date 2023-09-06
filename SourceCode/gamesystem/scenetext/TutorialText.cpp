#include "TutorialText.h"
TutorialText::TutorialText() {
};
//������
void TutorialText::Initialize(DirectXCommon* dxCommon) {
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::FIRST);
}
//�e�L�X�g�̑I��
void TutorialText::SelectText(TextManager::TUTORIAL name) {
	text_->SetTutorialConversation(name);
}
//�F�ύX
void TutorialText::ChangeColor(int row, const XMVECTOR& color) {
	text_->SetOnceColor(row, color);
}
//���ׂĂ̐F�ύX
void TutorialText::ChangeAllColor(const XMVECTOR& color) {
	text_->SetAllColor(color);
}

void TutorialText::SpriteDraw(DirectXCommon* dxCommon) {
	text_->TestDraw(dxCommon);
}