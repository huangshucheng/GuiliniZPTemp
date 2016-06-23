#include "PlayerZeroState.h"
#include "utils/Constant.h"
#include "utils/GetLayer.h"
#include "ChooseLayer.h"
#include "PlayerTwoState.h"
#include "GameLayer.h"
#include "utils/CommonFunction.h"
#include "layerUtils/ToastLayer/ToastManger.h"

USING_NS_CC;

#define GAMELAYER  GetLayer::getInstance()->getgameLayer()

PlayerZeroState::PlayerZeroState()
{
	std::cout << "上家[0]摸牌打牌" << std::endl;
	UserDefault::getInstance()->setIntegerForKey(GAMESTATE, 0);
	auto callfunc = CallFunc::create([this](){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PLAYERBLINK_0);
	});

	auto callfunc_1 = CallFunc::create([this](){
		GAMELAYER->getANewCard();
		myCheck();
	});

	auto delayTime = DelayTime::create(1.5f);
	auto seq = Sequence::create(delayTime, callfunc, delayTime->clone(), callfunc_1, nullptr);
	GAMELAYER->runAction(seq);
}

PlayerZeroState::~PlayerZeroState()
{

}

void PlayerZeroState::Update()
{
	//cocos2d::log("zero Update");
}

void PlayerZeroState::myCheck()
{
	if (GAMELAYER->checkHu())
	{
		std::cout << "=======胡========" << std::endl;

		auto chooseLayer = ChooseLayer::create();
		GAMELAYER->addChild(chooseLayer);
		chooseLayer->setBtnEnable(3);
		chooseLayer->setName(CHOOSELAYER);
	}
	else if (GAMELAYER->checkChongDuo())
	{
		std::cout << "重舵" << std::endl;
	}
	else if (GAMELAYER->checkKaiduo())
	{
		std::cout << "开舵" << std::endl;
	}
	else if (GAMELAYER->checkChi() && GAMELAYER->checkPeng())//可吃可碰
	{
		auto chooseLayer = ChooseLayer::create();
		GAMELAYER->addChild(chooseLayer);
		chooseLayer->setBtnEnable(4);
		chooseLayer->setName(CHOOSELAYER);
	}
	else if (GAMELAYER->checkChi() && !GAMELAYER->checkPeng())	//可吃不能碰
	{
		auto chooseLayer = ChooseLayer::create();
		GAMELAYER->addChild(chooseLayer);
		chooseLayer->setBtnEnable(1);
		chooseLayer->setName(CHOOSELAYER);
	}
	else if (!GAMELAYER->checkChi() && GAMELAYER->checkPeng())//可吃不能碰
	{
		auto chooseLayer = ChooseLayer::create();
		GAMELAYER->addChild(chooseLayer);
		chooseLayer->setBtnEnable(2);
		chooseLayer->setName(CHOOSELAYER);
	}
	else
	{
		GAMELAYER->changeState(new PlayerTwoState());
	}
}

void PlayerZeroState::zeroCheck()
{
	/*
	if (GAMELAYER->t_Player[1].checkChongDuo_kaiDuo(GAMELAYER->PopPai[2].m_Type, GAMELAYER->PopPai[2].m_Value))
	{
		//ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"下家重舵"));
		GAMELAYER->t_Player[1].doChongDuo_kaiDuo(GAMELAYER->PopPai[2].m_Type, GAMELAYER->PopPai[2].m_Value);
	}
	else if (GAMELAYER->t_Player[1].checkKaiduoACard(GAMELAYER->PopPai[2].m_Type, GAMELAYER->PopPai[2].m_Value))
	{
		//ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"下家开舵"));
		GAMELAYER->t_Player[1].doKaiDuo(GAMELAYER->PopPai[2].m_Type, GAMELAYER->PopPai[2].m_Value);
	}
	else if (GAMELAYER->t_Player[1].checkKaiDuo_peng(GAMELAYER->PopPai[2].m_Type, GAMELAYER->PopPai[2].m_Value))
	{
		//ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"下家开舵"));
		GAMELAYER->t_Player[1].doPeng_kaiDuo(GAMELAYER->PopPai[2].m_Type, GAMELAYER->PopPai[2].m_Value);
	}
	*/
	if (GAMELAYER->t_Player[0].checkKaiduoACard(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value))
	{
		ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"上家开舵"));
		GAMELAYER->t_Player[0].doKaiDuo(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value);
	}
	else if (GAMELAYER->t_Player[0].checkKaiDuo_peng(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value))
	{
		ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"上家开舵"));
		GAMELAYER->t_Player[0].doPeng_kaiDuo(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value);
	}
	else if (GAMELAYER->t_Player[0].checkPengACard(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value))
	{
		ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"上家碰"));
		GAMELAYER->t_Player[0].doPengACard(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value);
		GetLayer::getInstance()->getOneLayer()->showPengCard();
	}
	else if (GAMELAYER->t_Player[0].checkChiACard2_7_10(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value))
	{
		ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"上家吃牌"));
		GAMELAYER->t_Player[0].doChi2_7_10(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value, 0);
		GetLayer::getInstance()->getOneLayer()->showChiCard();
	}
	else if (GAMELAYER->t_Player[0].checkChiA_B_C(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value))
	{
		ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"上家吃牌"));
		GAMELAYER->t_Player[0].doChiA_B_C(GAMELAYER->m_newCard.m_Type, GAMELAYER->m_newCard.m_Value, 0);
		GetLayer::getInstance()->getOneLayer()->showChiCard();
	}
}