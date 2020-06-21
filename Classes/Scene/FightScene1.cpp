#include "Scene/FightScene1.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Hero/Hero.h"
#include "Hero/Knight.h"
#include "Hero/Ranger.h"
#include "Hero/Sorcerer.h"
#include "Weapon/weapon.h"
#include "Monster/SnowMan.h"
#include "Monster/MonsShooter.h"
#include "Monster/Wildboar.h"
#include "Monster/wizard.h"
#include <cstdlib>
#include <ctime>


using namespace CocosDenshion;

FightScene* FightScene1::createFightScene(Hero* _hero)
{
    FightScene1* scene = new FightScene1();
    if (scene && scene->init())
    {
        scene->autorelease();//�Զ������ڴ�
        scene->initScene(_hero);
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return NULL;
}

bool FightScene1::initScene(Hero* _hero)
{
    num = 1;
    //���ͼ��
    addLayer();
    //�����Ƭ��ͼ
    auto map = TMXTiledMap::create("Tmx/combat.tmx");
    scale = 0.8;
    map->setScale(scale);
    map->setTag(3);
    map->setAnchorPoint(Vec2(0, 0));
    map->setPosition(Vec2(0, -50));
    pos = Point(0, -50);
    height = 74;//
    gid = Point(14, 24);
    dynamicLayer->addChild(map, 0);
    //���С��ͼ
    littleMap = Sprite::create("Tmx/littleMapBG.png");
    littleMap->setPosition(Vec2(1000, 600));
    littleMap->setAnchorPoint(Vec2(0, 0));
    staticLayer->addChild(littleMap,1);
    //���عرյ���
    auto doorClose = map->getLayer("doorClose");
    doorClose->setVisible(false);
    //���ñ�������
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("Music/Glacier.mp3", true);
    //���Ӣ��
    addHero(_hero);
    //��ʼ���ó����Ĺ�����Ϣ
    initMonsterInformation();
    //��ʼ��������Ϣ
    initAreaInformation();
    //��ʼ��ս����������Ϣ
    initBoxPosition();
    //���ӱ���
    addBox(Vec2(8 * 32 * scale, 30 * 32 * scale));
    addBox(Vec2(21 * 32 * scale, 54 * 32 * scale));
    //��Ӽ��鲢���³��������Ķ�ʱ��
    schedule(schedule_selector(FightScene1::update), 0.01);
    //��Ӹ����������ݵĶ�ʱ��
    schedule(schedule_selector(FightScene1::updateArea), 0.2);
    return true;
}
//��ʼ���ó����Ĺ�����Ϣ
void FightScene1::initMonsterInformation()
{
    srand((unsigned)time(NULL));
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    //�ó����Ĺ����ʼλ����Ϣ
    Point posMonster[2][2][8] = //��ս������ÿ��������ÿ�����8��
    {
        {{ {1984 * scale,224 * scale}, {3296 * scale,224 * scale}, {2624 * scale,384 * scale}, {2336 * scale,704 * scale} ,{2912 * scale,704 * scale}, {2624 * scale,1024 * scale}, {2016 * scale,1184 * scale},{3264 * scale,1152 * scale} } ,
         { {1984 * scale,224 * scale}, {3296 * scale,224 * scale}, {2624 * scale,384 * scale}, {2336 * scale,704 * scale} ,{2912 * scale,704 * scale}, {2624 * scale,1024 * scale}, {2016 * scale,1184 * scale},{3264 * scale,1152 * scale} } },
        {{ {2304 * scale,1696 * scale}, {2976 * scale,1696 * scale}, {2976 * scale,2144 * scale}, {2304 * scale,2144 * scale} ,{2656 * scale,1984 * scale}, {-1,-1}, {-1,-1},{-1,-1} } ,
         { {2304 * scale,1696 * scale}, {2976 * scale,1696 * scale}, {2976 * scale,2144 * scale}, {2304 * scale,2144 * scale} ,{2656 * scale,1984 * scale}, {-1,-1}, {-1,-1},{-1,-1} } }//����С�ֵľ��ã�-1��-1����ʶ
    };
    //��С�ּ���������
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int num = rand() % 4;//ʵ��С���������
            for (int k = 0; k < 8; k++)
            {
                if (posMonster[i][j][k].x != -1 && posMonster[i][j][k].y != -1)
                {
                    //�ٴε���һ������
                    posMonster[i][j][k].x = posMonster[i][j][k].x + map->getPositionX();
                    posMonster[i][j][k].y = posMonster[i][j][k].y + map->getPositionY();
                    if (num % 4 == 0)
                    {
                        auto mons = Snowman::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    else if (num % 4 == 1)
                    {
                        auto mons = Wildboar::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    else if (num % 4 == 2)
                    {
                        auto mons = MonsShooter::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    else if (num % 4 == 3)
                    {
                        auto mons = Wizard::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    num++;
                } 
            }
        }
    }
    
}
//��ʼ��������Ϣ
void FightScene1::initAreaInformation()
{
    Area _area[4] =
    {
        {  Rect(6 * 32 * scale , 6 * 32 * scale,33 * 32 * scale,28 * 32 * scale),false,{-1,-1,1,-1},false,0 ,0,0 ,Point(1000,600)},
        {  Rect(60 * 32 * scale, 4 * 32 * scale,48 * 32 * scale,36 * 32 * scale),false,{0,2,-1,-1 },true,0,0,1 ,Point(1080,600)  },
        {  Rect(66 * 32 * scale,48 * 32 * scale,33 * 32 * scale,26 * 32 * scale),false,{3,-1,-1,1 },true,1,0,1,Point(1080,680)   },
        {  Rect(19 * 32 * scale,51 * 32 * scale,17 * 32 * scale,18 * 32 * scale),false,{-1,-1,2,-1},false,0,0,0 ,Point(1000,680) }
    };
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    for (int i = 0; i < 4; i++)
    {
        _area[i].rect.setRect(_area[i].rect.origin.x+map->getPositionX(), _area[i].rect.origin.y + map->getPositionY(), _area[i].rect.size.width, _area[i].rect.size.height);
        area.push_back(_area[i]);
    }
}
//�л�С��
void FightScene1::changeMonster(int count, int num)
{
    monster = allMonster[count][num];
    addMonster(monster);
}
//��ʼ������λ����Ϣ
void FightScene1::initBoxPosition()
{
    posBox.push_back(Point(90 * 32 * scale, 37 * 32 * scale));
    posBox.push_back(Point(68 * 32 * scale, 54 * 32 * scale));
}