﻿#pragma once

#include "cocos2d.h"
#include "GameLayer.h"
#include "Card/CardSprite.h"

using namespace cocos2d;
using namespace ui;

/*
我手里的牌
*/

class GameLayer;

class MyCardWall :public Layer
{
public:
	MyCardWall();
	~MyCardWall();

	virtual bool init(GameLayer* _layer);
	static MyCardWall* create(GameLayer* _layer);

	void initData();
	void initUI();
	virtual void onEnter();

	void removeCardWall();
	void createCardWall();
	void setCardState();

public:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

private:
	void setSortState();

	bool check_Four_Card();
	bool check_Three_Same_Card();
	bool check_Three_Diff_Card();
	bool check_Two_card();
	bool check_One_card();
	void refrishCardPos();							//刷新位置

	Vector<CardSprite* > _four_CardList[2];			//四张
	Vector<CardSprite* > _three_Same_CardList[2];	//三张相同
	Vector<CardSprite* > _three_Diff_CardList[2];	//三张不同
	Vector<CardSprite* > _two_CardList[2];			//两张
	Vector<CardSprite* > _one_CardList[2];			//一张

private:
	GameLayer*	m_GameLayer;
	Vector<CardSprite* > _cardList;		//牌数组(总)
	CardSprite* m_TempMoveCard;
	Vec2 m_OldPos;
	int m_OldZorder;
};