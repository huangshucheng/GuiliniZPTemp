
#include "AccountsLayer.h"
#include "ui/UIButton.h"
#include <iostream>
#include "utils/CommonFunction.h"
#include "layer/GameScene.h"
#include "layer/WelcomeScene.h"

using namespace std;
using namespace ui;

#define WIN_DATA Win::getInstance()

AccountsLayer::AccountsLayer()
{

}

AccountsLayer::~AccountsLayer()
{
	
}

Scene* AccountsLayer::createScene()
{
	auto s = Scene::create();
	auto l = AccountsLayer::create();
	s->addChild(l);
	return s;
}

void AccountsLayer::onEnter()
{
	Pop::onEnter();
}

void AccountsLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				close();
			}
		}
	}
}

bool AccountsLayer::init()
{
	if (!Pop::init())
	{
		return false;
	}

	addUI();
	return true;
}

void AccountsLayer::addUI()
{

	auto bg_sp = Sprite::create("zhuomian.png");
	if (bg_sp)
	{
		m_popNode->addChild(bg_sp);
	}

	auto gray_sp = Sprite::create("account/ima_gray.png");
	if (gray_sp && bg_sp)
	{
		bg_sp->addChild(gray_sp);
		gray_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg_sp, Vec2(0, 45)));
	}
	//返回
	auto backBtn = Button::create("account/btn_back.png");
	if (backBtn)
	{
		bg_sp->addChild(backBtn);
		backBtn->addClickEventListener(CC_CALLBACK_1(AccountsLayer::quiteCallback, this));
		float _width = backBtn->getContentSize().width ;
		float _heignt = backBtn->getContentSize().height ;

		backBtn->setPosition(CommonFunction::getVisibleAchor(1, 1, bg_sp, Vec2(-_width, -_heignt)));
	}
	//重新开始
	auto restartBtn = Button::create("account/btn_again.png");
	if (restartBtn)
	{
		restartBtn->addClickEventListener(CC_CALLBACK_1(AccountsLayer::restartCallback, this));
		auto _he = restartBtn->getContentSize().height;
		restartBtn->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg_sp, Vec2(0, _he)));
		bg_sp->addChild(restartBtn);
	}
}

void AccountsLayer::quiteCallback(Ref* sender)
{
	if (this->getParent())
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, WelcomeScene::createScene()));
		//this->removeFromParent();
	}
}

void AccountsLayer::restartCallback(Ref* sender)
{
	if (this->getParent())
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameScene::createScene()));
		//this->removeFromParent();
	}
}