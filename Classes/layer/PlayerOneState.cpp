﻿#include "PlayerOneState.h"
#include "utils/Constant.h"
#include "utils/GetLayer.h"
#include "PlayerZeroState.h"
#include "ChooseLayer.h"

USING_NS_CC;
#define GAMELAYER  GetLayer::getInstance()->getgameLayer()

PlayerOneState::PlayerOneState()
{
	std::cout << "下家[1]摸牌打牌" << std::endl;
	//检测，起牌

	auto callfunc = CallFunc::create([this](){
		GAMELAYER->getANewCard();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PLAYERBLINK_1);

		myCheck();
	});
	auto delayTime = DelayTime::create(1.5f);
	auto seq = Sequence::create(delayTime,callfunc, nullptr);
	GAMELAYER->runAction(seq);
}

PlayerOneState::~PlayerOneState()
{

}

void PlayerOneState::Update()
{
	//cocos2d::log("one Update");
}

void PlayerOneState::myCheck()
{
	if (GAMELAYER->checkChongDuo())
	{
		std::cout << "重舵" << std::endl;
	}
	else if (GAMELAYER->checkKaiduo())
	{
		std::cout << "开舵" << std::endl;
	}
	else if (GAMELAYER->checkPeng())
	{
		auto chooseLayer = ChooseLayer::create();
		GAMELAYER->addChild(chooseLayer);
		chooseLayer->setBtnEnable(2);
	}
	else
	{
		GAMELAYER->changeState(new PlayerZeroState());
	}
}