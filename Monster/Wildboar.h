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
	//攻击时产生的冲击波
	void shockWave();
	Sprite* wave;
	void removeWave();
	//猪的死亡有所不同
	virtual void killed();
};
#endif // !__WILDBOAR_H__
