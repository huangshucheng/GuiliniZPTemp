﻿#include "MyCardWall.h"
#include "utils/CommonFunction.h"
#include "utils/Constant.h"
#include "layerUtils/ToastLayer/ToastManger.h"
#include "Tools.h"

MyCardWall::MyCardWall():
m_GameLayer(nullptr),
m_TempMoveCard(nullptr),
count(0),
m_RowNum(0)
{

}

MyCardWall::~MyCardWall()
{

}

MyCardWall* MyCardWall::create(GameLayer* _layer)
{
	MyCardWall* ret = new MyCardWall();
	if (ret && ret->init(_layer))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool MyCardWall::init(GameLayer* _layer)
{
	if (!Layer::init())
	{
		return false;
	}

	m_GameLayer = _layer;
	initData();
	initUI();
	setSortState();
	createCardWall();
	return true;
}

void MyCardWall::onEnter()
{
	Layer::onEnter();

	auto listenerEvent = EventListenerTouchOneByOne::create();
	listenerEvent->onTouchBegan = CC_CALLBACK_2(MyCardWall::onTouchBegan, this);
	listenerEvent->onTouchMoved = CC_CALLBACK_2(MyCardWall::onTouchMoved, this);
	listenerEvent->onTouchEnded = CC_CALLBACK_2(MyCardWall::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerEvent, this);
}

void MyCardWall::initData()
{

}

void MyCardWall::initUI()
{

}

void MyCardWall::removeCardWall()
{
	for (auto it = _cardList.begin(); it != _cardList.end(); ++it)
	{
		if (*it && (*it)->getParent())
		{
			(*it)->removeFromParent();
		}
	}
	_cardList.clear();
}

void MyCardWall::createCardWall()
{
	removeCardWall();

	float x = 200;
	float y = 110;
	int count = 0;

	if (m_GameLayer->t_Player[2].m_MyCard[0].size() > 0)
	{
		for (int i = 0; i < m_GameLayer->t_Player[2].m_MyCard[0].size(); i++)
		{
			ShortCardSprite* _card = ShortCardSprite::create(0, m_GameLayer->t_Player[2].m_MyCard[0][i]);
			addChild(_card);
			if (_card)
			{
				_cardList.pushBack(_card);
			}
		}
	}
	if (m_GameLayer->t_Player[2].m_MyCard[1].size() > 0)
	{
		for (int i = 0; i < m_GameLayer->t_Player[2].m_MyCard[1].size(); i++)
		{
			ShortCardSprite* _card = ShortCardSprite::create(1, m_GameLayer->t_Player[2].m_MyCard[1][i]);
			addChild(_card);
			if (_card)
			{
				_cardList.pushBack(_card);
			}
		}
	}

	if (!_cardList.empty())
	{
		float _width = _cardList.at(0)->getContentSize().width;
		int _leftSize = 21 - _cardList.size();

		for (int i = 0; i < _cardList.size(); ++i)
		{
			_cardList.at(i)->setBoardIndex(i);
			//_cardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2(_width * i + 30 + _leftSize * (45 / 2), 0)));
			_cardList.at(i)->setPosition(Tools::getPosByIndex(i));
			//_cardList.at(i)->setLocalZOrder(Tools::getCellZOrder(i));
		}
	}

	setSortState();
	setCardState();
//	refrishCardPos();	//发牌的时候手牌整理 TODO
	//refrishCardPosByIndex();
}

bool MyCardWall::onTouchBegan(Touch *touch, Event *unused_event)
{
	for (auto iter = _cardList.begin(); iter != _cardList.end(); ++iter)
	{
		ShortCardSprite* _card = static_cast<ShortCardSprite*>(*iter);
		Point locationInNode = _card->convertToNodeSpace(touch->getLocation());

		Size s = _card->getContentSize();
		Rect rect = Rect(-s.width / 2, -s.height / 2, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			m_TempMoveCard = _card;
			m_OldPos = _card->getPosition();
			m_OldZorder = _card->getLocalZOrder();
			m_TempMoveCard->setLocalZOrder(CARD_ZORDER_2);

			if (_card->getsortState() == ShortCardSprite::SortState::FOUR_CARD)
			{
				std::cout << "四张一样" << std::endl;
			}
			else if (_card->getsortState() == ShortCardSprite::SortState::THREE_SAME_CARD)
			{
				std::cout << "三张一样" << std::endl;
			}
			else if (_card->getsortState() == ShortCardSprite::SortState::THREE_DIFF_CARD)
			{
				std::cout << "三张不同" << std::endl;

			}
			else if (_card->getsortState() == ShortCardSprite::SortState::TWO_CARD)
			{
				std::cout << "两张一样" << std::endl;

			}
			else if (_card->getsortState() == ShortCardSprite::SortState::ONE_CARD)
			{
				std::cout << "单张" << std::endl;

			}

			/*if (_card->getState() == ShortCardSprite::CardState::ONTouch)
			{
				return true;
			}
			else
			{
				return false;
			}*/
			log("Index=%d", m_TempMoveCard->getBoardIndex());
			return true;
		}
	}
	return false;
}

void MyCardWall::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (m_TempMoveCard)
	{
		m_TempMoveCard->setPosition(touch->getLocation());
	}
}

