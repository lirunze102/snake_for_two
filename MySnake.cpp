// MySnake.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "stdio.h"
#include "resource.h"
#include "time.h"
#include <mmsystem.h>
#include <string.h>
#pragma comment(lib,"winmm.lib")
#define MAX_LOADSTRING 100
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYSNAKE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MYSNAKE);
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MYSNAKE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_MYSNAKE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	return RegisterClassEx(&wcex);
}

//���ϵ��
HDC hdc,mdc,bufdc;
int row=20;
int col=25;
int kk = 0;
int heightT = 200;
HBITMAP bg,bg2,pi1,pi2, food,pn[2];//��ͼ
#define  WIDTH 30
#define  HEIGHT 30
#define  MAXSIZE  50
//���� ���ݽṹ
struct Snake
{
   POINT m_pos[MAXSIZE];
   int m_direction;
   int len;
};
Snake m_snake,m_snake2;//��ʨ ����
#define NUMBER 15 //ʳ������
POINT m_food[NUMBER];//ʳ������
int foods[30][25],body[30][25];//��ֹ�ظ���ӡ

//ʳ���ʼ��
void foodbegin() {
	for (int i = 0; i < NUMBER; i++) {
		m_food[i].x = rand() %(col-1)+1;
		m_food[i].y = rand() %(row-1)+1;
		foods[m_food[i].x][m_food[i].y] = 1;
	}
}

//ʳ�����
void generatefood(int i)
{
	srand((unsigned)time(NULL));
	do {
		m_food[i].x = rand() % (col - 1) + 1;
		m_food[i].y = rand() % (row - 1) + 1;
	} while (foods[m_food[i].x][m_food[i].y] == 1);    //����ʳ���ظ�����
	foods[m_food[i].x][m_food[i].y] = 1;
}

//����ϵ����ʼ��
void init_snake()
{
	//��ʨ �����ʼ��
	m_snake.len=3;
	m_snake.m_pos[0].x=1;
	m_snake.m_pos[0].y=5;
	m_snake.m_direction=1;
	for(int n=1;n<MAXSIZE;n++)
	{
		m_snake.m_pos[n].x=-1;
		m_snake.m_pos[n].y=-1;
	}
	//���� �����ʼ��
	m_snake2.len = 3;
	m_snake2.m_pos[0].x =1;
	m_snake2.m_pos[0].y =row-5;
	m_snake2.m_direction = 1;
	for (int n = 1; n < MAXSIZE; n++)
	{
		m_snake2.m_pos[n].x = -1;
		m_snake2.m_pos[n].y = -1;
	}
	foodbegin();
}

