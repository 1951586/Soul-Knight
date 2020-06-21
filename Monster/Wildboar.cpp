#include "Wildboar.h"
#include "Scene/FightScene.h"
#include "Hero/Hero.h"
#include "Monster/Monster.h"

USING_NS_CC;


Wildboar* Wildboar::createMonsterSprite(Point position, FightScene* scene)
{

	Wildboar* wildboar = new Wildboar();
	if (wildboar && wildboar->init())
	{
		wildboar->autorelease();//自动清理内存
		wildboar->monsterInit(position, scene);
		return wildboar;
	}
	CC_SAFE_DELETE(wildboar);
	return NULL;

}

void Wildboar::monsterInit(Point position, FightScene* scene)
{
	HP = 15;
	speed = 250;
	ATK = 3;
	direction = 1;
	moveDirection = Vec2(0, 0);
	isMove = false;
	isAttck = false;
	moveDistance = 300;
	attackDis = 250;
	attackSpeed = 300;
	//第一次攻击拉大点攻击间隔
	attackTime = static_cast<float> (rand() % 100) / 10.0 + 2.0;
	//状态
	isAlive = true;
	isOnStage = false;
	isCanTouched = false;
	this->setVisible(false);

	this->scene = scene;
	this->hero = scene->hero;
	wave = Sprite::create("monster/boarWave1_1.png");
	this->addChild(wave, 4);
	wave->setAnchorPoint(Vec2(0.5, 0.3));
	wave->setVisible(false);
	sprite = Sprite::create("monster/Wildboar11.png");
	this->addChild(sprite, 3);
	this->setPosition(position);
	sprite->setPosition(0, 0);
	//把锚点设在下方中间
	this->setAnchorPoint(Vec2(0.5, 0));
	sprite->setAnchorPoint(Vec2(0.5, 0));
	//为“闪烁”做准备
	blinkSprite = Sprite::create("monster/Wildboar11.png");
	blinkSprite->setAnchorPoint(ccp(0, 0));
	blinkSprite->setVisible(false);
	sprite->addChild(blinkSprite, 5);
	weapon = NULL;
}

void Wildboar::setAnimation(int status)
{

	//判断帧数
	int numFrame;
	if (status % 10 == 1) numFrame = 1;
	else if (status % 10 == 2) numFrame = 2;
	else if (status % 10 == 3) numFrame = 1;
	else if (status % 10 == 4) numFrame = 1;
	else if (status % 10 == 5) numFrame = 1;
	else if (status % 10 == 6) numFrame = 1;

	sprite->stopActionByTag(SETANIMATION);
	//shake();
	auto* animate = createAnimate(status, numFrame);
	animate->setTag(SETANIMATION);
	sprite->runAction(animate);

}

//由对应的描述和帧数创建动画
Animate* Wildboar::createAnimate(int status, int numFrame)
{

	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("monster/Wildboar.plist", "monster/Wildboar.png");
	Vector <SpriteFrame*> frameArray;
	for (int i = 1;i <= numFrame;i++)
	{
		auto* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("Wildboar%d_%d.png", status, i)->getCString());
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
//野猪的攻击是先原地不动，然后冲刺
void Wildboar::attack(float dt)
{
	this->stopAllActions();
	unschedule(schedule_selector(Monster::chase));
	float x = hero->position.x - this->position().x;
	int status;
	if (x >=0) {
		status = RIGHT_ATTACK;
		direction = RIGHT;
	}
	else {
		status = LEFT_ATTACK;
		direction = LEFT;
	}
	auto* move = MoveBy::create(0.4, Vec2(0, 0));
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Wildboar::attackPart2, this));
	auto* sequence = Sequence::create(move, callFunc, NULL);
	this->runAction(sequence);
	sequence->setTag(ATTACK);
	setAnimation(status);

}

void Wildboar::attackPart2()
{
	this->stopActionByTag(ATTACK);

	//monster和hero的距离
	float distance = getHeroDistance();
	float x = (hero->position.x - this->position().x) * attackDis / distance ;
	float y = (hero->position.y - this->position().y)*attackDis / distance ;
	int status;
	if (x >= 0) {
		status = RIGHT_ATTACK_PART2;
		direction = RIGHT;
	}
	else {
		status = LEFT_ATTACK_PART2;
		direction = LEFT;
	}
	moveDirection = Vec2(x / attackDis, y / attackDis);

	//进行伤害判定(move是有伤害的）
	schedule(schedule_selector(Monster::hurting), 1 / 60);

	auto* move = MoveBy::create(0.75, Vec2(x, y));
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(0.75, Vec2(0, 0));
	auto* callFunc1 = CallFunc::create(CC_CALLBACK_0(Wildboar::shockWave, this));
	auto* callFunc2 = CallFunc::create(CC_CALLBACK_0(Wildboar::removeWave, this));
	auto* callFunc3 = CallFunc::create(CC_CALLBACK_0(Monster::monsterResume, this));
	auto* sequence = Sequence::create(callFunc1, asitmove, callFunc2, callFunc3, NULL);
	this->runAction(move);
	this->runAction(sequence);
	sequence->setTag(ATTACK_PART2);
	setAnimation(status);

}

void Wildboar::shockWave()
{
	wave->setVisible(true);
	auto* animation = Animation::create();
	for (int i = 1;i <= 2;i++)
	{
		char s[30] = { 0 };
		sprintf(s, "monster/boarWave%d_%d.png", direction,i);
		animation->addSpriteFrameWithFile(s);
	}
	animation->setLoops(3);
	animation->setDelayPerUnit(0.15f);
	auto* action = Animate::create(animation);
	wave->runAction(action);
}

void Wildboar::removeWave()
{
	wave->setVisible(false);
}

void Wildboar::killed()
{
	//停止行动
	/*unschedule(schedule_selector(Monster::chase));
	unschedule(schedule_selector(Monster::moveOrStop));
	unschedule(schedule_selector(Monster::attack));
	unschedule(schedule_selector(Monster::resetAttackTime));
	unschedule(schedule_selector(Monster::hurting));*/
	unscheduleAllSelectors();
	schedule(schedule_selector(Monster::moveOrStop), 1 / 60);
	sprite->stopActionByTag(SHAKE);
	this->stopAllActions();
	blinkSprite->setVisible(false);
	removeWave();
	//死亡后不可碰撞,退场
	isCanTouched = false;
	isOnStage = false;
	isAlive = false;
	//飞出的距离
	float throughtDistance = 200;
	float distance = getHeroDistance();
	//朝英雄反向飞出
	float x = -(hero->position.x - this->position().x) * throughtDistance / distance;
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
	/*move->setTag(DYING);*/
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(throughtDistance / 300, Vec2(0,0));
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::dead, this));
	auto* sequence = Sequence::create(asitmove, callFunc, NULL);
	this->runAction(move);
	this->runAction(sequence);
	setAnimation(status);
}