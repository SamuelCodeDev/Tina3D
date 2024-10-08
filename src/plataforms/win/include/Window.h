#ifndef WINDOW_H
#define WINDOW_H

#include "Types.h"

namespace Tina
{
    enum WindowModes { FULLSCREEN, WINDOWED, BORDERLESS };

	class WindowDesc
	{
	protected:
		int32  windowWidth;
		int32  windowHeight;
		int32  windowStyle;
		int32  windowMode;
		float  windowCenterX;
		float  windowCenterY;
		float  windowPosX;
		float  windowPosY;
		string windowTitle;

	public:
		int32 Width() const noexcept;
		int32 Height() const noexcept;

		void Title(const string_view && title) noexcept;

		int32 Mode() const noexcept;
		float CenterX() const noexcept;
		float CenterY() const noexcept;
		constexpr float AspectRatio() const noexcept;
		string Title() const noexcept;
	};

	inline int32 WindowDesc::Width() const noexcept
	{ return windowWidth; }

	inline int32 WindowDesc::Height() const noexcept
	{ return windowHeight; }

	inline void WindowDesc::Title(const string_view && title) noexcept
	{ windowTitle = title; }

	inline int32 WindowDesc::Mode() const noexcept
	{ return windowMode; }

	inline float WindowDesc::CenterX() const noexcept
	{ return windowCenterX; }

	inline float WindowDesc::CenterY() const noexcept
	{ return windowCenterY; }

	inline constexpr float WindowDesc::AspectRatio() const noexcept
	{ return windowWidth / static_cast<float>(windowHeight); }

	inline string WindowDesc::Title() const noexcept
	{ return windowTitle; }
}

#include "WinInclude.h"
#include <Windows.h>
#include <Windowsx.h>

namespace Tina
{
	class Window final : public WindowDesc
	{
	private:
		HINSTANCE hInstance;
		HWND      windowHandle;
		HICON     windowIcon;
		HCURSOR   windowCursor;
		COLORREF  windowColor;

		static void (*inFocus)();
		static void (*lostFocus)();

	public:
		Window() noexcept;

		HINSTANCE AppId() const noexcept;
		HWND Id() const noexcept;
		void Icon(const uint32 icon) noexcept;
		void Cursor(const uint32 cursor) noexcept;
		void Size(const int32 width, const int32 height) noexcept;
		void Close() const noexcept;

		void Mode(const int32 mode) noexcept;

		COLORREF Color() const noexcept;
		void Color(const uint8 r, const uint8 g, const uint8 b) noexcept;
		bool Create();

		void InFocus(void(*func)()) noexcept;
		void LostFocus(void(*func)()) noexcept;

		static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

	inline HINSTANCE Window::AppId() const noexcept
	{ return hInstance; }

	inline HWND Window::Id() const noexcept
	{ return windowHandle; }

	inline void Window::Icon(const uint32 icon) noexcept
	{ windowIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(icon)); }

	inline void Window::Cursor(const uint32 cursor) noexcept
	{ windowCursor = LoadCursor(GetModuleHandle(nullptr), MAKEINTRESOURCE(cursor)); }

	inline void Window::Close() const noexcept
	{ PostMessage(windowHandle, WM_DESTROY, 0, 0); }

	inline COLORREF Window::Color() const noexcept
	{ return windowColor; }

	inline void Window::Color(const uint8 r, const uint8 g, const uint8 b) noexcept
	{ windowColor = RGB(r, g, b); }

	inline void Window::InFocus(void(*func)()) noexcept
	{ inFocus = func; }

	inline void Window::LostFocus(void(*func)()) noexcept
	{ lostFocus = func; }
}

#endif