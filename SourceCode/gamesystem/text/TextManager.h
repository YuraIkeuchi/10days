#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
#include <map>
using namespace std;         //  名前空間指定
class TextManager
{
private:
	struct Word {
		wchar_t* FirstWord;//ˆês–Ú
		wchar_t* SecondWord;//“ñs–Ú
		wchar_t* ThirdWord;//ŽOs–Ú
	};
	struct Conversation
	{
		Font* FirstFont;
		Font* SecondFont;
		Font* ThirdFont;
	};

public:
	enum TITLE
	{
		FIRST,
	};

	enum TUTORIAL
	{
		INTRO,
		MOVE,
		ATTACK,
		ATTACK2,
		ATTACK3,
		ATTACK4,
		ATTACK5,
		ATTACK6,
		ATTACK7,
		ATTACK8,
		ATTACK9,
		ATTACK10,
		ATTACK11,
		ATTACK12,
		ATTACK13,
		ATTACK14,
		ATTACK15,
		ATTACK16,
		END,
	};

	static TextManager* GetInstance();

	//
	void Create(DirectXCommon* dxcomon);

	void Initialize(DirectXCommon* dxcomon);

	void Draw(DirectXCommon* dxcommon);

	void TestDraw(DirectXCommon* dxcommon);

	void Test();

	void SetAllColor(const XMVECTOR& color={1.f,1.f,1.f,1.f});

	void SetOnceColor(int row, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void SetConversation(TITLE name= FIRST,const XMVECTOR& color={1.f,1.f,1.f,1.f});

	void SetTutorialConversation(TUTORIAL name = INTRO, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void NoneText();

	void NoneTutorialText();

	void GetWordSize(Word word);

	void SetRowPosition(float posX);
private:
	
	//
	void CreateWord(TITLE name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");

	void CreateTutorialWord(TUTORIAL name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	//
	Word SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3);
	//
	Conversation CreateConversation(Word word);

	void CreateCon(Conversation con, Word word);
private:
	std::map<TextManager::TITLE, Word> wordlist_;

	std::map<TextManager::TUTORIAL, Word> Tutorialwordlist_;

	Conversation conversation_ = {};
	Conversation old_conversation_ = {};

	Conversation bossconversation_ = {};

	XMVECTOR color_{ 1.f,1.f,1.f,1.f };
	size_t len[3];
	bool flag[3] = { true,true,true };
	bool next_f[3] = { false,false,false};

	bool testF = false;
	float time_ = 0.f;
	int length = 0;
	wchar_t* test;
	wchar_t* test1;
	wchar_t* test2;

	TITLE old= FIRST;

	TUTORIAL oldTutorial = INTRO;
};

