#include <algorithm>
#include <bitset>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "WinWrappers/WinWrappers.h"

template<typename TChar>
class SimpleWindow :
	public Window<TChar, ::SimpleWindow> {
public:
	SimpleWindow(HWND handle = nullptr) :
		Window<TChar, ::SimpleWindow>(handle) {
	}

	void Initialize(HINSTANCE hInstance) {
		constexpr int h = 30;
		constexpr int w = 100;
		m_progressBar = AddControl(WinCommonControl::ProgressBar, "ProgressBar", WS_CHILD | WS_VISIBLE, 0 * w, 0, w, h, NULL, hInstance, nullptr);
		m_button = AddControl(WinCommonControl::Button, "Button", WS_CHILD | WS_VISIBLE, 1 * w, 0, w, h, NULL, hInstance, nullptr);
		m_comboBox = AddControl(WinCommonControl::ComboBox, "ComboBox", WS_CHILD | WS_VISIBLE, 2 * w, 0, w, h, NULL, hInstance, nullptr);
		m_ipControl = AddControl(WinCommonControl::IP, "1.0.0.1", WS_CHILD | WS_VISIBLE, 3 * w, 0, w, h, NULL, hInstance, nullptr);
	}

private:
	HWND m_progressBar = nullptr;
	HWND m_button = nullptr;
	HWND m_comboBox = nullptr;
	HWND m_ipControl = nullptr;
};

template<typename TChar>
class SimpleWindowClass :
	public WindowClass<TChar, ::SimpleWindowClass> {
public:
	using Window = SimpleWindow<TChar>;
	using WindowClass<TChar, ::SimpleWindowClass>::WindowClass;
	static const TChar* GetName() { return "SimpleWindow"; }
};

template<typename TChar>
int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, TChar* lpCmdLine, int nCmdShow) {
	using WA = WinAPI<TChar>;
	UnusedVar(lpCmdLine, hPrevInstance);

	try {
		CallAndRethrow("WinMain", [&]() {
			// Make window class
			SimpleWindowClass<TChar> simpleWindowClass(hInstance);

			// Make window instance
			auto simpleWindow = simpleWindowClass.MakeWindow("SimpleWindow");

			// Show window
			simpleWindow->Show(nCmdShow);

			// Update window client area
			simpleWindow->Update();

			MSG msg;
			while (WA::GetMessage_(&msg, NULL, 0, 0)) {
				TranslateMessage(&msg);
				WA::DispatchMessage_(&msg);
			}
		});
	} catch (const std::exception& ex) {
		std::stringstream sstream;
		PrintException(sstream, ex);
		WA::MessageBox_(nullptr, sstream.str().c_str(), "Error", MB_ICONERROR);
	} catch (...) {
		WA::MessageBox_(nullptr, "Unexpected error!", "Error", MB_ICONERROR);
	}

	return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}