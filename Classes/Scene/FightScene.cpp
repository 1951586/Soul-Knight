#include "FightScene.h"
#include "Hero/Hero.h"
#include "Hero/Knight.h"
#include "Hero/Ranger.h"
#include "Hero/Sorcerer.h"
#include "Weapon/Weapon.h"
#include "Weapon/Bullet.h"

bool FightScene::init()
{
    if (!Scene::init())//判断是否初始化成功
    {
        return false;
    }
    return true;
}

void FightScene::addLayer()
{
    //初始化图层
    dynamicLayer = Layer::create();
    staticLayer = Layer::create();
    //添加图层
    this->addChild(dynamicLayer);
    dynamicLayer->setTag(-3);
    this->addChild(staticLayer);
    staticLayer->setTag(-2);
}

void FightScene::addHero(Hero* _hero)
{
    //添加英雄
    if (_hero->name == "Knight")
    {
        hero = Knight::createHeroSprite(Point(600, 400), this);
    }
    else if (_hero->name == "Ranger")
    {
        hero = Ranger::createHeroSprite(Point(600, 400), this);
    }
    else if (_hero->name == "Sorcerer")
    {
        hero = Sorcerer::createHeroSprite(Point(600, 400), this);
    }
    for (Weapon* wea : _hero->weapon)//遍历武器
    {
        auto weapon = Weapon::createWeapon(this, hero, wea->name.c_str());//true表示远程
        hero->changeWeapon(weapon);
    }
    for (Weapon* wea : hero->weapon)//遍历武器
    {
       if(wea->name == _hero->weaponNow->name)
       {
           hero->weaponNow->setVisible(false);
           hero->weaponNow = wea;
           hero->weaponNow->setVisible(true);
       }
    }

    hero->nextSceneHero(_hero);
    hero->startKeyRocker();
    hero->moveWithScene();
    hero->showInformation();
    hero->nextScene(this);//英雄切换场景
    hero->setPosition(Vec2(600, 400));
    dynamicLayer->addChild(hero, 5);
    hero->_num.push_back(num);
   
}
//增加宝箱
void FightScene::addBox(Vec2 vec)
{
    specialSprite* spec = new specialSprite;
    spec->sprite = Sprite::create("Map/boxClose.png");
    spec->sprite->setPosition(vec);
    dynamicLayer->addChild(spec->sprite,1);
    spec->type = 0;
    special.push_back(spec);
}
//添加其中一波怪物
void FightScene::addMonster(std::list<Monster*> _monster)
{
    for (Monster* mons : _monster)
    {
        mons->comeOnStage();
    }
}
//接触检验（子弹与英雄）
void FightScene::heroAndBullet()
{
    Bullet* bullet = nullptr;
    for (Bullet* bu : monsterBullet)
    {
        if (hero->boundingBox().containsPoint(Vec2(bu->position().x, bu->position().y)))
        {
            hero->beAttacked(bu->damagePower);
            bullet = bu;
        }
    }
    if (bullet != nullptr)
    {
        monsterBullet.remove(bullet);//从容器中移除子弹
        bullet->stopAllActions();
        bullet->removeFromParent();//从父节点中移除子弹
    }
}
//接触检验（子弹与小怪）
void FightScene::monsterAndBullet()
{
    Monster* remMons = nullptr;
    for (Monster* mons : monster)
    {
        if ( heroBullet.size() != 0)
        {
            Bullet* bullet = nullptr;
            for (Bullet* bu : heroBullet)
            {
                if (mons->boundingBox().containsPoint(Vec2(bu->position().x, bu->position().y)))
                {
                    mons->beAttacked(bu->damagePower+hero->weaponNow->damage);
                    bullet = bu;
                    if (!mons->isAlive)
                    {
                        remMons = mons;
                    }
                }
            }
            if (bullet != nullptr)
            {
                heroBullet.remove(bullet);
                bullet->stopAllActions();
                bullet->removeFromParent();
            }
        }
    }
    if (remMons != nullptr)
    {
        monster.remove(remMons);
    }
}

