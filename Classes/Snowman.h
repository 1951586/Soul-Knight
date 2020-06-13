#ifndef __SNOWMAN_H__
#define __SNOWMAN_H__
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;
class Snowman :
	public Monster
{
public:
	static Snowman* createMonsterSprite(Point position, HomeScene* scene);
	virtual void monsterInit(Point position, HomeScene* scene);
	virtual Animate* createAnimate(int status,int numFrame);
};
#endif // !__SNOWMAN_H__
