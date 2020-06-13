#include "Ranger.h"
#include "HRocker.h"
#include <cmath>
#include <algorithm>
Ranger* Ranger::createHeroSprite(Point position, Scene* scene)
{
	Ranger* hero = new Ranger();
	if (hero && hero->init())
	{
		hero->autorelease();//自动清理内存
		hero->heroInit(position,scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Ranger::heroInit(Point position, Scene* scene)//初始化状态，可自行修改
{
	//设置基本参数
	velocity = 7;//设置速度大小
	lifeNumberMax =5;
	lifeNumberNow = lifeNumberMax;
	powerNumberMax = 180;
	powerNumberNow = powerNumberMax;
	shieldNumberMax = 4;
	shieldNumberNow = shieldNumberMax;
	goldCoin = 0;
	abilityInterval = 40;
	rotateDistance = 300;//设置技能距离
	//共同数据初始化
	commonDataInit();
	//初始状态
	this->setPosition(position);
	sprite = Sprite::create("Map/Ranger1.png");
	//sprite->setPosition(position);
	this->addChild(sprite, 1);
	setAction(1);
	//加入摇杆并让rocker位于父节点中
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150, 150), this);
	this->rocker = rocker;
	scene->addChild(rocker);
	//scene->addChild(this, 31);
	//加入父节点
	this->scene = scene;
	//设置大小
	scale = 0.6;
	this->setScale(scale);
	//设置锚点
	this->setAnchorPoint(Vec2(0, 0));
	//初始化展示信息
	initInformation();
	//注册定时器,间隔为0.02秒
	this->schedule(schedule_selector(Ranger::myUpdate), 0.02f);
}

Animate* Ranger::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/RangerPictures.plist", "Map/RangerPictures.png");//使用图集
	Vector<SpriteFrame*> frameArry;
	if (status == 1)
	{
		auto* frame = m_frameCache->getSpriteFrameByName("Ranger1.png");
		frameArry.pushBack(frame);
	}
	else if (status == 2)
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("Ranger1.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("Ranger2.png");
		frameArry.pushBack(frame2);
	}
	else if (status == 3)
	{
		auto* frame = m_frameCache->getSpriteFrameByName("Ranger3.png");
		frameArry.pushBack(frame);
	}
	else if (status == 4)
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("Ranger3.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("Ranger4.png");
		frameArry.pushBack(frame2);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArry);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}

void Ranger::ability()
{
	log("ability!");
	auto animate = createRotateAnimate();
	sprite->stopAllActions();
	this->unschedule(schedule_selector(Ranger::myUpdate));
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Ranger::afterAbility,this));
	auto move = abilityMoveBy();
	auto spawn = Spawn::create(animate, move, NULL);
	auto sequence = Sequence::create(spawn, callFunc, NULL);
	sprite->runAction(sequence);
	/*
	if (withScene)
	{
		//获取运动图层
		auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
		layer->runAction(move->clone()->reverse());
	}
	*/
}

//private:
Animate* Ranger::createRotateAnimate()
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/RangerRotatePictures.plist", "Map/RangerRotatePictures.png");//使用图集
	Vector<SpriteFrame*> frameArry;
	if (faceDirection == 1)//向右翻转
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("RangerRotate1.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("RangerRotate2.png");
		frameArry.pushBack(frame2);
		auto* frame3 = m_frameCache->getSpriteFrameByName("RangerRotate3.png");
		frameArry.pushBack(frame3);
		auto* frame4 = m_frameCache->getSpriteFrameByName("RangerRotate4.png");
		frameArry.pushBack(frame4);
	}
	else if (faceDirection == -1)//向左翻转
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("RangerRotate_1.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("RangerRotate_2.png");
		frameArry.pushBack(frame2);
		auto* frame3 = m_frameCache->getSpriteFrameByName("RangerRotate_3.png");
		frameArry.pushBack(frame3);
		auto* frame4 = m_frameCache->getSpriteFrameByName("RangerRotate_4.png");
		frameArry.pushBack(frame4);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArry);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.05f);//技能时间：0.05*4 = 0.2秒
	auto animate = Animate::create(animation);
	return Animate::create(animation);
}

void Ranger::afterAbility()
{
	if (faceDirection == 1)
		setAction(1);
	else
		setAction(3);
	this->schedule(schedule_selector(Ranger::myUpdate), 0.02f);//再次注册定时器,间隔为0.02秒
}

MoveBy* Ranger::abilityMoveBy()
{
	float r = sqrt(pow(dx_, 2)+ pow(dy_, 2));
	dx_ = (dx_ / r) * rotateDistance/scale;
	dy_ = (dy_ / r) * rotateDistance/scale;
	auto move = MoveBy::create(0.2, Vec2(dx_, dy_));
	return move;
}