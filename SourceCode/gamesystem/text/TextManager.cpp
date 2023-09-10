#include "TextManager.h"

const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };


TextManager* TextManager::GetInstance()
{
	static TextManager instance;
	return &instance;
}

void TextManager::Create(DirectXCommon* dxcomon)
{
	conversation_.FirstFont = new Font();
	conversation_.SecondFont = new Font();
	conversation_.ThirdFont = new Font();
	conversation_.FirstFont->LoadFont(dxcomon);
	conversation_.SecondFont->LoadFont(dxcomon);
	conversation_.ThirdFont->LoadFont(dxcomon);
	conversation_.FirstFont->SetColor(color_);
	conversation_.SecondFont->SetColor(color_);
	conversation_.ThirdFont->SetColor(color_);
	old_conversation_.FirstFont = new Font();
	old_conversation_.SecondFont = new Font();
	old_conversation_.ThirdFont = new Font();
	old_conversation_.FirstFont->LoadFont(dxcomon);
	old_conversation_.SecondFont->LoadFont(dxcomon);
	old_conversation_.ThirdFont->LoadFont(dxcomon);
	old_conversation_.FirstFont->SetColor(color_);
	old_conversation_.SecondFont->SetColor(color_);
	old_conversation_.ThirdFont->SetColor(color_);
}

//初期化
void TextManager::Initialize(DirectXCommon* dxcomon)
{
	//ワード追加
	//タイトル
	CreateWord(FIRST, L"Bボタンでゲーム", L"Aボタンでチュートリアル", L"Xボタンでエディタ");
	//チュートリアル
	CreateTutorialWord(INTRO, L"いきなりだが", L"この戦いについて", L"説明しよう");
	CreateTutorialWord(MOVE, L"まずスティックを動かして", L"このステージの外周を", L"移動してみろ");
	CreateTutorialWord(ATTACK, L"その調子だ", L"次に攻撃について説明する", L"");
	CreateTutorialWord(ATTACK2, L"Aボタンを押すと", L"対角に移動しながら", L"攻撃をすることができる");
	CreateTutorialWord(ATTACK3, L"敵が出てきたぞ", L"しかしさっきの操作だけでは", L"敵を倒せない");
	CreateTutorialWord(ATTACK4, L"倒し方について説明する", L"敵に狙いを定めて", L"Aボタンを押してみろ");
	CreateTutorialWord(ATTACK5, L"敵に近づくと", L"動きが遅くなる", L"その瞬間に指定のボタンを押すと");
	CreateTutorialWord(ATTACK6, L"敵を倒すことができる", L"敵を倒すとスコア獲得だ", L"");
	CreateTutorialWord(ATTACK7, L"一回の攻撃で敵を多く倒すと", L"より多くのスコアがもらえる", L"つまり・・・");
	CreateTutorialWord(ATTACK8, L"敵が並んだ瞬間に", L"攻撃を仕掛けて", L"多くのスコアを獲得するのだ");
	CreateTutorialWord(ATTACK9, L"この後複数体の敵を出すので", L"実際に試してみると良い", L"");
	CreateTutorialWord(END, L"説明はここまでだ", L"健闘を祈る・・・", L"");
	//コンヴァージョン初期化
	Create(dxcomon);

	//フォントのあれこれ
	conversation_.FirstFont->SetPos(kFirstRowPos);
	conversation_.SecondFont->SetPos(kSecondRowPos);
	conversation_.ThirdFont->SetPos(kThirdRowPos);
}
//描画?
void TextManager::Draw(DirectXCommon* dxcommon)
{
	//コンヴァージョン.フォントドローする
	conversation_.FirstFont->Draw(dxcommon);	
	conversation_.SecondFont->Draw(dxcommon);
	conversation_.ThirdFont->Draw(dxcommon);
	Font::PostDraw(dxcommon);
}

