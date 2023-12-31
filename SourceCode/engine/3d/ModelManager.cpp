﻿#include"ModelManager.h"
#include "IKEFbxLoader.h"

std::map<ModelManager::ModelName, IKEModel*> ModelManager::model;
std::map<ModelManager::FBXName, IKEFBXModel*> ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instans;
	return &instans;
}

//モデル読み込み(最初から読み込むもの)
void ModelManager::StartInitialize()
{
	LoadModel(WALL_UP, "WallUp");
	LoadModel(WALL_UNDER, "WallUnder");
	LoadModel(PLAYER_1, "Samuraiidle2");

	LoadModel(PLAYER_2, "Samuraiidle");
	LoadModel(ENEMYRED, "slimeRed");
	LoadModel(ENEMYGREEN, "slimeGreen");
	LoadModel(ENEMYBLUE, "slimeBlue");
}

//モデル読み込み(非同期処理で読み込むもの)
void ModelManager::SecondInitialize()
{
}

void ModelManager::Finalize()
{
	model.clear();
	fbxModel.clear();
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new IKEModel();
	model[modelName] = IKEModel::LoadFromOBJ(fileName, true);
}

void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName)
{
	fbxModel[modelName] = new IKEFBXModel();
	fbxModel[modelName] = IKEFbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
