#pragma once

#include "cocos2d.h"

using namespace cocos2d;

/*
	显示我自己各种操作的牌
*/

class GameLayer;
class ShowCard;

class ShowLayer :public Layer
{
public:

	ShowLayer();
	~ShowLayer();

	virtual bool init(GameLayer* _layer);
	static ShowLayer* create(GameLayer* _layer);
private:

	void initData();
	void initUI();

	Sprite* createBigCardSprite(int p_Type, int p_Value);
	Sprite* createSmallCardSprite(int p_Type, int p_Value);

	void createACard();			//玩家出的牌

	void createANewCard();		//新摸的牌

private:
	void showPengCard();		//显示碰的牌
	void showKaiduoCard();		//显示开舵的牌
	void showSaochuanCard();	//显示扫穿的牌
	void showSaoCard();			//显示扫的牌
	void showChiCard();			//显示吃的牌

	void refrishCardPos();		//更新位置
private:
	Sprite* m_ACard;
	Sprite* m_NewCard;
	GameLayer*	m_GameLayer;

	Vector<ShowCard* > m_tmpPengCardVec;
	Vector<ShowCard* > m_tmpKaiDuoCardVec;
	Vector<ShowCard* > m_tmpSaoChuanCardVec;
	Vector<ShowCard* > m_tmpSaoCardVec;
	Vector<ShowCard* > m_tmpChiCardVec;

	Vector<ShowCard* > m_ThreeCardVec;		//存三张牌
	Vector<ShowCard* > m_FourCardVec;		//存四张牌
};

