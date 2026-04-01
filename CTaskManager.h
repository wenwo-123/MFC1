#pragma once
// 任务管理器类

class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();
public:
	void Start(int id = -1);
	void Stop(int id = -1);
	void Suspend(int id = -1);

private:
	tagTaskInfo* GetInfo(int id);
};

