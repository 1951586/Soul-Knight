#include "Scene/FightScene2.h"
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

FightScene* FightScene2::createFightScene(Hero* _hero)
{
    FightScene2* scene = new FightScene2();
    if (scene && scene->init())
    {
        scene->autorelease();//自动清理内存
        scene->initScene(_hero);
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return NULL;
}

bool FightScene2::initScene(Hero* _hero)
{
    num = 2;
    //添加图层
    addLayer();
    //添加瓦片地图
    auto map = TMXTiledMap::create("Tmx/fightingscene1.tmx");
    scale = 1;
    map->setScale(scale);
    map->setTag(3);
    map->setAnchorPoint(Vec2(0, 0));
    pos = Point(2 * 32 * scale, -118 * 32 * scale);//
    map->setPosition(pos);
    height = 149;//
    gid = Point(27, 7);
    dynamicLayer->addChild(map, 0);
    //添加小地图
    littleMap = Sprite::create("Tmx/littleMapBG.png");
    littleMap->setPosition(Vec2(1100, 700));
    littleMap->setAnchorPoint(Vec2(0, 0));
    staticLayer->addChild(littleMap, 1);
    //隐藏关闭的门
    auto doorClose = map->getLayer("doorClose");
    doorClose->setVisible(false);
    //设置背景音乐
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("Music/Glacier.mp3", true);
    //添加英雄
    addHero(_hero);
    //初始化该场景的怪物信息
    initMonsterInformation();
    //初始化区块信息
    initAreaInformation();
    //初始化战斗区域宝箱信息
    initBoxPosition();
    //增加宝箱
    addBox(Vec2(10 * 32 * scale + pos.x, 126 * 32 * scale + pos.y));
    addBox(Vec2(9 * 32 * scale + pos.x, 7 * 32 * scale + pos.y));
    //添加检验并更新场景容器的定时器
    schedule(schedule_selector(FightScene2::update), 0.01);
    //添加更新区块数据的定时器
    schedule(schedule_selector(FightScene2::updateArea), 0.2);
    return true;
}
//初始化该场景的怪物信息
void FightScene2::initMonsterInformation()
{
    srand((unsigned)time(0));
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    //该场景的怪物初始位置信息
    Point posMonster[2][2][8] = //两战斗区，每区两波，每波最多8个
    {
        {{ {11 ,90}, {18 ,96 }, {26,89}, {26,104} ,{10,104}, {-1,-1},{-1,-1},{-1,-1} } ,
         { {11 ,90}, {18 ,96 }, {26,89}, {26,104} ,{10,104}, {-1,-1},{-1,-1},{-1,-1} } },
        {{ {9 ,68 }, {19,64}, {29,68}, {18,56} ,{9,48}, {28,48}, {-1,-1},{-1,-1} } ,
         { {9 ,68 }, {19,64}, {29,68}, {18,56} ,{9,48}, {28,48}, {-1,-1},{-1,-1} } }//不加小怪的就用（-1，-1）标识
    };
    //把小怪加在容器中
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int num = rand() % 4;//实现小怪随机生成
            for (int k = 0; k < 8; k++)
            {
                if (posMonster[i][j][k].x != -1 && posMonster[i][j][k].y != -1)
                {
                    //再次调整一下坐标
                    posMonster[i][j][k].x = 32*posMonster[i][j][k].x*scale + pos.x;
                    posMonster[i][j][k].y = 32*posMonster[i][j][k].y*scale + pos.y;
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
//初始化区块信息
void FightScene2::initAreaInformation()
{
    Area _area[4] =
    {
        {  Rect(9,125,19,18),false,{-1,-1,-1,1},false,0 ,0,0 ,Point(1100,700)},
        {  Rect(6,84,25,23),false,{-1,0,-1,2 },true,0,0,1 ,Point(1100,620)  },
        {  Rect(5,45,27,25),false,{-1,1,-1,3 },true,1,0,1,Point(1100,540)   },
        {  Rect(8,6,21,19),false,{-1,2,-1,-1},false,0,0,0 ,Point(1100,460) }
    };
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    for (int i = 0; i < 4; i++)
    {
        int x = _area[i].rect.origin.x * 32 * scale + pos.x;
        int y = _area[i].rect.origin.y * 32 * scale + pos.y;
        int dx = _area[i].rect.size.width * 32 * scale;
        int dy= _area[i].rect.size.height * 32 * scale;
        _area[i].rect.setRect(x,y, dx,dy);
        area.push_back(_area[i]);
    }
}
//切换小怪
void FightScene2::changeMonster(int count, int num)
{
    monster = allMonster[count][num];
    addMonster(monster);
}
//初始化宝箱位置信息
void FightScene2::initBoxPosition()
{
    posBox.push_back(Point(28 * 32 * scale+pos.x, 86 * 32 * scale+pos.y));
    posBox.push_back(Point(8 * 32 * scale+pos.x, 68 * 32 * scale+pos.y));
}