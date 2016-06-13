﻿#include "ShowLayer.h"
#include "utils/CommonFunction.h"
#include "utils/Constant.h"
#include "GameLayer.h"

ShowLayer::ShowLayer():
m_GameLayer(nullptr),
m_ACard(nullptr),
m_NewCard(nullptr)
{
	//显示卡牌
	auto _listener_1 = EventListenerCustom::create(CREATE_CARD, [=](EventCustom*event){
		createACard();
	});
	
	//新的牌
	auto _listener_2 = EventListenerCustom::create(NEW_CARD, [=](EventCustom*event){
		createANewCard();
	});

	//碰的牌
	auto _listener_3 = EventListenerCustom::create(SHOW_PENGCARD, [=](EventCustom*event){
		showPengCard();
	});

	//开舵的牌
	auto _listener_4 = EventListenerCustom::create(SHOW_KAIDUOCARD, [=](EventCustom*event){
		showKaiduoCard();
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_listener_1, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_2, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_3, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_4, 1);
}

ShowLayer::~ShowLayer()
{
	_eventDispatcher->removeCustomEventListeners(CREATE_CARD);
	_eventDispatcher->removeCustomEventListeners(NEW_CARD);
	_eventDispatcher->removeCustomEventListeners(SHOW_PENGCARD);
	_eventDispatcher->removeCustomEventListeners(SHOW_KAIDUOCARD);
}

ShowLayer* ShowLayer::create(GameLayer* _layer)
{
	ShowLayer* ret = new ShowLayer();
	if (ret && ret->init(_layer))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool ShowLayer::init(GameLayer* _layer)
{
	if (!Layer::init())
	{
		return false;
	}
	m_GameLayer = _layer;

	initData();

	initUI();

	return true;
}

void ShowLayer::initData()
{
	auto layercolor = LayerColor::create(Color4B::BLACK);
	layercolor->setOpacity(100);
	addChild(layercolor);

}

void ShowLayer::initUI()
{
	/*float x = 200;
	float y = 210;
	int count = 0;

	for (int i = 1; i <= 10;i++)
	{
		auto _card = createSmallCardSprite(0, i);
		addChild(_card);
		if (_card)
		{
			_card->setPosition(Point(x + 50 * count, y));
		}
		count++;
	}*/
}

//创建长的牌card
Sprite* ShowLayer::createBigCardSprite(int p_Type, int p_Value)
{
	Sprite* card =nullptr;
	if (p_Type == 0)
	{
		//小写 
		card = Sprite::create(StringUtils::format("chang_x%0d.png", p_Value));
	}
	if (p_Type == 1)
	{
		//大写 1-10.png	
		card = Sprite::create(StringUtils::format("chang_d%0d.png", p_Value));
	}
	return card;
}

//短牌创建
Sprite* ShowLayer::createSmallCardSprite(int p_Type, int p_Value)
{
	Sprite* card = nullptr;
	if (p_Type == 0)
	{
		card = Sprite::create(StringUtils::format("duanpai_x%0d.png", p_Value));
	}
	if (p_Type == 1)
	{
		card = Sprite::create(StringUtils::format("duanpai_d%0d.png", p_Value));
	}

	if (card)
	{
		card->setScale(0.3f);
		return card;
	}
	return nullptr;
}

void ShowLayer::createACard()
{

	int _type = m_GameLayer->PopPai[2].m_Type;
	int _value = m_GameLayer->PopPai[2].m_Value;

	if (_type < 0 || _type > 1 || _value <= 0 || _value > 10)
	{
		log("_value or _type error0");
		return;
	}

	if (m_ACard)
	{
		if (m_ACard->getParent())
		{
			m_ACard->removeFromParent();
		}
	}

	m_ACard = createBigCardSprite(_type, _value);
	addChild(m_ACard);

	if (m_ACard)
	{
		m_ACard->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 0)));
	}
}

