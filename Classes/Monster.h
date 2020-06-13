#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "cocos2d.h"
#include "Hero/Hero.h"
#include "Scene/HomeScene.h"
#include <ctime>
USING_NS_CC;
//前置声明
class Hero;
class HomeScene;

enum monster_Animation   //怪物的各种动画状态
{
	//status:
	RIGHT = 1,//向右（以下同理）
	RIGHT_STAND = 11,
	RIGHT_MOVE = 12,
	RIGHT_ATTCK = 13,
	RIGHT_DYING=14,
	RIGHT_DEAD=15,

	LEFT = 2,
	LEFT_STAND = 21,
	LEFT_MOVE = 22,
	LEFT_ATTCK = 23,
	LEFT_DYING=24,
	LEFT_DEAD=25,

	//ActionTag
	MOVE = 100,
	SETANIMATION = 99,
	SHAKE = 98,
	BLINK = 97,
	DYING=96,

};

class Monster 
	:public Sprite
{
protected:
	//生命
	int HP;
	//速度(1代表每秒移动像素50(可修改））
	float speed;
	//攻击力
	int ATK;
	////位置 （在sprite里自带有，获取位置的用xx.sprite：：getPosition）
	//Point position;
	//面部方向（1表示向右，2表示向左）
	int direction;
	//是否正在移动
	bool isMove;
	//是否在攻击
	bool isAttck;
	//一次移动的距离
	float moveDistance;
	//精灵实体(精灵负责动画，this负责位移及其他的)
	Sprite* sprite;
	//场景
	HomeScene* scene;
	//用指针解决相互包含的问题
	Hero* hero;
	//模拟抖动
	void shake();

	//径直向玩家走去
	void directMove();
	//随机移动
	void randomMove();
	//获得到hero的距离
	float getDistance();
	//恢复默认状态
	void monsterResume();
	//被杀
    void killed();


public:
	void setSpeed();
	void setAttack();
	

	//初始化
	virtual bool init();
	virtual void monsterInit(Point position, HomeScene* scene) = 0;//（先暂时用HomeScene类的指针）
	//创建动画(参数为动作描述、动作帧数
	virtual Animate* createAnimate(int status, int numFrame) = 0;

	//判断能否移动(是否点(x,y)有障碍物)
	bool isCanMove(int x, int y);

	//自带的定时器
	void myUpdate(float dt);

	//设置动画（只负责动画，并不涉及动作）
	void setAnimation(int status);

	//追踪玩家
	void chase(float dt);

	//对monster实例化hero
	void getHeroInstance(Hero* hero);

	//被攻击，hurt为伤害数值（血量为零是自动触发死亡）
	void beAttcked(int hurt);

	//闪烁
	Sprite* blinkSprite;//在闪烁时用到
	void blink();
	void blinkPart2(float dt);

	//已死亡
	void dead();
	//void deadPart2(float dt);
	//是否能被碰撞(死亡后不可碰撞）
	bool isCanTouched;
};

#endif // !__MONSTER_H__