#include "Hero/Knight.h"
#include "Hero/HRocker.h"
#include "Weapon/Weapon.h"
//#include "FightScene.h"
USING_NS_CC;

Knight* Knight::createHeroSprite(Point position, FightScene* scene)
{
	Knight* hero = new Knight();
	if (hero && hero->init())
	{
		hero->autorelease();//自动清理内存
		hero->heroInit(position, scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Knight::heroInit(Point position, FightScene* scene)
{
	//设置基本参数
	velocity = 6;//设置速度大小
	lifeNumberMax = 7;
	lifeNumberNow = lifeNumberMax;
	powerNumberMax = 200;
	powerNumberNow = powerNumberMax;
	shieldNumberMax = 6;
	shieldNumberNow = shieldNumberMax;
	goldCoin = 0;
	name = "Knight";
	//abilityInterval = 300;//300*0.02=6s
	//技能时间与间隔时间
	abilityBetweenTime = 8;
	abilityTime = 4;
	//护盾恢复时间
	recoverTime = 2;
	//初始状态
	this->setPosition(position);
	sprite = Sprite::create("character/Knight.png");
	//sprite->setAnchorPoint(Vec2(0, 0));
	//addChild(weaponNow,3);
	//sprite->setPosition(position);
	this->addChild(sprite,1);
	setAction(1);
	//加入摇杆并让rocker位于父节点中
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150,150),this);
	this->rocker = rocker;
	//加入父节点
	this->scene = scene;
	scene->addChild(rocker);
	//static_cast<Layer*>(scene->getChildByTag(-1))->addChild(this, 31);
	//设置大小
	scale = 0.85;
	this->setScale(scale);
	//初始化展示信息
	initInformation();
	//设置锚点
	this->setAnchorPoint(Vec2(0, 0));
	//注册定时器,间隔为0.02秒
	this->schedule(schedule_selector(Knight::myUpdate),betweenTime);
	//共同数据初始化
	commonDataInit();
	
}

Animate* Knight::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("character/KnightPictures.plist", "character/KnightPictures.png");//使用图集
	Vector<SpriteFrame*> frameArry;
	if (status == 1)//向右站立（动画）
	{
		auto* frame = m_frameCache->getSpriteFrameByName("Knight3.png");
		frameArry.pushBack(frame);
	}
	else if (status == 2)//向右跑（动画）
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("Knight3.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("Knight4.png");
		frameArry.pushBack(frame2);
	}
	else if (status == 3)//向左站立（动画）
	{
		auto* frame = m_frameCache->getSpriteFrameByName("Knight1.png");
		frameArry.pushBack(frame);
	}
	else if (status == 4)//向左跑（动画）
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("Knight1.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("Knight2.png");
		frameArry.pushBack(frame2);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArry);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}

void Knight::ability()
{
	log("ability!");
	auto fire = ParticleFire::create();//暂时用火焰粒子
	fire->setPosition(0,35);
	fire->setStartSize(100);
	fire->setEndSize(120);
	fire->setDuration(abilityTime);//设置显示时间
	fire->setLife(0.15);
	this->addChild(fire,0);
	for (int i = 0; i < weapon.size(); i++)
	{
		weapon[i]->speed = weapon[i]->speed / 2;
	}
	if (weaponNow != nullptr)
	{
		attackTime = weaponNow->speed / betweenTime;
	}
	abilityCount_ = abilityTime / betweenTime;
}

void Knight::removeAbilityFunction()
{
	if (weaponNow)
	{
		
		for (int i = 0; i < weapon.size(); i++)
		{
			weapon[i]->speed = weapon[i]->speed * 2;
		}
		attackTime = weaponNow->speed / betweenTime;
	}
}