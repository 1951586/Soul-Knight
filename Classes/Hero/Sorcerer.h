#ifndef __SORCERER_H__
#define __SORCERER_H__
#include "Hero/Hero.h"
USING_NS_CC;

class Sorcerer :public Hero
{
public:
	CREATE_FUNC(Sorcerer);

	static Sorcerer* createHeroSprite(Point position, FightScene* scene);

	virtual void heroInit(Point position, FightScene* scene);
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status);
	//����
	virtual void ability();
	//���ܹ���Ч��
	virtual void removeAbilityFunction();
};

#endif