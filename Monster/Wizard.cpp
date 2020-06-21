#include "Wizard.h"
#include "Scene/FightScene.h"
#include "Hero/Hero.h"
#include "Monster/Monster.h"
#include "Weapon/MonsWeapon.h"
USING_NS_CC;


Wizard* Wizard::createMonsterSprite(Point position, FightScene* scene)
{

	Wizard* wizard = new Wizard();
	if (wizard && wizard->init())
	{
		wizard->autorelease();//自动清理内存
		wizard->monsterInit(position, scene);
		return wizard;
	}
	CC_SAFE_DELETE(wizard);
	return NULL;

}

void Wizard::monsterInit(Point position, FightScene* scene)
{
	HP = 18;
	speed = 250;
	ATK = 3;
	direction = 1;
	moveDirection = Vec2(0, 0);
	isMove = false;
	isAttck = false;
	moveDistance = 200;
	attackDis = 200;
	attackSpeed = 300;
	//第一次攻击拉大点攻击间隔
	attackTime = static_cast<float> (rand() % 100) / 10.0 + 2.0;
	//状态
	isAlive = true;
	isOnStage = false;
	isCanTouched = false;

	this->scene = scene;
	this->hero = scene->hero;
	this->setVisible(false);
	sprite = Sprite::create("monster/Wizard11.png");
	this->addChild(sprite,3);
	this->setPosition(position);
	sprite->setPosition(0, 0);
	//把锚点设在下方中间
	this->setAnchorPoint(Vec2(0.5, 0));
	sprite->setAnchorPoint(Vec2(0.5, 0));
	//武器
	weapon = MonsWeapon::createMonsWeapon(scene, this, hero, "airwand");
	this->addChild(weapon,4);	
	//为“闪烁”做准备
	blinkSprite = Sprite::create("monster/Wizard11.png");
	blinkSprite->setAnchorPoint(ccp(0, 0));
	blinkSprite->setVisible(false);
	sprite->addChild(blinkSprite,5);
	
}

void Wizard::setAnimation(int status)
{

	//判断帧数
	int numFrame;
	if (status % 10 == 1) numFrame = 1;
	else if (status % 10 == 2) numFrame = 2;
	else if (status % 10 == 3) numFrame = 2;
	else if (status % 10 == 5) numFrame = 5;


	sprite->stopActionByTag(SETANIMATION);
	auto* animate = createAnimate(status, numFrame);
	animate->setTag(SETANIMATION);
	sprite->runAction(animate);

}

//由对应的描述和帧数创建动画
Animate* Wizard::createAnimate(int status, int numFrame)
{

	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("monster/Wizard.plist", "monster/Wizard.png");
	Vector <SpriteFrame*> frameArray;
	for (int i = 1;i <= numFrame;i++)
	{
		auto* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("Wizard%d_%d.png", status, i)->getCString());
		frameArray.pushBack(frame);

	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	if (status == RIGHT_ATTACK || status == LEFT_ATTACK) {
		animation->setLoops(1);
		animation->setDelayPerUnit(0.15f);
	}
	else if (status == RIGHT_DEAD || status == LEFT_DEAD) {
		animation->setLoops(1);
		animation->setDelayPerUnit(0.15f);
	}
	else {
		animation->setLoops(-1);
		animation->setDelayPerUnit(0.3f);
	}
	return Animate::create(animation);
}
//
//void Wizard::comeOnStage() 
//{
//	isOnStage = true;
//	isCanTouched = true;
//	this->setVisible(true);
//	weapon->setVisible(true);
//	shake();
//	schedule(schedule_selector(Wizard::chase), moveDistance / speed);
//	scheduleOnce(schedule_selector(Wizard::attack), attackTime);
//	scheduleOnce(schedule_selector(Monster::resetAttackTime), attackTime + 1);
//	
//}

void Wizard::chase(float dt)
{	
	int random = rand()%6 ;
	if (random > 0 && random < 1) directMove();
	else if (1 <= random && random < 5)randomMove();
	else monsterResume();
}


void Wizard::attack(float dt)
{

	float x = hero->position.x - this->position().x;
	float y = hero->position.y - this->position().y;

	int status;
	if (x > 0) {
		status = RIGHT_ATTACK;
		direction = RIGHT;
	}
	else {
		status = LEFT_ATTACK;
		direction = LEFT;
	}
	scheduleOnce(schedule_selector(Wizard::attackPart2), 0.3);
	setAnimation(status);

}

void Wizard::attackPart2(float dt)
{
	weapon->attack();
}

void Wizard::killed()
{
	unscheduleAllSelectors();
	blinkSprite->setVisible(false);
	sprite->stopActionByTag(SHAKE);
	this->stopAllActions();
	moveDirection = Vec2(0, 0);
	//死亡后不可碰撞,退场
	isCanTouched = false;
	isOnStage = false;
	isAlive = false;
	if (direction == RIGHT)
		setAnimation(RIGHT_DEAD);
	else
		setAnimation(LEFT_DEAD);
}