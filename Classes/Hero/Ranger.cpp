#include "Ranger.h"
#include "HRocker.h"
#include <cmath>
#include <algorithm>
Ranger* Ranger::createHeroSprite(Point position, Scene* scene)
{
	Ranger* hero = new Ranger();
	if (hero && hero->init())
	{
		hero->autorelease();//�Զ������ڴ�
		hero->heroInit(position,scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Ranger::heroInit(Point position, Scene* scene)//��ʼ��״̬���������޸�
{
	//���û�������
	dx = 0, dy = 0;
	dx_ = 0, dy_ = 0;
	faceDirection = 1;
	isRun = 0;
	velocity = 7;//�����ٶȴ�С
	lifeNumber =5;
	powerNumber = 180;
	shieldNumber = 4;
	goldCoin = 0;
	isAbility = true;
	abilityInterval = 40;
	count = 0;
	//��ʼ״̬
	this->position = position;
	sprite = Sprite::create("Map/Ranger1.png");
	sprite->setPosition(position);
	this->addChild(sprite, 1);
	setAction(1);
	//����ҡ�˲���rockerλ�ڸ��ڵ���
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150, 150), this);
	this->rocker = rocker;
	scene->addChild(rocker, 30);
	scene->addChild(this, 31);
	//���ô�С
	this->setScale(0.6);
	//���ü��ܾ���
	rotateDistance = 500;
	//ע�ᶨʱ��,���Ϊ0.02��
	this->schedule(schedule_selector(Ranger::myUpdate), 0.02f);
}

Animate* Ranger::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/RangerPictures.plist", "Map/RangerPictures.png");//ʹ��ͼ��
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
}

//private:
Animate* Ranger::createRotateAnimate()
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/RangerRotatePictures.plist", "Map/RangerRotatePictures.png");//ʹ��ͼ��
	Vector<SpriteFrame*> frameArry;
	if (faceDirection == 1)//���ҷ�ת
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
	else if (faceDirection == -1)//����ת
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
	animation->setDelayPerUnit(0.05f);//����ʱ�䣺0.05*4 = 0.2��
	auto animate = Animate::create(animation);
	return Animate::create(animation);
}

void Ranger::afterAbility()
{
	if (faceDirection == 1)
		setAction(1);
	else
		setAction(3);
	this->schedule(schedule_selector(Ranger::myUpdate), 0.02f);//�ٴ�ע�ᶨʱ��,���Ϊ0.02��
}

MoveBy* Ranger::abilityMoveBy()
{
	float r = sqrt(pow(dx_, 2)+ pow(dy_, 2));
	dx_ = (dx_ / r) * rotateDistance;
	dy_ = (dy_ / r) * rotateDistance;
	auto move = MoveBy::create(0.2, Vec2(dx_, dy_));
	return move;
}