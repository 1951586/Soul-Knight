#ifndef __MYTRANSITION_SCENE_H__
#define __MYTRANSITION_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"Hero/Hero.h"
#include"Weapon/Weapon.h"
#include"Scene/FightScene1.h"
#include "SimpleAudioEngine.h"
#include"Hero/Knight.h"
#include"Hero/Ranger.h"
#include"Hero/Sorcerer.h"

USING_NS_CC;

class Hero;
class MyTransitionScene: public FightScene
{
public:
	static MyTransitionScene* createMyTransitionScene(Hero* _hero);
	void initMyTransitionScene(Hero* _hero);
private:
	//FightScene* next;
	//Hero* hero;
	//void addHero(Hero *_hero);
	//ui::Button *button1;  //加血量
	//ui::Button *button2;  //加蓝条
	//ui::Button *button3;  //加攻速
	//ui::Button *button4;  //加武器上限
	//ui::Button *button5;  //移动速度
	//ui::Button *button6;  //加护甲
	Point p[3];	
	virtual void changeMonster(int count, int num){}
	

	CREATE_FUNC(MyTransitionScene);
};
#endif