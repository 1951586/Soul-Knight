#include "Knight.h"
#include "HRocker.h"
USING_NS_CC;

Knight* Knight::createHeroSprite(Point position,Scene* scene)
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

void Knight::heroInit(Point position, Scene* scene)
{
	//设置基本参数
	dx = 0, dy = 0;
	dx_ = 0, dy_ = 0;
	faceDirection = 1;
	isRun = 0;
	velocity = 6;//设置速度大小
	lifeNumber = 7;
	powerNumber = 180;
	shieldNumber = 6;
	goldCoin = 0;
	isAbility = true;
	abilityInterval = 300;
	count = 0;
	//初始状态
	this->position = position;
	sprite = Sprite::create("Map/Knight3.png");
	sprite->setPosition(position);
	this->addChild(sprite,1);
	setAction(1);
	//加入摇杆并让rocker位于父节点中
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150,150),this);
	this->rocker = rocker;
	scene->addChild(rocker,30);
	scene->addChild(this, 31);
	//设置大小
	this->setScale(0.85);
	//注册定时器,间隔为0.02秒
	this->schedule(schedule_selector(Knight::myUpdate),0.02f);
}

Animate* Knight::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/KnightPictures.plist", "Map/KnightPictures.png");//使用图集
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
	auto sun = ParticleSun::create();//暂时用太阳粒子，动起来很奇怪
	sun->setPosition(position.x,position.y+35);
	sun->setEndSize(70);
	sun->setDuration(3.0f);//设置显示时间
	this->addChild(sun,0);
}