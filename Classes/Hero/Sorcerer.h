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
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status);
	//����
	virtual void ability();
};

#endif