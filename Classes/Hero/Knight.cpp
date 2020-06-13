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
	velocity = 6;//�����ٶȴ�С
	lifeNumberMax = 7;
	lifeNumberNow = lifeNumberMax;
	powerNumberMax = 200;
	powerNumberNow = powerNumberMax;
	shieldNumberMax = 6;
	shieldNumberNow = shieldNumberMax;
	goldCoin = 0;
	abilityInterval = 300;
	//��ͬ���ݳ�ʼ��
	commonDataInit();
	//��ʼ״̬
	this->setPosition(position);
	sprite = Sprite::create("Map/Knight3.png");
	//sprite->setPosition(position);
	this->addChild(sprite,1);
	setAction(1);
	//����ҡ�˲���rockerλ�ڸ��ڵ���
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150,150),this);
	this->rocker = rocker;
	//���븸�ڵ�
	this->scene = scene;
	scene->addChild(rocker);
	//static_cast<Layer*>(scene->getChildByTag(-1))->addChild(this, 31);
	//���ô�С
	scale = 0.85;
	this->setScale(scale);
	//��ʼ��չʾ��Ϣ
	initInformation();
	//����ê��
	this->setAnchorPoint(Vec2(0, 0));
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
	auto fire = ParticleFire::create();//��ʱ�û�������
	fire->setPosition(0,35);
	fire->setStartSize(100);
	fire->setEndSize(120);
	fire->setDuration(5.0f);//������ʾʱ��
	fire->setLife(0.15);
	this->addChild(fire,0);
}