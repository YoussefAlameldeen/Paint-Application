#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cmath>

#define FILE_MENU_LOAD_FILE 1
#define FILE_MENU_SAVE 2
#define FILE_MENU_CLEAR 3
#define COLOR_MENU_BLUE 4
#define COLOR_MENU_RED 5
#define COLOR_MENU_GREEN 6

using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void AddMenu(HWND);
HMENU hmenu;

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
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

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
           600,                 /* The programs width */
           550,                 /* and height in pixels */
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
int *loadData(HWND hwnd)
{
    ifstream load;
    load.open("DATA.txt");
    int a1,b1,a2,b2;
    int Size=4;
    load >> a1>>b1>>a2>>b2;
    load.close();
    int* d=new int[Size];
    d[0]=a1;
    d[1]=b1;
    d[2]=a2;
    d[3]=b2;
    return d;
}

void saveData(HWND hwnd,int a1,int b1,int a2,int b2)
{
    ofstream save;
    save.open("Current data.txt");
    save<<a1<<endl;
    save<<b1<<endl;
    save<<a2<<endl;
    save<<b2<<endl;
    save.close();
}
void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
SetPixel(hdc, xc+a, yc+b, color);
SetPixel(hdc, xc-a, yc+b, color);
SetPixel(hdc, xc-a, yc-b, color);
SetPixel(hdc, xc+a, yc-b, color);
SetPixel(hdc, xc+b, yc+a, color);
SetPixel(hdc, xc-b, yc+a, color);
SetPixel(hdc, xc-b, yc-a, color);
SetPixel(hdc, xc+b, yc-a, color);
}
void Draw4Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
SetPixel(hdc, xc+a, yc+b, color);
SetPixel(hdc, xc-a, yc+b, color);
SetPixel(hdc, xc-a, yc-b, color);
SetPixel(hdc, xc+a, yc-b, color);
}
void Swap(int x,int y)
{
int tmp=x;
x=y;
y=tmp;
}
                                        ////////////////////////////////////////////////////////////
                                        //////////////////////////LINE//////////////////////////////
                                        ////////////////////////////////////////////////////////////
//DDA
void LineSimpleDDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
int dx=xe-xs;
int dy=ye-ys;
SetPixel(hdc,xs,ys,color);
if(abs(dx)>=abs(dy))
{
int x=xs,xinc= dx>0?1:-1;
double y=ys,yinc=(double)dy/dx*xinc;
while(x!=xe)
{
x+=xinc;
y+=yinc;
SetPixel(hdc,x,round(y),color);
}
}

else
{
int y=ys,yinc= dy>0?1:-1;
double x=xs,xinc=(double)dx/dy*yinc;
while(y!=ye)
{
x+=xinc;
y+=yinc;
SetPixel(hdc,round(x),y,color);
}
}
}
//midpoint
void LineMidpoint(HDC hdc ,int x1,int y1 ,int x2,int y2,COLORREF color)
{

    int x=x1;
    int y=y1;
    int dx=x2-x1;
    int dy=y2-y1;
    int p=(dy*dy)-dx;
if(x1>=x2)
{
    Swap(x1,x2);
    Swap(y1,y2);


    for(x=x1;x<=x2;x++)
    {
        SetPixel(hdc,x,y,color);
        if(p<0)
        {
            p=p+(dy*dy);
        }
        else
            p=p+(dy*dy)-(dx*dx);
        y++;

    }}
 // if(y1>=y2)
else

{
    Swap(x1,x2);
    Swap(y1,y2);
}

    for(y=y1;y<=y2;y++)
    {
        SetPixel(hdc,x,y,color);
        if(p<0)
        {
            p=p+(dy*dy);


        }
        else
            p=p+(dy*dy)-(dx*dx);
        x++;

    }
    }
//parametric
void LineParametric(HDC hdc , int x1, int y1, int x2 , int y2 , COLORREF color)
{
    int dx=x2-x1;
    int dy=y2-y1;
    //double dt=1/maax(dx,dy);
    for (double t=0;t<1;t+=0.0001)
    {
        double x=x1+t*dx;
        double y=y1+t*dy;
        SetPixel(hdc,round(x),round(y),color);
    }
}
                                      /////////////////////////////////////////////////////////////////////////////
                                      /////////////////////////////////CIRCLE//////////////////////////////////////
                                      /////////////////////////////////////////////////////////////////////////////