void MyCardWall::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (!m_TempMoveCard)
	{
		return;
	}
	
	if (Tools::getXValueByPos(touch->getLocation()) != Tools::getXValueByIndex(m_TempMoveCard->getBoardIndex()) 
		&& Tools::getXValueByPos(touch->getLocation()) <10 
		&& Tools::getXValueByPos(touch->getLocation()) >= 0
		)
	{
		log("move...");
		int moveOutX = Tools::getXValueByIndex(m_TempMoveCard->getBoardIndex());
		//获取新的index
		cocos2d::Vector<ShortCardSprite*> cells;
		int _count = 0;
		bool isMAX_Y = false;
		for (auto var : _cardList)
		{
			if (Tools::getXValueByIndex(var->getBoardIndex()) == Tools::getXValueByPos(touch->getLocation()))
				_count++;
		}
		if (_count >= MAX_Y)
		{
			isMAX_Y = true;
		}
		else
		{
			for (auto var : _cardList)
			{
				if (Tools::getXValueByIndex(var->getBoardIndex()) == Tools::getXValueByPos(touch->getLocation()))
				{
					cells.pushBack(var);
				}
			}
		}

		if (!isMAX_Y)
		{
			cells.pushBack(m_TempMoveCard);
			//TODO 重新排列cells的大小
			for (int i = 0; i<cells.size(); i++)
			{
				cells.at(i)->setBoardIndex(10 * i + Tools::getXValueByPos(touch->getLocation()));
				cells.at(i)->setPosition(Tools::getPosByIndex(cells.at(i)->getBoardIndex()));
				cells.at(i)->setLocalZOrder(CARD_ZORDER_1);
			}
			//离开的那列重新整理
			cocos2d::Vector<ShortCardSprite*> cells2;
			for (auto var2 : _cardList)
			{
				if (Tools::getXValueByIndex(var2->getBoardIndex()) == moveOutX)
				{
					cells2.pushBack(var2);
				}
			}
			for (int j = 0; j<cells2.size(); j++)
			{
				cells2.at(j)->setBoardIndex(10 * j + moveOutX);
				cells2.at(j)->setPosition(Tools::getPosByIndex(cells2.at(j)->getBoardIndex()));
			}
			//若后面是空的，把后面的往前移动
			if (cells2.empty())
			{
				cocos2d::Vector<ShortCardSprite*> cells3;
				for (auto var2 : _cardList)
				{
					if (Tools::getXValueByIndex(var2->getBoardIndex()) > moveOutX)
					{
						cells3.pushBack(var2);
					}
				}
				for (int j = 0; j<cells3.size(); j++)
				{
					cells3.at(j)->setBoardIndex(cells3.at(j)->getBoardIndex() - 1);
					cells3.at(j)->setPosition(Tools::getPosByIndex(cells3.at(j)->getBoardIndex()));
				}
			}
			//若前面是空的，把当前往左边移动
			/*cocos2d::Vector<ShortCardSprite*> cells4;
			for (auto var2 : _cardList)
			{
				if (Tools::getXValueByIndex(var2->getBoardIndex()) == moveOutX)
				{
					
				}
			}*/
			std::vector<int> indexList;
			for (auto var2 : _cardList)
			{
				int _index = var2->getBoardIndex() % 10;
				//log("index=%d", _index);
				indexList.push_back(_index);
			}
			//std::cout << "列数为："<<*std::max_element(indexList.begin(), indexList.end()) + 1 << std::endl;
			m_RowNum = *std::max_element(indexList.begin(), indexList.end()) + 1;
		}
		else
		{
			m_TempMoveCard->setPosition(Tools::getPosByIndex(m_TempMoveCard->getBoardIndex()));
			m_TempMoveCard->setLocalZOrder(m_OldZorder);
			//m_TempMoveCard->setPosition(m_OldPos);
		}
	}
	else
	{
		m_TempMoveCard->setPosition(Tools::getPosByIndex(m_TempMoveCard->getBoardIndex()));
		m_TempMoveCard->setLocalZOrder(m_OldZorder);
		//m_TempMoveCard->setPosition(m_OldPos);
		
	}
	/*
	if (touch->getLocation().y > VISIBLESIZE.height / 2 + 150)
	{
		int _type = m_TempMoveCard->getCardData()->m_Type;
		int _value = m_TempMoveCard->getCardData()->m_Value;

		if (_type == 0)
		{
			cout << "打了张：小" << _value << endl;
			//ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"我打了张小") + Value(_value).asString());
		}
		else
		{
			cout << "打了张：大" << _value << endl;
			//ToastManger::getInstance()->createToast(CommonFunction::WStrToUTF8(L"我打了张大") + Value(_value).asString());
		}

		m_GameLayer->t_Player[2].delACard(_type, _value);

		createCardWall();
	}
	else
	{
		//m_TempMoveCard->setLocalZOrder(CARD_ZORDER_1);
		m_TempMoveCard->setLocalZOrder(m_OldZorder);
		auto moveTo = MoveTo::create(0.2, m_OldPos);
		m_TempMoveCard->runAction(moveTo);
	}
	*/
	if (m_TempMoveCard)
	{
		m_TempMoveCard = nullptr;
	}
}

