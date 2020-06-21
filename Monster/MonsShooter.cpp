#include "MonsShooter.h"
#include "Scene/FightScene.h"
#include "Hero/Hero.h"
#include "Monster/Monster.h"
#include "Weapon/MonsWeapon.h"
USING_NS_CC;


MonsShooter* MonsShooter::createMonsterSprite(Point position, FightScene* scene)
{

	MonsShooter* monsShooter = new MonsShooter();
	if (monsShooter && monsShooter->init())
	{
		monsShooter->autorelease();//自动清理内存
		monsShooter->monsterInit(position, scene);
		return monsShooter;
	}
	CC_SAFE_DELETE(monsShooter);
	return NULL;

}

void MonsShooter::monsterInit(Point position, FightScene* scene)
{
	HP = 18;
	speed = 250;
	ATK = 2;
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
	sprite = Sprite::create("monster/MonsShooter11.png");
	this->addChild(sprite,3);
	this->setPosition(position);
	sprite->setPosition(0, 0);
	//把锚点设在下方中间
	this->setAnchorPoint(Vec2(0.5, 0));
	sprite->setAnchorPoint(Vec2(0.5, 0));
	//武器
	weapon = MonsWeapon::createMonsWeapon(scene, this, hero, "blowgun");
	this->addChild(weapon,4);	
	//为“闪烁”做准备
	blinkSprite = Sprite::create("monster/MonsShooter11.png");
	blinkSprite->setAnchorPoint(ccp(0, 0));
	blinkSprite->setVisible(false);
	sprite->addChild(blinkSprite,5);

}

void MonsShooter::setAnimation(int status)
{

	//判断帧数
	int numFrame;
	if (status % 10 == 1) numFrame = 1;
	else if (status % 10 == 2) numFrame = 2;
	else if (status % 10 == 3) numFrame = 2;
	else if (status % 10 == 4) numFrame = 1;
	else if (status % 10 == 5) numFrame = 1;


	sprite->stopActionByTag(SETANIMATION);
	auto* animate = createAnimate(status, numFrame);
	animate->setTag(SETANIMATION);
	sprite->runAction(animate);

}

//由对应的描述和帧数创建动画
Animate* MonsShooter::createAnimate(int status, int numFrame)
{

	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("monster/MonsShooter.plist", "monster/MonsShooter.png");
	Vector <SpriteFrame*> frameArray;
	for (int i = 1;i <= numFrame;i++)
	{
		auto* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("MonsShooter%d_%d.png", status, i)->getCString());
		frameArray.pushBack(frame);

	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	if (status == RIGHT_ATTACK || status == LEFT_ATTACK) {
		animation->setLoops(1);
		animation->setDelayPerUnit(0.15f);
	}
	else if (status == RIGHT_ATTACK_PART2 || status == LEFT_ATTACK_PART2) {
		animation->setLoops(1);
		animation->setDelayPerUnit(0.15f);
	}
	else {
		animation->setLoops(-1);
		animation->setDelayPerUnit(0.3f);
	}
	return Animate::create(animation);
}

void MonsShooter::comeOnStage() 
{
	isOnStage = true;
	isCanTouched = true;
	this->setVisible(true);
	weapon->setVisible(true);
	shake();
	schedule(schedule_selector(MonsShooter::adjustRad), 0.1);
	schedule(schedule_selector(MonsShooter::chase), moveDistance / speed);
	schedule(schedule_selector(Monster::moveOrStop), 1.0 / 60.0);
	scheduleOnce(schedule_selector(MonsShooter::attack), attackTime);
	scheduleOnce(schedule_selector(Monster::resetAttackTime), attackTime + 1);
	
}

void MonsShooter::chase(float dt)
{
	int random = rand()%6 ;
	if (random > 0 && random < 1) directMove();
	else if (1 <= random && random < 5)randomMove();
	else monsterResume();
}

void MonsShooter::adjustRad(float dt)
{
	weapon->adjustDirection();
}

void MonsShooter::attack(float dt)
{
	//this->stopActionByTag(MOVE);
	//unschedule(schedule_selector(Monster::chase));
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
	scheduleOnce(schedule_selector(MonsShooter::attackPart2), 0.3);
	setAnimation(status);

}

void MonsShooter::attackPart2(float dt)
{
	weapon->attack();
}
void MonsShooter::killed()
{
	//停止行动
	//unschedule(schedule_selector(Monster::chase));
	//unschedule(schedule_selector(Monster::moveOrStop));
	//unschedule(schedule_selector(Monster::attack));
	//unschedule(schedule_selector(Monster::resetAttackTime));
	//unschedule(schedule_selector(MonsShooter::adjustRad));
	//unschedule(schedule_selector(MonsShooter::attackPart2));
	unscheduleAllSelectors();
	schedule(schedule_selector(Monster::moveOrStop), 1 / 60);
	sprite->stopActionByTag(SHAKE);
	this->stopAllActions();
	blinkSprite->setVisible(false);
	//死亡后不可碰撞,退场
	isCanTouched = false;
	isOnStage = false;
	isAlive = false;
	//飞出的距离
	float throughtDistance = 200;
	float distance = getHeroDistance();
	//朝英雄反向飞出
	float x = -(hero->position.x - this->position().x)*throughtDistance / distance;
	float y = -(hero->position.y - this->position().y)*throughtDistance / distance;
	moveDirection = Vec2(x / throughtDistance, y / throughtDistance);
	int status;
	if (x < 0) {
		status = RIGHT_DYING;
		direction = RIGHT;
	}
	else {
		status = LEFT_DYING;
		direction = LEFT;
	}
	//击飞
	auto* move = MoveBy::create(throughtDistance / 300, Vec2(x, y));
	auto* asitmove = MoveBy::create(throughtDistance / 300, Vec2(0, 0));
	move->setTag(MOVE);
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::dead, this));
	auto* sequence = Sequence::create(asitmove, callFunc, NULL);
	this->runAction(move);
	this->runAction(sequence);

	//击飞时旋转
	Action* rotate;
	if (direction == RIGHT)
		rotate = RotateTo::create(throughtDistance / 300 / 2, -90);
	else
		rotate = RotateBy::create(throughtDistance / 300 / 2, 90);
	sprite->runAction(rotate);
	setAnimation(status);
}