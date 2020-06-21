#include <cmath>
#include <algorithm>
#include <sstream>
#include <string>
#include "Hero/Hero.h"
#include "Hero/HRocker.h"
#include "Weapon/Bullet.h"
#include "Weapon/Weapon.h"
#include "Scene/FightScene.h"
#include <cstdlib>
#include <ctime>
#include "Scene/HomeScene.h"
#include "Scene/HomeScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Hero/Knight.h"
#include "Hero/Ranger.h"
#include "Hero/Sorcerer.h"
#include "Monster/SnowMan.h"
#include "Weapon/weapon.h"
#include "Hero/Hero.h"
#include "Scene/FightScene1.h"
#include "Scene/FightScene2.h"
#include "Scene/FightScene3.h"
#include "Scene/LoadingScene.h"
#include "Scene/FightScene4.h"
#include "Scene/FightScene5.h"
#include "Scene/MyTransitionScene.h"
//#define Pi 3.1415926;?
USING_NS_CC;

bool Hero::init()
{
	if (!Layer::init())//判断是否初始化成功
	{
		return false;
	}
	return true;
}

void Hero::moveBy(double dx, double dy)//移动到某位置
{
	//获取运动图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
	if (dx == 0 && dy == 0)
	{
		if (withScene)
		{
			//判断是否位于窗口中心
			float x = position.x + layer->getPositionX();//获取相对窗口坐标
			float y = position.y + layer->getPositionY();
			//log("%f %f", layer->getPositionX(), layer->getPositionY());
			if (fabs(x - 600) > 8 || fabs(y - 400) > 8)//浮点数相等的范围判断：避免抖动
			{
				float r = sqrt(pow(x - 600, 2) + pow(y - 400, 2));
				float _dx = x - 600;
				float _dy = y - 400;
				//场景相对屏幕动
				double x_ = layer->getPositionX();
				double y_ = layer->getPositionY();
				layer->setPosition(x_ - 8 * _dx / r, y_ - 8 * _dy / r);
			}
		}
	}
	else
	{
		if (canMove)
		{
			//人物相对场景动
			float x = this->getPositionX();
			float y = this->getPositionY();
			this->setPosition(x + dx, y + dy);
			if (withScene)
			{
				//场景相对屏幕动
				double x_ = layer->getPositionX();
				double y_ = layer->getPositionY();
				layer->setPosition(x_ - dx, y_ - dy);
			}
		}
		else
		{
			if (withScene)
			{
				float x = position.x + layer->getPositionX();//获取相对窗口坐标
				float y = position.y + layer->getPositionY();
				int dis = sqrt(pow(fabs(x - 600), 2) + pow(fabs(y - 400), 2));//获取到中心点的距离
				if(dis < 100)
				{
					//场景还是相对屏幕动，只是慢一些
					double x_ = layer->getPositionX();
					double y_ = layer->getPositionY();
					layer->setPosition(x_ - dx / 4, y_ - dy / 4);
				}
			}
		}
	}
	//修改坐标
	position.x = sprite->getPositionX() * scale + this->getPositionX();
	position.y = sprite->getPositionY() * scale + this->getPositionY();
}

