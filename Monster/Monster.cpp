#include<cmath>
#include "cocos2d.h"
#include "Monster.h"
#include "Hero/Hero.h"
#include "Scene/FightScene.h"
#include "buff/Buff.h"

USING_NS_CC;

bool Monster::init()
{
	if (!Sprite::init())//判断是否初始化成功
	{
		return false;
	}
	return true;
}
bool Monster::isCanMove()
{
	//获取运动图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
	auto map = static_cast<TMXTiledMap*>(layer->getChildByTag(3));
	if (map != nullptr)
	{
		float x = position().x + moveDirection.x * 30 - map->getPositionX();
		float y = position().y + moveDirection.y * 30 - map->getPositionY();	
		float _scale = scene->scale;
		int mapX = static_cast<int>((x - 16 * _scale) / (32 * _scale));
		int mapY = static_cast<int>((74 - (y - 16 * _scale) / (32 * _scale)));
		//障碍物
		auto meta = map->getLayer("meta");
		int tileGid = meta->getTileGIDAt(Vec2(mapX, mapY));//获取唯一对应值
		//door
		auto doorClose = map->getLayer("doorClose");
		int tileGid3 = doorClose->getTileGIDAt(Vec2(mapX, mapY));
		//log("%d",tileGid3);
		if (tileGid == 14 || (tileGid3 == 24 && doorClose->isVisible()))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
void Monster::moveOrStop(float dt)
{
	if (!isCanMove())
	{
		this->stopActionByTag(MOVE);
		log("stop");
	}
}

void Monster::chase(float dt)
{
	

	//可能朝主角走去，可能乱走，可能发呆
	//距离远时，更多的靠近
	if (getHeroDistance() > moveDistance + 200)
	{
		int random = rand() % 8;
		if (0 <= random && random < 3) directMove();
		else if (3 <= random && random < 6)	randomMove();
		else monsterResume();
		
	}
	//近时，更多随机走动
	else
	{
		int random = rand() % 8;
		if (0 <= random && random < 2) directMove();
		else if (2 <= random && random < 6)	randomMove();
		else monsterResume();
	}
	
}

void Monster::getHeroInstance(Hero* hero)
{
	this->hero = hero;
}


void Monster::directMove()
{

	this->stopActionByTag(MOVE);
	this->stopActionByTag(ATTACK);

	//monster和hero的距离
	float distance = getHeroDistance();
	float x = (hero->position.x - this->position().x) * moveDistance / distance;
	float y = (hero->position.y - this->position().y)*moveDistance / distance;

	int status;
	if (x >= 0) {
		status = RIGHT_MOVE;
		direction = RIGHT;
	}
	else {
		status = LEFT_MOVE;
		direction = LEFT;
	}
	moveDirection = Vec2(x / moveDistance, y / moveDistance);
	auto* move = MoveBy::create(moveDistance/speed, Vec2(x, y));
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(moveDistance / speed, Vec2(0,0));
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::monsterResume, this));
	auto* sequence = Sequence::create(asitmove, callFunc, NULL);
	this->runAction(move);
	this->runAction(sequence);
	/*this->runAction(move);*/
	//sequence->setTag(MOVE);
	setAnimation(status);


}

void Monster::randomMove()
{
	this->stopActionByTag(MOVE);
	this->stopActionByTag(ATTACK);
	float angle =static_cast<float> (rand() % 628) / 100.0;	
	float x = moveDistance*cos(angle);
	float y = moveDistance*sin(angle);

	int status;
	if (x >= 0) {
		status = RIGHT_MOVE;
		direction = RIGHT;
	}
	else {
		status = LEFT_MOVE;
		direction = LEFT;
	}
	moveDirection = Vec2(x / moveDistance, y / moveDistance);
	auto* move = MoveBy::create(moveDistance / speed, Vec2(x, y));
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(moveDistance / speed, Vec2(0, 0));
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(Monster::monsterResume, this));
	auto* sequence = Sequence::create(asitmove, callFunc, NULL);
	this->runAction(move);
	this->runAction(sequence);
	
	/*sequence->setTag(MOVE);*/
	setAnimation(status);


}


