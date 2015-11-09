#include "stdafx.h"
#include "console.h"
#include <conio.h> //from windows
#include <cassert>
#include <windows.h>
#include <iostream>
#include <cassert>
#include <limits>

namespace memory
{
  template<class T>
  void clearbits(T& v, int bit_index, int nbits)
  {
    T mask = ~(T(0)) << (sizeof(T) * CHAR_BIT - T(nbits));
    mask =  mask >> (sizeof(T) * CHAR_BIT - T(bit_index) - T(nbits));
    v &= ~mask;
  }

  template<class T, class T2>
  void setbits(T& v, int bit_index, int nbits, T2 number)
  {
    //static_assert(is_unsigned<T>::value);
    assert(number <= (std::numeric_limits<T>::max)());
    assert(number >= 0);
    clearbits(number, nbits, sizeof(T2) * CHAR_BIT - nbits);
    T big(number);
    big = (big << bit_index);
    clearbits(v, bit_index, nbits);
    v |= big;
  }

  template<class T>
  T getbits(T v, int bit_index, int nbits)
  {
    T r = v >> bit_index;
    clearbits(r, nbits, sizeof(T) * CHAR_BIT - nbits);
    return r;
  }

  template<class T>
  bool is_bit_on(T v, int bit_index)
  {
    return (v & (T(1) << bit_index)) != 0;
  }

} //namespace memory

namespace console
{
  using namespace memory;

/*  char* _cgets(char* buffer)
  {
    return::_cgets(buffer);
  }*/

  int getch(void)
  {
    return _getch();
  }

  int getche(void)
  {
    return _getche();
  }

  int kbhit(void)
  {
    return _kbhit();
  }

  int cputs(const char* str)
  {
    return _cputs(str);
  }

  void gettextinfo(text_info* r)
  {
    if (r == 0)
      return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    r->attribute = (unsigned char) csbi.wAttributes;
    r->curx = (unsigned char)csbi.dwCursorPosition.X;
    r->cury = (unsigned char)csbi.dwCursorPosition.Y;
    r->screenwidth = (unsigned char)csbi.dwMaximumWindowSize.X;
    r->screenheight = (unsigned char)csbi.dwMaximumWindowSize.X;
    r->winleft = (unsigned char)csbi.srWindow.Left;
    r->wintop = (unsigned char)csbi.srWindow.Top;
    r->winright = (unsigned char)csbi.srWindow.Right;
    r->winbottom = (unsigned char)csbi.srWindow.Bottom;
    r->normattr = 0;
    r->currmode = 3;
  }

  void _setcursortype(int cur_t)
  {
    CONSOLE_CURSOR_INFO ci;

    switch (cur_t)
    {
      case _NOCURSOR://     (turns off the cursor)
        ci.bVisible = FALSE;
        ci.dwSize = 0;
        break;

      case _SOLIDCURSOR://  (solid block cursor)
        ci.bVisible = TRUE;
        ci.dwSize = 100;
        break;

      default:
      case _NORMALCURSOR: // (normal underscore cursor)
        ci.bVisible = TRUE;
        ci.dwSize = 50;
        break;
    }

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
  }

  void textattr(int newattr)
  {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), newattr);
  }

  int putch(int c)
  {
    return _putch(c);
  }

  void clreol(void)
  {
    //clreol clears all characters from the cursor
    //position to the end of the line within the
    //current text window, without moving the
    //cursor.
  }

  char* getpass(const char* prompt)
  {
    //reads password
    //  getpass reads a password from the system
    //console, after prompting with the
    //null-terminated string prompt and disabling
    //the echo.
    //It returns a pointer to a null-terminated
    //string of up to eight characters (not
    //counting the null terminator).
    return 0;
  }

  void delline(void)
  {
    //tODO inserts blank line
  }

  void insline(void)
  {
    //tODO inserts blank line
  }

  int movetext(int left, int top, int right, int bottom, int destleft, int desttop)
  {
    //TODO
    return 0;
  }

  int gettext(int left, int top, int right, int bottom, void* destin)
  {
    if (destin == 0)
      return 0;

    char* pszText = (char*) destin;
    int count = 0;

    for (int k = top; (k <= bottom) && (*pszText); k++)
    {
      for (int i = left; (i <= right) && (*pszText); i++)
      {
        COORD point = {SHORT(i), SHORT(k)
                      };
        TCHAR ch;
        DWORD c;
        ReadConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, point, &c);
        *pszText = (char) ch;
        count++;
        pszText++;
      }
    }

    *pszText = 0;
    return count;
  }

  int puttext(int left, int top, int right, int bottom, const char* source)
  {
    if (source == 0)
      return 0;

    const char* pszText = (const char*) source;
    text_info ti;
    gettextinfo(&ti);
    int count = 0;

    for (int k = top; (k <= bottom) && (*pszText); k++)
    {
      for (int i = left; (i < right) && (*pszText); i++)
      {
        gotoxy(i, k);
        putch(*pszText);
        count++;
        pszText++;
      }
    }

    gotoxy(ti.curx, ti.cury);
    return count;
  }

  int textbackground(int newcolor)
  {
    text_info ti;
    gettextinfo(&ti);
    unsigned char wColor = ti.attribute;
    unsigned char old = getbits(wColor, 4, 4);
    setbits(wColor, 4, 4, newcolor);
    textattr(wColor);
    return old;
  }

  int textcolor(int newcolor)
  {
    text_info ti;
    gettextinfo(&ti);
    unsigned char wColor = ti.attribute;
    int old = getbits(wColor, 0, 4);
    setbits(wColor, 0, 4, newcolor);
    textattr(wColor);
    return old;
  }

  void highvideo()
  {
    text_info ti;
    gettextinfo(&ti);
    setbits(ti.attribute, 3, 1, 1);
    textattr(ti.attribute);
  }

  void lowvideo()
  {
    text_info ti;
    gettextinfo(&ti);
    setbits(ti.attribute, 3, 1, 0);
    textattr(ti.attribute);
  }

  void normvideo()
  {
    text_info ti;
    gettextinfo(&ti);
    setbits(ti.attribute, 3, 1, 0);
    setbits(ti.attribute, 7, 1, 0);
    textattr(ti.attribute);
  }

  int wherex()
  {
    text_info ti;
    gettextinfo(&ti);
    return ti.curx;
  }

  int wherey()
  {
    text_info ti;
    gettextinfo(&ti);
    return ti.cury;
  }

  void gotoxy(int x, int y)
  {
    COORD point = {SHORT(x), SHORT(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
  }

  void clrscr()
  {
    COORD coordScreen = {0, 0};
    unsigned long cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    unsigned long dwConSize;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
  }

  int ungetch(int ch)
  {
    return _ungetch(ch);
  }



  void Print(const wchar_t* s)
  {
    std::wcout << s;
  }

  void PrintLn(const wchar_t* s)
  {
    Print(s);
    std::wcout << std::endl;
  }

  void Print(const char* s)
  {
    std::cout << s;
  }

  void PrintLn(const char* s)
  {
    Print(s);
    std::cout << std::endl;
  }

  void Print(const wchar_t* s, int COLOR)
  {
    const int old = textcolor(COLOR);
    Print(s);
    textcolor(old);
  }
  void PrintLn(const wchar_t* s, int COLOR)
  {
    Print(s, COLOR);
    std::wcout << std::endl;
  }

  void Print(const char* s, int COLOR)
  {
    const int old = textcolor(COLOR);
    Print(s);
    textcolor(old);
  }
  void PrintLn(const char* s, int COLOR)
  {
    Print(s, COLOR);
    std::cout << std::endl;
  }


} // namespace console
