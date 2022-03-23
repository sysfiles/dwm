#include "Dwm_Dram.h"
#include <stdlib.h>
extern CDwmMsg* pMsgObj;



BOOL Dwm_Dram::InitDwm()
{
	return Init();
}

BOOL Dwm_Dram::Dwm_StartDraw()
{
	return StartDraw();
}

VOID Dwm_Dram::Dwm_EndDraw()
{
	return EndDraw();
}

VOID Dwm_Dram::draw_text(float X, float Y, ImVec4 col, const char * text)
{
	return pMsgObj->DrawNewText(X, Y, col, false, text);
}

VOID Dwm_Dram::draw_rect(float X, float Y, float W, float H, ImVec4 color, int T)
{

	return pMsgObj->DrawRect(X, Y, W, H, color, T);
}

VOID Dwm_Dram::draw_line(float X1, float Y1, float X2, float Y2, ImVec4 Color, float thickness)
{
	return pMsgObj->DrawLine(X1, Y1, X2, Y2, Color, thickness);
}

VOID Dwm_Dram::DrawCircleFilled(float X, float Y, float Radius, ImVec4 Color, int Segments)
{
	return pMsgObj->DrawCircleFilled(X,Y, Radius, Color, Segments);
}

float Dwm_Dram::GetFps()
{
	return pMsgObj->GetFps();
}

/*
判断 点是否在窗口范围内
参数一:窗口左顶点
参数二：窗口宽
参数三:窗口高
参数四: 需要判断的点
*/
BOOL Dwm_Dram::IsPointInWindowsRect(POINT wnd_left_pos, ULONG 窗口宽, ULONG 窗口高, POINT target_pos)
{


	if ((target_pos.x > wnd_left_pos.x) && (target_pos.y > wnd_left_pos.y) && (target_pos.x < (wnd_left_pos.x + 窗口宽)) &&
		(target_pos.y < (wnd_left_pos.y + 窗口高)))
	{
		return TRUE;
	}

	return FALSE;
}

