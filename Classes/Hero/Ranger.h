#ifndef __RANGER_H__
#define __RANGER_H__
#include "Hero.h"
class Ranger :public Hero
{
public:
	CREATE_FUNC(Ranger);

	static Ranger* createHeroSprite(Point position, Scene* scene);

	virtual void heroInit(Point position, Scene* scene);
	//status = 1:向右站立；2：向右跑动；-1：向左站立；-2：向左跑动
	virtual Animate* createAnimate(int status);
	//技能
	virtual void ability();
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
