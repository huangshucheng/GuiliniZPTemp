#include "ShowOneLayer.h"
#include "GameLayer.h"

ShowOneLayer::ShowOneLayer()
{

}

ShowOneLayer::~ShowOneLayer()
{

}

ShowOneLayer* ShowOneLayer::create(GameLayer* _layer)
{
	ShowOneLayer* ret = new ShowOneLayer();
	if (ret && ret->init(_layer))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool ShowOneLayer::init(GameLayer* _layer)
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

void ShowOneLayer::initData()
{

}

void ShowOneLayer::initUI()
{

}
