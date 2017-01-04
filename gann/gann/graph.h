#pragma once
#include <Windows.h>
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
class graph
{
public:
	static HDC hdc;
	static BITMAPINFO bmi;
	static int width, height, *data;
	static void init();
	static void update();
	static void setHdc(HDC _hdc);
	static void setPixel(int x, int y, int color);
	static void setBackColor(int color);
	static void drawLine(int x0, int y0, int x1, int y1, int color);
	static void drawCircle(int x, int y, int r, int color);
	static void fillCircle(int x, int y, int r, int color);
};