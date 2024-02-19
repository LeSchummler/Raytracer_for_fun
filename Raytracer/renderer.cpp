#include "renderer.h"

namespace renderer {
	window::~window() {
		VirtualFree(memory, 0, MEM_RELEASE);
	}

	void window::windowResize(HWND& hWnd, RECT& rect) {

		if (!(GetWindowRect(hWnd, &rect))) exit(1);
		
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		if (memory)
		{
			VirtualFree(memory, 0, MEM_RELEASE);
		}

		bitInfo.bmiHeader.biSize = sizeof(bitInfo.bmiHeader);
		bitInfo.bmiHeader.biPlanes = 1;
		bitInfo.bmiHeader.biBitCount = 32;
		bitInfo.bmiHeader.biCompression = BI_RGB;
		bitInfo.bmiHeader.biWidth = width;
		bitInfo.bmiHeader.biHeight = -(height);

		memory = VirtualAlloc(0, (bytesPerPixel * height * width), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		pitch = width * bytesPerPixel;
	}
	void window::drawWindow(HDC hdc) {
		StretchDIBits(
			hdc,
			0, 0, width, height,
			0, 0, width, height,
			memory, &(bitInfo),
			DIB_RGB_COLORS, SRCCOPY
		);
		std::cout << "drawn screen" << std::endl;
	}

	void window::setPixel(int x, int y, uint32_t color) {
		if (x < 0 || x > width - 1 || y < 0 || y > height - 1)
			return;

		uint8_t* row = (uint8_t*)memory + x * bytesPerPixel + y * pitch;
		uint32_t* pixel = (uint32_t*)row;
		*pixel = color;
		//std::cout << "set Pixel " << x << "|" << y << " to " << color << std::endl;
	}
}