#ifndef __HERO1_H__
#define __HERO1_H__
#include "cocos2d.h"

USING_NS_CC;

class HRocker;
class Hero :public Layer//?
{
public:
	//生命值
	int lifeNumber;
	//能量值
	int powerNumber;
	//护盾值
	int shieldNumber;
	//金币数
	int goldCoin;
	//位置
	Point position;
	//移动速度
	double velocity;
	//精灵实体
	Sprite* sprite;
	//父节点
	Scene* scene;
	virtual bool init();

	virtual void heroInit(Point position, Scene* scene) = 0;
	//移动到某位置
	void moveTo(double dx, double dy);

	//创建动画
	//status = 1:向右站立；2：向右跑动；-1：向左站立；-2：向左跑动
	virtual Animate* createAnimate(int status) = 0;
	//动作合成
	void setAction(int status);
	//判断能否移动(障碍物)
	bool isCanMove(int dx,int dy);
	//技能
	virtual void ability() = 0;
	//自带的定时器
	void myUpdate(float dt);
	//摇杆控制
	//启动键盘遥杆
	void startKeyRocker();
	//启动触碰摇杆
	void startTouchRocker();
	//停止键盘摇杆
	void stopKeyRocker();
	//停止触碰摇杆
	void stopTouchRocker();
	
protected:
	//移动的微小量
	float dx, dy;
	//对上次运动的记忆
	float dx_, dy_;
	//1：向右；-1：向左
	int faceDirection;
	//1：向右；-1：向左；0：站立
	int isRun;
	//判断是否能释放技能
	bool isAbility;
	//技能时间间隔
	int abilityInterval;
	//次数记录
	int count;	
	//有元类
	friend class HRocker;
	//添加摇杆
	HRocker *rocker;
};

#endif //__HERO_H__