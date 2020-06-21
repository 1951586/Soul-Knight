#ifndef __SNOWMAN_H__
#define __SNOWMAN_H__
#include "cocos2d.h"
#include "Monster/Monster.h"
USING_NS_CC;

class Snowman :
	public Monster
{
public:
	static Snowman* createMonsterSprite(Point position, FightScene* scene);
	virtual void monsterInit(Point position, FightScene* scene);
	virtual void setAnimation(int status);
	virtual Animate* createAnimate(int status,int numFrame);
	virtual void attack(float dt);
	virtual void killed();
	void attackPart2();
	//攻击时产生的冲击波
	void shockWave();
	Sprite* wave;
	void removeWave();
};
#endif // !__SNOWMAN_H__