void MyCardWall::setCardState()
{
	vector<ShortCardSprite*> _tempList;
	vector<ShortCardSprite*> _temp_0;

	for (auto& _card : _cardList)
	{
		_tempList.push_back(_card);
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 0 && _type == 0)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count >= 3)
					{
						_temp_0.push_back(_card);
					}
				}

			}
		}
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 1 && _type == 1)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count >= 3)
					{
						_temp_0.push_back(_card);
					}
				}

			}
		}
	}

	auto Iter = std::unique(std::begin(_temp_0), std::end(_temp_0));
	_temp_0.erase(Iter, _temp_0.end());

	for (auto &_tcard : _temp_0)
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					_card->setState(ShortCardSprite::CardState::OFFTouch);
				}
			}
		}
	}

	_tempList.clear();
	_temp_0.clear();
}

void MyCardWall::setSortState()
{
	check_Four_Card();
	check_Three_Same_Card();
	check_Two_card();

	check_Three_Diff_Card();
	check_One_card();
}

void MyCardWall::refrishCardPosByIndex()
{
	if (_cardList.empty())
	{
		return;
	}
	if (!_two_CardList[0].empty())
	{
		for (int i = 0; i < _two_CardList[0].size(); i++)
		{
			_two_CardList[0].at(i)->setBoardIndex(10 * i);
			_two_CardList[0].at(i)->setPosition(Tools::getPosByIndex(_two_CardList[0].at(i)->getBoardIndex()));
		}
	}

	if (!_two_CardList[1].empty())
	{
		for (int i = 0; i < _two_CardList[1].size(); i++)
		{
			_two_CardList[1].at(i)->setBoardIndex(10 * i );
			_two_CardList[1].at(i)->setPosition(Tools::getPosByIndex(_two_CardList[1].at(i)->getBoardIndex()));
		}
	}
}

