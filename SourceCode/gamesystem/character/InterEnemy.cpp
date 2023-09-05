#include "InterEnemy.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	//�s��
	Action();
}
void InterEnemy::SetResPos(PosSt pos)
{
	constexpr float PosX_Min = -10.f;
	constexpr float PosX_Max = 10.f;

	constexpr float PosZ_Min = -10.f;
	constexpr float PosZ_Max = 10.f;

	if (pos == PosSt::UPRES)m_Position.z = PosZ_Min;

	else if (pos == PosSt::BOTTOMRES)m_Position.z = PosZ_Max;

	else if (pos == PosSt::RIGHTRES)m_Position.x = PosX_Max;

	else if (pos == PosSt::LEFTRES)m_Position.x = PosX_Min;

}

//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::ImGuiDraw() {
	ImGui::Begin("--Pos--");
	ImGui::End();
	ImGui_Origin();
}