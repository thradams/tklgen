//
// Copyright (C) 2009, Thiago Adams (thiago.adams@gmail.com)
// http://www.thradams.com/
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
#ifndef __CONSOLE_HEADER__
#define __CONSOLE_HEADER__
#include <iostream>

namespace console
{
    enum COLORS
    {
        BLACK        =  0,
        BLUE         =  1,
        GREEN        =  2,
        CYAN         =  3,
        RED          =  4,
        MAGENTA      =  5,
        BROWN        =  6,
        LIGHTGRAY    =  7,
        DARKGRAY     =  8,
        LIGHTBLUE    =  9,
        LIGHTGREEN   = 10,
        LIGHTCYAN    = 11,
        LIGHTRED     = 12,
        LIGHTMAGENTA = 13,
        YELLOW       = 14,
        WHITE        = 15,
        BLINK        = 128
    };

    enum CURSORTYPE
    {
        _NOCURSOR,//     (turns off the cursor)
        _SOLIDCURSOR,//  (solid block cursor)
        _NORMALCURSOR // (normal underscore cursor)
    };

    struct text_info
    {
        unsigned char winleft;        /* left window coordinate */
        unsigned char wintop;         /* top window coordinate */
        unsigned char winright;       /* right window coordinate */
        unsigned char winbottom;      /* bottom window coordinate */
        unsigned char attribute;      /* text attribute */
        unsigned char normattr;       /* normal attribute */
        unsigned char currmode;       /* current video mode:
                                  BW40, BW80, C40, C80, or C4350 */
        unsigned char screenheight;   /* text screen's height */
        unsigned char screenwidth;    /* text screen's width */
        unsigned char curx;           /* x-coordinate in current window */
        unsigned char cury;           /* y-coordinate in current window */
    };

    char *cgets(char *buffer);

    void clreol(void);
    void clrscr();
    int cputs(const char *str);

    void delline(void);
    int getch(void);
    int getche(void);

    char *getpass(const char *prompt);
    int gettext(int left, int top, int right, int bottom, void*destin);
    void gettextinfo(struct text_info *r);
    void gotoxy(int x, int y);
    void highvideo();

    void insline(void);
    int kbhit(void);

    void lowvideo();
    int movetext(int left, int top, int right, int bottom, int destleft, int desttop);
    void normvideo();

    int putch(int c);

    int puttext(int left, int top, int right, int bottom, const char*source);
    void _setcursortype(int cur_t);
    void textattr(int newattr);
    int textbackground(int newcolor);
    int textcolor(int newcolor);
    int ungetch(int ch);
    int wherex();
    int wherey();
    //window    //TODO


    //////////

    class Ident
    {
      int m_i;
      Ident();//
    public:
      Ident(int i) : m_i(i){}
      operator int() const
      {
        return m_i; 
      }
    };

    template<class _Elem, class _Tr>
    std::basic_ostream<_Elem, _Tr> &
      operator << (std::basic_ostream<_Elem, _Tr> & stream,
      const Ident & ident)
    {
      for ( int i = 0; i < ident; i++)
        stream << " ";    
      return stream;
    }

    
    void Print(const wchar_t * s);
    void PrintLn(const wchar_t * s);

    void Print(const char * s);
    void PrintLn(const char * s);

    void Print(const wchar_t * s, int COLOR);
    void PrintLn(const wchar_t * s, int COLOR);

    void Print(const char * s, int COLOR);
    void PrintLn(const char * s, int COLOR);


} // namespace console

#endif //__CONSOLE_HEADER__