//接触检验（子弹与障碍物）
void FightScene::bulletAndMeta()
{
    //英雄子弹
    Bullet* bullet1 = nullptr;
    for (Bullet* bu : heroBullet)
    {
       if (isCollision(bu->position().x, bu->position().y))
       {
           bullet1 = bu;
       }
    }
    if (bullet1 != nullptr)
    {
        heroBullet.remove(bullet1);//从容器中移除子弹
        bullet1->stopAllActions();
        bullet1->removeFromParent();//从父节点中移除子弹
    }
    //小怪子弹
    Bullet* bullet2 = nullptr;
    for (Bullet* bu : monsterBullet)
    {
        if (isCollision(bu->position().x, bu->position().y))
        {
            bullet2 = bu;
        }
    }
    if (bullet2 != nullptr)
    {
        monsterBullet.remove(bullet2);//从容器中移除子弹
        bullet2->stopAllActions();
        bullet2->removeFromParent();//从父节点中移除子弹
    }
}
//接触检验（英雄与小怪）
void FightScene::heroAndMonster()
{
    for (Monster* mons : monster)
    {
        Rect rect = hero->boundingBox();
        rect.setRect(rect.origin.x + hero->dx, rect.origin.y + hero->dy, rect.size.width, rect.size.height);
        if (rect.intersectsRect(mons->boundingBox()))//这里用了判断两个矩形是否相交
        {
            //hero->isCollision = true;
           
            //怪物的碰撞接口？
            //return;
        }
    }
    hero->isCollision = false;
}
//接触检验(武器与小怪)
void FightScene::weaponAndMonster()
{
    if (hero->weaponNow->type == 0)
    {

    }
}
//接触检验(武器与子弹)
void FightScene::weaponAndBullet()
{

}
//接触检验(英雄与特殊物质)
void FightScene::heroAndSpecial()
{
    for (specialSprite* spec : special)
    {
        if (spec->sprite->getBoundingBox().intersectsRect(hero->boundingBox()))
        {
            hero->isOtherFunction = true;
            hero->spec = spec;
            return;
        }
    }
    hero->isOtherFunction = false;
}
//从容器中移除死去的小怪
void FightScene::removeMonster()
{
    Monster* _monster;
    for (Monster* mons : monster)
    {
        if (!mons->isAlive)
        {
            _monster = mons;
        }
    }
    monster.remove(_monster);
}
//检验并更新场景的定时器
void FightScene::update(float dt)
{
    //子弹与英雄
    heroAndBullet();
    //子弹与小怪
    monsterAndBullet();
    //英雄与小怪
    heroAndMonster();
    //子弹与障碍物
    bulletAndMeta();
    //接触检验(英雄与特殊物质)
    heroAndSpecial();
    //从容器中移除死去的小怪
    removeMonster();
}
//检验相对于运动图层的点是否在障碍上
bool FightScene::isCollision(int _x,int _y)
{
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    float x = _x - map->getPositionX();
    float y = _y - map->getPositionY();
    int mapX = static_cast<int>((x - 16 * scale) / (32 * scale));
    int mapY = static_cast<int>((height - (y - 16 * scale) / (32 * scale)));
    //障碍物
    auto meta = map->getLayer("meta");
    int tileGid = meta->getTileGIDAt(Vec2(mapX, mapY));//获取唯一对应值
    //door
    auto doorClose = map->getLayer("doorClose");
    int tileGid3 = doorClose->getTileGIDAt(Vec2(mapX, mapY));
    //log("%d",tileGid3);
    if (tileGid == gid.x || (tileGid3 == gid.y && doorClose->isVisible()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//更新区块数据的定时器
void FightScene::updateArea(float dt)
{
    for (int i = 0; i < area.size(); i++)
    {
        auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
        auto doorClose = map->getLayer("doorClose");
        if(area[i].rect.containsPoint(Vec2(hero->position.x,hero->position.y)))
        {
            if (!area[i].haveCome)
            {
                area[i].haveCome = true;
                //显示相应小地图代码：
                auto sprite = Sprite::create("Tmx/littleMapWhite.png");
                sprite->setPosition(area[i].posLittleMap);
                sprite->setAnchorPoint(Vec2(0, 0));
                staticLayer->addChild(sprite, 2);
                for (int j = 0; j < 4; j++)
                {
                    if (area[i].around[j] != -1)
                    {
                        int k = area[i].around[j];
                        auto sprite_ = Sprite::create("Tmx/littleMapBG.png");
                        sprite_->setPosition(area[k].posLittleMap);
                        sprite_->setAnchorPoint(Vec2(0, 0));
                        staticLayer->addChild(sprite_, 1);
                        Point roadPoint;
                        if (j == 0)
                        {
                            roadPoint.x = area[i].posLittleMap.x - 20;
                            roadPoint.y = area[i].posLittleMap.y + 20;
                        }
                        else if (j == 1)
                        {
                            roadPoint.x = area[i].posLittleMap.x + 20;
                            roadPoint.y = area[i].posLittleMap.y + 60;
                        }
                        else if (j == 2)
                        {
                            roadPoint.x = area[i].posLittleMap.x + 60;
                            roadPoint.y = area[i].posLittleMap.y + 20;
                        }
                        else if (j == 3)
                        {
                            roadPoint.x = area[i].posLittleMap.x + 20;
                            roadPoint.y = area[i].posLittleMap.y - 20;
                        }
                        auto _sprite = Sprite::create("Tmx/littleMapRoad.png");
                        _sprite->setPosition(roadPoint);
                        _sprite->setAnchorPoint(Vec2(0, 0));
                        staticLayer->addChild(_sprite, 1);
                    }
                }
                //判断是否在战斗区域代码：
                if (area[i].isFightArea)
                {
                    changeMonster(area[i].monsterCount, 0);//产生小怪
                    area[i].monsterNum++;
                    doorClose->setVisible(true);
                    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Dungeon.mp3");
                }
            }
            else
            {
                //判断是否在战斗区域代码：
                if (area[i].isFightArea && monster.size() == 0)
                {
                    static int _count = 0;
                    if (area[i].monsterNum <= area[i].monsterNumMax)
                    {                      
                        if (_count == 10)//20次调用后产生小怪
                        {
                            changeMonster(area[i].monsterCount, area[i].monsterNum);//产生小怪
                            area[i].monsterNum++;
                            _count = 0;   
                        }
                        _count++;
                    }
                    else
                    {
                        doorClose->setVisible(false);
                        if(_count== 1)
                        {
                            SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Glacier.mp3");
                            addBox(posBox[area[i].monsterCount]);
                            _count++;
                            if (area[i].monsterCount == (posBox.size() - 1))
                            {
                                int x = area[area.size() - 1].rect.origin.x + area[area.size() - 1].rect.size.width / 2;
                                int y = area[area.size() - 1].rect.origin.y + area[area.size() - 1].rect.size.height / 2;
                                addLoop(Vec2(x, y));
                            }
                        }
                    }
                   
                   
                }
            }
        }
    }
}
//添加旋涡
void FightScene::addLoop(Vec2 vec)
{
    auto transfer = Sprite::create("map/Transfer.png");
    transfer->setPosition(vec);
    transfer->setScale(1.5);
    dynamicLayer->addChild(transfer, 1);

    auto* m_frameCache = SpriteFrameCache::getInstance();
    m_frameCache->addSpriteFramesWithFile("map/TransferDoor.plist", "map/TransferDoor.png");//使用图集
    Vector<SpriteFrame*> frameArray;
    for (int i = 1; i <= 4; i++)
    {
        auto* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("Transfer%d.png", i)->getCString());
        frameArray.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(frameArray);
    animation->setLoops(-1);
    animation->setDelayPerUnit(0.08f);
    auto animate = Animate::create(animation);
    transfer->runAction(animate);
    specialSprite* loop;
    loop->sprite = transfer;
    loop->type = 5;
    special.push_back(loop);
}