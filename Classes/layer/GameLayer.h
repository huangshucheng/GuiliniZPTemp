﻿#pragma once

#include "cocos2d.h"
#include "ZiPai.h"
#include "ZiPaiManage.h"
#include "ui/UIButton.h"
#include "Card/CardSprite.h"

using namespace cocos2d;
using namespace ui;

/*
	0 下家
	1 上家
	2 自己
*/

class GameLayer :public Layer
{
public:

	enum GameState
	{
		NPCTurn_0,	//我下家
		NPCTurn_1,	//我上家
		MyTurn,
		OFF
	};

	GameLayer();
	~GameLayer();

	virtual bool init();
	CREATE_FUNC(GameLayer);
	virtual void onEnter();

public:

	void startCallBack(Ref* ref);
	void overCallBack(Ref* ref);
public:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

private:

	void initData();
	void initUI();

	void getANewCard();						//起一张新牌

	void xipai();

	void createMyCardWall();				//创建我的牌墙
	void removeMyCardWall();				//移除牌墙
	void setCardState();

	void logAllCard();

	virtual void update(float dt);
	void playNPC_0();						//下家
	void playNPC_1();						//上家

	void playCard();						//打牌流程
private:
	//我自己检测
	void checkPeng();
	void doPengACard();
	void chooseLayerClose();
	
	void checkKaiduo();
	void checkChongDuo();

public:
	CardData	PopPai[3];					//玩家出的牌
	CardData    m_newCard;					//玩家起的牌

	ZiPai		t_Player[3];				//三个玩家(2是自己)

private:
	bool m_isStartGame;
	ZiPaiManage t_ZPManage;					//管理器

	DrawNode*	m_line;						//创建线
	CardSprite* m_TempMoveCard;		

	Vec2 m_OldPos;							//原来坐标

	Vector<CardSprite* > m_CardList;		//牌数组

	GameState m_GameState;
};

