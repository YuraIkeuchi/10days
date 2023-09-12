#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include <array>
/// �N���A�V�[��
class ClearSceneActor : public BaseActor {
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw();
private:
	void ResultMove();
	//������Ԃ�
	int getDigits(int value, int m, int n);
private:
	static const int NUMBER_MAX = 10;
	static const int NEXT_MAX = 2;
	static const int HYOUKA_MAX = 5;
private://�����o�ϐ�
	//�X�R�A
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Score_Fourth;
	//���̕]��
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Next_Fourth;
	array<unique_ptr<IKESprite>, HYOUKA_MAX> Hyouka_;
	array<unique_ptr<IKESprite>, NEXT_MAX> Next_Hyouka;

	unique_ptr<IKESprite> back;
	int m_TargetScore = {};//�ŏI�X�R�A
	int m_TargetNowScore = {};//�ŏI�X�R�A
	int m_FirstNumber = 0;//�ꌅ��
	int m_SecondNumber = 0;//�񌅂�
	int m_ThirdNumber = 0;//�O����
	int m_FourthNumber = 0;//�l����

	int m_NextFirstNumber = 0;//�ꌅ��
	int m_NextSecondNumber = 0;//�񌅂�
	int m_NextThirdNumber = 0;//�O����
	int m_NextFourthNumber = 0;//�l����

	int m_counterTimer = 0;//�X�R�A�̃I�[�f�B�I�^�C�}�[

	enum HyoukaType {
		BUT,//��ԉ�
		NORMAL,//����
		GOOD,//�ǂ�
		VERYGOOD,//�ƂĂ��ǂ�
		SUPER,//�ō�
	};
	int _HyoukaType = BUT;

	array<int, HYOUKA_MAX> m_ScoreLine;

	int m_NextTarget = {};
	int m_NextScore = {};

	enum ResultType {
		SCORE,//�X�R�A���o��
		HYOUKA,//�]�����o��
		NEXT_SCORE,//���܂ł̕]�����o��
		END,//�I���
	};
	int _ResultType = SCORE;

	float m_Alpha = 0.0f;
	XMFLOAT2 m_HyoukaSize = { 400.0f,400.0f };
	float m_Frame = 0.0f;
};
