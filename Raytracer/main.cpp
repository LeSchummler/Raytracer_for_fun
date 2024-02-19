#include <Windows.h>
#include "renderer.h"
#include "raytracer.h"
#include "objects.h"
#include "datatypes.h"
#include <vector>
#include <math.h>

#include <cassert> //remove if assert isn't needed anymore
#include <iostream>

// Data for window handling
struct UserHandler {
	bool running = true;
	bool render = true;
};



//MessageHandler for WinAPI window
LRESULT CALLBACK messageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	UserHandler *hnd = (UserHandler*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (uMsg) {
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		PostQuitMessage(0);
		exit(0);
		return 0;
	case WM_SIZE:
		hnd->render = true;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCommandShow) {
int main() {
	HINSTANCE hInstance = GetModuleHandle(0);

	// WinAPI Window preperation
	WNDCLASS wc = {};
	MSG msg;
	RECT rect = {};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = messageHandler;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = L"Raytracer";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // for Games important

	//while Loop for Application
	UserHandler hnd;

	assert(RegisterClass(&wc)); //Check why

	HWND hWnd = CreateWindow(L"Raytracer", L"Raytracer by LeSchummler", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1920, 0, 0, hInstance, 0);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)&hnd);

	ShowWindow(hWnd, SW_SHOW); //use this line and remove WS_VISIBLE when you want to change some things before showing the window

	//HDC for some functions
	HDC hdc = GetDC(hWnd);
	

	//Create renderer Object
	renderer::window window;
	window.windowResize(hWnd, rect);

	
	//work in progress sphere
	Sphere sphere1 = { { 0, 0, -10 }, {((255 << 16) | (255 << 8) | (0 << 0)), 128}, 8.0f};
	Sphere sphere2 = { { 12, 0, -8 }, {((155 << 16) | (12 << 8) | (240 << 0)), 128}, 2.0f };
	Sphere sphere3 = { { -12, 0, -8 }, {((155 << 16) | (12 << 8) | (240 << 0)), 128}, 2.0f };
	Sphere sphere4 = { { 0, 12, -8 }, {((155 << 16) | (12 << 8) | (240 << 0)), 128}, 2.0f };
	Sphere sphere5 = { { 0, -12, -8 }, {((155 << 16) | (12 << 8) | (240 << 0)), 128}, 2.0f };
	std::vector<Sphere> objects;
	objects.push_back(sphere1);
	objects.push_back(sphere2);
	objects.push_back(sphere3);
	objects.push_back(sphere4);
	objects.push_back(sphere5);

	//renderObject init
	GameScene::GameScene gameScene(window.height, window.width, 3, 160.0F);
	gameScene.updateObjects(objects);

	while (hnd.running) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			switch (msg.message) {
			case WM_CLOSE:
			case WM_DESTROY:
			case WM_QUIT:
				hnd.running = false;
				hnd.render = false;
				PostQuitMessage(0);
				exit(0);
				return 0;
			//replace following in process
			case WM_NCMOUSELEAVE:
				break;
			}

			//renderer
			if (hnd.render) {
				window.windowResize(hWnd, rect); // maybe move this to messageHandler
				gameScene.resizeBuffer(window.height, window.width); // up
				gameScene.rewriteScreen(window);
				window.drawWindow(hdc);
				hnd.render = false;
				std::cout << "render done" << std::endl;
			}
		}
	}
	exit(0);
}