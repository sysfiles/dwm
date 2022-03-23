#pragma once
#include "TongLdll.h"
class Dwm_Dram
{

public:	 

	BOOL InitDwm();
	BOOL Dwm_StartDraw();
	VOID Dwm_EndDraw();
	VOID draw_text(float X, float Y, ImVec4	col, const char* text);
	VOID draw_rect(float X, float Y, float W, float H, ImVec4	col, int T );
	VOID draw_line(float X1, float Y1, float X2, float Y2, ImVec4 Color, float thickness);
	VOID DrawCircleFilled(float X, float Y, float Radius, ImVec4 Color, int Segments);

	float GetFps();
	BOOL IsPointInWindowsRect(POINT wnd_left_pos, ULONG ´°¿Ú¿í, ULONG ´°¿Ú¸ß, POINT target_pos);
};


