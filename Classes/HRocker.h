#ifndef __HROCCKER_H__
#define __HROCCKER_H__

#include "cocos2d.h"


USING_NS_CC;

//ö���࣬ʶ��ҡ���뱳��
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
class Hero;//Ԥ�����������������
class HRocker :	public cocos2d::Layer
{
public:
	//����һ�£���ҡ������Ӿ���
	Hero* hero;
	EventListenerTouchOneByOne* listener;
	EventListenerKeyboard* listenerKeyBoard;
	//����ҡ��
	static HRocker* createHRocker(const char* rockerImageName,
		const char* rockerBGImageName, Point posistion, Hero* hero);
	//��������ң��
	void startKeyRocker();
	//��������ҡ��
	void startTouchRocker();
	//ֹͣ����ҡ��
	void stopKeyRocker();
	//ֹͣ����ҡ��
	void stopTouchRocker();
	//�ƶ������Լ��ƶ��ٶȵĻ���
	float dx, dy;
	//�������ű���(���ڿ���)
	float scale;
private:
	void rockerInit(const char* rockerImageName,
		const char* rockerBGImageName, Point position, Hero* hero);
	//�õ��뾶Ϊr��Բ��һ���Ƕȶ�Ӧ��x,y����
	Point getAnglePosition(float r, float angel);
	//�Ƿ�ɲ���ҡ��
	bool Enabled;
	//�õ�ҡ�˺��û�������ĽǶ�
	float getRad(Point p1, Point p2);
	//ҡ�˱�������
	Point rockerBGPosition;
	//ҡ�˱����İ뾶
	float rockerBG_r;
	//�����¼�
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	//����״̬
	//bool _wState, _aState, _sState, _dState;
	//Direction _direction;
	//�����¼�
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
