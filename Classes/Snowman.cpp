#include "Snowman.h"

USING_NS_CC;


Snowman* Snowman::createMonsterSprite(Point position, HomeScene* scene)
{

	Snowman* snowman = new Snowman();
	if (snowman && snowman->init())
	{
		snowman->autorelease();//自动清理内存
		snowman->monsterInit(position,scene);		
		return snowman;
	}
	CC_SAFE_DELETE(snowman);
	return NULL;

}

void Snowman::monsterInit(Point position, HomeScene* scene)
{
	HP = 10;
	speed = 250;
	ATK = 3;
	direction = 1;
	isMove = 0;
	isAttck = 0;
	isCanTouched = 1;
	moveDistance = 200;
	this->scene = scene;
	this->hero = scene->hero;
	sprite = Sprite::create("monster/Snowman11.png");
	this->addChild(sprite);
	this->setPosition(position);
	//把锚点设在下方中间
	this->setAnchorPoint(Vec2(0.5, 0));
	sprite->setAnchorPoint(Vec2(0.5, 0));
	//使用随机数做准备
	srand((unsigned)time(NULL));
	//一直都抖动，应该没什么问题吧
	shake();
	schedule(schedule_selector(Monster::chase), moveDistance/speed) ;

}

Animate* Snowman::createAnimate(int status,int numFrame)
{

	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("monster/Snowman.plist", "monster/Snowman.png");
	Vector <SpriteFrame*> frameArray;
	for (int i = 1;i <= numFrame;i++)
	{
		auto* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("Snowman%d_%d.png",status,i)->getCString());
		frameArray.pushBack(frame);

	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.5f);
	return Animate::create(animation);
}