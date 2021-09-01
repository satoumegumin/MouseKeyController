#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream> 

#include "ControllerClass.h"
#include "RobotController.h"
using namespace std;

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //必要的，我是背下来的   

#pragma comment (lib, "User32.lib")

void GetClickPosition()
{

	//----------移除快速编辑模式(对于win10用户)----------
	//HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	//DWORD mode;
	//GetConsoleMode(hStdin, &mode);
	//mode &= ~ENABLE_QUICK_EDIT_MODE;
	//SetConsoleMode(hStdin, mode);

	POINT p;
	while (1) {

		if (KEY_DOWN(VK_LBUTTON))
		{
			GetCursorPos(&p);//获取鼠标坐标 
			cout << p.x;
			cout << ",";
			cout << p.y << endl;
		}
		Sleep(100);
	}
}

void SetPoint(POINT p)
{
	SetCursorPos(p.x, p.y); 
	cout << "设置鼠标位置" << endl;
}

void Click(POINT p)
{
	SetPoint(p);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	cout << "单击" << endl;
}

void DoubleClickk(POINT p)
{
	SetPoint(p);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	cout << "双击" << endl;
}

void DragMouse(POINT p)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_MOVE, p.x,p.y, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	cout << "按下移动" << endl;
}


void PressKey(int keyCode)
{
	keybd_event(keyCode, 0, 0, 0);
	keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
}

size_t GetNum(string str)
{
	return atoi(str.c_str());
}

POINT GetPoint(string pointStr)
{
	POINT point;

	size_t sIndex = pointStr.find(",");

	point.x = GetNum(pointStr.substr(0,sIndex));
	point.y = GetNum(pointStr.substr(sIndex+1));

	return point;
}

void ExcuteCommand(string str)
{
	size_t len = str.length();

	char strCmd = str[0];

	string strCont = "";
	if (str[len - 1] == ';')
		strCont = str.substr(2, len - 3);
	else
		strCont = str.substr(2, len - 2);

	POINT p;
	size_t t = 0;

	switch (strCmd)
	{
	case'S':
		p = GetPoint(strCont);
		SetPoint(p);
		break;
	case'C':
		p = GetPoint(strCont);
		Click(p);
		break;
	case'D':
		p = GetPoint(strCont);
		DoubleClickk(p);
		break;
	case'M':
		p = GetPoint(strCont);
		DragMouse(p);
		break;
	case'T':
		t = GetNum(strCont);
		Sleep(t * 1000);
		cout<<"暂停：" + t<<endl;
		break;
	case'P':
		t = GetNum(strCont);
		PressKey(t);
		break;
	};

	//cout << strCont << endl;
}

void DoLineCommand(string temp)
{

	size_t startIndex = 0;
	size_t indexCount = 0;
	size_t length = temp.length();

	for (size_t i = 0; i < length; i++)
	{
		indexCount++;

		if (temp[i] == ';' || i == length - 1)
		{
			string command = temp.substr(startIndex, indexCount);

			ExcuteCommand(command);

			Sleep(20);

			//cout << command << endl;

			startIndex += indexCount;
			indexCount = 0;

		}
	}
}

int main()
{

	Sleep(5000);

	ifstream myfile("controller.ctr");
	//ifstream myfile("C:\\Users\\Administrator\\Desktop\\获取点击坐标\\controller.ctr");

	string temp;
	if (!myfile.is_open())
	{
		cout << "未成功打开文件" << endl;
	}
	while (getline(myfile, temp))
	{	

		if (temp[0] =='W')
		{
			size_t firstIndex = temp.find(';');

			string countStr = temp.substr(2, firstIndex - 2);

			temp = temp.substr(firstIndex + 1);

			if (countStr == "*" )
			{
				while (true)
				{
					DoLineCommand(temp);
				}
			}

			size_t count = GetNum(countStr);

			for (size_t i = 0; i < count; i++)
			{
				DoLineCommand(temp);
			}
		}
		else
		{
			DoLineCommand(temp);
		}



	}
	myfile.close();

	system("pause");

	return 0;

	//Sleep(10000);

	//keybd_event(81, 0, 0, 0);
	//keybd_event(81, 0, KEYEVENTF_KEYUP, 0);//作者：Love丶伊卡洛斯 https ://www.bilibili.com/read/cv4954650/ 出处：bilibili

	//return 0;

	//SetCursorPos(115, 372);//更改鼠标坐标

	//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//www.bilibili.com/read/cv4954650/ 出处：bilibili
	////mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//www.bilibili.com/read/cv4954650/ 出处：bilibili

	//mouse_event(MOUSEEVENTF_MOVE, 300, 0, 0, 0);
	////SetCursorPos(300, 450);//更改鼠标坐标
	//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//www.bilibili.com/read/cv4954650/ 出处：bilibili

	//return 0;

	//cout << "hello";
}