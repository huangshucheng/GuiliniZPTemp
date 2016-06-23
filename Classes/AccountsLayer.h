#pragma once

#include "cocos2d.h"
#include "Win.h"
#include "layerUtils/Pop.h"
#include "layer/ShowCard.h"
#include <vector>

using	namespace std;

using namespace cocos2d;
/*
	结算界面
*/

class AccountsLayer: public Layer
{
public:
	AccountsLayer();
	~AccountsLayer();

	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(AccountsLayer);
	virtual void onEnter();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
public:

	void addUI();

	//显示底牌
	void showDiPai(Node* node);
	//显示赢家胡牌牌型
	void showWinCard(Node* node,int player);

	void showThreeVer(Node* node, vector<int> ver[2], vector<ShowCard*> verCard);
	void showFourVer(Node* node, vector<int> ver[2], vector<ShowCard*> verCard);
	void showThreeCardVer(vector<ShowCard*> verCard, Node* node);
	void showFourCardVer(vector<ShowCard*> verCard, Node* node);
	
public:

	void quiteCallback(Ref* sender);
	void restartCallback(Ref* sender);

private:
	std::vector<Sprite*> SpriteVec;

	static int count;
};