void Hero::setAction(int status)
{
	if (status == 1 || status == 3)
	{
		//抖动代码：
		sprite->stopActionByTag(99);
		sprite->setAnchorPoint(Vec2(0.5, 0));
		auto scaleTo1 = ScaleTo::create(0.3, 0.99, 1.03);
		auto scaleTo2 = ScaleTo::create(0.3, 1.01, 0.98);
		auto action = RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, NULL)->clone());//抖动
		action->setTag(99);
		sprite->runAction(action);
		//走动动画：
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
	//获取运动图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
	auto map = static_cast<TMXTiledMap*>(layer->getChildByTag(3));
	if (map != nullptr)
	{
		float x = position.x + dx - map->getPositionX();
		float y = position.y + dy - map->getPositionY();
		//float x = position.x - map->getPositionX();
		//float y = position.y - map->getPositionY();
		float _scale = scene->scale;
		int mapX = static_cast<int>((x - 16 * _scale) / (32 * _scale));
		int mapY = static_cast<int>((scene->height - (y - 16 * _scale) / (32 * _scale)));
		//障碍物
		auto meta = map->getLayer("meta");
		int tileGid = meta->getTileGIDAt(Vec2(mapX, mapY));//获取唯一对应值
		//door
		auto doorClose = map->getLayer("doorClose");
		int tileGid3 = doorClose->getTileGIDAt(Vec2(mapX, mapY));
		log("%d %d", tileGid, tileGid3);
		if (tileGid == scene->gid.x || (tileGid3 == scene->gid.y && doorClose->isVisible()))
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

void Hero::myUpdate(float dt)//定时器
{
	//log("%f %f",position.x,position.y);
	dx = rocker->dx;
	dy = rocker->dy;
	if (pow(rocker->dx, 2) + pow(rocker->dy, 2) == 2)
	{
		dx = rocker->dx / sqrt(2);
	    dy = rocker->dy / sqrt(2);
	}
	dx = dx*velocity;
	dy = dy*velocity;//控制速度
	if (dx != 0 || dy != 0)
	{
		dx_ = dx, dy_ = dy;
	}
	//产生走动动画
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
	if (isCanMove(dx, dy) && !isCollision)//判断能否移动
	{
		canMove = true;
	}
	else
	{
		canMove = false;
	}
	moveBy(dx, dy);//移动
	//实现技能间隔
	if (!isAbility)
	{
		abilityCount++;
		if (abilityCount == abilityInterval)
		{
			isAbility = true;
			abilityCount = 0;
		}
	}
	//技能时间
	if (abilityCount_ != 0)
	{
		abilityCount_--;
		if (abilityCount_ == 0)
		{
			removeAbilityFunction();
		}
	}
	//实现信息更新
	if (isShow)
	{
		changeInformation();
	}
	//实现护盾恢复
	recover();
	//实现主动攻击
	if (canAttack)
	{
		if (attackCount == 0)
		{
			attackCount = attackTime;
		}
	}
	if (attackCount != 0)
	{
		if (attackCount == attackTime)
		{
			attack();
		}
		attackCount--;
	}
	//改变弧度制方向
	changeDirection();
	if (weaponNow != nullptr)
	{
		weaponNow->adjustDirection();
	}
	if (lifeNumberNow <= 0)
	{
		dead();
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
//显示人物信息
void Hero::showInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	if (!layer->isVisible())
	{
		layer->setVisible(true);
		isShow = true;
	}
}

void Hero::hideInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	if (layer->isVisible())
	{
		layer->setVisible(false);
		isShow = false;
	}
}

void Hero::initInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	//显示板
	auto board = Sprite::create("information/Information.png");
	board->setAnchorPoint(Vec2(0, 1));
	board->setPosition(Vec2(0, 800));
	layer->addChild(board);
	Sprite* background[3];
	//滑动条背景
	for (int i = 0; i < 3; i++)
	{
		int distance = i * 33;
		background[i] = Sprite::create("information/BG.png");
		background[i]->setAnchorPoint(Vec2(0, 0.5));
		background[i]->setPosition(Vec2(44, 780 - distance));
		layer->addChild(background[i]);
	}
	//设置血量条等
	ProgressTimer* progress[3];
	auto HP = Sprite::create("information/HP.png");
	progress[0] = ProgressTimer::create(HP);
	auto shield = Sprite::create("information/shield.png");
	progress[1] = ProgressTimer::create(shield);
	auto MP = Sprite::create("information/MP.png");
	progress[2] = ProgressTimer::create(MP);
	for (int i = 0; i < 3; i++)
	{
		float distance = i * 33.7;
		progress[i]->setType(ProgressTimer::Type::BAR);
		progress[i]->setMidpoint(Vec2(0, 0.5));//从右到左减少
		progress[i]->setBarChangeRate(Vec2(1, 0));//只改变水平方向
		progress[i]->setAnchorPoint(Vec2(0, 0.5));
		progress[i]->setPosition(Vec2(45, 780 - distance));
		layer->addChild(progress[i]);
		progress[i]->setTag(i);
		progress[i]->setPercentage(100);
	}
	//设置标签
	std::ostringstream msg[3];//标签内容
	msg[0] << lifeNumberNow << "/" << lifeNumberMax;
	msg[1] << shieldNumberNow << "/" << shieldNumberMax;
	msg[2] << powerNumberNow << "/" << powerNumberMax;
	Label* label[3];
	for (int i = 0; i < 3; i++)
	{
		float distance = i * 34;
		label[i] = Label::createWithTTF(msg[i].str().c_str(), "fonts/arial.ttf", 24);
		label[i]->setAnchorPoint(Vec2(0.5, 0.5));
		label[i]->setPosition(Vec2(128, 780 - distance));
		layer->addChild(label[i]);
		label[i]->setTag(i + 3);
	}
	//设置金币袋
	auto pocket = Sprite::create("information/pocket.png");
	pocket->setAnchorPoint(Vec2(0.5, 0.5));
	pocket->setPosition(Vec2(1040, 780));
	pocket->setScale(0.7);
	layer->addChild(pocket);
	std::ostringstream goldMsg;//标签内容
	goldMsg << goldCoin;
	auto coinLabel = Label::createWithTTF(goldMsg.str().c_str(), "fonts/arial.ttf", 24);
	coinLabel->setAnchorPoint(Vec2(0, 0.5));
	coinLabel->setPosition(Vec2(1060, 780));
	layer->addChild(coinLabel);
	coinLabel->setTag(6);
	layer->setVisible(false);
	isShow = false;
}

