#include "Hero/Sorcerer.h"
#include "Hero/HRocker.h"
#include "Weapon/Weapon.h"
USING_NS_CC;

Sorcerer* Sorcerer::createHeroSprite(Point position, FightScene* scene)
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

void Sorcerer::heroInit(Point position, FightScene* scene)
{
	//设置基本参数
	velocity = 6;//设置速度大小
	lifeNumberMax = 8;
	lifeNumberNow = lifeNumberMax;
	powerNumberMax = 250;
	powerNumberNow = powerNumberMax;
	shieldNumberMax = 7;
	shieldNumberNow = shieldNumberMax;
	goldCoin = 0;
	name = "Sorcerer";
	//abilityInterval = 300;
	abilityBetweenTime = 10;
	abilityTime = 5;
	//护盾恢复时间
	recoverTime = 2;
	//初始状态
	this->setPosition(position);
	sprite = Sprite::create("character/sorcerer.png");
	//sprite->setPosition(position);
	this->addChild(sprite, 1);
	setAction(1);
	//加入摇杆并让rocker位于父节点中
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150, 150), this);
	this->rocker = rocker;
	scene->addChild(rocker);
	//加入父节点
	this->scene = scene;
	//scene->addChild(this, 31);
	//调整大小
	scale = 0.8;
	this->setScale(scale);
	//初始化展示信息
	initInformation();
	//设置锚点
	this->setAnchorPoint(Vec2(0, 0));
	//注册定时器,间隔为0.02秒
	this->schedule(schedule_selector(Sorcerer::myUpdate), betweenTime);
	//共同数据初始化
	commonDataInit();
}

Animate* Sorcerer::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("character/SorcererPictures.plist", "character/SorcererPictures.png");//使用图集
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

void Sorcerer::ability()
{
	auto galaxy = ParticleGalaxy::create();//暂时用星系粒子
	galaxy->setPosition(0, 35);
	galaxy->setStartSize(180);
	galaxy->setEndSize(200);
	galaxy->setDuration(abilityTime);//设置显示时间
	galaxy->setLife(0.5);
	this->addChild(galaxy, 0);
}
//技能攻击效果
void Sorcerer::removeAbilityFunction()
{

}