//direct circle algorithms
void CircleDirect(HDC hdc,int xc,int yc,int x,int y, int R,COLORREF color)
{
 x=0 ,y=R;
int R2=R*R;
Draw8Points(hdc,xc,yc,x,y,color);
while(x<y)
{
x++;
y=round(sqrt((double)(R2-x*x)));
Draw8Points(hdc,xc,yc,x,y,color);
}
}
//iterative polar
void CircleIterativePolar(HDC hdc,int xc,int yc,double x,double y, int R,COLORREF color)
{
 x=R,y=0;
double dtheta=1.0/R;
double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
Draw8Points(hdc,xc,yc,R,0,color);
while(x>y)
{
double x1=x*cdtheta-y*sdtheta;
y=x*sdtheta+y*cdtheta;
x=x1;
Draw8Points(hdc,xc,yc,round(x),round(y),color);
}
}
//midpoint
void CircleBresenhammidpoint(HDC hdc,int xc,int yc,int x,int y, int R,COLORREF color)
{
x=0,y=R;
int d=1-R;
Draw8Points(hdc,xc,yc,x,y,color);
while(x<y)
{
if(d<0)
d+=2*x+2;
else
{

d+=2*(x-y)+5;
y--;
}
x++;
Draw8Points(hdc,xc,yc,x,y,color);
}
}
//modified midpoint
void CircleFasterBresenhammidpoint(HDC hdc,int xc,int yc,int x,int y, int R,COLORREF color)
{
x=0,y=R;
int d=1-R;
int c1=3, c2=5-2*R;
Draw8Points(hdc,xc,yc,x,y,color);
while(x<y)
{
if(d<0)
{
d+=c1;
c2+=2;
}
else
{

d+=c2;
c2+=4;
y--;
}
c1+=2;
x++;
Draw8Points(hdc,xc,yc,x,y,color);
}
}
                                    //////////////////////////////////////////////////////////////////////////////////
                                    //////////////////////////////////ELLIPS//////////////////////////////////////////
                                    //////////////////////////////////////////////////////////////////////////////////
void EllipseDirect(HDC hdc,int xc,int yc,int a,int b, int R,COLORREF color)
{
    double x=0,y=R;
    int y2=y*y, a2=a*a, b2=b*b;
    Draw4Points(hdc,xc,yc,R,0,color);
    while(x<a)
    {
        x++;
        int x2=x*x;
       // x=sqrt(((a2*b2 - y2*a2)/b2));
        y=sqrt(((a2*b2 - x2*b2)/a2));
        Draw4Points(hdc,xc,yc,x,round(y),color);
    }
}
void EllipsePolar(HDC hdc,int xc,int yc,int a,int b, int R,COLORREF color)
{
 double x=R,y=0;
 double theta;
 double dtheta=1.0/R;
 double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
 Draw4Points(hdc,xc,yc,R,0,color);
for(theta=0;theta<6.28;theta+=dtheta)
{
    x=a*cos(theta)*cdtheta - a*sin(theta)*sdtheta;
    y=b*cos(theta)*sdtheta + b*sin(theta)*cdtheta;
    Draw4Points(hdc,xc,yc,round(x),round(y),color);
}
}
                                             //////////////////////////////////////////////////////////////////
                                             ////////////////////////FILLING CIRCLE////////////////////////////
                                             //////////////////////////////////////////////////////////////////
