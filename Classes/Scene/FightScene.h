#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

#include "cocos2d.h"
#include <vector>//感觉用这容器还是好点
#include <list>


USING_NS_CC;
class Hero;
class Monster;
class Bullet;
class weapon;

//战斗区域数据结构
struct Area
{
	//区域矩形
	Rect rect;
	//是否来过
	bool haveCome;
	//四周，-1代表没有
	int around[4];//左、上、右、下
	//区域是否有小怪
	bool isFightArea;
	//第几场有小怪的区域(与allMonster有关)
	int monsterCount;
	//区域已出现小怪波数
	int monsterNum;
	//区域小怪波数的最大值
	int monsterNumMax;
	//小地图坐标
	Point posLittleMap;
};
//宝箱、药水的数据结构
struct specialSprite
{
	Sprite* sprite;
	int type;//0:宝箱；1:生命药水；2:能量药水；3:武器；4:金币;5：旋涡
	std::string name;
};
//场景基类
class FightScene :	public cocos2d::Scene
{
public:
	//数据：
	//英雄
	Hero *hero;  
	//运动图层
	Layer* dynamicLayer;
	//静止图层
	Layer* staticLayer;
	//地图高度
	int height;
	//障碍物信息
	Point gid;
	//小地图
	Sprite* littleMap;
	//怪物容器(其中的一波)
	std::list<Monster*> monster; 
	//英雄子弹容器
	std::list<Bullet*> heroBullet;
	//小怪子弹容器
	std::list<Bullet*> monsterBullet;
	//战斗区块容器
	std::vector<Area> area;
	//特殊精灵容器
	std::list<specialSprite*> special;
	//战斗场景的宝箱位置容器
	std::vector<Point> posBox;
	//运动图层与tmx的初始差距
	Point pos;
	//哪一场
	int num;


	//函数：
	//初始化Scene基类数据
	bool init();
	//添加图层
	void addLayer();
	//添加英雄（传递时用）
	void addHero(Hero* _hero);
	//接触检验（子弹与英雄）
	void heroAndBullet();
	//接触检验（子弹与小怪）
	void monsterAndBullet();
	//接触检验（英雄与小怪）
	void heroAndMonster();
	//接触检验（子弹与障碍物）
	void bulletAndMeta();
	//接触检验(武器与小怪)
	void weaponAndMonster();
	//接触检验(武器与子弹)
	void weaponAndBullet();
	//接触检验(英雄与特殊物质)
	void heroAndSpecial();
	//检验相对于运动场景的点是否在障碍上
	bool isCollision(int _x,int _y);
	//切换小怪
	virtual void changeMonster(int count, int num) = 0;
	//从容器中移除死去的小怪
	void removeMonster();
	//增加宝箱
	void addBox(Vec2 vec);
	//添加旋涡
	void addLoop(Vec2 vec);
	//检验并更新场景容器的定时器
	void update(float dt);
	//更新区块数据的定时器
	void updateArea(float dt);
protected:
	//设置缩放
	float scale;
	//添加其中一波怪物
	void addMonster(std::list<Monster*> _monster);
	
	friend class Hero;
	friend class Monster;
};
#endif
