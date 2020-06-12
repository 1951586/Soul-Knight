#include<cmath>
#include<algorithm>
#include "Hero.h"
#include"HRocker.h"
USING_NS_CC;

bool Hero::init()
{
	if (!Layer::init())//�ж��Ƿ��ʼ���ɹ�
	{
		return false;
	}
	return true;
}

void Hero::moveTo(double dx, double dy)//�ƶ���ĳλ��
{
	double x = this->getPositionX();
	double y = this->getPositionY();
	this->setPosition(x + dx, y + dy);
}

void Hero::setAction(int status)
{
	if (status == 1 || status == 3)
	{
		//�������룺
		sprite->stopActionByTag(99);
		sprite->setAnchorPoint(Vec2(0.5, 0));
		auto scaleTo1 = ScaleTo::create(0.3, 0.99, 1.03);
		auto scaleTo2 = ScaleTo::create(0.3, 1.01, 0.98);
		auto action = RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, NULL)->clone());//����
		action->setTag(99);
		sprite->runAction(action);
		//�߶�������
		sprite->stopActionByTag(100);
		auto* animate = createAnimate(status);
		animate->setTag(100);
		sprite->runAction(animate);
	}
	else
	{
		sprite->stopActionByTag(99);
		sprite->stopActionByTag(100);
		auto* animate = createAnimate(status);
		animate->setTag(100);
		sprite->runAction(animate);
	}
}

bool Hero::isCanMove(int dx,int dy)
{
	//�жϴ��룺

	return true;
}

void Hero::myUpdate(float dt)//��ʱ��
{
	//log("Update:   .");
	dx = rocker->dx;
	dy = rocker->dy;
	if (pow(rocker->dx, 2) + pow(rocker->dy, 2) == 2)
	{
		dx = rocker->dx / sqrt(2);
	    dy = rocker->dy / sqrt(2);
	}
	dx = dx*velocity;
	dy = dy*velocity;//�����ٶ�
	if (dx != 0 || dy != 0)
	{
		dx_ = dx, dy_ = dy;
	}
	if (isCanMove(dx, dy))//�ж��ܷ��ƶ�
	{
		if (dx == 0 && dy == 0 && isRun != 0)
		{
			if (faceDirection == 1)
				setAction(1);
			else
				setAction(3);
			isRun = 0;
		}
		else if (dx > 0 && isRun != 1)
		{
			setAction(2);
			isRun = 1;
			faceDirection = 1;
		}
		else if (dx < 0 && isRun != -1)
		{
			setAction(4);
			isRun = -1;
			faceDirection = -1;
		}
		else if (dx == 0 && dy != 0 && isRun != faceDirection)
		{
			if (faceDirection == 1)
				setAction(2);
			else
				setAction(4);
			isRun = faceDirection;
		}
		moveTo(dx, dy);//�ƶ�
		//ʵ�ּ��ܼ��
		if (!isAbility)
		{
			count++;
			if (count == abilityInterval)
			{
				isAbility = true;
				count = 0;
			}
		}
	}
}

void Hero::startKeyRocker()
{
	rocker->startKeyRocker();
}

void Hero::stopKeyRocker()
{
	rocker->stopKeyRocker();
}

void Hero::startTouchRocker()
{
	rocker->startTouchRocker();
}

void Hero::stopTouchRocker()
{
	rocker->stopTouchRocker();
}