void Draw_line(HDC hdc,int x1 ,int y1,int x2, int y2, COLORREF COLOR)
{
 double dx=x2-x1;
 double dy=y2-y1;
 double dt=1.0/std::max(dx,dy);
 for(double t=0; t<=1;t+=0.001)
 {
 int x=round(x1+t*(dx));
 int y=round(y1+t*(dy));
 SetPixel(hdc, x,y,RGB(255,99,71));
 }
}
void drawrightdownquarter(HDC hdc, int xc , int yc , int x, int y, COLORREF color)
{

SetPixel(hdc, xc+x , yc+y,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc+x,yc+y,RGB(255,99,71));
SetPixel(hdc, xc+y , yc+x,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc+y,yc+x,RGB(255,99,71));

}
void drawrightupquarter(HDC hdc, int xc , int yc , int x, int y, COLORREF color)
{

 SetPixel(hdc, xc+x , yc-y,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc+x,yc-y,RGB(255,99,71));
 SetPixel(hdc, xc+y , yc-x,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc+y,yc-x,RGB(255,99,71));
}
void drawleftupquarter(HDC hdc, int xc , int yc , int x, int y, COLORREF color)
{

 SetPixel(hdc, xc-x , yc-y,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc-x,yc-y,RGB(255,99,71));
 SetPixel(hdc, xc-y , yc-x,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc-y,yc-x,RGB(255,99,71));

}
void drawleftdownquarter(HDC hdc, int xc , int yc , int x, int y, COLORREF color)
{

 SetPixel(hdc, xc-x , yc+y,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc-x,yc+y,RGB(255,99,71));
 SetPixel(hdc, xc-y , yc+x,RGB(255,99,71));
 Draw_line(hdc,xc,yc,xc-y,yc+x,RGB(255,99,71));

}



 void circleDirect1_RD(HDC hdc,int xc, int yc ,int x,int y,int R,COLORREF color)
{
  x=0;
  y=R;

 while(x<y)
 {
 x++;
 y=round(sqrt((double)(R*R) -(x*x)));
drawrightdownquarter( hdc ,xc,yc,x, y,color);
 }
}
 void circleDirect2_RU(HDC hdc,int xc, int yc ,int x,int y,int R,COLORREF color)
{
  x=0;
  y=R;

 while(x<y)
 {
 x++;
 y=round(sqrt((double)(R*R) -(x*x)));
 drawrightupquarter( hdc ,xc,yc,x, y,color);
 }
}
 void circleDirect3_LU(HDC hdc,int xc, int yc ,int x,int y,int R,COLORREF color)
{
  x=0;
  y=R;

 while(x<y)
 {
 x++;
 y=round(sqrt((double)(R*R) -(x*x)));
 drawleftupquarter( hdc ,xc,yc,x, y,color);
 }
}
 void circleDirect4_LD(HDC hdc,int xc, int yc ,int x,int y,int R,COLORREF color)
{
x=0;
y=R;

 while(x<y)
 {
 x++;
 y=round(sqrt((double)(R*R) -(x*x)));
 drawleftdownquarter( hdc ,xc,yc,x, y,color);
 }
}

                                             //////////////////////////////////////////////////////////////////
                                             //////////////////////////CLIPPING////////////////////////////////
                                             //////////////////////////////////////////////////////////////////
// Defining region codes
const int INSIDE = 0; // 0000
const int LEFT = 1; // 0001
const int RIGHT = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8; // 1000

// Defining x_max, y_max and x_min, y_min for
// clipping rectangle. Since diagonal points are
// enough to define a rectangle
const int x_max = 300;
const int y_max = 220;
const int x_min = 100;
const int y_min = 100;

// Function to compute region code for a point(x, y)
int computeCode(double x, double y)
{
    // initialized as being inside
    int code = INSIDE;

    if (x < x_min) // to the left of rectangle
        code |= LEFT;
    else if (x > x_max) // to the right of rectangle
        code |= RIGHT;
    if (y < y_min) // below the rectangle
        code |= BOTTOM;
    else if (y > y_max) // above the rectangle
        code |= TOP;

    return code;
}

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
void cohenSutherlandClip(HDC hdc,double x1, double y1,double x2, double y2)
{
    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // If both endpoints lie within rectangle
            LineParametric(hdc,x1,y1,x2,y2,RGB(0,0,255));
            cout<<"accepted\n";
            break;
        }
        else if (code1 & code2) {
            // If both endpoints are outside rectangle,
            // in same region
            LineParametric(hdc,x1,y1,x2,y2,RGB(255,0,0));
            cout<<"not accepted\n";
            break;
        }
        else {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            double x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP) {
                // point is above the clip rectangle
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM) {
                // point is below the rectangle
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT) {
                // point is to the right of rectangle
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT) {
                // point is to the left of rectangle
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            // Now intersection point x, y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
            LineParametric(hdc,x1,y1,x2,y2,RGB(0,0,255));
            cout<<"one endpoint is inside\n";
        }
    }
}

