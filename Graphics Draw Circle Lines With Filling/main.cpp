#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void DrawLowPoint(HDC hdc,int xs,int ys,int xe,int ye,COLORREF c)
{
    int dx=xe-xs;
    int dy=ye-ys;
    int yi=1;
    if(dy<0)
    {
        dy=-dy;
        yi=-1;
    }
    int d=2*dy-dx;
    int y=ys;
    for(int x=xs; x<xe; x++)
    {
        SetPixel(hdc,x,y,c);
        if(d>0)
        {
            y+=yi;
            d+=2*(dy-dx);
        }
        else
            d+=2*dy;
    }
}
void DrawHighPoint(HDC hdc,int xs,int ys,int xe,int ye,COLORREF c)
{
    int dx=xe-xs;
    int dy=ye-ys;
    int xi=1;
    if(dx<0)
    {
        dx=-dx;
        xi=-1;
    }
    int d=2*dx-dy;
    int x=xs;
    for(int y=ys; y<ye; y++)
    {
        SetPixel(hdc,x,y,c);
        if(d>0)
        {
            x+=xi;
            d+=2*(dx-dy);
        }
        else
            d+=2*(dx);
    }
}
void DrawLineBresenhams(HDC hdc,int xs,int ys,int xe,int ye,COLORREF c)
{
    if(abs(xe-xs)>abs(ye-ys))
    {
        if(xs>xe)
            DrawLowPoint(hdc,xe,ye,xs,ys,c);
        else
            DrawLowPoint(hdc,xs,ys,xe,ye,c);

    }
    else
    {
        if(ys>ye)
            DrawHighPoint(hdc,xe,ye,xs,ys,c);
        else
            DrawHighPoint(hdc,xs,ys,xe,ye,c);

    }
}
void Draw8Points(HDC hdc,int xc,int yc,int a,int b)
{
    DrawLineBresenhams(hdc,xc,yc,xc+a,yc+b,RGB(0,0,255));
    DrawLineBresenhams(hdc,xc,yc,xc+a,yc-b,RGB(255,0,0));
    DrawLineBresenhams(hdc,xc,yc,xc-a,yc+b,RGB(0,255,0));
    DrawLineBresenhams(hdc,xc,yc,xc-a,yc-b,RGB(100,255,0));
    DrawLineBresenhams(hdc,xc,yc,xc+b,yc+a,RGB(255,0,200));
    DrawLineBresenhams(hdc,xc,yc,xc-b,yc+a,RGB(0,200,255));
    DrawLineBresenhams(hdc,xc,yc,xc+b,yc-a,RGB(0,0,0));
    DrawLineBresenhams(hdc,xc,yc,xc-b,yc-a,RGB(255,255,255));
}
void DrawCircleBresenhams(HDC hdc,int xc,int yc,int radius)
{
    int x=0;
    int y=radius;
    Draw8Points(hdc,xc,yc,x,y);
    int d=1-radius;
    int d1=3;
    int d2=5-2*radius;
    while(x<y)
    {
        if(d<0)
        {
            d+=d1;
            d1+=2;
            d2+=2;
            x++;
        }
        else
        {
            d+=d2;
            d1+=2;
            d2+=4;
            y--;
            x++;
        }
        Draw8Points(hdc,xc,yc,x,y);
    }
}

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
int xs,ys,xe,ye;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDOWN:
    {
        xs=LOWORD(lParam);
        ys=HIWORD(lParam);
        break;
    }
    case WM_LBUTTONUP:
    {
        HDC hdc=GetDC(hwnd);
        xe=LOWORD(lParam);
        ye=HIWORD(lParam);
        // DrawLineBresenhams(hdc,xs,ys,xe,ye,RGB(255,0,0));
        double r=sqrt((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys));
        DrawCircleBresenhams(hdc,xs,ys,r) ;
        ReleaseDC(hwnd,hdc);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}