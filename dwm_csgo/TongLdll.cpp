#include "TongLdll.h"
CDwmMsg* pMsgObj = nullptr;
//绘制矩形
void DrawBox(float X, float Y, float W, float H, float R, float G, float B, float A, int T)
{
	ImVec4	color;
	color.x = R / 100.0f;
	color.y = G / 100.0f;
	color.z = B / 100.0f;
	color.w = A / 100.0f;
	pMsgObj->DrawRect(X, Y, W, H, color, T);
}
//绘制直线
void DrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A, int T)
{
	ImVec4	color;
	color.x = R / 100.0f;
	color.y = G / 100.0f;
	color.z = B / 100.0f;
	color.w = A / 100.0f;
	pMsgObj->DrawLine(X1, Y1, X2, Y2, color, T);
}
//绘制文本
void DrawString(float X, float Y, float R, float G, float B, float A, const char* text)
{
	ImVec4	col;
	col.x = R / 100.0f;
	col.y = G / 100.0f;
	col.z = B / 100.0f;
	col.w = A / 100.0f;
	pMsgObj->DrawNewText(X, Y, col, false, text);
}
//绘制圆形
void DrawCircle(float X, float Y, float radius, float R, float G, float B, float A, int num_segments, float W)
{
	ImVec4	col;
	col.x = R / 100.0f;
	col.y = G / 100.0f;
	col.z = B / 100.0f;
	col.w = A / 100.0f;
	pMsgObj->DrawCircle(X, Y, radius, col, num_segments, W);
}
//描边文本
void DrawStrokeText(float X, float Y, float R, float G, float B, float A, float R1, float G1, float B1, float A1, const char* text) {
	DrawString(X - 1.0f, Y, R1,G1,B1,A1, text);
	DrawString(X + 1.0f, Y, R1, G1, B1, A1, text);
	DrawString(X, Y - 1.0f, R1, G1, B1, A1, text);
	DrawString(X, Y + 1.0f, R1, G1, B1, A1, text);
	DrawString(X, Y, R,G,B,A, text);
}
//绘制角框
void DrawCornerframe(float x, float y, float w, float h, float R, float G, float B, float A, int T) {
	DrawLine(x, y, x + w * 0.33f, y, R, G, B, A, T);
	DrawLine(x, y, x, y + h * 0.33f, R, G, B, A, T);
	DrawLine(x + w * 0.66f, y, x + w + 1.0f, y, R, G, B, A, T);
	DrawLine(x + w, y, x + w, y + h * 0.33f, R, G, B, A, T);
	DrawLine(x, y + h, x + w * 0.33f, y + h, R, G, B, A, T);
	DrawLine(x, y + h * 0.66f, x, y + h + 1.0f, R, G, B, A, T);
	DrawLine(x + w * 0.66f, y + h, x + w, y + h, R, G, B, A, T);
	DrawLine(x + w, y + h * 0.66f, x + w, y + h + 1.0f, R, G, B, A, T);
}
BOOL Init(){
	if (pMsgObj == nullptr)
	{
		pMsgObj = new CDwmMsg();
		if (!pMsgObj->InitMsg())
		{
			return FALSE;
		}
	}
	return TRUE;
}
bool StartDraw() {
	return pMsgObj->ExeStartDraw();
}

void EndDraw(){
	return pMsgObj->ExeEndDraw();
}

