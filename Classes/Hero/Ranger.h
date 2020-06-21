#ifndef __RANGER_H__
#define __RANGER_H__
#include "Hero/Hero.h"

class Ranger :public Hero
{
public:
	CREATE_FUNC(Ranger);

	static Ranger* createHeroSprite(Point position, FightScene* scene);

	virtual void heroInit(Point position, FightScene* scene);
	//status = 1:向右站立；2：向右跑动；-1：向左站立；-2：向左跑动
	virtual Animate* createAnimate(int status);
	//技能
	virtual void ability();
	//技能攻击效果
	virtual void removeAbilityFunction();
private:
	//翻滚距离
	int rotateDistance;
	//创建旋转动作
	Animate* createRotateAnimate();
	//技能运动方向
	MoveBy* abilityMoveBy();
	//动作监听
	void afterAbility();

};

#endif