void TextManager::TestDraw(DirectXCommon* dxcommon)
{

	length = 0;
	test = conversation_.FirstFont->GetString();
	test1 = conversation_.SecondFont->GetString();
	test2 = conversation_.ThirdFont->GetString();


	len[0] = wcslen(test);
	len[1] = wcslen(test1);
	len[2] = wcslen(test2);

	
	conversation_.FirstFont->TestSet(test,len[0],flag[0],next_f[0]);
	conversation_.FirstFont->Draw(dxcommon);
	if (next_f[0] == true) {
		conversation_.SecondFont->TestSet(test1, len[1], flag[1],next_f[1]);
		conversation_.SecondFont->Draw(dxcommon);
	}
	if (next_f[1] == true) {
		conversation_.ThirdFont->TestSet(test2, len[2], flag[2],next_f[2]);
		conversation_.ThirdFont->Draw(dxcommon);
	}

	Font::PostDraw(dxcommon);
}

void TextManager::Test()
{
	wchar_t* test = conversation_.FirstFont->GetString();
	wchar_t* test1 = conversation_.SecondFont->GetString();
	wchar_t* test2 = conversation_.ThirdFont->GetString();

	conversation_.FirstFont->SetString(test);
	conversation_.SecondFont->SetString(test1);
	conversation_.ThirdFont->SetString(test2);
}

void TextManager::SetAllColor(const XMVECTOR& color)
{
	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);
}

void TextManager::SetOnceColor(int row, const XMVECTOR& color)
{
	assert(row < 3);
	if (row == 0) {
		conversation_.FirstFont->SetColor(color);
	}
	else if (row == 1) {
		conversation_.SecondFont->SetColor(color);
	}
	else if (row == 2) {
		conversation_.ThirdFont->SetColor(color);
	}
}

//名前から文字列を呼び出しセットする(タイトル)
void TextManager::SetConversation(TITLE name,const XMVECTOR& color)
{
	std::map<TextManager::TITLE, Word>::iterator itr = wordlist_.find(name);

	if (old != itr->first) {
		for (int i = 0; i < 3; i++) {
			flag[i] = true;
			next_f[i] = false;
		}
	}

	old = itr->first;

	GetWordSize(itr->second);
	
	CreateCon(conversation_, itr->second);

	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);
}

//名前から文字列を呼び出しセットする(チュートリアル)
void TextManager::SetTutorialConversation(TUTORIAL name, const XMVECTOR& color)
{
	std::map<TextManager::TUTORIAL, Word>::iterator itr = Tutorialwordlist_.find(name);

	if (oldTutorial != itr->first) {
		for (int i = 0; i < 3; i++) {
			flag[i] = true;
			next_f[i] = false;
		}
	}

	oldTutorial = itr->first;

	GetWordSize(itr->second);

	CreateCon(conversation_, itr->second);

	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);

}

//名前と文字列セットで保存
void TextManager::CreateWord(TITLE name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);

	wordlist_.insert(std::make_pair(name, temp));
}

//名前と文字列セットで保存
void TextManager::CreateTutorialWord(TUTORIAL name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);

	Tutorialwordlist_.insert(std::make_pair(name, temp));
}

void TextManager::SetRowPosition(float posX)
{
	//フォントのあれこれ
	conversation_.FirstFont->SetPos({ posX,kFirstRowPos.y });
	conversation_.SecondFont->SetPos({posX, kSecondRowPos.y });
	conversation_.ThirdFont->SetPos({ posX,kThirdRowPos.y });
}

void TextManager::GetWordSize( Word word)
{
	len[0] = wcslen(word.FirstWord);
	len[1] = wcslen(word.SecondWord);
	len[2] = wcslen(word.ThirdWord);
}

//文字列保存
TextManager::Word TextManager::SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word word = {};
	word.FirstWord = tex1;
	word.SecondWord = tex2;
	word.ThirdWord = tex3;
	return word;
}
void TextManager::NoneText()
{
	std::map<TextManager::TITLE, Word>::iterator itr = wordlist_.find(FIRST);
	
	CreateCon(conversation_, itr->second);
}

void TextManager::NoneTutorialText()
{
	std::map<TextManager::TUTORIAL, Word>::iterator itr = Tutorialwordlist_.find(INTRO);

	CreateCon(conversation_, itr->second);
}

void TextManager::CreateCon(Conversation con, Word word)
{
	con.FirstFont->SetString(word.FirstWord);
	con.SecondFont->SetString(word.SecondWord);
	con.ThirdFont->SetString(word.ThirdWord);
}
