#pragma once
#include "WHThread.h"

class dmsoft;

class CTaskThread : public CWHThread
{
public:
	CTaskThread(tagWndInfo* pWnd); // 构造函数
	~CTaskThread();


private:
	dmsoft* m_pDm;

	tagWndInfo* m_pWnd;
	bool m_bind;

protected:
	virtual bool OnLoop() = 0; // 纯虚函数，子类必须实现具体的任务逻辑

protected:
	bool Bind();

	//找图
	bool Find(CString strUI, CPoint& ptFind, CRect rtArea = CRect(0, 0, 0, 0));    // 查找图片，返回找到的坐标位置
	bool Find(CString strUI, CRect rtArea = CRect(0, 0, 0, 0));                    // 查找图片，只返回是否找到

	//鼠标移动操作
	void MoveTo(int x, int y);                   // 移动鼠标到绝对坐标(x, y)
	void MoveR(int rx, int ry);                  // 移动鼠标相对当前位置偏移(rx, ry)

	//鼠标左键操作
	void LeftClick();                            // 鼠标左键单击
	void LeftDown();                             // 鼠标左键按下
	void LeftUp();                               // 鼠标左键抬起

	//鼠标右键操作
	void RightClick();                           // 鼠标右键单击
	void RightDown();                            // 鼠标右键按下
	void RightUp();                              // 鼠标右键抬起


	void WheelDown();                            // 鼠标滚轮向下滚动
	void WheelUp();                              // 鼠标滚轮向上滚动


	//左键点击操作（先移动再点击）
	void Click(CPoint ptClk);                    // 移动到指定点并左键单击
	void Click(int x, int y);                    // 移动到指定坐标并左键单击
	void DClick(CPoint ptClk);                   // 移动到指定点并左键双击
	void DClick(int x, int y);                   // 移动到指定坐标并左键双击

	//右键点击操作（先移动再点击）
	void RClick(CPoint ptClk);                   // 移动到指定点并右键单击
	void RClick(int x, int y);                   // 移动到指定坐标并右键单击
	void DRClick(CPoint ptClk);                  // 移动到指定点并右键双击
	void DRClick(int x, int y);                  // 移动到指定坐标并右键双击

	//键盘操作
	void KeyDown(long vk);                       // 键盘按键按下（vk为虚拟键码）
	void KeyPress(long vk);                      // 键盘按键按下并抬起（完整按键）
	void KeyUp(long vk);                         // 键盘按键抬起

	//OCR文字识别
	CString Ocr(CString strUI, CRect rtArea = CRect(0, 0, 0, 0));    // 在指定区域进行OCR文字识别，返回识别结果


	void DoTask(); // 执行任务的函数


public:
	virtual bool OnEventThreadRun()override;  // 线程运行事件
	virtual bool OnEventThreadStart()override; // 线程开始事件
	virtual bool OnEventThreadConclude()override; // 线程终止事件


};







