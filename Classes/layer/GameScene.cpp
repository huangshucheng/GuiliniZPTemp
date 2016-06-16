﻿#include "GameScene.h"
#include "GameLayer.h"
#include "GUILayer.h"
#include "BackLayer.h"
#include "utils/GetLayer.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto gamelayer = GameLayer::create();
	addChild(gamelayer);

	auto guilayer = GUILayer::create();
	addChild(guilayer);

	GetLayer::getInstance()->setgameLayer(gamelayer);

	return true;
}

void GameScene::onEnter()
{
	Layer::onEnter();

	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				addChild(BackLayer::create(), 100);
			}
		}
	}
}