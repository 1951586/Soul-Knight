#ifndef __MONSSHOOTER_H__
#define __MONSSHOOTER_H__
#include "cocos2d.h"
#include "Monster/Monster.h"
USING_NS_CC;

class MonsWeapon;
class MonsShooter :
	public Monster
{
public:
	static MonsShooter* createMonsterSprite(Point position, FightScene* scene);
	virtual void monsterInit(Point position, FightScene* scene);
	virtual void setAnimation(int status);
	virtual Animate* createAnimate(int status, int numFrame);
	virtual void attack(float dt);
	void attackPart2(float dt);
	virtual void comeOnStage();
	virtual void killed();
	virtual void chase(float dt);
	void adjustRad(float dt);
};
#endif // !__MONSSHOOTER_H__
