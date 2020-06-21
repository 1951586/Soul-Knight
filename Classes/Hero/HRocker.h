#ifndef __HROCCKER_H__
#define __HROCCKER_H__

#include "cocos2d.h"


USING_NS_CC;

//枚举类，识别摇杆与背景
typedef enum {
	tag_rocker,
	tag_rockerBG
}tagForHRocker;

/*
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT,
	NODIR
};
*/
class Hero;//预声明来解决包含问题
class HRocker :	public cocos2d::Layer
{
public:
	//改了一下：在摇杆中添加精灵
	Hero* hero;
	EventListenerTouchOneByOne* listener;
	EventListenerKeyboard* listenerKeyBoard;
	//创建摇杆
	static HRocker* createHRocker(const char* rockerImageName,
		const char* rockerBGImageName, Point posistion, Hero* hero);
	//启动键盘遥杆
	void startKeyRocker();
	//启动触碰摇杆
	void startTouchRocker();
	//停止键盘摇杆
	void stopKeyRocker();
	//停止触碰摇杆
	void stopTouchRocker();
	//移动方向以及移动速度的基数
	float dx, dy;
	//设置缩放倍速(便于控制)
	float scale;
private:
	void rockerInit(const char* rockerImageName,
		const char* rockerBGImageName, Point position, Hero* hero);
	//得到半径为r的圆上一个角度对应的x,y坐标
	Point getAnglePosition(float r, float angel);
	//是否可操纵摇杆
	bool Enabled;
	//得到摇杆和用户触屏点的角度
	float getRad(Point p1, Point p2);
	//摇杆背景坐标
	Point rockerBGPosition;
	//摇杆背景的半径
	float rockerBG_r;
	//触屏事件
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	//按键状态
	//bool _wState, _aState, _sState, _dState;
	//Direction _direction;
	//按键事件
	/*
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* envet);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* envet);
	virtual bool updateState(EventKeyboard::KeyCode keyCode, int type);
	virtual bool updateDirection();
	*/
	void HRocker::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void HRocker::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	CREATE_FUNC(HRocker);
};

#endif
