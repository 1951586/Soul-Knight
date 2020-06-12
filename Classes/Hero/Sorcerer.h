#ifndef __SORCERER_H__
#define __SORCERER_H__
#include "Hero.h"
USING_NS_CC;

class Sorcerer :public Hero
{
public:
	CREATE_FUNC(Sorcerer);

	static Sorcerer* createHeroSprite(Point position, Scene* scene);

	virtual void heroInit(Point position, Scene* scene);
	//status = 1:向右站立；2：向右跑动；-1：向左站立；-2：向左跑动
	virtual Animate* createAnimate(int status);
	//技能
	virtual void ability();
};

#endif