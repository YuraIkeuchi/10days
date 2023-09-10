#include "SlashEffect.h"
#include "ImageManager.h"
#include "Random.h"
#include "Helper.h"
#include "Easing.h"
SlashEffect::SlashEffect(const XMFLOAT3& StartPos) {
	m_Position = StartPos;

	slash.reset(IKETexture::Create(ImageManager::SLASH, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	slash->TextureCreate();
	float l_RotZ = static_cast<float>(Random::GetRanNum(0, 180));
	slash->SetRotation({ 90.0f,0.0f,0.0f });
	slash->SetPosition(m_Position);
	slash->SetScale(m_Scale);
	//slash->SetIsBillboard(true);
}
//‰Šú‰»
void SlashEffect::Initialize() {

}
//XV
void SlashEffect::Update() {
	const float l_AddFrame = 0.025f;
	if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
		m_Alive = false;
	}

	m_Scale = { Ease(In, Cubic, m_Frame, m_Scale.x, 1.5f),
		Ease(In, Cubic, m_Frame, m_Scale.y, 1.5f),
		Ease(In, Cubic, m_Frame, m_Scale.z, 1.5f),
	};
	m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);

	slash->SetPosition(m_Position);
	slash->SetScale(m_Scale);
	slash->SetColor(m_Color);
	slash->Update();
}
//•`‰æ
void SlashEffect::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	if (m_Alive) {
		slash->Draw();
	}
	IKETexture::PostDraw();
}
void SlashEffect::ImGuiDraw() {
	ImGui::Begin("Slash");
	ImGui::Text("Alpha%f", m_Color.w);
	ImGui::End();
}