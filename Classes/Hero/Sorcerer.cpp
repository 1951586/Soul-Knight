#include "Sorcerer.h"
#include "HRocker.h"
USING_NS_CC;

Sorcerer* Sorcerer::createHeroSprite(Point position, Scene* scene)
{
	Sorcerer* hero = new Sorcerer();
	if (hero && hero->init())
	{
		hero->autorelease();//�Զ������ڴ�
		hero->heroInit(position,scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Sorcerer::heroInit(Point position, Scene* scene)
{
	//���û�������
	velocity = 6;//�����ٶȴ�С
	lifeNumberMax = 8;
	lifeNumberNow = lifeNumberMax;
	powerNumberMax = 250;
	powerNumberNow = powerNumberMax;
	shieldNumberMax = 7;
	shieldNumberNow = shieldNumberMax;
	goldCoin = 0;
	abilityInterval = 300;
	//��ͬ���ݳ�ʼ��
	commonDataInit();
	//��ʼ״̬
	this->setPosition(position);
	sprite = Sprite::create("Map/ant1.png");
	//sprite->setPosition(position);
	this->addChild(sprite, 1);
	setAction(1);
	//����ҡ�˲���rockerλ�ڸ��ڵ���
	auto rocker = HRocker::createHRocker("Map/Rocker.png", "Map/RockerBG.png", Point(150, 150), this);
	this->rocker = rocker;
	scene->addChild(rocker);
	//���븸�ڵ�
	this->scene = scene;
	//scene->addChild(this, 31);
	//������С
	scale = 0.8;
	this->setScale(scale);
	//��ʼ��չʾ��Ϣ
	initInformation();
	//����ê��
	this->setAnchorPoint(Vec2(0, 0));
	//ע�ᶨʱ��,���Ϊ0.02��
	this->schedule(schedule_selector(Sorcerer::myUpdate), 0.02f);
}

Animate* Sorcerer::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("Map/SorcererPictures.plist", "Map/SorcererPictures.png");//ʹ��ͼ��
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

void Sorcerer::ability()//��ûд
{
	auto galaxy = ParticleGalaxy::create();//��ʱ����ϵ����
	galaxy->setPosition(0, 35);
	galaxy->setStartSize(180);
	galaxy->setEndSize(200);
	galaxy->setDuration(5.0f);//������ʾʱ��
	galaxy->setLife(0.5);
	this->addChild(galaxy, 0);
}