float Monster::getHeroDistance()
{
	float x = hero->position.x - this->position().x;
	float y = hero->position.y - this->position().y;
	float r = sqrt(x*x + y * y);
	return r;
}
void Monster::monsterResume() 
{
	this->stopActionByTag(MOVE);
	this->stopActionByTag(ATTACK);
	this->stopActionByTag(ATTACK_PART2);
	schedule(schedule_selector(Monster::chase), moveDistance / speed);
	unschedule(schedule_selector(Monster::hurting));
	moveDirection = Vec2(0,0);
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

void Monster::beAttacked(int hurt)
{
	HP = HP - hurt;
	blink();
	if (HP <= 0) killed();
}

void Monster::blink()
{
	blinkSprite->setDisplayFrame(sprite->displayFrame());
    ccBlendFunc cbl = { GL_DST_COLOR, GL_ONE };
	blinkSprite->setBlendFunc(cbl);
	blinkSprite->setColor(Color3B::WHITE);
	blinkSprite->setVisible(true);
	scheduleOnce(schedule_selector(Monster::blinkPart2), 0.2);
}

void Monster::blinkPart2(float dt)
{
	blinkSprite->setVisible(false);
}

void Monster::killed()
{
	unscheduleAllSelectors();
	schedule(schedule_selector(Monster::moveOrStop), 1.0 / 60.0);
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
	//击飞
	auto* move = MoveBy::create(throughtDistance /300, Vec2(x, y));
	move->setTag(MOVE);
	auto* asitmove = MoveBy::create(throughtDistance / 300, Vec2(0,0));
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

void Monster::dead()
{
	moveDirection = Vec2(0,0);
	if (direction == RIGHT)
		setAnimation(RIGHT_DEAD);
	else
		setAnimation(LEFT_DEAD);
	//死后随机给英雄加一部分能量
	int addpower = rand() % 20 + 5;
	if (hero->powerNumberNow + addpower >= hero->powerNumberMax)
		hero->powerNumberNow = hero->powerNumberMax;
	else hero->powerNumberNow += addpower;
}

void Monster::comeOnStage()
{
	isOnStage = true;
	isCanTouched = true;
	this->setVisible(true);
	shake();
	schedule(schedule_selector(Monster::chase), moveDistance / speed);
	schedule(schedule_selector(Monster::moveOrStop), 1.0/60.0);
	scheduleOnce(schedule_selector(Monster::attack),attackTime);
	scheduleOnce(schedule_selector(Monster::resetAttackTime), attackTime + 1);
}

void Monster::resetAttackTime(float dt)
{
	unschedule(schedule_selector(Monster::attack));
	attackTime = static_cast<float> (rand() % 40) / 10.0 + 6.5;
	schedule(schedule_selector(Monster::attack),attackTime);
}


Point Monster::position()
{
	float tmpX = this->getPosition().x;
	float tmpY = this->getPosition().y + this->sprite->getBoundingBox().getMidY();	
	return Point(tmpX,tmpY);

}
//伤害判定
void Monster::hurting(float dt)
{
	//距离足够小则造成伤害，造成伤害后，不再判定伤害
	if (getHeroDistance() < this->sprite->getBoundingBox().getMaxX() + hero->sprite->getBoundingBox().getMaxX()+20)
	{
		log("dis:%f,%f", getHeroDistance(), this->sprite->getBoundingBox().getMaxX() + hero->sprite->getBoundingBox().getMaxX() + 20);
		if (hero->shieldNumberNow >=ATK) hero->shieldNumberNow -= ATK;
		else {
			hero->lifeNumberNow = hero->lifeNumberNow - (this->ATK - hero->shieldNumberNow);
			hero->shieldNumberNow = 0;
		}
		unschedule(schedule_selector(Monster::hurting));
	}
}

//void Monster::getBuff(int type)
//{
//	auto buff=Buff::createBuff(type);
//	buff->addBuff(this);
//}


Sprite* Monster::getSprite()
{
	return this->sprite;
}

//重写boundingBox()
Rect Monster::boundingBox()
{
	Rect rect = sprite->getBoundingBox();
	rect.setRect(position().x - rect.size.width / 2, position().y - rect.size.height / 2, rect.size.width, rect.size.height);
	return rect;
}

int Monster::getDirection()
{
	return direction;
}

Vec2 Monster::getMoveDirection()
{
	return moveDirection;
}