#ifndef __FIGHTSCENE2_H__
#define __FIGHTSCENE2_H__

#include "Scene/FightScene.h"

class FightScene2 :public FightScene
{
public:
	//创建函数
	static FightScene* createFightScene(Hero* _hero);
	//初始化函数
	virtual bool initScene(Hero* _hero);

	//切换小怪
	virtual void changeMonster(int count, int num);
	//怪物容器（储存当前场景所有怪物）
	std::list<Monster*> allMonster[2][2];//代表这个场景有两个区域需要怪物，每个区域两波
private:
	//初始化该场景的怪物信息
	void initMonsterInformation();
	//初始化区块信息
	void initAreaInformation();
	//初始化宝箱位置信息
	void initBoxPosition();

	CREATE_FUNC(FightScene2);
};
#endif
