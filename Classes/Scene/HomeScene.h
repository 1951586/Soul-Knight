#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "Scene/FightScene.h"
#include "Scene/LoadingScene.h"

class  HomeScene : public FightScene
{
public:
    static FightScene* createScene();

    virtual bool init();

    void callBack1(cocos2d::Ref* pSender);

    //void callBack2(cocos2d::Ref* pSender,cocos2d::Sprite* sprite);

	virtual void update(float delta);  //修改此函数来实现移动

	virtual void changeMonster(int count, int num) {}

	void startGame();  //进入传送门开始游戏
private:
	Sprite* Transfer;
	void creatHero();
	bool hadChosen;  //判断是否选择过角色
	//Monster* snowman;
	CREATE_FUNC(HomeScene);
};
#endif //__HOME_SCENE_H__