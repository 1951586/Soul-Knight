#ifndef __KNIGHT1_H__
#define __KNIGHT1_H__
#include "Hero.h"
USING_NS_CC;

class Knight :public Hero
{
public:
	CREATE_FUNC(Knight);

	static Knight* createHeroSprite(Point position, Scene* scene);

	virtual void heroInit(Point position, Scene* scene);
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status);
	//����
	virtual void ability();
};

#endif