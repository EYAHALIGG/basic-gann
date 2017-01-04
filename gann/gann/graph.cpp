#include "graph.h"

HDC graph::hdc;
BITMAPINFO graph::bmi = { sizeof(BITMAPINFOHEADER),WINDOW_WIDTH,-WINDOW_HEIGHT,1,32 };
int graph::width;
int graph::height;
int *graph::data;

void graph::init()
{
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	data = (int*)malloc(width*height * 4);
}

void graph::update()
{
	StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, data, &bmi, 0, SRCCOPY);
	SwapBuffers(hdc);
	setBackColor(RGB(45, 45, 45));
}

void graph::setHdc(HDC _hdc)
{
	hdc = _hdc;
}

void graph::setPixel(int x, int y, int color)
{
	if ((x >= 0) && (y >= 0) && (x <= width) && (y <= height))
		data[x + y*width] = color;
}

void graph::setBackColor(int color)
{
	for (int i = 0; i < width*height; i++)
		data[i] = color;
}

void graph::drawLine(int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	for (;;) {
		setPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void graph::drawCircle(int x, int y, int r, int color)
{
	int xx = r;
	int yy = 0;
	int err = 0;
	while (xx >= yy)
	{
		setPixel(x + xx, y + yy, color);
		setPixel(x + yy, y + xx, color);
		setPixel(x - yy, y + xx, color);
		setPixel(x - xx, y + yy, color);
		setPixel(x - xx, y - yy, color);
		setPixel(x - yy, y - xx, color);
		setPixel(x + yy, y - xx, color);
		setPixel(x + xx, y - yy, color);
		yy++;
		err += 1 + 2 * yy;
		if (2 * (err - xx) + 1 > 0)
		{
			xx--;
			err += 1 - 2 * xx;
		}
	}
}

void graph::fillCircle(int x, int y, int r, int color)
{
	int xx = r;
	int yy = 0;
	int err = 0;
	int i;
	while (xx >= yy)
	{
		for (i = y - yy; i < y + yy; i++)
		{
			setPixel(x - xx, i, color);
			setPixel(x + xx, i, color);
		}
		for (i = y - xx; i < y + xx; i++)
		{
			setPixel(x - yy, i, color);
			setPixel(x + yy, i, color);
		}
		yy++;
		err += 1 + 2 * yy;
		if (2 * (err - xx) + 1 > 0)
		{
			xx--;
			err += 1 - 2 * xx;
		}
	}
}