//��������
void MyPaint(HDC hdc)
{
	//�볡Ч��
	kk++;
	
	//����λ��ϵ����
	memset(body, 0, sizeof(body));
	for (int ii = m_snake.len; ii > -1; ii--) 
		body[m_snake.m_pos->x][m_snake.m_pos->y] = 1;
	for (int ii = m_snake2.len; ii > -1; ii--) 
		body[m_snake2.m_pos->x][m_snake2.m_pos->y] = 1;
	
	//����������
	HBRUSH hbr=CreateSolidBrush(RGB(220, 205, 50));
	SelectObject(hdc,hbr);
	for(int y=1;y<row;y++)
		for (int x = 1; x < col; x++)
			if(foods[x][y]!=1&&body[x][y]!=1)
				Rectangle(hdc,x*WIDTH,y*HEIGHT,(x+1)*WIDTH,(y+1)*HEIGHT);
	
	//��Եǽ����
	HBRUSH hbr2 =CreateSolidBrush(RGB(255,0,0));
	SelectObject(hdc, hbr2);
	for (int y = 0; y <= row; y++)
		Rectangle(hdc, col*WIDTH, y*HEIGHT, (col + 1)*WIDTH, (y + 1)*HEIGHT);
	for (int y = 0; y <= row; y++)
		Rectangle(hdc, 0*WIDTH, y*HEIGHT, (0 + 1)*WIDTH, (y + 1)*HEIGHT);
	for (int x = 0; x < col; x++)
		Rectangle(hdc, x*WIDTH, row*HEIGHT, (x + 1)*WIDTH, (row + 1)*HEIGHT);
	for (int x = 0; x < col; x++)
		Rectangle(hdc, x*WIDTH, 0*HEIGHT, (x + 1)*WIDTH, (0 + 1)*HEIGHT);
	
	//�Ҳ�����������
	HFONT font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));
	SelectObject(hdc, font);
	//SetBkMode(hdc, TRANSPARENT); //��ɫ͸��
	char str1[8];
	sprintf(str1, "%4d km", m_snake.len);   //��ʨ����
	char str2[8];
	sprintf(str2, "%4d km", m_snake2.len);  //���޷���
	TextOut(hdc, col*WIDTH + 50, 90, "�ո�", 6);
	TextOut(hdc, col*WIDTH + 50, 120, "��ʼ/����", 10);
	TextOut(hdc, col*WIDTH + 60, heightT, "��ʨ",4);
	TextOut(hdc, col*WIDTH + 80, heightT+40, "��", 2);
	TextOut(hdc, col*WIDTH + 50, heightT+70, "������", 6);
	TextOut(hdc, col*WIDTH + 80, heightT+100, str1, 8);
	TextOut(hdc, col*WIDTH + 60, heightT+200, "����",4);
	TextOut(hdc, col*WIDTH + 80, heightT+240, "W ", 2);
	TextOut(hdc, col*WIDTH + 50, heightT+270, "A S D ", 6);
	TextOut(hdc, col*WIDTH + 80, heightT+300, str2, 8);
	SelectObject(mdc, bg2);
	BitBlt(hdc, col*WIDTH + 40, 0, 150, 80, mdc, 0, 0, SRCCOPY);  //ճ������ͼƬ
	
	//ʳ������
	for (int i = 0; i < NUMBER; i++) {
		SelectObject(mdc, food);
		BitBlt(hdc, m_food[i].x * WIDTH, m_food[i].y * HEIGHT, 30, 30, mdc, 0, 0, SRCCOPY);
	}

	//��ʨ��������
	HBRUSH hbrgreen=CreateSolidBrush(RGB(0,109,163));
	SelectObject(hdc,hbrgreen);
	for (int n = 1; n < m_snake.len; n++) {
		SelectObject(mdc, pi1);
		BitBlt(hdc, m_snake.m_pos[n].x * 30, m_snake.m_pos[n].y * 30, 30, 30, mdc, 0, 0, SRCCOPY);
	}
	/*	Rectangle(hdc,m_snake.m_pos[n].x*WIDTH,m_snake.m_pos[n].y*HEIGHT,(m_snake.m_pos[n].x+1)*WIDTH,
			(m_snake.m_pos[n].y+1)*HEIGHT);*/

	//������������
	HBRUSH hbrgreen2 = CreateSolidBrush(RGB(235, 95, 37));
	SelectObject(hdc, hbrgreen2);
	for (int n = 1; n < m_snake2.len; n++) {
		SelectObject(mdc, pi2);
		BitBlt(hdc, m_snake2.m_pos[n].x * 30, m_snake2.m_pos[n].y * 30, 30, 30, mdc, 0, 0, SRCCOPY);
	}
	/*	Rectangle(hdc, m_snake2.m_pos[n].x * WIDTH, m_snake2.m_pos[n].y * HEIGHT, (m_snake2.m_pos[n].x + 1) * WIDTH,
			(m_snake2.m_pos[n].y + 1) * HEIGHT);*/

	//ͷ������
	SelectObject(mdc, pn[0]);
	BitBlt(hdc, m_snake.m_pos[0].x * 30, m_snake.m_pos[0].y * 30, 30, 30, mdc, 0, 0, SRCCOPY);
	SelectObject(mdc, pn[1]);
	BitBlt(hdc, m_snake2.m_pos[0].x * 30, m_snake2.m_pos[0].y * 30, 30, 30, mdc, 0, 0, SRCCOPY);
	
}

//��Ϸ�ؿ�  ��ʼ
void OnStart(HWND hWnd)
{
	//ʳ���ʼ��
	memset(foods, 0, sizeof(foods));

	//�������ݳ�ʼ��
	init_snake();

	//���ü�ʱ��
	SetTimer(hWnd, 1, 100, NULL);
} 

