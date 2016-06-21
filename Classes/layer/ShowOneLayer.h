#pragma once

#include "cocos2d.h"

using namespace cocos2d;
/*
显示下家的牌
*/
class GameLayer;

class ShowOneLayer : public Layer
{
public:

	ShowOneLayer();
	~ShowOneLayer();

	virtual bool init(GameLayer* _layer);
	static ShowOneLayer* create(GameLayer* _layer);

	void initData();
	void initUI();
	GameLayer*	m_GameLayer;
};