void ShowLayer::createANewCard()
{
	int _type = m_GameLayer->m_newCard.m_Type;
	int _value = m_GameLayer->m_newCard.m_Value;

	if (_type < 0 || _type > 1 || _value <= 0 || _value > 10)
	{
		log("_value or _type error1");
		return;
	}

	if (m_NewCard)
	{
		if (m_NewCard->getParent())
		{
			m_NewCard->removeFromParent();
		}
	}

	m_NewCard = createBigCardSprite(_type, _value);
	addChild(m_NewCard);

	if (!m_NewCard)return;

	float height = m_NewCard->getContentSize().height;
	m_NewCard->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2(0, height / 2)));

	auto moveTo = MoveTo::create(0.3f, CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, height + 25)));
	auto easeAction = EaseBackOut::create(moveTo);
	m_NewCard->runAction(easeAction);
}

void ShowLayer::showPengCard()
{
	if (m_tmpPengCardList.size()>0)
	{
		for (auto &_card : m_tmpPengCardList)
		{
			if (_card->getParent())
			{
				_card->removeFromParent();
			}
		}
		m_tmpPengCardList.clear();
	}

	std::vector<int > _pengList[2];
	_pengList[0] = m_GameLayer->t_Player[2].m_PengCardVec[0];
	_pengList[1] = m_GameLayer->t_Player[2].m_PengCardVec[1];

	if (_pengList[0].size()>0)
	{
		for (int i = 0; i < _pengList[0].size(); i++)
		{
			auto _card = createSmallCardSprite(0, _pengList[0][i]);

			if (_card)
			{
				addChild(_card);
				m_tmpPengCardList.pushBack(_card);
			}
		}
	}

	if (_pengList[1].size()>0)
	{
		for (int i = 0; i < _pengList[1].size(); i++)
		{
			auto _card = createSmallCardSprite(1, _pengList[1][i]);
			
			if (_card)
				addChild(_card);
			{
				m_tmpPengCardList.pushBack(_card);
			}
		}
	}

	if (m_tmpPengCardList.size()>0)
	{
		for (int i = 0; i < m_tmpPengCardList.size(); i++)
		{
			int _height = m_tmpPengCardList.at(i)->getContentSize().height;
			m_tmpPengCardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2((i % 3)*(_height / 4) + 50, i / 3 * (_height - 73) - 120)));
			//_tmpSpriteList.at(i)->setPosition(Vec2((i % 3)*(_height / 4) + 100, (i / 3)*(_height- 73) + 200));
		}
	}
}

void ShowLayer::showKaiduoCard()
{
	showPengCard();
	if (m_tmpKaiDuoCardList.size()>0)
	{
		for (auto &_card : m_tmpKaiDuoCardList)
		{
			if (_card->getParent())
			{
				_card->removeFromParent();
			}
		}
		m_tmpKaiDuoCardList.clear();
	}

	std::vector<int > _kaiduoList[2];
	_kaiduoList[0] = m_GameLayer->t_Player[2].m_KaiDuoCardVec[0];
	_kaiduoList[1] = m_GameLayer->t_Player[2].m_KaiDuoCardVec[1];
	
	if (_kaiduoList[0].size()>0)
	{
		for (int i = 0; i < _kaiduoList[0].size(); i++)
		{
			auto _card = createSmallCardSprite(0, _kaiduoList[0][i]);
			addChild(_card);

			if (_card)
			{
				m_tmpKaiDuoCardList.pushBack(_card);
			}
		}
	}

	if (_kaiduoList[1].size()>0)
	{
		for (int i = 0; i < _kaiduoList[1].size(); i++)
		{
			auto _card = createSmallCardSprite(1, _kaiduoList[1][i]);
			addChild(_card);

			if (_card)
			{
				m_tmpKaiDuoCardList.pushBack(_card);
			}
		}
	}

	if (m_tmpKaiDuoCardList.size()>0)
	{
		for (int i = 0; i < m_tmpKaiDuoCardList.size(); i++)
		{
			int _height = m_tmpKaiDuoCardList.at(i)->getContentSize().height;
			m_tmpKaiDuoCardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2((i % 4)*(_height / 4) + 150, i / 4 * (_height - 73) - 120)));
			//_tmpSpriteList.at(i)->setPosition(Vec2((i % 3)*(_height / 4) + 100, (i / 3)*(_height- 73) + 200));
		}
	}
}
