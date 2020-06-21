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

	virtual void update(float delta);  //�޸Ĵ˺�����ʵ���ƶ�

	virtual void changeMonster(int count, int num) {}

	void startGame();  //���봫���ſ�ʼ��Ϸ
private:
	Sprite* Transfer;
	void creatHero();
	bool hadChosen;  //�ж��Ƿ�ѡ�����ɫ
	//Monster* snowman;
	CREATE_FUNC(HomeScene);
};
#endif //__HOME_SCENE_H__