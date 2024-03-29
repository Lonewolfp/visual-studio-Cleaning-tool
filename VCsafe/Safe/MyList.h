#pragma once


// MyList

class MyList : public CListCtrl
{
	DECLARE_DYNAMIC(MyList)

public:
	MyList();
	virtual ~MyList();

protected:
	DECLARE_MESSAGE_MAP()

	//进程目录表排序
	static bool Isint;		//是否int型排序
	static bool method;		//排序方向;
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, 
		                            LPARAM lParamSort);	//倒序函数：
public:
	void AddColumn(DWORD dwCount, ...);					//添加列数
	void AddItem(DWORD dwCount, ...);					//添加行数
	void SortItemsPlus(int nColum, bool Isint = true);	//进行排序

	//进进程目录表排序结构体;
	typedef struct  _DATA
	{
		int SubItem;
		MyList* pList;
	}DATA;
};


