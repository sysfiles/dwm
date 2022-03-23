#pragma once
#include "CDwmMsg.h"
BOOL  Init();
bool  StartDraw();
void  EndDraw();
void   DrawBox(float X, float Y, float W, float H, float R, float G, float B, float A, int T);
void   DrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A, int T);
void   DrawString(float X, float Y, float R, float G, float B, float A, const char* text);
void   DrawCircle(float X, float Y, float radius, float R, float G, float B, float A, int num_segments, float W);
void   DrawStrokeText(float X, float Y, float R, float G, float B, float A, float R1, float G1, float B1, float A1, const char* text);
void   DrawCornerframe(float x, float y, float w, float h, float R, float G, float B, float A, int T);