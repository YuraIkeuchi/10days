#include "BackObj.h"
#include "ModelManager.h"
#include "ImageManager.h"
#include "JsonLoader.h"
BackObj* BackObj::GetInstance() {
	static BackObj instance;

	return &instance;
}

//初期化
void BackObj::Initialize() {
	for (int i = 0; i < Up_Obj.size(); i++) {
		Up_Obj[i].reset(new IKEObject3d());
		Up_Obj[i]->Initialize();
		Up_Obj[i]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::WALL_UP));
		Up_Obj[i]->SetScale({ 1.0f,1.0f,1.8f });
		//Up_Obj[i]->SetTiling(2.0f);
	}

	Up_Obj[0]->SetPosition({ 0.0f,1.3f,20.0f });
	Up_Obj[1]->SetPosition({  0.0f,1.3f,-20.0f });
	Up_Obj[2]->SetPosition({ -20.0f,1.3f,0.0f });
	Up_Obj[3]->SetPosition({ 20.0f,1.3f,0.0f });
	//Up_Obj[2]->SetPosition({ -16.5f,1.3f, 15.0f });
	//Up_Obj[3]->SetPosition({ 16.5f,1.3f,  15.0f });
	//Up_Obj[4]->SetPosition({ -16.5f,1.3f,-8.0f });
	//Up_Obj[5]->SetPosition({  -16.5f,1.3f,8.0f });
	//Up_Obj[6]->SetPosition({ 16.5f,1.3f, -8.0f });
	//Up_Obj[7]->SetPosition({ 16.5f,1.3f,  8.0f });

	for (int i = 0; i < 2; i++) {
		Up_Obj[i]->SetRotation({ 0.0f,90.0f,0.0f });
	}
	for (int i = 2; i < WALL_NUM; i++) {
		Up_Obj[i]->SetRotation({ 0.0f,0.0f,0.0f });
	}
	//for (int i = 4; i < WALL_NUM; i++) {
	//	Up_Obj[i]->SetRotation({ 0.0f,0.0f,0.0f });
	//}

	for (int i = 0; i < Under_Obj.size(); i++) {
		Under_Obj[i].reset(new IKEObject3d());
		Under_Obj[i]->Initialize();
		Under_Obj[i]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::WALL_UNDER));
		
		Under_Obj[i]->SetRotation(Up_Obj[i]->GetRotation());
		Under_Obj[i]->SetPosition({ Up_Obj[i]->GetPosition().x,0.0f,Up_Obj[i]->GetPosition().z});
		Under_Obj[i]->SetScale(Up_Obj[i]->GetScale());
	}

	//テクスチャ
	ground.reset(IKETexture::Create(ImageManager::GROUND, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	ground->TextureCreate();
	ground->SetRotation({ 90.0f,0.0f,0.0f });
	ground->SetPosition({ 0.0f,0.1f,0.0f });
	ground->SetScale({ 2.1f,1.8f,1.8f });

	grass.reset(IKETexture::Create(ImageManager::GRASS, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	grass->TextureCreate();
	grass->SetRotation({ 90.0f,0.0f,0.0f });
	grass->SetPosition({ 0.0f,0.2f,0.0f });
	grass->SetScale({ 1.8f,1.5f,1.5f });

	//テクスチャ
	base_ground.reset(IKETexture::Create(ImageManager::BASE_GROUND, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	base_ground->TextureCreate();
	base_ground->SetRotation({ 90.0f,0.0f,0.0f });
	base_ground->SetPosition({ 0.0f,0.0f,0.0f });
	base_ground->SetScale({ 8.0f,8.0f,8.0f });
	base_ground->SetTiling(10.0f);
}
//更新
void BackObj::Update() {
	for (int i = 0; i < Under_Obj.size(); i++) {
		Up_Obj[i]->Update();
		Up_Obj[i]->UpdateWorldMatrix();
		Under_Obj[i]->Update();
		Under_Obj[i]->UpdateWorldMatrix();
	}

	ground->Update();
	grass->Update();
	base_ground->Update();
}
//描画
void BackObj::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (int i = 0; i < Under_Obj.size(); i++) {
		Up_Obj[i]->Draw();
		Under_Obj[i]->Draw();
	}
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon,AlphaBlendType);
	base_ground->Draw();
	ground->Draw();
	grass->Draw();
	IKETexture::PostDraw();
}