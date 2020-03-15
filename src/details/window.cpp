/**
 * @file   window.cpp
 * @author Vic P.
 * @brief  Implementation for Window
 */

#include "Vutils.h"

namespace vu
{

HWND vuapi GetConsoleWindow()
{
  typedef HWND (WINAPI *PfnGetConsoleWindow)();

  HWND hwConsole = NULL;

  PfnGetConsoleWindow pfnGetConsoleWindow = (PfnGetConsoleWindow)CLibrary::QuickGetProcAddress(
    _T("kernel32.dll"),
    _T("GetConsoleWindow")
  );

  if (pfnGetConsoleWindow)
  {
    hwConsole = pfnGetConsoleWindow();
  }

  return hwConsole;
}

typedef std::pair<ulong, HWND> PairPIDHWND;

BOOL CALLBACK fnFindTopWindowCallback(HWND hWnd, LPARAM lParam)
{
  auto& params = *(PairPIDHWND*)lParam;

  DWORD thePID = 0;

  if (GetWindowThreadProcessId(hWnd, &thePID) && thePID == params.first)
  {
    params.second = hWnd;
    return FALSE;
  }

  return TRUE;
}

HWND vuapi FindTopWindow(ulong ulPID)
{
  PairPIDHWND params(ulPID, nullptr);

  auto ret = EnumWindows(fnFindTopWindowCallback, (LPARAM)&params);

  return params.second;
}

std::string vuapi DecodeWMA(const ulong id)
{
  static struct { ulong id; char* text; } m[] = \
  {
    { 0x0000, (char*)"WM_NULL"},
    { 0x0001, (char*)"WM_CREATE" },
    { 0x0002, (char*)"WM_DESTROY" },
    { 0x0003, (char*)"WM_MOVE" },
    { 0x0005, (char*)"WM_SIZE" },
    { 0x0006, (char*)"WM_ACTIVATE" },
    { 0x0007, (char*)"WM_SETFOCUS" },
    { 0x0008, (char*)"WM_KILLFOCUS" },
    { 0x000A, (char*)"WM_ENABLE" },
    { 0x000B, (char*)"WM_SETREDRAW" },
    { 0x000C, (char*)"WM_SETTEXT" },
    { 0x000D, (char*)"WM_GETTEXT" },
    { 0x000E, (char*)"WM_GETTEXTLENGTH" },
    { 0x000F, (char*)"WM_PAINT" },
    { 0x0010, (char*)"WM_CLOSE" },
    { 0x0011, (char*)"WM_QUERYENDSESSION" },
    { 0x0013, (char*)"WM_QUERYOPEN" },
    { 0x0016, (char*)"WM_ENDSESSION" },
    { 0x0012, (char*)"WM_QUIT" },
    { 0x0014, (char*)"WM_ERASEBKGND" },
    { 0x0015, (char*)"WM_SYSCOLORCHANGE" },
    { 0x0018, (char*)"WM_SHOWWINDOW" },
    { 0x001A, (char*)"WM_WININICHANGE" },
    { 0x001B, (char*)"WM_DEVMODECHANGE" },
    { 0x001C, (char*)"WM_ACTIVATEAPP" },
    { 0x001D, (char*)"WM_FONTCHANGE" },
    { 0x001E, (char*)"WM_TIMECHANGE" },
    { 0x001F, (char*)"WM_CANCELMODE" },
    { 0x0020, (char*)"WM_SETCURSOR" },
    { 0x0021, (char*)"WM_MOUSEACTIVATE" },
    { 0x0022, (char*)"WM_CHILDACTIVATE" },
    { 0x0023, (char*)"WM_QUEUESYNC" },
    { 0x0024, (char*)"WM_GETMINMAXINFO" },
    { 0x0026, (char*)"WM_PAINTICON" },
    { 0x0027, (char*)"WM_ICONERASEBKGND" },
    { 0x0028, (char*)"WM_NEXTDLGCTL" },
    { 0x002A, (char*)"WM_SPOOLERSTATUS" },
    { 0x002B, (char*)"WM_DRAWITEM" },
    { 0x002C, (char*)"WM_MEASUREITEM" },
    { 0x002D, (char*)"WM_DELETEITEM" },
    { 0x002E, (char*)"WM_VKEYTOITEM" },
    { 0x002F, (char*)"WM_CHARTOITEM" },
    { 0x0030, (char*)"WM_SETFONT" },
    { 0x0031, (char*)"WM_GETFONT" },
    { 0x0032, (char*)"WM_SETHOTKEY" },
    { 0x0033, (char*)"WM_GETHOTKEY" },
    { 0x0037, (char*)"WM_QUERYDRAGICON" },
    { 0x0039, (char*)"WM_COMPAREITEM" },
    { 0x003D, (char*)"WM_GETOBJECT" },
    { 0x0041, (char*)"WM_COMPACTING" },
    { 0x0044, (char*)"WM_COMMNOTIFY" },
    { 0x0046, (char*)"WM_WINDOWPOSCHANGING" },
    { 0x0047, (char*)"WM_WINDOWPOSCHANGED" },
    { 0x0048, (char*)"WM_POWER" },
    { 0x004A, (char*)"WM_COPYDATA" },
    { 0x004B, (char*)"WM_CANCELJOURNAL" },
    { 0x004E, (char*)"WM_NOTIFY" },
    { 0x0050, (char*)"WM_INPUTLANGCHANGEREQUEST" },
    { 0x0051, (char*)"WM_INPUTLANGCHANGE" },
    { 0x0052, (char*)"WM_TCARD" },
    { 0x0053, (char*)"WM_HELP" },
    { 0x0054, (char*)"WM_USERCHANGED" },
    { 0x0055, (char*)"WM_NOTIFYFORMAT" },
    { 0x007B, (char*)"WM_CONTEXTMENU" },
    { 0x007C, (char*)"WM_STYLECHANGING" },
    { 0x007D, (char*)"WM_STYLECHANGED" },
    { 0x007E, (char*)"WM_DISPLAYCHANGE" },
    { 0x007F, (char*)"WM_GETICON" },
    { 0x0080, (char*)"WM_SETICON" },
    { 0x0081, (char*)"WM_NCCREATE" },
    { 0x0082, (char*)"WM_NCDESTROY" },
    { 0x0083, (char*)"WM_NCCALCSIZE" },
    { 0x0084, (char*)"WM_NCHITTEST" },
    { 0x0085, (char*)"WM_NCPAINT" },
    { 0x0086, (char*)"WM_NCACTIVATE" },
    { 0x0087, (char*)"WM_GETDLGCODE" },
    { 0x0088, (char*)"WM_SYNCPAINT" },
    { 0x00A0, (char*)"WM_NCMOUSEMOVE" },
    { 0x00A1, (char*)"WM_NCLBUTTONDOWN" },
    { 0x00A2, (char*)"WM_NCLBUTTONUP" },
    { 0x00A3, (char*)"WM_NCLBUTTONDBLCLK" },
    { 0x00A4, (char*)"WM_NCRBUTTONDOWN" },
    { 0x00A5, (char*)"WM_NCRBUTTONUP" },
    { 0x00A6, (char*)"WM_NCRBUTTONDBLCLK" },
    { 0x00A7, (char*)"WM_NCMBUTTONDOWN" },
    { 0x00A8, (char*)"WM_NCMBUTTONUP" },
    { 0x00A9, (char*)"WM_NCMBUTTONDBLCLK" },
    { 0x00AB, (char*)"WM_NCXBUTTONDOWN" },
    { 0x00AC, (char*)"WM_NCXBUTTONUP" },
    { 0x00AD, (char*)"WM_NCXBUTTONDBLCLK" },
    { 0x00FE, (char*)"WM_INPUT_DEVICE_CHANGE" },
    { 0x00FF, (char*)"WM_INPUT" },
    { 0x0100, (char*)"WM_KEYFIRST" },
    { 0x0100, (char*)"WM_KEYDOWN" },
    { 0x0101, (char*)"WM_KEYUP" },
    { 0x0102, (char*)"WM_CHAR" },
    { 0x0103, (char*)"WM_DEADCHAR" },
    { 0x0104, (char*)"WM_SYSKEYDOWN" },
    { 0x0105, (char*)"WM_SYSKEYUP" },
    { 0x0106, (char*)"WM_SYSCHAR" },
    { 0x0107, (char*)"WM_SYSDEADCHAR" },
    { 0x0109, (char*)"WM_UNICHAR" },
    { 0x0109, (char*)"WM_KEYLAST" },
    { 0x0108, (char*)"WM_KEYLAST" },
    { 0x010D, (char*)"WM_IME_STARTCOMPOSITION" },
    { 0x010E, (char*)"WM_IME_ENDCOMPOSITION" },
    { 0x010F, (char*)"WM_IME_COMPOSITION" },
    { 0x010F, (char*)"WM_IME_KEYLAST" },
    { 0x0110, (char*)"WM_INITDIALOG" },
    { 0x0111, (char*)"WM_COMMAND" },
    { 0x0112, (char*)"WM_SYSCOMMAND" },
    { 0x0113, (char*)"WM_TIMER" },
    { 0x0114, (char*)"WM_HSCROLL" },
    { 0x0115, (char*)"WM_VSCROLL" },
    { 0x0116, (char*)"WM_INITMENU" },
    { 0x0117, (char*)"WM_INITMENUPOPUP" },
    { 0x0119, (char*)"WM_GESTURE" },
    { 0x011A, (char*)"WM_GESTURENOTIFY" },
    { 0x011F, (char*)"WM_MENUSELECT" },
    { 0x0120, (char*)"WM_MENUCHAR" },
    { 0x0121, (char*)"WM_ENTERIDLE" },
    { 0x0122, (char*)"WM_MENURBUTTONUP" },
    { 0x0123, (char*)"WM_MENUDRAG" },
    { 0x0124, (char*)"WM_MENUGETOBJECT" },
    { 0x0125, (char*)"WM_UNINITMENUPOPUP" },
    { 0x0126, (char*)"WM_MENUCOMMAND" },
    { 0x0127, (char*)"WM_CHANGEUISTATE" },
    { 0x0128, (char*)"WM_UPDATEUISTATE" },
    { 0x0129, (char*)"WM_QUERYUISTATE" },
    { 0x0132, (char*)"WM_CTLCOLORMSGBOX" },
    { 0x0133, (char*)"WM_CTLCOLOREDIT" },
    { 0x0134, (char*)"WM_CTLCOLORLISTBOX" },
    { 0x0135, (char*)"WM_CTLCOLORBTN" },
    { 0x0136, (char*)"WM_CTLCOLORDLG" },
    { 0x0137, (char*)"WM_CTLCOLORSCROLLBAR" },
    { 0x0138, (char*)"WM_CTLCOLORSTATIC" },
    { 0x0200, (char*)"WM_MOUSEFIRST" },
    { 0x0200, (char*)"WM_MOUSEMOVE" },
    { 0x0201, (char*)"WM_LBUTTONDOWN" },
    { 0x0202, (char*)"WM_LBUTTONUP" },
    { 0x0203, (char*)"WM_LBUTTONDBLCLK" },
    { 0x0204, (char*)"WM_RBUTTONDOWN" },
    { 0x0205, (char*)"WM_RBUTTONUP" },
    { 0x0206, (char*)"WM_RBUTTONDBLCLK" },
    { 0x0207, (char*)"WM_MBUTTONDOWN" },
    { 0x0208, (char*)"WM_MBUTTONUP" },
    { 0x0209, (char*)"WM_MBUTTONDBLCLK" },
    { 0x020A, (char*)"WM_MOUSEWHEEL" },
    { 0x020B, (char*)"WM_XBUTTONDOWN" },
    { 0x020C, (char*)"WM_XBUTTONUP" },
    { 0x020D, (char*)"WM_XBUTTONDBLCLK" },
    { 0x020E, (char*)"WM_MOUSEHWHEEL" },
    { 0x020E, (char*)"WM_MOUSELAST" },
    { 0x020D, (char*)"WM_MOUSELAST" },
    { 0x020A, (char*)"WM_MOUSELAST" },
    { 0x0209, (char*)"WM_MOUSELAST" },
    { 0x0210, (char*)"WM_PARENTNOTIFY" },
    { 0x0211, (char*)"WM_ENTERMENULOOP" },
    { 0x0212, (char*)"WM_EXITMENULOOP" },
    { 0x0213, (char*)"WM_NEXTMENU" },
    { 0x0214, (char*)"WM_SIZING" },
    { 0x0215, (char*)"WM_CAPTURECHANGED" },
    { 0x0216, (char*)"WM_MOVING" },
    { 0x0218, (char*)"WM_POWERBROADCAST" },
    { 0x0219, (char*)"WM_DEVICECHANGE" },
    { 0x0220, (char*)"WM_MDICREATE" },
    { 0x0221, (char*)"WM_MDIDESTROY" },
    { 0x0222, (char*)"WM_MDIACTIVATE" },
    { 0x0223, (char*)"WM_MDIRESTORE" },
    { 0x0224, (char*)"WM_MDINEXT" },
    { 0x0225, (char*)"WM_MDIMAXIMIZE" },
    { 0x0226, (char*)"WM_MDITILE" },
    { 0x0227, (char*)"WM_MDICASCADE" },
    { 0x0228, (char*)"WM_MDIICONARRANGE" },
    { 0x0229, (char*)"WM_MDIGETACTIVE" },
    { 0x0230, (char*)"WM_MDISETMENU" },
    { 0x0231, (char*)"WM_ENTERSIZEMOVE" },
    { 0x0232, (char*)"WM_EXITSIZEMOVE" },
    { 0x0233, (char*)"WM_DROPFILES" },
    { 0x0234, (char*)"WM_MDIREFRESHMENU" },
    { 0x0238, (char*)"WM_POINTERDEVICECHANGE" },
    { 0x0239, (char*)"WM_POINTERDEVICEINRANGE" },
    { 0x023A, (char*)"WM_POINTERDEVICEOUTOFRANGE" },
    { 0x0240, (char*)"WM_TOUCH" },
    { 0x0241, (char*)"WM_NCPOINTERUPDATE" },
    { 0x0242, (char*)"WM_NCPOINTERDOWN" },
    { 0x0243, (char*)"WM_NCPOINTERUP" },
    { 0x0245, (char*)"WM_POINTERUPDATE" },
    { 0x0246, (char*)"WM_POINTERDOWN" },
    { 0x0247, (char*)"WM_POINTERUP" },
    { 0x0249, (char*)"WM_POINTERENTER" },
    { 0x024A, (char*)"WM_POINTERLEAVE" },
    { 0x024B, (char*)"WM_POINTERACTIVATE" },
    { 0x024C, (char*)"WM_POINTERCAPTURECHANGED" },
    { 0x024D, (char*)"WM_TOUCHHITTESTING" },
    { 0x024E, (char*)"WM_POINTERWHEEL" },
    { 0x024F, (char*)"WM_POINTERHWHEEL" },
    { 0x0251, (char*)"WM_POINTERROUTEDTO" },
    { 0x0252, (char*)"WM_POINTERROUTEDAWAY" },
    { 0x0253, (char*)"WM_POINTERROUTEDRELEASED" },
    { 0x0281, (char*)"WM_IME_SETCONTEXT" },
    { 0x0282, (char*)"WM_IME_NOTIFY" },
    { 0x0283, (char*)"WM_IME_CONTROL" },
    { 0x0284, (char*)"WM_IME_COMPOSITIONFULL" },
    { 0x0285, (char*)"WM_IME_SELECT" },
    { 0x0286, (char*)"WM_IME_CHAR" },
    { 0x0288, (char*)"WM_IME_REQUEST" },
    { 0x0290, (char*)"WM_IME_KEYDOWN" },
    { 0x0291, (char*)"WM_IME_KEYUP" },
    { 0x02A1, (char*)"WM_MOUSEHOVER" },
    { 0x02A3, (char*)"WM_MOUSELEAVE" },
    { 0x02A0, (char*)"WM_NCMOUSEHOVER" },
    { 0x02A2, (char*)"WM_NCMOUSELEAVE" },
    { 0x02B1, (char*)"WM_WTSSESSION_CHANGE" },
    { 0x02c0, (char*)"WM_TABLET_FIRST" },
    { 0x02df, (char*)"WM_TABLET_LAST" },
    { 0x02E0, (char*)"WM_DPICHANGED" },
    { 0x02E2, (char*)"WM_DPICHANGED_BEFOREPARENT" },
    { 0x02E3, (char*)"WM_DPICHANGED_AFTERPARENT" },
    { 0x02E4, (char*)"WM_GETDPISCALEDSIZE" },
    { 0x0300, (char*)"WM_CUT" },
    { 0x0301, (char*)"WM_COPY" },
    { 0x0302, (char*)"WM_PASTE" },
    { 0x0303, (char*)"WM_CLEAR" },
    { 0x0304, (char*)"WM_UNDO" },
    { 0x0305, (char*)"WM_RENDERFORMAT" },
    { 0x0306, (char*)"WM_RENDERALLFORMATS" },
    { 0x0307, (char*)"WM_DESTROYCLIPBOARD" },
    { 0x0308, (char*)"WM_DRAWCLIPBOARD" },
    { 0x0309, (char*)"WM_PAINTCLIPBOARD" },
    { 0x030A, (char*)"WM_VSCROLLCLIPBOARD" },
    { 0x030B, (char*)"WM_SIZECLIPBOARD" },
    { 0x030C, (char*)"WM_ASKCBFORMATNAME" },
    { 0x030D, (char*)"WM_CHANGECBCHAIN" },
    { 0x030E, (char*)"WM_HSCROLLCLIPBOARD" },
    { 0x030F, (char*)"WM_QUERYNEWPALETTE" },
    { 0x0310, (char*)"WM_PALETTEISCHANGING" },
    { 0x0311, (char*)"WM_PALETTECHANGED" },
    { 0x0312, (char*)"WM_HOTKEY" },
    { 0x0317, (char*)"WM_PRINT" },
    { 0x0318, (char*)"WM_PRINTCLIENT" },
    { 0x0319, (char*)"WM_APPCOMMAND" },
    { 0x031A, (char*)"WM_THEMECHANGED" },
    { 0x031D, (char*)"WM_CLIPBOARDUPDATE" },
    { 0x031E, (char*)"WM_DWMCOMPOSITIONCHANGED" },
    { 0x031F, (char*)"WM_DWMNCRENDERINGCHANGED" },
    { 0x0320, (char*)"WM_DWMCOLORIZATIONCOLORCHANGED" },
    { 0x0321, (char*)"WM_DWMWINDOWMAXIMIZEDCHANGE" },
    { 0x0323, (char*)"WM_DWMSENDICONICTHUMBNAIL" },
    { 0x0326, (char*)"WM_DWMSENDICONICLIVEPREVIEWBITMAP" },
    { 0x033F, (char*)"WM_GETTITLEBARINFOEX" },
    { 0x0358, (char*)"WM_HANDHELDFIRST" },
    { 0x035F, (char*)"WM_HANDHELDLAST" },
    { 0x0360, (char*)"WM_AFXFIRST" },
    { 0x037F, (char*)"WM_AFXLAST" },
    { 0x0380, (char*)"WM_PENWINFIRST" },
    { 0x038F, (char*)"WM_PENWINLAST" },
    { 0x0400, (char*)"WM_USER" },
  };

  std::string result = "";

  for (auto e : m)
  {
    if (e.id == id)
    {
      result = e.text;
      break;
    }
  }

  return result;
}

std::wstring vuapi DecodeWMW(const ulong id)
{
  return ToStringW(DecodeWMA(id));
}

TFontA vuapi GetFontA(HWND hw)
{
  TFontA result;

  if (IsWindow(hw))
  {
    HDC hdc = GetDC(hw);
    {
      HFONT hf = (HFONT)SendMessageA(hw, WM_GETFONT, 0, 0);
      if (hf != nullptr)
      {
        LOGFONTA lf = { 0 };
        GetObjectA(hf, sizeof(lf), &lf);
        result.Name  = lf.lfFaceName;
        result.Size = -MulDiv(lf.lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY)); // 72 pixel/inch
        result.Italic = lf.lfItalic != FALSE;
        result.Underline = lf.lfUnderline != FALSE;
        result.StrikeOut = lf.lfStrikeOut != FALSE;
        result.Weight = lf.lfWeight;
        result.CharSet = lf.lfCharSet;
        result.Orientation = lf.lfOrientation;
      }
    }
    ReleaseDC(hw, hdc);
  }

  return result;
}

TFontW vuapi GetFontW(HWND hw)
{
  TFontW result;

  if (IsWindow(hw))
  {
    HDC hdc = GetDC(hw);
    {
      HFONT hf = (HFONT)SendMessageW(hw, WM_GETFONT, 0, 0);
      if (hf != nullptr)
      {
        LOGFONTW lf = { 0 };
        GetObjectW(hf, sizeof(lf), &lf);
        result.Name = lf.lfFaceName;
        result.Size = -MulDiv(lf.lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY)); // 72 pixel/inch
        result.Italic = lf.lfItalic != FALSE;
        result.Underline = lf.lfUnderline != FALSE;
        result.StrikeOut = lf.lfStrikeOut != FALSE;
        result.Weight = lf.lfWeight;
        result.CharSet = lf.lfCharSet;
        result.Orientation = lf.lfOrientation;
      }
    }
    ReleaseDC(hw, hdc);
  }

  return result;
}

} // namespace vu