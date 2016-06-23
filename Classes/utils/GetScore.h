#pragma once
#include "cocos2d.h"
#include <vector>
#include "ZiPaiManage.h"

using namespace std;

class GetScore
{
public:

	static GetScore* getInstance();
	bool purgeInstance();

	int getScore();
	void setScore(int num);

	int getFanXin();
	void setFanXin(int num);

	vector<CardData> showCardList; //要显示的底牌

private:
	GetScore();
	~GetScore();

	static GetScore* _instance;
	int score;

	int fanXin;
};