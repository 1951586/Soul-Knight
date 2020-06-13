#ifndef __HERO_H__
#define __HERO_H__
#include "cocos2d.h"

USING_NS_CC;

class HRocker;
class Hero :public Layer
{
public:
	//生命值
	int lifeNumberNow;
	int lifeNumberMax;
	//能量值
	int powerNumberNow;
	int powerNumberMax;
	//护盾值
	int shieldNumberNow;
	int shieldNumberMax;
	//金币数
	int goldCoin;
	//位置
	Point position;//相对于运动场景左下角的坐标
	//移动速度
	double velocity;
	//能否能移动
	bool canMove;
	//精灵实体
	Sprite* sprite;
	//父节点
	Scene* scene;
	virtual bool init();

	virtual void heroInit(Point position, Scene* scene) = 0;
	//移动障碍判断
	bool isCanMove(int dx,int dy);
	//摇杆控制
	//启动键盘遥杆
	void startKeyRocker();
	//启动触碰摇杆
	void startTouchRocker();
	//停止键盘摇杆
	void stopKeyRocker();
	//停止触碰摇杆
	void stopTouchRocker();
	//显示人物信息
	void showInformation();
	//隐藏人物信息
	void hideInformation();
	//让人物跟随场景动
	void moveWithScene();
	//停止人物与场景的互动
	void moveWithoutScene();
	//自定义的定时器
	void myUpdate(float dt);
	//地图层
	//CCTMXTiledMap* map;
	/* 检测碰撞的地图层 */
	//CCTMXLayer* meta;
	//CCTMXObjectGroup* meta;
	
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
	//设置缩放(初始化时用)
	float scale;
	//是否展示
	bool isShow;
	
	//是否跟随场景动
	bool withScene;
	//次数记录
	int count;	
	//内部函数
	//创建动画
	//status = 1:向右站立；2：向右跑动；-1：向左站立；-2：向左跑动
	virtual Animate* createAnimate(int status) = 0;
	//动作合成
	void setAction(int status);
	//判断能否移动(障碍物)
	//技能
	virtual void ability() = 0;
	//初始化信息
	void initInformation();
	//改变显示信息
	void changeInformation();
	//共有属性初始化
	void commonDataInit();
	//移动到某位置
	void moveBy(double dx, double dy);
	//有元类
	friend class HRocker;
	//添加摇杆
	HRocker *rocker;
};

#endif //__HERO_H__