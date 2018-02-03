//=====================================================================
// This file is part of FlightOS.
//
// FlightOS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FlightOS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlightOS.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Platform.h"
#include "Log.h"

namespace FlightOS
{
  static HWND s_hWindow = 0;
  static HDC s_hDeviceContext = 0;

  void* Platform::getDeviceContext()
  {
    return (void*)s_hDeviceContext;
  }

  void* Platform::getWindow()
  {
    return (void*)s_hWindow;
  }

  Platform::Platform()
  {

  }

  int Platform::initialize()
  {
    // init timer
    LARGE_INTEGER qwfreq;
    QueryPerformanceFrequency(&qwfreq);
    mClockFreq = (float)qwfreq.QuadPart;
    mStartSysTimeMs = 0;
    mStartSysTimeMs = mLastSysTimeMs = getTimeMs();

    Log::info("System: Initialized");
    return 1;
  }

  int Platform::update()
  {
    // main thread pump
    MSG msg = { 0 };
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
    {
      if (msg.message == WM_QUIT)
        return 0;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    return 1;
  }

  int Platform::shutdown()
  {
    return 1;
  }

  uint64 Platform::getTimeMs()
  {
    LARGE_INTEGER qwNow;
    QueryPerformanceCounter(&qwNow);

    return (uint64)((((double)qwNow.QuadPart / (double)mClockFreq) * 1000.0) - mStartSysTimeMs);
  }

  LRESULT WINAPI windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    LRESULT  lRet = 1;
    POINT      point;
    GetCursorPos(&point);
    ScreenToClient(hWnd, &point);

    ShowCursor(true);

    //auto& io = ImGui::GetIO();

    switch (uMsg)
    {
    case WM_CREATE:
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    case WM_KEYDOWN:
      //s_system.aInputKeyMap[ wParam ] = true;
      /*if (wParam < 256)
        io.KeysDown[wParam] = 0;*/
      break;

    case WM_KEYUP:
      //s_system.aInputKeyMap[ wParam ] = false;
     /* if (wParam < 256)
        io.KeysDown[wParam] = 0;*/
      break;

    case WM_LBUTTONDOWN:
      ShowCursor(false);
      //s_system.aMouseButtons[Input::LEFT_BUTTON] = true;
      //io.MouseDown[0] = true;
      break;

    case WM_LBUTTONUP:
      while (ShowCursor(true) < 0);
      //s_system.aMouseButtons[Input::LEFT_BUTTON] = false;
      //io.MouseDown[0] = false;
      break;

    case WM_RBUTTONDOWN:
      ShowCursor(false);
      //s_system.aMouseButtons[Input::RIGHT_BUTTON] = true;
      //io.MouseDown[1] = true;
      break;

    case WM_RBUTTONUP:
      while (ShowCursor(true) < 0);
      //s_system.aMouseButtons[Input::RIGHT_BUTTON] = false;
      //io.MouseDown[1] = false;
      break;

    case WM_MOUSEMOVE:
      //s_system.iMouseX = point.x;
      //s_system.iMouseY = point.y;
      //io.MousePos.x = (signed short)(lParam);
      //io.MousePos.y = (signed short)(lParam >> 16);
      break;

    case WM_MOUSEWHEEL:
      //io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
      break;

    case WM_CHAR:
      // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
      /*if (wParam > 0 && wParam < 0x10000)
        io.AddInputCharacter((unsigned short)wParam);*/
      break;

    default:
      lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
      break;
    }

    return lRet;
  }


  int Platform::createWindow(int width, int height, const char* title)
  {
    WNDCLASS wndclass = { 0 };
    DWORD    wStyle = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HWND     hWnd = NULL;

    wndclass.style = CS_OWNDC;
    wndclass.lpfnWndProc = (WNDPROC)windowProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = "g3Window";

    if (!RegisterClass(&wndclass))
      return 0;

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = width;
    windowRect.bottom = height;

    AdjustWindowRect(&windowRect, wStyle, FALSE);

    hWnd = CreateWindow(
      "FlightOS",
      title,
      wStyle,
      1440 / 2 - (width / 2),
      900 / 2 - (height / 2),
      windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top,
      NULL,
      NULL,
      hInstance,
      NULL);

    if (hWnd == NULL)
      return NULL;

    ShowWindow(hWnd, TRUE);

    s_hWindow = hWnd;
    s_hDeviceContext = GetDC(hWnd);

    return 1;
  }

  void Platform::CreateFilePath(const std::string& path)
  {
    char str[MAX_PATH];
    assert(path.length() < MAX_PATH);
    strncpy_s(str, path.c_str(), path.length());

    char* context;
    const char* c = strtok_s(str, "/", &context);
    std::string folder;
    folder.reserve(path.length());

    while (c != NULL)
    {
      folder += c;
      if (!CreateDirectory(folder.c_str(), 0))
      {
        int err = GetLastError();
        if (err != ERROR_ALREADY_EXISTS)
        {
          Log::error("Unabled to create directory: %s, err: %d", folder.c_str(), err);
          return;
        }
      }
      folder += "\\";
      c = strtok_s(NULL, "/", &context);
    }

  }

  void Platform::DrawUI(float s)
  {

  }
}