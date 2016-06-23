
#include "GetScore.h"

GetScore* GetScore::_instance = nullptr;

GetScore::GetScore():
score(0),
fanXin(0)
{
}


GetScore::~GetScore()
{
	purgeInstance();
}

GetScore* GetScore::getInstance()
{
	if (!_instance)
	{
		_instance = new (std::nothrow) GetScore();
	}
	return _instance;
}

int GetScore:: getScore()
{
	return score;
}

void GetScore::setScore(int num)
{
	score = num;
}

int GetScore::getFanXin()
{
	return fanXin;
}
void GetScore::setFanXin(int num)
{
	fanXin = num;
}

bool GetScore::purgeInstance()
{
	CCASSERT(_instance, "instance can not emptoy");
	if (_instance){
		delete _instance;
		_instance = nullptr;
	}
	return true;
}