//��������
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//���ڳ�ʼ��
   HWND hWnd;
   hInst = hInstance; 
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   RECT crc,wrc;
   GetClientRect(hWnd,&crc);
   GetWindowRect(hWnd,&wrc);
   int borderwidth=(wrc.right-wrc.left)-(crc.right-crc.left);
   int borderheight=(wrc.bottom-wrc.top)-(crc.bottom-crc.top);
   SetWindowPos(hWnd,NULL,0,0,950+borderwidth,630+borderheight,SWP_NOMOVE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //�������
   hdc=GetDC(hWnd);
   mdc = CreateCompatibleDC(hdc);
   bufdc = CreateCompatibleDC(hdc);

   //�زĵ���
   pn[0] = (HBITMAP)LoadImage(NULL, "H1.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
   pn[1] = (HBITMAP)LoadImage(NULL, "H2.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
   pi1 = (HBITMAP)LoadImage(NULL, "pi1.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
   pi2 = (HBITMAP)LoadImage(NULL, "pi2.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
   bg = (HBITMAP)LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, 333, 179, LR_LOADFROMFILE);
   bg2 = (HBITMAP)LoadImage(NULL, "bg2.bmp", IMAGE_BITMAP, 150, 80, LR_LOADFROMFILE);
   food = (HBITMAP)LoadImage(NULL, "shi.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);

   //���ű�������
   PlaySound("bgm.wav", NULL, SND_FILENAME | SND_ASYNC);

   return TRUE;
}

//�볡����
void start() 
{
	SelectObject(mdc, bg2);
	BitBlt(hdc, col*WIDTH + 40, 0, 150, 80, mdc, 0, 0, SRCCOPY);
	SelectObject(mdc, bg);
	BitBlt(hdc, 300, 200, 333, 3179, mdc, 0, 0, SRCCOPY);
}

//��Ϸ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int n;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
	//���뿪������
	if (kk == 1) 
		start();

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			switch (wmId)
			{
				case IDM_EXIT:
				   DestroyWindow(hWnd);         //�˳���Ϸ
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
			case WM_KEYDOWN:
				switch(wParam)
				{
				//���� �ؿ�
				case 32:
					OnStart(hWnd);
					break;
				//��ʨ����
				case VK_UP:
					m_snake.m_direction=0;
					break;
				case VK_RIGHT:
					m_snake.m_direction=1;
					break;
				case VK_DOWN:
					m_snake.m_direction=2;
					break;
				case VK_LEFT:
					m_snake.m_direction=3;
					break;
				//���޿���
				case 87:
					m_snake2.m_direction = 0;
					break;
				case 68:
					m_snake2.m_direction = 1;
					break;
				case 83:
					m_snake2.m_direction = 2;
					break;
				case 65:
					m_snake2.m_direction = 3;
					break;
				}
				break;
				case WM_TIMER:
					//��ʨǰ��
					for( n=m_snake.len-1;n>=1;n--)
						m_snake.m_pos[n]=m_snake.m_pos[n-1];
					if(0==m_snake.m_direction)
						m_snake.m_pos[0].y=m_snake.m_pos[0].y-1;
					if(1==m_snake.m_direction)
						m_snake.m_pos[0].x=m_snake.m_pos[0].x+1;
					if(2==m_snake.m_direction)
						m_snake.m_pos[0].y=m_snake.m_pos[0].y+1;
					if( 3==m_snake.m_direction)
						m_snake.m_pos[0].x=m_snake.m_pos[0].x-1;

					//����ǰ��
					for (n = m_snake2.len - 1; n >= 1; n--)
						m_snake2.m_pos[n] = m_snake2.m_pos[n - 1];
					if (0 == m_snake2.m_direction)
						m_snake2.m_pos[0].y = m_snake2.m_pos[0].y - 1;
					if (1 == m_snake2.m_direction)
						m_snake2.m_pos[0].x = m_snake2.m_pos[0].x + 1;
					if (2 == m_snake2.m_direction)
						m_snake2.m_pos[0].y = m_snake2.m_pos[0].y + 1;
					if (3 == m_snake2.m_direction)
						m_snake2.m_pos[0].x = m_snake2.m_pos[0].x - 1;

					//����ʳ���ж�
					for (int i = 0; i < NUMBER; i++) {
						if(m_snake.m_pos[0].x==m_food[i].x&&m_snake.m_pos[0].y==m_food[i].y)
						{
							m_snake.len++;  //��������
							foods[m_food[i].x][m_food[i].y] = 0;    //��ֹʳ���ظ���ӡϵ�� ����
							generatefood(i);  //ʳ�����
						}
						if (m_snake2.m_pos[0].x == m_food[i].x && m_snake2.m_pos[0].y == m_food[i].y)
						{
							m_snake2.len++;
							foods[m_food[i].x][m_food[i].y] = 0;
							generatefood(i);
						}
					}
				
					//��ʨ  �����ж�
					for (int i = 1; i < m_snake2.len; i++)
					{
						if (m_snake.m_pos[0].x == m_snake2.m_pos[i].x && m_snake.m_pos[0].y == m_snake2.m_pos[i].y)
						{
							KillTimer(hWnd, 1);
							if (IDYES == MessageBox(hWnd, "���޶��", "��ʾ", MB_YESNO))
								OnStart(hWnd);
							else
								PostQuitMessage(0);
						}
					}
					if(m_snake.m_pos[0].x>col||m_snake.m_pos[0].y>row||m_snake.m_pos[0].x<0||m_snake.m_pos[0].y<0)
					{
						KillTimer(hWnd,1);
						if(IDYES == MessageBox(hWnd, "���޶��", "��ʾ",MB_YESNO))
							OnStart(hWnd);
						else
							PostQuitMessage(0);
					}

					//����  �����ж�
					for (int i = 1; i < m_snake.len; i++)
					{
						if (m_snake2.m_pos[0].x == m_snake.m_pos[i].x && m_snake2.m_pos[0].y == m_snake.m_pos[i].y)
						{
							KillTimer(hWnd, 1);
							if (IDYES == MessageBox(hWnd, "��ʨ���", "��ʾ", MB_YESNO))
								OnStart(hWnd);
							else
								PostQuitMessage(0);
						}
					}
					if (m_snake2.m_pos[0].x > col || m_snake2.m_pos[0].y > row || m_snake2.m_pos[0].x < 0 || m_snake2.m_pos[0].y < 0)
					{
						KillTimer(hWnd,1);
						if (IDYES == MessageBox(hWnd, "��ʨ���", "��ʾ", MB_YESNO))
							OnStart(hWnd);
						else
							PostQuitMessage(0);
					}

					//����ˢ��
					hdc=GetDC(hWnd);
					MyPaint(hdc);
					break;
		
		case WM_PAINT:
			//�������
			hdc = BeginPaint(hWnd, &ps);
			MyPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}