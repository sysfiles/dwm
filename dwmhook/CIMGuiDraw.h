#pragma once
#include "IMGUI/MyImGui.h"
#include "ck.h"
/*���ֵĳ���*/
#define BUF_LEN 256
/*����������*/
#define MAX_DRAW 500
/*�����ڴ�*/
#define MAPNAME L"hzw_dwm"

#define MAPNAME_Mutext L"hzw_dwm_Mutext"

struct DrawTextStr
{
	float	m_X;
	float	m_Y;
	ImVec4	m_Color;
	bool	m_Outlined;
	char	m_Str[BUF_LEN];
};


struct DrawCircleFillStr
{
	float		m_X;
	float		m_Y;
	float		m_Radius;
	ImVec4		m_Color;
	int		m_Segments;
};


struct DrawCircleStr
{
	float		m_X;
	float		m_Y;
	float		m_Radius;
	ImVec4		m_Color;
	int		m_Segments;
	float m_thickness;
};

struct DrawRectStr
{
	float m_X;
	float m_Y;
	float m_W;
	float m_H;
	ImVec4 m_Color;
	float m_thickness;
};

struct DrawRectExStr
{
	float m_X;
	float m_Y;
	float m_W;
	float m_H;
	ImVec4	m_Color;
	float	m_thickness;
};

struct DrawFilledRectStr
{
	float m_X;
	float m_Y;
	float m_W;
	float m_H;
	ImVec4	m_Color;
};

struct DrawLineStr
{
	float m_X1;
	float m_Y1;
	float m_X2;
	float m_Y2;
	ImVec4	m_Color;
	float thickness;
};
struct FPS {
	float Fps;
};
/*ͼ������ö��*/
enum DRAWTYPE { Text_M, Text2_M,CircleFill_M, Circle_M, Rect_M, RectEx_M, FilledRect_M, Line_M,FPS_M};

//ͼ�ε���Ϣ
struct DrawInfo
{
	DRAWTYPE m_DrawType;
	union
	{
		DrawTextStr			Text;
		DrawCircleFillStr	CircleFill;
		DrawCircleStr		Circle;
		DrawRectStr			Rect;
		DrawRectExStr		RectEx;
		DrawFilledRectStr	FilledRect;
		DrawLineStr			Line;
		FPS					Fps;
	}From;
};


//�Ի�ͼ״̬ �Լ����Ķ�������
typedef struct DrawArr
{
	/*
	* ������־
	*/
	bool		m_DrawDll;//true exe���Ի� falseֹͣ��
	/*
	* ��߿����Լ���չ��Ӧ�Ľṹ������ͼ��
	*/
	int			m_DrawCount;

	DrawInfo	m_DrawInfoArr[MAX_DRAW];
}DRAWARR, * PDRWARR;

class CIMGuiDraw
{
public:
	CIMGuiDraw();
	~CIMGuiDraw();
	
public:
	/*
	* �ύ���л�
	*/
	void ImGuiDx11Draw();
	bool InitMiGuiDx11(IDXGISwapChain* pSwapChain, ID3D11Device* pd3dDevice, HWND hwnd);
	/*
	* ��ʼ��ͨѶ m_pDrawAll
	*/
	bool InitMessage();

private:
	/*
	* �����ڴ�
	*/
	bool InitFileMapping();

	/*
	* 
	*/

private:
	PDRWARR m_pDrawAll = nullptr;
};


class Lock {
private:
	static inline std::mutex mutex;
	std::scoped_lock<std::mutex> lock{ mutex };
};