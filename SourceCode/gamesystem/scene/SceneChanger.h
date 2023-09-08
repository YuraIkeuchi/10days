#pragma once
#include "Easing.h"
#include "IKESprite.h"
#include <memory>
#include <array>

using namespace std;         //  ���O��Ԏw��
//�V�[���`�F���W�N���X
class SceneChanger {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static SceneChanger* GetInstance();
	
	/// <summary>
	/// ����k����
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// ����k����
	/// </summary>
	void InitializeOver();
	//�`��
	void Draw();

private:
	static const int DOOR_NUM = 2;
public:
	const bool GetChange() { return m_Change; }
	void SetChange(const bool Change) { m_Change = Change; }
	void SetChangeStart(const bool ChangeStart) { m_ChangeStart = ChangeStart; }
private:
	unique_ptr<IKESprite> left_door;
	unique_ptr<IKESprite> right_door;
	array<XMFLOAT2, DOOR_NUM> m_Position = {};
	array<XMFLOAT2, DOOR_NUM> m_AfterPos = {};
	float m_Frame = {};
	enum DOORTYPE {
		DOOR_LEFT,
		DOOR_RIGHT,
	};

	enum ChangeType {
		OPEN_DOOR,
		CLOSE_DOOR
	}_doorState = OPEN_DOOR;

	bool m_ChangeStart = false;
	bool m_Change = false;
	int m_ChangeTimer = {};
};