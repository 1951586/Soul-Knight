#include "Knight.h"
#include "HRocker.h"
USING_NS_CC;

Knight* Knight::createHeroSprite(Point position,Scene* scene)
{
	Knight* hero = new Knight();
	if (hero && hero->init())
	{
		hero->autorelease();//�Զ������ڴ�
		hero->heroInit(position, scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Knight::heroInit(Point position, Scene* scene)
{
	//���û�������
	dx = 0, dy = 0;
	dx_ = 0, dy_ = 0;
	faceDirection = 1;
	isRun = 0;
	velocity = 6;//�����ٶȴ�С
	lifeNumber = 7;
	powerNumber = 180;
	shieldNumber = 6;
	goldCoin = 0;
	isAbility = true;
	abilityInterval = 300;
	count = 0;
	//��ʼ״̬
	this->position = position;
	sprite = Sprite::create("Map/Knight3.png");
	sprite->setPosition(position);
	this->addChild(sprite,1);
	setAction(1);
	//����ҡ�˲���rockerλ�ڸ��ڵ���
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150,150),this);
	this->rocker = rocker;
	scene->addChild(rocker,30);
	scene->addChild(this, 31);
	//���ô�С
	this->setScale(0.85);
	//ע�ᶨʱ��,���Ϊ0.02��
	this->schedule(schedule_selector(Knight::myUpdate),0.02f);
}

Animate* Knight::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/KnightPictures.plist", "Map/KnightPictures.png");//ʹ��ͼ��
	Vector<SpriteFrame*> frameArry;
	if (status == 1)//����վ����������
	{
		auto* frame = m_frameCache->getSpriteFrameByName("Knight3.png");
		frameArry.pushBack(frame);
	}
	else if (status == 2)//�����ܣ�������
	{
		auto* frame1 = m_frameCache->getSpriteFrameByName("Knight3.png");
		frameArry.pushBack(frame1);
		auto* frame2 = m_frameCache->getSpriteFrameByName("Knight4.png");
		frameArry.pushBack(frame2);
	}
	else if (status == 3)//����վ����������
	{
		auto* frame = m_frameCache->getSpriteFrameByName("Knight1.png");
		frameArry.pushBack(frame);
	}
	else if (status == 4)//�����ܣ�������
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
	auto sun = ParticleSun::create();//��ʱ��̫�����ӣ������������
	sun->setPosition(position.x,position.y+35);
	sun->setEndSize(70);
	sun->setDuration(3.0f);//������ʾʱ��
	this->addChild(sun,0);
}