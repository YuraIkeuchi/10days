#include "TutorialText.h"
TutorialText::TutorialText() {
};
//初期化
void TutorialText::Initialize(DirectXCommon* dxCommon) {
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::FIRST);
}
//テキストの選択
void TutorialText::SelectText(TextManager::TUTORIAL name) {
	text_->SetTutorialConversation(name);
}
//色変更
void TutorialText::ChangeColor(int row, const XMVECTOR& color) {
	text_->SetOnceColor(row, color);
}
//すべての色変更
void TutorialText::ChangeAllColor(const XMVECTOR& color) {
	text_->SetAllColor(color);
}

void TutorialText::SpriteDraw(DirectXCommon* dxCommon) {
	text_->TestDraw(dxCommon);
}