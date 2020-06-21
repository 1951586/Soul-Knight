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
		hero->autorelease();//�Զ������ڴ�
		hero->heroInit(position, scene);
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Knight::heroInit(Point position, FightScene* scene)
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
	name = "Knight";
	//abilityInterval = 300;//300*0.02=6s
	//����ʱ������ʱ��
	abilityBetweenTime = 8;
	abilityTime = 4;
	//���ָܻ�ʱ��
	recoverTime = 2;
	//��ʼ״̬
	this->setPosition(position);
	sprite = Sprite::create("character/Knight.png");
	//sprite->setAnchorPoint(Vec2(0, 0));
	//addChild(weaponNow,3);
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
	this->schedule(schedule_selector(Knight::myUpdate),betweenTime);
	//��ͬ���ݳ�ʼ��
	commonDataInit();
	
}

Animate* Knight::createAnimate(int status)
{
	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("character/KnightPictures.plist", "character/KnightPictures.png");//ʹ��ͼ��
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
	fire->setDuration(abilityTime);//������ʾʱ��
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