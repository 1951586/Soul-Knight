#include "Sorcerer.h"
#include "HRocker.h"
USING_NS_CC;

Sorcerer* Sorcerer::createHeroSprite(Point position, Scene* scene)
{
	Sorcerer* hero = new Sorcerer();
	if (hero && hero->init())
	{
		hero->autorelease();//自动清理内存
		hero->heroInit(position,scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Sorcerer::heroInit(Point position, Scene* scene)
{
	//设置基本参数
	dx = 0, dy = 0;
	dx_ = 0, dy_ = 0;
	faceDirection = 1;
	isRun = 0;
	velocity = 6;//设置速度大小
	lifeNumber = 6;
	powerNumber = 180;
	shieldNumber = 6;
	goldCoin = 0;
	isAbility = true;
	abilityInterval = 300;
	count = 0;
	//初始状态
	this->position = position;
	sprite = Sprite::create("Map/ant1.png");
	sprite->setPosition(position);
	this->addChild(sprite, 1);
	setAction(1);
	//加入摇杆并让rocker位于父节点中
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150, 150), this);
	this->rocker = rocker;
	scene->addChild(rocker, 30);
	scene->addChild(this, 31);
	//调整大小
	this->setScale(0.8);
	//注册定时器,间隔为0.02秒
	this->schedule(schedule_selector(Sorcerer::myUpdate), 0.02f);
}

Animate* Sorcerer::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/SorcererPictures.plist", "Map/SorcererPictures.png");//使用图集
	Vector<SpriteFrame*> frameArry;
	if (status == 1)
	{
		auto* frame = m_frameCache->getSpriteFrameByName("ant1.png");
		frameArry.pushBack(frame);
	}
	else if (status == 2)
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("ant1.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("ant1(3).png");
		frameArry.pushBack(frame2);
	}
	else if (status == 3)
	{
		auto* frame = m_frameCache->getSpriteFrameByName("ant1(2).png");
		frameArry.pushBack(frame);
	}
	else if (status == 4)
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("ant1(2).png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("ant1(4).png");
		frameArry.pushBack(frame2);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArry);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}

void Sorcerer::ability()//还没写
{

}