void MyCardWall::refrishCardPos()
{
	int count1 = 0;
	if (_cardList.empty())
	{
		return;
	}

	int _width = _cardList.at(0)->getContentSize().width;
	auto _height = _cardList.at(0)->getContentSize().height;
	int _leftSize = 21 - _cardList.size();

	//单张
	if (!_one_CardList[0].empty())
	{
		for (int i = 0; i < _one_CardList[0].size(); i++)
		{
			if (i%1 == 0)
			{
				count1++;
			}
			log("cout1:%d", count1);
			_one_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2(_width * i + 30 + _leftSize * (_width / 2), 0))); ///*+ _leftSize * (_width / 2)*/
			//_cardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2(_width * i + 30 + _leftSize * (45 / 2), 0)));
			log("1_width * count1:%d", _width * count1);
		}
		//count--;
	}
	//对子
	if (!_two_CardList[0].empty())
	{
		for (int i = 0; i < _two_CardList[0].size(); i++)
		{
			if (!_one_CardList[0].empty())
			{
				//有单张牌的  放在单张牌后面
				_two_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2((i / 2)*(_width - 28) + _one_CardList[0].back()->getPositionX() + _width /2 - 1, i % 2 * (_height))));
				
			}
			else
			{
				//没有单张牌的 放在离左边180的地方(NO)
				_two_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2((i / 2)*(_width - 28) + 30, i % 2 * (_height))));
			}
			
			if ((i%2) == 0)
			{
				count1++;
			}
			//log("cout2:%d", count1);
			//log("i%2:%d", i % 2);
			//_two_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2( (_width * count1)+30, i % 2 * (_height))));
			//log("2_width * count1:%d", _width * count1);
			
		}
		//
	}
	//三张 有对子的 放对子后面，没有对子的放单张后面，没有单张的，放离左边180的地方

	/*for (int i = 0; i < 6;i++)
	{
		auto card = ShortCardSprite::create(0,3);
		addChild(card);
		_three_Same_CardList[0].pushBack(card);
	}*/

	if (!_three_Same_CardList[0].empty())
	{
		for (int i = 0; i < _three_Same_CardList[0].size(); i++)
		{
			//if (_two_CardList[0].size()>0)
			//{
			//	//verCard.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2((i / 4)*(_height - 8) + (35 * count) + 100, i % 4 * (_height - 70) + 177)));

			//	//有对子的 放对子后面
			//	_three_Same_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2((i / 3)*(_width) + _two_CardList[0].back()->getPosition().x - _width, i % 3 * (_height))));
			//}
			//else if (_one_CardList[0].size()>0)
			//{
			//	//有单张牌的  放在单张牌后面
			//	_three_Same_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2((i / 3)*(_width)+_one_CardList[0].back()->getPosition().x - _width, i % 3 * (_height))));
			//}
			//else
			//{
			//	//没有单张牌的 放在离左边30的地方
			//	_three_Same_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2((i / 3)*(_width) + 30, i % 3 * (_height))));
			//}
			if ((i%3) == 0)
			{
				count1++;
			}
			log("cout3:%d", count1);
			log("i%3:%d", i % 3);
			/*_three_Same_CardList[0].at(i)->setAnchorPoint(Vec2(0,0));*/
			_three_Same_CardList[0].at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2(_width/2  * count1 + _width + _leftSize * (_width / 4), i % 3 * (_height))));
			log("3_width * count1:%d", _width * count1);
		}
	}

	/*
	if (!_cardList.empty())
	{
		for (int i = 0; i < _cardList.size(); ++i)
		{
			//_cardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2(45 * i + 180 + _leftSize * (45 / 2), 0)));
			if (_cardList.at(i)->getsortState() == CardSprite::SortState::FOUR_CARD)
			{
				//_card->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, Vec2((i / 4)*(_height - 8) + m_ThreeCardVec.back()->getPosition().x + _height, i % 4 * (_height - 70) - 30)));
				//_cardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2((i / 4)*(_height - 8) + 180 + _leftSize * (45 / 2), i % 4 * (_height - 70) - 30)));
				//_cardList.at(i)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2(45 * i + 180 + _leftSize * (45 / 2), 0)));
			}
			if (_cardList.at(i)->getsortState() == CardSprite::SortState::THREE_SAME_CARD)
			{
				if (_cardList.at(i)->getCardData()->m_Type == 0)
				{
					if (_cardList.at(i + 1)->getsortState() == CardSprite::SortState::THREE_SAME_CARD)
					{
						//_cardList.at(i+1)->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, 0, Vec2((i / 3)*(_height - 8) + 180 + _leftSize * (45 / 2), i % 3 * (_height - 70) - 30)));
						_cardList.at(i + 1)->setPosition(Vec2(_cardList.at(i)->getPositionX() + (i / 3)*(_height - 8), _cardList.at(i)->getPositionY() + i % 3 + _height / 2));
					}
				}

			}
			if (_cardList.at(i)->getsortState() == CardSprite::SortState::TWO_CARD)
			{

			}
		}
	}
	*/
}

bool MyCardWall::check_Four_Card()
{
	vector<ShortCardSprite*> _tempList;
	vector<ShortCardSprite*> _tempVec[2];

	_four_CardList[0].clear();
	_four_CardList[1].clear();

	for (auto& _card : _cardList)
	{
		_tempList.push_back(_card);
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 0 && _type == 0)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count == 4)
					{
						_tempVec[0].push_back(_card);
					}
				}

			}
		}
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 1 && _type == 1)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count == 4)
					{
						_tempVec[1].push_back(_card);
					}
				}

			}
		}
	}

	for (auto &_tcard : _tempVec[0])
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					_card->setsortState(ShortCardSprite::SortState::FOUR_CARD);
					_four_CardList[0].pushBack(_card);
				}
			}
		}
	}

	for (auto &_tcard : _tempVec[1])
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					_card->setsortState(ShortCardSprite::SortState::FOUR_CARD);
					_four_CardList[1].pushBack(_card);
				}
			}
		}
	}
	_tempList.clear();
	_tempVec[0].clear();
	_tempVec[1].clear();
	return true;
}

