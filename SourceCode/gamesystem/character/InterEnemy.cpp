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
void InterEnemy::SetResPos(PosSt pos,float otpos)
{
	constexpr float PosX_Min = -10.f;
	constexpr float PosX_Max = 10.f;

	constexpr float PosZ_Min = -10.f;
	constexpr float PosZ_Max = 10.f;

	if (pos == PosSt::UPRES)
	{
		m_Position.x = otpos;
	m_Position.z = PosZ_Max;
		StartState = CharaState::STATE_DOWN;
	}

	else if (pos == PosSt::BOTTOMRES) {
		m_Position.x = otpos;
		m_Position.z = PosZ_Min;
		StartState = CharaState::STATE_UP;
		
	}
	else if (pos == PosSt::RIGHTRES) {
		m_Position.z = otpos;
		StartState = CharaState::STATE_LEFT;
		m_Position.x = PosX_Max;
	}
	else if (pos == PosSt::LEFTRES) {
		m_Position.z = otpos;
		StartState = CharaState::STATE_RIGHT;
		m_Position.x = PosX_Min;
	}
}

//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::ImGuiDraw() {
	ImGui::Begin("--Pos--");
	ImGui::End();
	ImGui_Origin();
}