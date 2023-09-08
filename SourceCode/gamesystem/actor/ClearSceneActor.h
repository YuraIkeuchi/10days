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
	static const int NUMBER_MAX = 10;
private://�����o�ϐ�
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_First;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_Second;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_Third;
	array<unique_ptr<IKESprite>, NUMBER_MAX> Sprite_Fourth;

	unique_ptr<IKESprite> back;
};
