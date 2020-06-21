#include "FightScene.h"
#include "Hero/Hero.h"
#include "Hero/Knight.h"
#include "Hero/Ranger.h"
#include "Hero/Sorcerer.h"
#include "Weapon/Weapon.h"
#include "Weapon/Bullet.h"

bool FightScene::init()
{
    if (!Scene::init())//�ж��Ƿ��ʼ���ɹ�
    {
        return false;
    }
    return true;
}

void FightScene::addLayer()
{
    //��ʼ��ͼ��
    dynamicLayer = Layer::create();
    staticLayer = Layer::create();
    //���ͼ��
    this->addChild(dynamicLayer);
    dynamicLayer->setTag(-3);
    this->addChild(staticLayer);
    staticLayer->setTag(-2);
}

void FightScene::addHero(Hero* _hero)
{
    //���Ӣ��
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
    for (Weapon* wea : _hero->weapon)//��������
    {
        auto weapon = Weapon::createWeapon(this, hero, wea->name.c_str());//true��ʾԶ��
        hero->changeWeapon(weapon);
    }
    for (Weapon* wea : hero->weapon)//��������
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
    hero->nextScene(this);//Ӣ���л�����
    hero->setPosition(Vec2(600, 400));
    dynamicLayer->addChild(hero, 5);
    hero->_num.push_back(num);
   
}
//���ӱ���
void FightScene::addBox(Vec2 vec)
{
    specialSprite* spec = new specialSprite;
    spec->sprite = Sprite::create("Map/boxClose.png");
    spec->sprite->setPosition(vec);
    dynamicLayer->addChild(spec->sprite,1);
    spec->type = 0;
    special.push_back(spec);
}
//�������һ������
void FightScene::addMonster(std::list<Monster*> _monster)
{
    for (Monster* mons : _monster)
    {
        mons->comeOnStage();
    }
}
//�Ӵ����飨�ӵ���Ӣ�ۣ�
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
        monsterBullet.remove(bullet);//���������Ƴ��ӵ�
        bullet->stopAllActions();
        bullet->removeFromParent();//�Ӹ��ڵ����Ƴ��ӵ�
    }
}
//�Ӵ����飨�ӵ���С�֣�
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

//�Ӵ����飨�ӵ����ϰ��
void FightScene::bulletAndMeta()
{
    //Ӣ���ӵ�
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
        heroBullet.remove(bullet1);//���������Ƴ��ӵ�
        bullet1->stopAllActions();
        bullet1->removeFromParent();//�Ӹ��ڵ����Ƴ��ӵ�
    }
    //С���ӵ�
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
        monsterBullet.remove(bullet2);//���������Ƴ��ӵ�
        bullet2->stopAllActions();
        bullet2->removeFromParent();//�Ӹ��ڵ����Ƴ��ӵ�
    }
}
//�Ӵ����飨Ӣ����С�֣�
void FightScene::heroAndMonster()
{
    for (Monster* mons : monster)
    {
        Rect rect = hero->boundingBox();
        rect.setRect(rect.origin.x + hero->dx, rect.origin.y + hero->dy, rect.size.width, rect.size.height);
        if (rect.intersectsRect(mons->boundingBox()))//���������ж����������Ƿ��ཻ
        {
            //hero->isCollision = true;
           
            //�������ײ�ӿڣ�
            //return;
        }
    }
    hero->isCollision = false;
}
//�Ӵ�����(������С��)
void FightScene::weaponAndMonster()
{
    if (hero->weaponNow->type == 0)
    {

    }
}
//�Ӵ�����(�������ӵ�)
void FightScene::weaponAndBullet()
{

}
//�Ӵ�����(Ӣ������������)
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
//���������Ƴ���ȥ��С��
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
//���鲢���³����Ķ�ʱ��
void FightScene::update(float dt)
{
    //�ӵ���Ӣ��
    heroAndBullet();
    //�ӵ���С��
    monsterAndBullet();
    //Ӣ����С��
    heroAndMonster();
    //�ӵ����ϰ���
    bulletAndMeta();
    //�Ӵ�����(Ӣ������������)
    heroAndSpecial();
    //���������Ƴ���ȥ��С��
    removeMonster();
}
//����������˶�ͼ��ĵ��Ƿ����ϰ���
bool FightScene::isCollision(int _x,int _y)
{
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    float x = _x - map->getPositionX();
    float y = _y - map->getPositionY();
    int mapX = static_cast<int>((x - 16 * scale) / (32 * scale));
    int mapY = static_cast<int>((height - (y - 16 * scale) / (32 * scale)));
    //�ϰ���
    auto meta = map->getLayer("meta");
    int tileGid = meta->getTileGIDAt(Vec2(mapX, mapY));//��ȡΨһ��Ӧֵ
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
//�����������ݵĶ�ʱ��
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
                //��ʾ��ӦС��ͼ���룺
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
                //�ж��Ƿ���ս��������룺
                if (area[i].isFightArea)
                {
                    changeMonster(area[i].monsterCount, 0);//����С��
                    area[i].monsterNum++;
                    doorClose->setVisible(true);
                    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Dungeon.mp3");
                }
            }
            else
            {
                //�ж��Ƿ���ս��������룺
                if (area[i].isFightArea && monster.size() == 0)
                {
                    static int _count = 0;
                    if (area[i].monsterNum <= area[i].monsterNumMax)
                    {                      
                        if (_count == 10)//20�ε��ú����С��
                        {
                            changeMonster(area[i].monsterCount, area[i].monsterNum);//����С��
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
//�������
void FightScene::addLoop(Vec2 vec)
{
    auto transfer = Sprite::create("map/Transfer.png");
    transfer->setPosition(vec);
    transfer->setScale(1.5);
    dynamicLayer->addChild(transfer, 1);

    auto* m_frameCache = SpriteFrameCache::getInstance();
    m_frameCache->addSpriteFramesWithFile("map/TransferDoor.plist", "map/TransferDoor.png");//ʹ��ͼ��
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