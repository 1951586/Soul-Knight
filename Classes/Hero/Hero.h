#ifndef __HERO_H__
#define __HERO_H__
#include "cocos2d.h"
#include <vector>
#include <list>
#include <string>

USING_NS_CC;

class HRocker;
class Monster;
class Bullet;//前置声明
class Weapon;
class FightScene;
struct specialSprite;
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
	//英雄名字
	std::string name;
	//位置（相对于运动图层左下角的坐标）
	Point position;
	//移动速度
	float velocity;
	//判断是否碰撞
	bool isCollision;
	//能否能移动
	bool canMove;
	//精灵实体
	Sprite* sprite;
	//父节点
	FightScene* scene;
	//判断是否有其他功能
	bool isOtherFunction;
	//其他功能结构
	specialSprite* spec;
	//走了几场
	std::vector<int> _num;
	//攻击频率
	float k;
	//其它功能函数
	void otherFunction(specialSprite* _spec);
	virtual bool init();
	//纯虚函数
	virtual void heroInit(Point position, FightScene* scene) = 0;
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
	//获取英雄矩形框（重写）
	Rect boundingBox();
	//自定义的定时器
	void myUpdate(float dt);
	//地图层
	//CCTMXTiledMap* map;
	/* 检测碰撞的地图层 */
	//CCTMXLayer* meta;
	//CCTMXObjectGroup* meta;


	//子弹容器
	std::vector<Bullet*> bullet;
	//障碍物容器
	std::vector<Node*> barrier;
	//武器容器
	std::vector<Weapon*> weapon;
	//当前所持有的武器
	Weapon* weaponNow;
	//最多拥有的武器数
	int weaponNumberMax;
	//目前拥有的武器数
	int weaponNumberNow;
	//添加或切换武器
	void changeWeapon(Weapon* _weapon);
	//下一个场景英雄
	void nextSceneHero(Hero* hero);
	//下一个场景
	void nextScene(FightScene* scene);
	//被袭击函数
	void beAttacked(int damage);
	
protected:
	//移动的微小量
	float dx, dy;
	//对上次运动的记忆
	float dx_, dy_;
	//1：向右；-1：向左
	int faceDirection;
	//1：向右；-1：向左；0：站立
	int isRun;
	//移动的方向(弧度制)
	double moveDirection;


	//敌人的方向（如果有的话，弧度制）
	double enemyDirection;


	//判断是否能释放技能
	bool isAbility;
	//技能时间
	float abilityTime;
	//技能时间间隔
	float abilityBetweenTime;
	//恢复一个护盾的时间
	float recoverTime;
	//定时器每两次调用时间间隔
	float betweenTime;
	//设置缩放(初始化时用)
	float scale;
	//是否展示
	bool isShow;
	//记录次数(技能间隔)
	int abilityCount;
	//记录次数(技能时间)
	int abilityCount_;
	//记录次数（恢复）
	int recoverCount;
	//记录次数（武器使用）
	int attackCount;
	//技能间隔时间调用次数
	int abilityInterval;
	//护盾调用次数
	int loopTime;
	//每两次袭击调用的次数
	int attackTime;
	//是否跟随场景动
	bool withScene;
	//是否能攻击
	bool canAttack;
	


	//内部函数
	//创建动画
	//status = 1:向右站立；2：向右跑动；-1：向左站立；-2：向左跑动
	virtual Animate* createAnimate(int status) = 0;
	//动作合成
	void setAction(int status);
	//技能图像效果
	virtual void ability() = 0;
	//取消技能攻击效果
	virtual void removeAbilityFunction() = 0;
	//初始化信息
	void initInformation();
	//改变显示信息
	void changeInformation();
	//共有属性初始化
	void commonDataInit();
	//移动到某位置
	void moveBy(double dx, double dy);
	//护盾恢复函数
	void recover();
	//死亡
	void dead();
	
	//攻击函数
	void attack();
	//改变弧度制方向
	void changeDirection();
	//添加武器
	void addWeapon(Weapon* _weapon);
	//切换已拥有的武器
	void changeWeaponNow();
	//随机武器
	std::string randWeapon(int num);
	//有元类
	friend class HRocker;
	friend class Weapon;
	friend class FightScene;
	//添加摇杆
	HRocker *rocker;
	
};

#endif //__HERO_H__