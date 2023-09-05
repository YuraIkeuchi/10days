#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Helper.h"
#include "CsvLoader.h"
#include "Input.h"
#include "Easing.h"
#include "Slow.h"
#include "Collision.h"
#include "ParticleEmitter.h"
#include"Timer.h"
#define MapMinX -10
#define MapMaxX 10

#define MapMinZ -10
#define MapMaxZ 10

//���f���ǂݍ���
NormalEnemy::NormalEnemy() {
	
}
//������
bool NormalEnemy::Initialize() {

	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));

	m_Object->SetColor({ 1.0f,0.0f,0.0,1.0f });
//	m_Position = { 15.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.0f,1.0f };
	m_Object->VertexCheck();

	if(StartState==0)
	{
	//	_charaState =
	}
	_charaState =  StartState;
	//_charaState =  StartState;
	m_BaseSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "speed")));
	return true;
}

void (NormalEnemy::* NormalEnemy::stateTable[])() = {
	&NormalEnemy::Inter,//�����̍���
	&NormalEnemy::RightMove,//�E�Ɉړ�
	&NormalEnemy::LeftMove,//���Ɉړ�
	&NormalEnemy::UpMove,//��Ɉړ�
	&NormalEnemy::BottomMove,//���Ɉړ�

};

//�s��
void NormalEnemy::Action() {
	if (!StopF&&Timer::GetInstance()->getNowTime()<MovingTime) {
		(this->*stateTable[_charaState])();

		//�����蔻
		SlowCollide();
	}
	Obj_SetParam();
}
//�`��
void NormalEnemy::Draw(DirectXCommon* dxCommon) {

	if (StopF|| _charaState != STATE_INTER) {
		Obj_Draw();
	}
}
//ImGui�`��
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("Slow:%f", m_velocity);
	ImGui::End();
}
//�J��
void NormalEnemy::Finalize() {

}

//���X�|�[��
void NormalEnemy::Inter() {
	m_ResPornTimer++;
	if (m_ResPornTimer == 100) {
		m_ResPornTimer = {};
		m_Slow = false;
		_charaState = CharaState::STATE_LEFT;
	}
}
//�E�ɓ���
void NormalEnemy::RightMove() {

	const float l_MAX = MapMaxX;
	m_velocity = m_BaseSpeed;
	m_Position.x += m_velocity * Slow::GetInstance()->GetSlowPower();
	
	if (m_SlowMove) {
		m_velocity = m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = m_BaseSpeed;
	}
	m_Position.x += m_velocity;

	if (Helper::GetInstance()->CheckMin(m_Position.x, l_MAX, m_velocity)) {
		m_Position.x = MapMinX;
		m_Slow = false;
	}
}
//���ɓ���
void NormalEnemy::LeftMove() {
	const float l_MIN =MapMinX;
	m_velocity = -m_BaseSpeed;
	m_Position.x += m_velocity * Slow::GetInstance()->GetSlowPower();
	if (m_SlowMove) {
		m_velocity = -m_BaseSpeed * Slow::GetInstance()->GetSlowPower();
	}
	else {
		m_velocity = -m_BaseSpeed;
	}
	m_Position.x += m_velocity;

	if (Helper::GetInstance()->CheckMax(m_Position.x, l_MIN, m_velocity)) {
		m_Position.x = MapMaxX;
		m_Slow = false;
	}
}
//���ɓ���
void NormalEnemy::BottomMove() {
	const float l_MIN = MapMinZ;
	m_velocity = -m_BaseSpeed;
	m_Position.z += m_velocity * Slow::GetInstance()->GetSlowPower();

	if (Helper::GetInstance()->CheckMax(m_Position.z, l_MIN, m_velocity)) {
		m_Position.z = MapMaxZ;
		m_Slow = false;
	}
}
//���ɓ���
void NormalEnemy::UpMove() {
	const float l_MIN =MapMaxZ;
	m_velocity = m_BaseSpeed;
	m_Position.z += m_velocity * Slow::GetInstance()->GetSlowPower();

	if (Helper::GetInstance()->CheckMin(m_Position.z, l_MIN, m_velocity)) {
		m_Position.z = MapMinZ;
		m_Slow = false;
	}
}

void NormalEnemy::SlowCollide() {
	Input* input = Input::GetInstance();
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, m_radius)) {
		if (!m_Slow) {
			m_Slow = true;
			Slow::GetInstance()->SetSlow(true);
		}
		else {
			if ((input->TriggerButton(input->A))) {
				_charaState = STATE_INTER;
				m_ResPornTimer = {};
				ParticleEmitter::GetInstance()->SplatterEffect(120, Player::GetInstance()->GetPosition(), { 0, 0, 0 }, 1.0f, 1.0f, { 1, 0, 0, 1 });
			}
		}
	}
	else {
		m_Slow = false;
	}
}