void PointClipping(HDC hdc,int x1,int y1,int R,COLORREF color)
{
for(int angle = 0; angle <= 360; angle++ ){
    float const curve = 2 * 3.14 * (float)angle / (float)360;
    double x=x1 + sin(curve) * R;
    double y=y1 + cos(curve) * R;
    if(x>=x_min && x<= x_max && y<=y_max && y>=y_min)
    {
        SetPixel(hdc,x,y,RGB(0,0,255));
    }
}

}
void EllipseClipping(HDC hdc,int a,int b,int R,COLORREF color)
{
double theta;
double dtheta=1.0/R;
double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
for(theta=0;theta<6.28;theta+=dtheta)
{
    double x=a*cos(theta)*cdtheta - a*sin(theta)*sdtheta;
    double y=b*cos(theta)*sdtheta + b*sin(theta)*cdtheta;
    if(x>=x_min && x<= x_max && y<=y_max && y>=y_min)
    {
        SetPixel(hdc,x,y,RGB(0,0,255));
    }
}

}
                                              //////////////////////////////////////////////////////////////////
                                             //////////////////////////////////////////////////////////////////
                                             //////////////////////////////////////////////////////////////////
// int x1,y1,x2,y2;
 HWND hwnd;
int*d=loadData(hwnd);
 int X1=d[0];
 int Y1=d[1];
 int X2=d[2];
 int Y2=d[3];