void Hero::changeInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	//改变进度条
	static_cast<ProgressTimer*>(layer->getChildByTag(0))->setPercentage((((float)lifeNumberNow) / lifeNumberMax) * 100);
	static_cast<ProgressTimer*>(layer->getChildByTag(1))->setPercentage((((float)shieldNumberNow) / shieldNumberMax) * 100);
	static_cast<ProgressTimer*>(layer->getChildByTag(2))->setPercentage((((float)powerNumberNow) / powerNumberMax) * 100);
	//标签内容
	std::ostringstream msg[4];
	msg[0] << lifeNumberNow << "/" << lifeNumberMax;
	msg[1] << shieldNumberNow << "/" << shieldNumberMax;
	msg[2] << powerNumberNow << "/" << powerNumberMax;
	msg[3] << goldCoin;
	//改变标签
	static_cast<Label*>(layer->getChildByTag(3))->setString(msg[0].str());
	static_cast<Label*>(layer->getChildByTag(4))->setString(msg[1].str());
	static_cast<Label*>(layer->getChildByTag(5))->setString(msg[2].str());
	static_cast<Label*>(layer->getChildByTag(6))->setString(msg[3].str());
}

void Hero::moveWithScene()
{
	withScene = true;
}

void Hero::moveWithoutScene()
{
	withScene = false;
}
//判断是否被袭击
void Hero::beAttacked(int damage)
{
	if (shieldNumberNow - damage >= 0)//护盾值大于等于破坏力
	{
		shieldNumberNow -= damage;
	}
	else
	{
		shieldNumberNow = 0;
		if (lifeNumberNow - (damage - shieldNumberNow) > 0)//生命力未降至0
		{
			lifeNumberNow -= (damage - shieldNumberNow);
		}
		else
		{
			lifeNumberNow = 0;
			dead();
		}
	}

}
//护盾恢复函数
void Hero::recover()
{
	if (shieldNumberNow < shieldNumberMax)
	{
		recoverCount++;
		if (recoverCount == loopTime)
		{
			recoverCount = 0;
			shieldNumberNow++;
		}
	}
}
//获取矩形框
Rect Hero::boundingBox()
{
	Rect rect= sprite->getBoundingBox();
    rect.setRect(position.x - rect.size.width / 2, position.y - rect.size.height / 2, rect.size.width, rect.size.height);
	return rect;
}
//死亡
void Hero::dead()
{
	//还没写
	auto scene= HomeScene::createScene();
	Director::getInstance()->replaceScene(scene);

}
//攻击函数
void Hero::attack()
{
	weaponNow->adjustDirection();
	if (powerNumberNow > weaponNow->cost)
	{
		powerNumberNow -= weaponNow->cost;
		weaponNow->attack();
	}
	
	
}
//改变弧度制方向
void Hero::changeDirection()
{
	//moveDirection
	
	if (dx == 0)
	{
		if (dy > 0)
		{
			moveDirection = M_PI /2;
		}
		else if (dy < 0)
		{
			moveDirection = -M_PI /2;
		}//注：dy=0时不用改变
	}
	else if (dx > 0)
	{
		moveDirection=atan(dy / dx);
	}
	else if (dx < 0)
	{
		if (dy > 0)
		{
			moveDirection = atan(dy / dx) + M_PI;
		}
		else
		{
			moveDirection = atan(dy / dx) - M_PI;
		}
		
	}
	//enemyDirection


}
//添加武器
void Hero::addWeapon(Weapon* _weapon)
{
	weapon.push_back(_weapon);
	if (weaponNow)
	{
		weaponNow->setVisible(false);
	}
	weaponNow = _weapon;
	weaponNow->setVisible(true);//添加下一个武器
}
//添加或切换武器
void Hero::changeWeapon(Weapon* _weapon)
{
	if (weaponNumberNow < weaponNumberMax)
	{
		addWeapon(_weapon);
		weaponNumberNow++;
	}
	else
	{
		int i = 0;
		for (; i < weapon.size(); i++)
		{
			if (weapon[i] == weaponNow)
				break;
		}
		weapon[i] = _weapon;
		weaponNow->removeFromParent();
		weaponNow = weapon[i];
		weaponNow->setVisible(true);
	}
}
//切换已拥有的武器
void Hero::changeWeaponNow()
{
	if (weaponNumberNow > 1)
	{
		int i = 0;
		for (; i < weapon.size(); i++)
		{
			if (weapon[i] == weaponNow)
				break;
		}
		if (i < weapon.size() - 1)
		{
			weaponNow->setVisible(false);
			weaponNow = weapon[i + 1];
			weaponNow->setVisible(true);
			attackTime = weaponNow->speed / betweenTime;
		}
		else
		{
			weaponNow->setVisible(false);
			weaponNow = weapon[0];
			weaponNow->setVisible(true);
			attackTime = weaponNow->speed / betweenTime;
		}
	}
}
//下一个场景英雄
void Hero::nextSceneHero(Hero* hero)
{
	lifeNumberNow = hero->lifeNumberNow;
	lifeNumberMax = hero->lifeNumberMax;
	powerNumberNow = hero->powerNumberNow;
	powerNumberMax = hero->powerNumberMax;
	shieldNumberNow = hero->shieldNumberNow;
	shieldNumberMax = hero->shieldNumberMax;
	velocity = hero->velocity;
	// 避免一开始就运动
	rocker->dx = hero->rocker->dx;
	rocker->dy = hero->rocker->dy;
	k = hero->k;
}
//下一个场景
void Hero::nextScene(FightScene* scene)
{
	this->scene = scene;
}
//其它功能函数
void Hero::otherFunction(specialSprite* _spec)
{
	srand((unsigned)time(0));
	if (_spec->type == 0)
	{
		_spec->sprite->setVisible(false);
		auto sprite = Sprite::create("Map/boxOpen.png");
		sprite->setPosition(_spec->sprite->getPosition());
		scene->dynamicLayer->addChild(sprite);
		int num = rand() % 6;
		//int num = 5;
		int type;
		if (num <= 0)
		{
			specialSprite* newSpec = new specialSprite;//记得清理
			newSpec->sprite = Sprite::create("Map/HPpotion.png");
			newSpec->sprite->setPosition(_spec->sprite->getPosition());
			scene->dynamicLayer->addChild(newSpec->sprite, 1);
			scene->special.push_back(newSpec);
			newSpec->type = 1;
		}
		else if (num >= 1 && num <= 2)
		{
			specialSprite* newSpec = new specialSprite;
			newSpec->sprite = Sprite::create("Map/MPpotion.png");
			newSpec->sprite->setPosition(_spec->sprite->getPosition());
			scene->dynamicLayer->addChild(newSpec->sprite, 1);
			scene->special.push_back(newSpec);
			newSpec->type = 2;
		}
		else if (num >= 3 && num <= 4)
		{
			specialSprite* newSpec = new specialSprite;
			int num = rand() % 13 + 1;
			std::string str = randWeapon(num);
			newSpec->sprite = Sprite::create("weapon/" + str + "_right.png");//
			newSpec->sprite->setScale(0.7);
			newSpec->sprite->setPosition(_spec->sprite->getPosition());
			scene->dynamicLayer->addChild(newSpec->sprite, 1);
			scene->special.push_back(newSpec);
			newSpec->type = 3;
			newSpec->name = str;
		}
		else if (num == 5)
		{
			specialSprite* newSpec = new specialSprite;
			newSpec->sprite = Sprite::create("Map/goldPocket.png");
			newSpec->sprite->setPosition(_spec->sprite->getPosition());
			scene->dynamicLayer->addChild(newSpec->sprite, 1);
			scene->special.push_back(newSpec);
			newSpec->type = 4;
		}
		scene->special.remove(_spec);
		_spec->sprite->removeFromParent();
		delete _spec;//
	}
	else if (_spec->type == 1)
	{
		int effect = rand() % 2 + 2;
		lifeNumberNow = lifeNumberNow + effect >lifeNumberMax ? lifeNumberMax : lifeNumberNow + effect;
		scene->special.remove(_spec);
		_spec->sprite->removeFromParent();
		delete _spec;//
	}
	else if (_spec->type == 2)
	{
		int effect = (rand() % 3 + 5) * 10;
		powerNumberNow = powerNumberNow + effect > powerNumberMax ?powerNumberMax :powerNumberNow + effect;
		scene->special.remove(_spec);
		_spec->sprite->removeFromParent();
		delete _spec;//
	}
	else if (_spec->type == 3)
	{
		//
		auto weapon = Weapon::createWeapon(scene,this,_spec->name.c_str());
		//
		specialSprite* newSpec = new specialSprite;
		newSpec->sprite = Sprite::create("weapon/" + this->weaponNow->name + "_right.png");//
		newSpec->sprite->setPosition(_spec->sprite->getPosition());
		newSpec->sprite->setScale(0.7);
		scene->dynamicLayer->addChild(newSpec->sprite, 1);
		newSpec->type = 3;
		newSpec->name = this->weaponNow->name;
		scene->special.push_back(newSpec);
		changeWeapon(weapon);
		//
		scene->special.remove(_spec);
		_spec->sprite->removeFromParent();//
		delete _spec;
	}
	else if (_spec->type == 4)
	{
		int effect = (rand() % 3 + 5) * 5;
		goldCoin += effect;
		scene->special.remove(_spec);
		_spec->sprite->removeFromParent();
		delete _spec;//
	}
	else if (_spec->type == 5)
	{
		FightScene* scene;
		if (_num.size() == 3)
		{
			scene->special.remove(_spec);
			_spec->sprite->removeFromParent();
			delete _spec;//
			dead();
		}
		int m = 1;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < _num.size(); j++)
			{
				if (i = _num[j])
				{
					m = 0;
				}
			}
			if (m == 1)
			{
				int num = i;
				FightScene* nextScene;
				if (num == 0)
				{
					nextScene = FightScene1::createFightScene(this);
				}
				else if (num == 1)
				{
					nextScene = FightScene2::createFightScene(this);
				}
				else if (num == 2)
				{
					nextScene = FightScene3::createFightScene(this);
				}
				else if (num == 3)
				{
					nextScene = FightScene4::createFightScene(this);
				}
				else if (num == 4)
				{
					nextScene = FightScene5::createFightScene(this);
				}
				scene->special.remove(_spec);
				_spec->sprite->removeFromParent();
				delete _spec;
				Director::getInstance()->replaceScene(nextScene);//切换下一个场景
			}

		}
		
	}
}
//随机武器
std::string Hero::randWeapon(int num)
{
	string str;
	if (num == 1)
	{
		str = "gun";
	}
	else if (num == 2)
	{
		str = "fireGun";
	}
	else if (num == 3)
	{
		str = "ejectionGun";
	}
	else if (num == 4)
	{
		str = "iceGun";
	}
	else if (num == 5)
	{
		str = "aurora";
	}
	else if (num == 6)
	{
		str = "phantom";
	}
	else if (num == 7)
	{
		str = "poisonRifle";
	}
	else if (num == 8)
	{
		str = "rifle";
	}
	else if (num == 9)
	{
		str = "broadsword";
	}
	else if (num == 10)
	{
		str = "lightsaberBlue";
	}
	else if (num == 11)
	{
		str = "lightsaberRed";
	}
	else if (num == 12)
	{
		str = "lightsaberGreen";
	}
	else if (num == 13)
	{
		str = "machetes";
	}
	return str;
}
//基本数据初始化
void Hero::commonDataInit()
{
	dx = 0, dy = 0;
	dx_ = 0, dy_ = 0;
	faceDirection = 1;
	isRun = 0;
	isAbility = true;
	canMove = true;
	withScene = false;
	abilityCount = 0;
	abilityCount_ = 0;
	recoverCount = 0;
	betweenTime = 0.02;//设置为0.02s
	abilityInterval = abilityBetweenTime / betweenTime;
	loopTime = recoverTime / betweenTime;
	moveDirection = 0;
	enemyDirection = -1;
	canAttack = false;
	attackCount = 0;
	isCollision = false;
	k = 1;

	weaponNumberMax = 2;
	weaponNumberNow = 0;
	
}

