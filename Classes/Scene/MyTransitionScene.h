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
	//ui::Button *button1;  //��Ѫ��
	//ui::Button *button2;  //������
	//ui::Button *button3;  //�ӹ���
	//ui::Button *button4;  //����������
	//ui::Button *button5;  //�ƶ��ٶ�
	//ui::Button *button6;  //�ӻ���
	Point p[3];	
	virtual void changeMonster(int count, int num){}
	

	CREATE_FUNC(MyTransitionScene);
};
#endif