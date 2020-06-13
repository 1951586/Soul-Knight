#include<cmath>
#include "cocos2d.h"
#include "Monster.h"

USING_NS_CC;

bool Monster::init()
{
	if (!Sprite::init())//判断是否初始化成功
	{
		return false;
	}
	return true;
}
bool Monster::isCanMove(int x, int y)
{
	//未完成
	//int tileGid = meta->tileGIDAt(Vec2(x, y));
	//if (tileGid == )
	//	return false;
	//else return true;

	return true;
}


void Monster::setAnimation(int status)
{

	//判断帧数
	int numFrame;
	if (status % 10 == 1) numFrame = 1;
	else if (status % 10 == 2) numFrame = 2;
	else if (status % 10 == 3) numFrame = 3;//攻击的帧数还未定
	else if (status % 10 == 4) numFrame = 1;
	else if (status % 10 == 5) numFrame = 1;

	sprite->stopActionByTag(SETANIMATION);
	//shake();
	auto* animate = createAnimate(status, numFrame);
	animate->setTag(SETANIMATION);
	sprite->runAction(animate);

}

void Monster::chase(float dt)
{

	//可能朝主角走去，可能乱走，可能发呆
	int random = rand()%9;
	if(0<=random&&random<3) directMove();
	else if ( 3<= random && random < 6)	randomMove();
	else monsterResume();

}

void Monster::getHeroInstance(Hero* hero)
{
	this->hero = hero;
}


void Monster::directMove()
{

	this->stopActionByTag(MOVE);

	//monster和hero的距离
	float distance = getDistance();
	float x = (hero->position.x - this->getPosition().x) * moveDistance / distance;
	float y = (hero->position.y - this->getPosition().y)*moveDistance / distance;

	int status;
	if (x > 0) {
		status = RIGHT_MOVE;
		direction = RIGHT;
	}
	else {
		status = LEFT_MOVE;
		direction = LEFT;
	}

	auto* move = MoveBy::create(moveDistance/speed, Vec2(x, y));
	/*move->setTag(MOVE);*/
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::monsterResume, this));
	auto* sequence = Sequence::create(move, callFunc, NULL);
	this->runAction(sequence);
	sequence->setTag(MOVE);
	setAnimation(status);


}

void Monster::randomMove()
{
	this->stopActionByTag(MOVE);
	float angle =static_cast<float> (rand() % 628) / 100.0;	
	float x = moveDistance*cos(angle);
	float y = moveDistance*sin(angle);

	int status;
	if (x > 0) {
		status = RIGHT_MOVE;
		direction = RIGHT;
	}
	else {
		status = LEFT_MOVE;
		direction = LEFT;
	}

	auto* move = MoveBy::create(moveDistance / speed, Vec2(x, y));
	//move->setTag(MOVE);
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::monsterResume, this));
	auto* sequence = Sequence::create(move, callFunc, NULL);
	this->runAction(sequence);
	sequence->setTag(MOVE);
	setAnimation(status);


}


float Monster::getDistance()
{
	float x = hero->position.x - this->getPosition().x;
	float y = hero->position.y - this->getPosition().y;
	float r = sqrt(x*x + y * y);
	return r;
}
void Monster::monsterResume() 
{
	this->stopActionByTag(MOVE);

	if (direction == RIGHT) setAnimation(RIGHT_STAND);
	else setAnimation(LEFT_STAND);
}

void Monster::shake()
{

	auto scaleTo1 = ScaleTo::create(0.3, 0.99, 1.07);
	auto scaleTo2 = ScaleTo::create(0.3, 1.01, 0.934);
	auto action = RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, NULL)->clone());//抖动
	action->setTag(SHAKE);
	sprite->runAction(action);
}

void Monster::beAttcked(int hurt)
{
	HP = HP - hurt;
	blink();
	if (HP == 0) killed();
}

void Monster::blink()
{

	blinkSprite = Sprite::createWithSpriteFrame(sprite->displayFrame());
	blinkSprite->setAnchorPoint(ccp(0, 0));
	blinkSprite->setVisible(true);
	ccBlendFunc cbl = { GL_DST_COLOR, GL_ONE };
	blinkSprite->setBlendFunc(cbl);
	blinkSprite->setColor(Color3B::WHITE);
	schedule(schedule_selector(Monster::blinkPart2), 0.1);
	sprite->addChild(blinkSprite);

}

void Monster::blinkPart2(float dt)
{
	sprite->removeChild(blinkSprite,this);
	unschedule(schedule_selector(Monster::blinkPart2));

}

void Monster::killed()
{

	this->stopActionByTag(MOVE);
	this->stopActionByTag(SHAKE);
	unschedule(schedule_selector(Monster::chase));
	//飞出的距离
	float throughtDistance = 200;
	float distance = getDistance();
	//朝英雄反向飞出
	float x = -(hero->position.x - this->getPosition().x) * throughtDistance / distance;
	float y = -(hero->position.y - this->getPosition().y)*throughtDistance / distance;

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
	auto* move = MoveBy::create(throughtDistance /300, Vec2(x, y));
	move->setTag(DYING);
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::dead, this));
	auto* sequence = Sequence::create(move, callFunc, NULL);
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

void Monster::dead()
{
	if (direction == RIGHT)
		setAnimation(RIGHT_DEAD);
	else
		setAnimation(LEFT_DEAD);
	//死亡后不可碰撞
	isCanTouched = 0;
	////5秒后尸体消失
	//schedule(schedule_selector(Monster::deadPart2), 1);
}

//void Monster::deadPart2(float dt)
//{
//	unschedule(schedule_selector(Monster::blinkPart2));
//	//?
//	delete sprite;
//	delete this;
//}