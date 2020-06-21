#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "cocos2d.h"
#include <ctime>
USING_NS_CC;
//前置声明
class Hero;
class FightScene;
class MonsWeapon;

enum monster_Animation   //怪物的各种动画状态
{
	//status:
	RIGHT = 1,//向右（以下同理）
	RIGHT_STAND = 11,
	RIGHT_MOVE = 12,
	RIGHT_ATTACK = 13,
	RIGHT_ATTACK_PART2=16,
	RIGHT_DYING=14,
	RIGHT_DEAD=15,

	LEFT = 2,
	LEFT_STAND = 21,
	LEFT_MOVE = 22,
	LEFT_ATTACK = 23,
	LEFT_ATTACK_PART2 = 26,
	LEFT_DYING=24,
	LEFT_DEAD=25,

	//ActionTag
	MOVE = 100,
	SETANIMATION = 99,
	SHAKE = 98,
	BLINK = 97,
	DYING=96,
	ATTACK=95,
	ATTACK_PART2=951,

};

class Monster 
	:public Sprite
{
protected:
	//生命
	int HP;
	//速度
	float speed;
	//攻击力
	int ATK;
	////位置 （在sprite里自带有，获取位置的用xx.sprite：：getPosition）
	//Point position;
	//面部方向（1表示向右，2表示向左）
	int direction;
	//移动方向（用（x，y）坐标来表示）
	Vec2 moveDirection;
	//是否正在移动
	bool isMove;
	//是否在攻击
	bool isAttck;
	//一次移动的距离
	float moveDistance;
	//攻击时前进的距离
	float attackDis;
	float attackSpeed;
	//精灵实体(精灵负责动画，this负责位移及其他的)
	Sprite* sprite;
	//场景
	FightScene* scene;
	//用指针解决相互包含的问题
	Hero* hero;
	//模拟抖动
	void shake();
	//径直向玩家走去
	void directMove();
	//随机移动
	void randomMove();
	//获得到hero的距离
	float getHeroDistance();
	//被杀
	virtual void killed();
	//攻击间隔
	float attackTime;
	//协助动作进行
	Sprite* actionAsit;


public:

	//状态
	//是否活着，1活着，0死亡
	bool isAlive;
	//是否登场
	bool isOnStage;
 	//是否能被碰撞(死亡后不可碰撞）
	bool isCanTouched;
	//初始化
	virtual bool init();
	virtual void monsterInit(Point position, FightScene* scene) = 0;//（先暂时用HomeScene类的指针）
	//创建动画(参数为动作描述、动作帧数
	virtual Animate* createAnimate(int status, int numFrame) = 0;
	//判断能否移动
	bool isCanMove();
	//设置动画（只负责动画，并不涉及动作）
	virtual void setAnimation(int status)=0;
	//追踪玩家
	virtual void chase(float dt);
	//对monster实例化hero
	void getHeroInstance(Hero* hero);
	//被攻击，hurt为伤害数值（血量为零是自动触发死亡）
	void beAttacked(int hurt);
	//闪烁
	Sprite* blinkSprite;//在闪烁时用到
	void blink();
	void blinkPart2(float dt);
	//已死亡
	void dead();
	//void deadPart2(float dt);
	//登场
	virtual void comeOnStage();
	//获得monster正中间的位置
	Point position();
	//恢复默认状态、动画
	void monsterResume();
	//有伤害判定的状态
	void hurting(float dt);
	//攻击
	virtual void attack(float dt) = 0;
	////被上buff
	//void getBuff(int type);
	//返回精灵
	Sprite* getSprite();
	//返回方向
	int getDirection();
	Vec2 getMoveDirection();
	//重写boundingBox()
	Rect boundingBox();
	//重设攻击间隔
	void resetAttackTime(float dt);
	//移动或停下
	void moveOrStop(float dt);
	MonsWeapon* weapon;
	
};

#endif // !__MONSTER_H__