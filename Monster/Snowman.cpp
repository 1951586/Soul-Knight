#include "Snowman.h"
#include "Scene/FightScene.h"
#include "Hero/Hero.h"
#include "Monster/Monster.h"

USING_NS_CC;


Snowman* Snowman::createMonsterSprite(Point position, FightScene* scene)
{

	Snowman* snowman = new Snowman();
	if (snowman && snowman->init())
	{
		snowman->autorelease();//�Զ������ڴ�
		snowman->monsterInit(position,scene);		
		return snowman;
	}
	CC_SAFE_DELETE(snowman);
	return NULL;

}

void Snowman::monsterInit(Point position, FightScene* scene)
{
	HP = 18;
	speed = 250;
	ATK = 3;
	direction = 1;
	moveDirection = Vec2(0, 0);
	isMove = false;
	isAttck = false;
	moveDistance = 300;
	attackDis = 200;
	attackSpeed = 300;
	//��һ�ι�������㹥�����
	attackTime = static_cast<float> (rand() % 100) / 10.0+2.0;
	//״̬
	isAlive = true;
	isOnStage = false;
	isCanTouched = false;
	this->scene = scene;
	this->hero = scene->hero;
	this->setVisible(false);
	wave = Sprite::create("monster/shockWave1.png");
	this->addChild(wave, 1);
	wave->setAnchorPoint(Vec2(0.5, 0.3));
	wave->setVisible(false);
	sprite = Sprite::create("monster/Snowman11.png");
	this->addChild(sprite,3);
	this->setPosition(position);
	sprite->setPosition(0, 0);
	//��ê�������·��м�
	this->setAnchorPoint(Vec2(0.5, 0));
	sprite->setAnchorPoint(Vec2(0.5, 0));
	//Ϊ����˸����׼��
	blinkSprite = Sprite::create("monster/Snowman11.png");
	blinkSprite->setAnchorPoint(ccp(0, 0));
	blinkSprite->setVisible(false);
	sprite->addChild(blinkSprite,5);
	weapon = NULL;
}

void Snowman::setAnimation(int status)
{

	//�ж�֡��
	int numFrame;
	if (status % 10 == 1) numFrame = 1;
	else if (status % 10 == 2) numFrame = 2;
	else if (status % 10 == 3) numFrame = 4;
	else if (status % 10 == 4) numFrame = 1;
	else if (status % 10 == 5) numFrame = 1;
	else if (status % 10 == 6) numFrame = 5;

	sprite->stopActionByTag(SETANIMATION);
	auto* animate = createAnimate(status, numFrame);
	animate->setTag(SETANIMATION);
	sprite->runAction(animate);

}

//�ɶ�Ӧ��������֡����������
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
//ѩ�˵Ĺ���������ǰһ���־��룬Ȼ����й����ж�
void Snowman::attack(float dt)
{
	this->stopAllActions();
	unschedule(schedule_selector(Monster::chase));

	//monster��hero�ľ���
	float distance = getHeroDistance();
	float x = (hero->position.x - this->position().x) * attackDis / distance/2;
	float y = (hero->position.y - this->position().y)*attackDis / distance/2;

	int status;
	if (x > 0) {
		status = RIGHT_ATTACK;
		direction = RIGHT;
	}
	else {
		status = LEFT_ATTACK;
		direction = LEFT;
	}
	moveDirection = Vec2(x / attackDis*2, y / attackDis*2);
	auto* move = MoveBy::create(0.6, Vec2(x, y));
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(0.6, Vec2(0,0));
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Snowman::attackPart2, this));
	auto* sequence = Sequence::create(asitmove, callFunc, NULL);
	this->runAction(move);
	this->runAction(sequence);
	sequence->setTag(ATTACK);
	setAnimation(status);

}

void Snowman::attackPart2()
{
	this->stopActionByTag(ATTACK);

	//monster��hero�ľ���

	float x = moveDirection.x*attackDis/2;
	float y = moveDirection.y*attackDis / 2;

	int status;
	if (x > 0) {
		status = RIGHT_ATTACK_PART2;
		direction = RIGHT;
	}
	else {
		status = LEFT_ATTACK_PART2;
		direction = LEFT;
	}
	//�����˺��ж�(move��move2�׶������˺��ģ�
	schedule(schedule_selector(Monster::hurting), 1 / 60);

	auto* move = MoveBy::create(0.3, Vec2(x, y));
	move->setTag(MOVE);
	//�������ų��������Ķ���
	auto* asitmove = MoveBy::create(0.3, Vec2(0, 0));
	auto* move2= MoveBy::create(0.5, Vec2(0,0));
	auto* callFunc1= CallFunc::create(CC_CALLBACK_0(Snowman::shockWave, this));
	auto* callFunc2 = CallFunc::create(CC_CALLBACK_0(Snowman::removeWave, this));
	auto* callFunc3 = CallFunc::create(CC_CALLBACK_0(Monster::monsterResume, this));
	auto* sequence = Sequence::create(asitmove, callFunc1, move2, callFunc2, callFunc3, NULL);
	this->runAction(move);
	this->runAction(sequence);
	sequence->setTag(ATTACK_PART2);
	setAnimation(status);

}

void Snowman::shockWave()
{
	wave->setVisible(true);
	auto* animation = Animation::create();
	for (int i = 1;i <= 3;i++)
	{
		char s[30] = { 0 };
		sprintf(s, "monster/snowmanShockWave%d.png", i);
		animation->addSpriteFrameWithFile(s);
	}
	animation->setLoops(1);
	animation->setDelayPerUnit(0.15f);
	auto* action = Animate::create(animation);
	wave->runAction(action);
}

void Snowman::removeWave()
{
	wave->setVisible(false);
}

void Snowman::killed()
{
	unscheduleAllSelectors();
	schedule(schedule_selector(Monster::moveOrStop), 1.0 / 60.0);
	sprite->stopActionByTag(SHAKE);
	this->stopAllActions();
	blinkSprite->setVisible(false);
	removeWave();

	//�����󲻿���ײ,�˳�
	isCanTouched = false;
	isOnStage = false;
	isAlive = false;
	//�ɳ��ľ���
	float throughtDistance = 200;
	float distance = getHeroDistance();
	//��Ӣ�۷���ɳ�
	float x = -(hero->position.x - this->position().x)*throughtDistance / distance;
	float y = -(hero->position.y - this->position().y)*throughtDistance / distance;

	int status;
	if (x < 0) {
		status = RIGHT_DYING;
		direction = RIGHT;
	}
	else {
		status = LEFT_DYING;
		direction = LEFT;
	}
	moveDirection = Vec2(x / throughtDistance, y / throughtDistance);
	//����
	auto* move = MoveBy::create(throughtDistance / 300, Vec2(x, y));
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(throughtDistance / 300, Vec2(0, 0));
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::dead, this));
	auto* sequence = Sequence::create(asitmove, callFunc, NULL);
	this->runAction(move);
	this->runAction(sequence);

	//����ʱ��ת
	Action* rotate;
	if (direction == RIGHT)
		rotate = RotateTo::create(throughtDistance / 300 / 2, -90);
	else
		rotate = RotateBy::create(throughtDistance / 300 / 2, 90);
	sprite->runAction(rotate);
	setAnimation(status);


}