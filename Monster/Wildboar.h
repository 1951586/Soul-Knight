#ifndef __WILDBOAR_H__
#define __WILDBOAR_H__
#include "cocos2d.h"
#include "Monster/Monster.h"
USING_NS_CC;

class Wildboar :
	public Monster
{
public:
	static Wildboar* createMonsterSprite(Point position, FightScene* scene);
	virtual void monsterInit(Point position, FightScene* scene);
	virtual void setAnimation(int status);
	virtual Animate* createAnimate(int status, int numFrame);
	virtual void attack(float dt);
	void attackPart2();
	//����ʱ�����ĳ����
	void shockWave();
	Sprite* wave;
	void removeWave();
	//�������������ͬ
	virtual void killed();
};
#endif // !__WILDBOAR_H__