int FChoice,SChoice;
int r=50;
COLORREF COLOR;
/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    RECT ret;
    ret.left=0,ret.right=1366;
    ret.bottom=768,ret.top=0;
    int a1,b1,a2,b2;
    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
            switch(wParam)
            {
            case FILE_MENU_LOAD_FILE:
                a1=X1,b1=Y1,a2=X2,b2=Y2;
                loadData(hwnd);
                cout<<"What to draw?\n1-> Line\n2-> Circle\n3-> Ellipse\n4-> Filling circle\n5-> Clipping\n\n";
                cin>>FChoice;
                switch(FChoice)
                {
                case 1:     //Line
                    cout<<"1->for dda\n2->for midpoint\n3->for parametric\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                        LineSimpleDDA(hdc,a1,b1,a2,b2,COLOR);
                        break;
                    case 2:
                        LineMidpoint(hdc,a1,b1,a2,b2,COLOR);
                        break;
                    case 3:
                        LineParametric(hdc,a1,b1,a2,b2,COLOR);
                        break;
                    }
                    break;
                case 2:     //Circle
                    cout<<"1->direct circle\n2->iterative polar\n3->midpoint\n4->modified midpoint\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                        CircleDirect(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                    case 2:
                        CircleIterativePolar(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                    case 3:
                        CircleBresenhammidpoint(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                    case 4:
                        CircleFasterBresenhammidpoint(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                    }
                    break;
                case 3:      //Ellipse
                    cout<<"1->direct ellipse\n2->polar ellipse\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                        EllipseDirect(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                    case 2:
                        EllipsePolar(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                    }
                    break;
                case 4:      //Filling
                     cout<<"1->FOR RIGHT DOWN QUARTER\n2->FOR RIGHT UP QUARTER\n3->LEFT UP QUARTER\n4->LEFT DOWN QUARTER\n\n";
                     cin>>SChoice;
                     switch(SChoice)
                     {
                     case 1:
                        circleDirect1_RD(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                     case 2:
                        circleDirect2_RU(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                     case 3:
                        circleDirect3_LU(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                     case 4:
                        circleDirect4_LD(hdc,a1,b1,a2,b2,r,COLOR);
                        break;
                     }
                     break;
                case 5:        //Clipping
                    cout<<"1->Clipping window\n2->line clipping\n3->circle clipping\n4->ellipse clipping\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                    LineParametric(hdc,100,100,100,220,RGB(0,0,0));
                    LineParametric(hdc,300,100,300,220,RGB(0,0,0));
                    LineParametric(hdc,100,220,300,220,RGB(0,0,0));
                    LineParametric(hdc,100,100,300,100,RGB(0,0,0));
                        break;
                    case 2:
                        cohenSutherlandClip(hdc,a1,b1,a2,b2);
                        break;
                    case 3:
                        PointClipping(hdc,a1,b1,r,RGB(0,0,0));
                        break;
                    case 4:
                        EllipseClipping(hdc,a1,b1,r,RGB(0,0,0));
                        break;
                    }
                    break;
                }
                break;
            case FILE_MENU_SAVE:
                saveData(hwnd,X1,Y1,X2,Y2);
                break;
            case FILE_MENU_CLEAR:
                FillRect(hdc,&ret,CreateSolidBrush(RGB(255,255,255)));
                break;
            case COLOR_MENU_BLUE:
                COLOR=RGB(0,0,255);
                break;
            case COLOR_MENU_RED:
                COLOR=RGB(255,0,0);
                break;
            case COLOR_MENU_GREEN:
                COLOR=RGB(0,255,0);
                break;
            }
            break;

        case WM_CREATE:
            AddMenu(hwnd);
            break;
        case WM_LBUTTONDBLCLK:
            X1=LOWORD(lParam);
            Y1=HIWORD(lParam);
            break;
        case WM_RBUTTONDBLCLK:
            X2=LOWORD(lParam);
            Y2=HIWORD(lParam);
            cout<<"What to draw?\n1-> Line\n2-> Circle\n3-> Ellipse\n4-> Filling circle\n5-> Clipping\n\n";
                cin>>FChoice;
                switch(FChoice)
                {
                case 1:     //Line
                    cout<<"1->for dda\n2->for midpoint\n3->for parametric\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                        LineSimpleDDA(hdc,X1,Y1,X2,Y2,COLOR);
                        break;
                    case 2:
                        LineMidpoint(hdc,X1,Y1,X2,Y2,COLOR);
                        break;
                    case 3:
                        LineParametric(hdc,X1,Y1,X2,Y2,COLOR);
                        break;
                    }
                    break;
                case 2:     //Circle
                    cout<<"1->direct circle\n2->iterative polar\n3->midpoint\n4->modified midpoint\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                        CircleDirect(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                    case 2:
                        CircleIterativePolar(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                    case 3:
                        CircleBresenhammidpoint(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                    case 4:
                        CircleFasterBresenhammidpoint(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                    }
                    break;
                case 3:      //Ellipse
                    cout<<"1->direct ellipse\n2->polar ellipse\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                        EllipseDirect(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                    case 2:
                        EllipsePolar(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                    }
                    break;
                case 4:      //Filling
                     cout<<"1->FOR RIGHT DOWN QUARTER\n2->FOR RIGHT UP QUARTER\n3->LEFT UP QUARTER\n4->LEFT DOWN QUARTER\n\n";
                     cin>>SChoice;
                     switch(SChoice)
                     {
                     case 1:
                        circleDirect1_RD(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                     case 2:
                        circleDirect2_RU(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                     case 3:
                        circleDirect3_LU(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                     case 4:
                        circleDirect4_LD(hdc,X1,Y1,X2,Y2,r,COLOR);
                        break;
                     }
                     break;
                case 5:        //Clipping
                    cout<<"1->Clipping window\n2->line clipping\n3->circle clipping\n4->ellipse clipping\n\n";
                    cin>>SChoice;
                    switch(SChoice)
                    {
                    case 1:
                    LineParametric(hdc,100,100,100,220,RGB(0,0,0));
                    LineParametric(hdc,300,100,300,220,RGB(0,0,0));
                    LineParametric(hdc,100,220,300,220,RGB(0,0,0));
                    LineParametric(hdc,100,100,300,100,RGB(0,0,0));
                        break;
                    case 2:
                        cohenSutherlandClip(hdc,X1,Y1,X2,Y2);
                        break;
                    case 3:
                        PointClipping(hdc,X1,Y1,r,RGB(0,0,0));
                        break;
                    case 4:
                        EllipseClipping(hdc,X1,Y1,r,RGB(0,0,0));
                        break;
                    }
                    break;
                }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

}

void AddMenu(HWND hwnd)
{
    hmenu = CreateMenu();
    HMENU hFilemenu = CreateMenu();
    HMENU hColormenu = CreateMenu();

    AppendMenu(hFilemenu,MF_STRING,FILE_MENU_LOAD_FILE,"Load File");
    AppendMenu(hFilemenu,MF_STRING,FILE_MENU_SAVE,"Save");
    AppendMenu(hFilemenu,MF_STRING,FILE_MENU_CLEAR,"Clear");
    AppendMenu(hColormenu,MF_STRING,COLOR_MENU_BLUE,"Blue");
    AppendMenu(hColormenu,MF_STRING,COLOR_MENU_RED,"Red");
    AppendMenu(hColormenu,MF_STRING,COLOR_MENU_GREEN,"Green");

    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hFilemenu,"File");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hColormenu,"Color");

    SetMenu(hwnd,hmenu);
}