bool MyCardWall::check_Three_Same_Card()
{
	vector<ShortCardSprite*> _tempList;
	vector<ShortCardSprite*> _tempVec[2];

	_three_Same_CardList[0].clear();
	_three_Same_CardList[1].clear();

	for (auto& _card : _cardList)
	{
		_tempList.push_back(_card);
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 0 && _type == 0)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count == 3)
					{
						_tempVec[0].push_back(_card);
					}
				}

			}
		}
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 1 && _type == 1)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count == 3)
					{
						_tempVec[1].push_back(_card);
					}
				}

			}
		}
	}

	for (auto &_tcard : _tempVec[0])
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					if (_card->getsortState() == ShortCardSprite::SortState::TWO_CARD ||
						_card->getsortState() == ShortCardSprite::SortState::FOUR_CARD)
					{
						continue;
					}
					_card->setsortState(ShortCardSprite::SortState::THREE_SAME_CARD);
						_three_Same_CardList[0].pushBack(_card);
				}
			}
		}
	}

	for (auto &_tcard : _tempVec[1])
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					if (_card->getsortState() == ShortCardSprite::SortState::TWO_CARD ||
						_card->getsortState() == ShortCardSprite::SortState::FOUR_CARD)
					{
						continue;
					}
					_card->setsortState(ShortCardSprite::SortState::THREE_SAME_CARD);
					_three_Same_CardList[1].pushBack(_card);
				}
			}
		}
	}
	_tempList.clear();
	_tempVec[0].clear();
	_tempVec[1].clear();
	return true;
}

bool MyCardWall::check_Three_Diff_Card()
{

	return false;
}

bool MyCardWall::check_Two_card()
{
	vector<ShortCardSprite*> _tempList;
	vector<ShortCardSprite*> _tempVec[2];

	_two_CardList[0].clear();
	_two_CardList[1].clear();

	for (auto& _card : _cardList)
	{
		_tempList.push_back(_card);
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 0 && _type == 0)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count == 2)
					{
						_tempVec[0].push_back(_card);
					}
				}

			}
		}
	}

	for (auto& _tcard : _tempList)
	{
		int _ttype = _tcard->getCardData()->m_Type;
		int _tvalue = _tcard->getCardData()->m_Value;
		int _count = 0;

		for (auto &_card : _cardList)
		{
			int _type = _card->getCardData()->m_Type;
			int _value = _card->getCardData()->m_Value;

			if (_ttype == 1 && _type == 1)
			{
				if (_tvalue == _value)
				{
					_count++;
					if (_count == 2)
					{
						_tempVec[1].push_back(_card);
					}
				}

			}
		}
	}

	for (auto &_tcard : _tempVec[0])
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					if (_card->getsortState() == ShortCardSprite::SortState::FOUR_CARD ||
						_card->getsortState() == ShortCardSprite::SortState::THREE_SAME_CARD)
					{
						continue;
					}
					_card->setsortState(ShortCardSprite::SortState::TWO_CARD);
					_two_CardList[0].pushBack(_card);
				}
			}
		}
	}

	for (auto &_tcard : _tempVec[1])
	{
		for (auto& _card : _cardList)
		{
			if (_tcard->getCardData()->m_Type == _card->getCardData()->m_Type)
			{
				if (_tcard->getCardData()->m_Value == _card->getCardData()->m_Value)
				{
					if (_card->getsortState() == ShortCardSprite::SortState::FOUR_CARD ||
						_card->getsortState() == ShortCardSprite::SortState::THREE_SAME_CARD)
					{
						continue;
					}
					_card->setsortState(ShortCardSprite::SortState::TWO_CARD);
					_two_CardList[1].pushBack(_card);
				}
			}
		}
	}
	_tempList.clear();
	_tempVec[0].clear();
	_tempVec[1].clear();
	return false;
}

bool MyCardWall::check_One_card()
{
	_one_CardList[0].clear();
	_one_CardList[1].clear();

	for (auto &_card:_cardList)
	{
		if (_card->getCardData()->m_Type == 0)
		{
			if (_card->getsortState() == ShortCardSprite::SortState::ONE_CARD)
			{
				_one_CardList[0].pushBack(_card);
			}
		}

		if (_card->getCardData()->m_Type == 1)
		{
			if (_card->getsortState() == ShortCardSprite::SortState::ONE_CARD)
			{
				_one_CardList[1].pushBack(_card);
			}
		}
	}
	return false;
}