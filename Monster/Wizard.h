#ifndef __WIZARD_H__
#define __WIZARD_H__
#include "cocos2d.h"
#include "Monster/Monster.h"
USING_NS_CC;

class MonsWeapon;
class Wizard :
	public Monster
{
public:
	static Wizard* createMonsterSprite(Point position, FightScene* scene);
	virtual void monsterInit(Point position, FightScene* scene);
	virtual void setAnimation(int status);
	virtual Animate* createAnimate(int status, int numFrame);
	virtual void attack(float dt);
	void attackPart2(float dt);
	//virtual void comeOnStage();
	virtual void killed();
	virtual void chase(float dt);
};
#endif // !__WIZARD_H__
