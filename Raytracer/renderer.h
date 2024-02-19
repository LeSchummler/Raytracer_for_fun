#pragma once

#include <Windows.h>
#include <stdint.h>

#include <iostream>

namespace renderer {
	class window {
	private:
		static const int bytesPerPixel = 4;
		int pitch;
		BITMAPINFO bitInfo;
		void* memory;
	public:
		int width, height;

		~window();

		void windowResize(HWND& hWnd, RECT& rect);
		void drawWindow(HDC hdc);

		void setPixel(int x, int y, uint32_t color);
		// Here you can add new functions for writing directly to the memory
	};
}