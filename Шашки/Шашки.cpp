#include "framework.h"
#include "Шашки.h"
#include <iostream>
#include <io.h>
#include <ctime>
#include <windows.h>
#include <cmath>
using namespace std;
int doska[8][8], c;
bool pred_xod[5] = { false , false,  false , false , false }, nuzno_li_est = false;
int hmen = 1, xchoose = 4, ychoose = 4, write1, xoldchoose = 4, yoldchoose = 4, sm = 1;
int cour = 2, indxod;
int xod, kol_vo_ed_shah_polz = -1, eat_shah_polz[2][12], kol_vo_eat_sh = 0, xold, yold;
char text3[] = "Игра против человека", text1[] = "Шашки", text4[] = "Правила", text5[] = "Выход", text2[] = "Игра против бота";
int enemy2[4], me2[4];
struct shaka {
	int x, y;
	bool alive;
	int name;
};
shaka enemy3[12], me3[12];
void about(HDC hdc);
void create_desk(HDC hdc);
void textwr(HDC hdc);
void men(HDC hdc);
void pokaz_win(HDC hdc, bool win);
void pravila(HDC hdc);
void red(HDC hdc);
void dark(HDC hdc, int x, int y);
bool next(int enemy1[], int me1[], shaka enemy[], shaka me[], bool* h, int may_eat[2][12]/*, int counter*/, int* kol_vo);
void zopoln_arr(int me1[], int enemy1[], shaka me[], shaka enemy[], int arr[2][18], int counter);
void sravn(int arr[2][18], int a[2][18], int counter);
bool xod_enemy(int me1[], int enemy1[], shaka me[], shaka enemy[], bool obaz_eat, int x, int y, int x1, int y1, int  may_xod[2][12], int* kol_vo);
void xod_bot(int enemy1[], int me1[], shaka enemy[], shaka me[], int arr[2][18], bool obaz_eat = false, int counter = 1, int may_xod[2][12] = { 0 }, int kol_vo = 1000000, int* eaten = NULL, int kol_vo1 = 0, int xp = 0, int yp = 0, int xe = -1, int ye = -1);
bool proverka_eat_i_vibor_ediashix(int me1[], int enemy1[], shaka enemy[], int may_xod[][12], int* kol_vo);
bool proverka_eat(int me1[], int enemy1[], shaka enemy[], int i, int may_xod[][12], int kol_vo);
bool proverka_win(int me1[], int enemy1[], shaka enemy[]);

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
RECT clientRect;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		GetClientRect(hWnd, &clientRect);
		HDC hdc = BeginPaint(hWnd, &ps);
		SetBkColor(hdc, RGB(58, 58, 148));
		HBRUSH newbrush = CreateSolidBrush(RGB(58, 58, 148));
		SelectObject(hdc, newbrush);
		FillRect(ps.hdc, &ps.rcPaint, newbrush);
		DeleteObject(newbrush);
		if (hmen == 1) {
			men(hdc);
		}
		else if (hmen == 4) {
			pravila(hdc);
		}
		else if (hmen == 3 || hmen == 2) {
			if (sm == 1) {
				create_desk(hdc);
			}
			else if (sm == 2) {
				dark(hdc, xoldchoose, yoldchoose);
				red(hdc);
				xoldchoose = xchoose;
				yoldchoose = ychoose;
				sm = 1;
			}
			else if (sm == 3) {
				textwr(hdc);
				sm = 1;
			}
			else if (sm == 4) {
				dark(hdc, xold, yold);
				red(hdc);
				sm = 1;
			}
		}
		else if (hmen == 5) {
			about(hdc);
		}
		else if (hmen == 7) {
			sm = 1;
			pokaz_win(hdc, nuzno_li_est);
		}
		//UpdateWindow(hWnd);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_UP: {
			if (hmen == 1) {
				cour = (cour + 2) % 5 + 2;
				InvalidateRect(hWnd, 0, true);
			}
			else if (hmen == 3 || hmen == 2) {
				ychoose = (ychoose + 1) % 8;
				if (ychoose % 2 == 1)
					xchoose++;
				else
					xchoose--;
				sm = 2;
				RECT r;
				r.left = xoldchoose * 52 + 300;
				r.right = xoldchoose * 52 + 52 + 300;
				r.bottom = 416 - yoldchoose * 52 + 15;
				r.top = 364 - yoldchoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
				r.left = xchoose * 52 + 300;
				r.right = xchoose * 52 + 52 + 300;
				r.bottom = 416 - ychoose * 52 + 15;
				r.top = 364 - ychoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
			}
		}break;
		case VK_DOWN: {
			if (hmen == 1) {
				cour = (cour - 1) % 5 + 2;
				InvalidateRect(hWnd, 0, true);
			}
			else if (hmen == 3 || hmen == 2) {
				ychoose = (ychoose + 7) % 8;
				if (ychoose % 2 == 1)
					xchoose++;
				else
					xchoose--;
				sm = 2;
				RECT r;
				r.left = xoldchoose * 52 + 300;
				r.right = xoldchoose * 52 + 52 + 300;
				r.bottom = 416 - yoldchoose * 52 + 15;
				r.top = 364 - yoldchoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
				r.left = xchoose * 52 + 300;
				r.right = xchoose * 52 + 52 + 300;
				r.bottom = 416 - ychoose * 52 + 15;
				r.top = 364 - ychoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
			}
		}break;
		case VK_RIGHT: {
			if (hmen == 3 || hmen == 2) {
				sm = 2;
				xchoose = (xchoose + 2) % 8;
				RECT r;
				r.left = xoldchoose * 52 + 300;
				r.right = xoldchoose * 52 + 52 + 300;
				r.bottom = 416 - yoldchoose * 52 + 15;
				r.top = 364 - yoldchoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
				r.left = xchoose * 52 + 300;
				r.right = xchoose * 52 + 52 + 300;
				r.bottom = 416 - ychoose * 52 + 15;
				r.top = 364 - ychoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
			}
		}break;
		case VK_LEFT: {
			if (hmen == 3 || hmen == 2) {
				sm = 2;
				xchoose = (xchoose + 6) % 8;
				RECT r;
				r.left = xoldchoose * 52 + 300;
				r.right = xoldchoose * 52 + 52 + 300;
				r.bottom = 416 - yoldchoose * 52 + 15;
				r.top = 364 - yoldchoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
				r.left = xchoose * 52 + 300;
				r.right = xchoose * 52 + 52 + 300;
				r.bottom = 416 - ychoose * 52 + 15;
				r.top = 364 - ychoose * 52 + 15;
				InvalidateRect(hWnd, &r, true);
			}
		}break;
		case VK_RETURN: {
			if (hmen == 1) {
				if (cour != 6) {
					hmen = cour;
					if (hmen == 3) {
						srand(static_cast<unsigned int>(time(0)));
						if (rand() % 2) {
							me2[0] = 1;
							enemy2[0] = 2;
							me2[1] = 3;
							enemy2[1] = 4;
						}
						else {
							me2[0] = 2;
							enemy2[0] = 1;
							me2[1] = 4;
							enemy2[1] = 3;
						}
						me2[2] = 0;
						enemy2[2] = 7;
						enemy2[3] = 1;
						me2[3] = -1;
						int* ind = new int, * ind1 = new int;
						*ind = 0;
						*ind1 = 0;
						for (int i = 0;i < 8;i++)
							for (int k = 0;k < 8;k++)
								if (i % 2 == 0 && k % 2 == 0 && (i == 0 || i == 2) || i % 2 && k % 2 && i == 1) {
									doska[i][k] = enemy2[0];
									enemy3[*ind].y = i;
									enemy3[*ind].x = k;
									enemy3[*ind].alive = true;
									enemy3[(*ind)++].name = enemy2[0];
								}
								else if (i % 2 && k % 2 && (i == 7 || i == 5) || i % 2 == 0 && k % 2 == 0 && i == 6) {
									doska[i][k] = me2[0];
									me3[*ind1].y = i;
									me3[*ind1].x = k;
									me3[*ind1].alive = true;
									me3[(*ind1)++].name = me2[0];
								}
								else
									doska[i][k] = 0;
						delete ind;
						delete ind1;

						/*for (int i = 0;i < 8;i++)
							for (int k = 0;k < 8;k++)
								doska[i][k] = 0;
						for (int i = 1;i < 12;i++)
							me3[i].alive = false;
						for (int i = 4;i < 12;i++)
							enemy3[i].alive = false;
						me3[0].name += 2;
						me3[0].x = 7;
						me3[0].y = 1;
						enemy3[0].x = 1;
						enemy3[0].y = 3;
						enemy3[1].x = 1;
						enemy3[1].y = 5;
						enemy3[2].x = 5;
						enemy3[2].y = 3;
						enemy3[3].x = 5;
						enemy3[3].y = 5;
						for (int i = 0;i < 1;i++)
							doska[me3[i].y][me3[i].x] = me3[i].name;
						for (int i=0;i<4;i++)
							doska[enemy3[i].y][enemy3[i].x] = enemy3[i].name;*/

						xod = (enemy2[0] == 1) ? 1 : 0;
						if (xod)
							nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2, /*me3,*/ enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
						else
							nuzno_li_est = proverka_eat_i_vibor_ediashix(enemy2, me2, /*enemy3,*/ me3, eat_shah_polz, &kol_vo_ed_shah_polz);
						kol_vo_ed_shah_polz++;
						kol_vo_eat_sh = 0;
						write1 = 1;
						InvalidateRect(hWnd, 0, true);

					}
					else if (hmen == 2) {
						srand(static_cast<unsigned int>(time(0)));
						if (rand() % 2) {
							me2[0] = 1;
							enemy2[0] = 2;
							me2[1] = 3;
							enemy2[1] = 4;
						}
						else {
							me2[0] = 2;
							enemy2[0] = 1;
							me2[1] = 4;
							enemy2[1] = 3;
						}
						me2[2] = 0;
						enemy2[2] = 7;
						enemy2[3] = 1;
						me2[3] = -1;
						int* ind = new int, * ind1 = new int;
						*ind = 0;
						*ind1 = 0;
						for (int i = 0;i < 8;i++)
							for (int k = 0;k < 8;k++)
								if (i % 2 == 0 && k % 2 == 0 && (i == 0 || i == 2) || i % 2 && k % 2 && i == 1) {
									doska[i][k] = enemy2[0];
									enemy3[*ind].y = i;
									enemy3[*ind].x = k;
									enemy3[*ind].alive = true;
									enemy3[(*ind)++].name = enemy2[0];
								}
								else if (i % 2 && k % 2 && (i == 7 || i == 5) || i % 2 == 0 && k % 2 == 0 && i == 6) {
									doska[i][k] = me2[0];
									me3[*ind1].y = i;
									me3[*ind1].x = k;
									me3[*ind1].alive = true;
									me3[(*ind1)++].name = me2[0];
								}
								else
									doska[i][k] = 0;
						delete ind;
						delete ind1;
						xod = (enemy2[0] == 1) ? 1 : 0;
						if (xod)
							nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2, /*me3,*/ enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
						else {
							int arr[2][18];
							arr[0][0] = 1;
							arr[1][0] = 1;
							c = 0;
							kol_vo_ed_shah_polz = -1;
							nuzno_li_est = proverka_eat_i_vibor_ediashix(enemy2, me2, /*enemy3,*/ me3, eat_shah_polz, &kol_vo_ed_shah_polz);
							pred_xod[2] = true;
							pred_xod[0] = nuzno_li_est;
							xod_bot(enemy2, me2, enemy3, me3, arr, nuzno_li_est, 1, eat_shah_polz, kol_vo_ed_shah_polz);
							xod = (xod + 1) % 2;
							nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2, /*me3,*/ enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
						}
						kol_vo_ed_shah_polz++;
						kol_vo_eat_sh = 0;
						write1 = 1;
						InvalidateRect(hWnd, 0, true);

					}
					else
						InvalidateRect(hWnd, 0, true);
				}
				else
					PostQuitMessage(0);
			}
			else if (hmen == 4) {
				hmen = 1;
				InvalidateRect(hWnd, 0, true);
			}
			else if (hmen == 3) {
				if (!pred_xod[0]) {
					if (xod) {
						if (nuzno_li_est) {
							for (int i = 0;i < kol_vo_ed_shah_polz;i++)
								if (xchoose == eat_shah_polz[0][i] && ychoose == eat_shah_polz[1][i]) {
									indxod = 0;
									while (!enemy3[indxod].alive || enemy3[indxod].x != eat_shah_polz[0][i] || enemy3[indxod].y != eat_shah_polz[1][i])indxod++;
									pred_xod[0] = true;
									break;
								}
						}
						else {
							for (int i = 0;i < 12;i++)
								if (enemy3[i].alive && enemy3[i].x == xchoose && enemy3[i].y == ychoose && (!doska[enemy3[i].y + 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y + 1][enemy3[i].x - 1] && enemy3[i].x > 0 || enemy3[i].name == enemy2[1] && (!doska[enemy3[i].y - 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y - 1][enemy3[i].x - 1] && enemy3[i].x > 0))) {
									indxod = i;
									pred_xod[0] = true;
									break;
								}
						}
					}
					else {
						if (nuzno_li_est) {
							for (int i = 0;i < kol_vo_ed_shah_polz;i++)
								if (xchoose == eat_shah_polz[0][i] && ychoose == eat_shah_polz[1][i]) {
									indxod = 0;
									while (!me3[indxod].alive || me3[indxod].x != eat_shah_polz[0][i] || me3[indxod].y != eat_shah_polz[1][i])indxod++;
									pred_xod[0] = true;
									break;
								}
						}
						else {
							for (int i = 0;i < 12;i++)
								if (me3[i].alive && me3[i].x == xchoose && me3[i].y == ychoose && (!doska[me3[i].y - 1][me3[i].x + 1] && me3[i].x < 7 || !doska[me3[i].y - 1][me3[i].x - 1] && me3[i].x > 0 || me3[i].name == me2[1] && (!doska[me3[i].y + 1][me3[i].x + 1] && me3[i].x < 7 || !doska[me3[i].y + 1][me3[i].x - 1] && me3[i].x > 0))) {
									indxod = i;
									pred_xod[0] = true;
									break;
								}
						}
					}
					write1 = (pred_xod[0]) ? 3 : 2;
					sm = 3;
					RECT r;
					r.left = 300;
					r.right = 716;
					r.bottom = 531;
					r.top = 431;
					InvalidateRect(hWnd, &r, true);
				}
				else {
					if (xod) {
						xold = enemy3[indxod].x;
						yold = enemy3[indxod].y;
						if (xod_enemy(me2, enemy2, me3, enemy3, nuzno_li_est, enemy3[indxod].x, enemy3[indxod].y, xchoose, ychoose, eat_shah_polz, &kol_vo_eat_sh)) {
							if (nuzno_li_est) {
								if (!proverka_eat(me2, enemy2,/* me3,*/ enemy3, indxod, eat_shah_polz, kol_vo_eat_sh)) {
									if (enemy3[indxod].name == enemy2[0] && enemy3[indxod].y == enemy2[2])
										enemy3[indxod].name += 2;
									if (proverka_win(enemy2, me2, /*enemy3,*/ me3)) {
										write1 = 1;
										xod = (xod + 1) % 2;
										pred_xod[0] = false;
										for (int i = 0;i < kol_vo_eat_sh;i++)
											doska[eat_shah_polz[1][i]][eat_shah_polz[0][i]] = 0;
										kol_vo_eat_sh = 0;
										kol_vo_ed_shah_polz = -1;
										nuzno_li_est = proverka_eat_i_vibor_ediashix(enemy2, me2, /*enemy3,*/ me3, eat_shah_polz, &kol_vo_ed_shah_polz);
										kol_vo_ed_shah_polz++;
										sm = 1;
										InvalidateRect(hWnd, 0, true);
									}
									else {
										nuzno_li_est = (me2[0] == 1) ? false : true;
										hmen = 7;
										InvalidateRect(hWnd, 0, true);
									}
								}
								else {
									sm = 4;
									RECT r;
									r.left = xold * 52 + 300;
									r.right = xold * 52 + 52 + 300;
									r.bottom = 416 - yold * 52 + 15;
									r.top = 364 - yold * 52 + 15;
									InvalidateRect(hWnd, &r, true);
									r.left = enemy3[indxod].x * 52 + 300;
									r.right = enemy3[indxod].x * 52 + 52 + 300;
									r.bottom = 416 - enemy3[indxod].y * 52 + 15;
									r.top = 364 - enemy3[indxod].y * 52 + 15;
									InvalidateRect(hWnd, &r, true);
								}
							}
							else {
								if (proverka_win(enemy2, me2, /*enemy3,*/ me3)) {
									if (enemy3[indxod].name == enemy2[0] && enemy3[indxod].y == enemy2[2])
										enemy3[indxod].name += 2;
									write1 = 1;
									pred_xod[0] = false;
									xod = (xod + 1) % 2;
									kol_vo_ed_shah_polz = -1;
									nuzno_li_est = proverka_eat_i_vibor_ediashix(enemy2, me2, /*enemy3,*/ me3, eat_shah_polz, &kol_vo_ed_shah_polz);
									kol_vo_ed_shah_polz++;
									sm = 4;
									RECT r;
									r.left = xold * 52 + 300;
									r.right = xold * 52 + 52 + 300;
									r.bottom = 416 - yold * 52 + 15;
									r.top = 364 - yold * 52 + 15;
									InvalidateRect(hWnd, &r, true);
									r.left = enemy3[indxod].x * 52 + 300;
									r.right = enemy3[indxod].x * 52 + 52 + 300;
									r.bottom = 416 - enemy3[indxod].y * 52 + 15;
									r.top = 364 - enemy3[indxod].y * 52 + 15;
									InvalidateRect(hWnd, &r, true);
								}
								else {
									nuzno_li_est = (me2[0] == 1) ? false : true;
									hmen = 7;
									InvalidateRect(hWnd, 0, true);
								}
							}
						}
						else {
							bool prav_choose = false;
							if (nuzno_li_est) {
								for (int i = 0;i < kol_vo_ed_shah_polz;i++)
									if (xchoose == eat_shah_polz[0][i] && ychoose == eat_shah_polz[1][i]) {
										indxod = 0;
										while (!enemy3[indxod].alive || enemy3[indxod].x != eat_shah_polz[0][i] || enemy3[indxod].y != eat_shah_polz[1][i])indxod++;
										prav_choose = true;
										break;
									}
							}
							else {
								for (int i = 0;i < 12;i++)
									if (enemy3[i].alive && enemy3[i].x == xchoose && enemy3[i].y == ychoose && (!doska[enemy3[i].y + 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y + 1][enemy3[i].x - 1] && enemy3[i].x > 0 || enemy3[i].name == enemy2[1] && (!doska[enemy3[i].y - 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y - 1][enemy3[i].x - 1] && enemy3[i].x > 0))) {
										indxod = i;
										prav_choose = true;
										break;
									}
							}
							write1 = (prav_choose) ? 3 : 4;
							sm = 3;
							RECT r;
							r.left = 300;
							r.right = 716;
							r.bottom = 531;
							r.top = 431;
							InvalidateRect(hWnd, &r, true);
						}
					}
					else {
						xold = me3[indxod].x;
						yold = me3[indxod].y;
						if (xod_enemy(enemy2, me2, enemy3, me3, nuzno_li_est, me3[indxod].x, me3[indxod].y, xchoose, ychoose, eat_shah_polz, &kol_vo_eat_sh)) {
							if (nuzno_li_est) {
								if (!proverka_eat(enemy2, me2, /*enemy3,*/ me3, indxod, eat_shah_polz, kol_vo_eat_sh)) {
									if (proverka_win(me2, enemy2, /*me3,*/ enemy3)) {
										if (me3[indxod].name == me2[0] && me3[indxod].y == me2[2])
											me3[indxod].name += 2;
										write1 = 1;
										xod = (xod + 1) % 2;
										pred_xod[0] = false;
										for (int i = 0;i < kol_vo_eat_sh;i++)
											doska[eat_shah_polz[1][i]][eat_shah_polz[0][i]] = 0;
										kol_vo_eat_sh = 0;
										kol_vo_ed_shah_polz = -1;
										nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2, /*me3,*/ enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
										kol_vo_ed_shah_polz++;
										sm = 1;
										InvalidateRect(hWnd, 0, true);
									}
									else {
										nuzno_li_est = (me2[0] == 1) ? true : false;
										hmen = 7;
										InvalidateRect(hWnd, 0, true);
									}
								}
								else {
									sm = 4;
									RECT r;
									r.left = xold * 52 + 300;
									r.right = xold * 52 + 52 + 300;
									r.bottom = 416 - yold * 52 + 15;
									r.top = 364 - yold * 52 + 15;
									InvalidateRect(hWnd, &r, true);
									r.left = me3[indxod].x * 52 + 300;
									r.right = me3[indxod].x * 52 + 52 + 300;
									r.bottom = 416 - me3[indxod].y * 52 + 15;
									r.top = 364 - me3[indxod].y * 52 + 15;
									InvalidateRect(hWnd, &r, true);
								}
							}
							else {
								if (proverka_win(me2, enemy2, /*me3,*/ enemy3)) {
									if (me3[indxod].name == me2[0] && me3[indxod].y == me2[2])
										me3[indxod].name += 2;
									write1 = 1;
									pred_xod[0] = false;
									xod = (xod + 1) % 2;
									kol_vo_ed_shah_polz = -1;
									nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2,/* me3,*/ enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
									kol_vo_ed_shah_polz++;
									sm = 4;
									RECT r;
									r.left = xold * 52 + 300;
									r.right = xold * 52 + 52 + 300;
									r.bottom = 416 - yold * 52 + 15;
									r.top = 364 - yold * 52 + 15;
									InvalidateRect(hWnd, &r, true);
									r.left = me3[indxod].x * 52 + 300;
									r.right = me3[indxod].x * 52 + 52 + 300;
									r.bottom = 416 - me3[indxod].y * 52 + 15;
									r.top = 364 - me3[indxod].y * 52 + 15;
									InvalidateRect(hWnd, &r, true);
								}
								else {
									nuzno_li_est = (me2[0] == 1) ? true : false;
									hmen = 7;
									InvalidateRect(hWnd, 0, true);
								}
							}
						}
						else {
							bool prav_choose = false;
							if (nuzno_li_est) {
								for (int i = 0;i < kol_vo_ed_shah_polz;i++)
									if (xchoose == eat_shah_polz[0][i] && ychoose == eat_shah_polz[1][i]) {
										indxod = 0;
										while (!me3[indxod].alive || me3[indxod].x != eat_shah_polz[0][i] || me3[indxod].y != eat_shah_polz[1][i])indxod++;
										prav_choose = true;
										break;
									}
							}
							else {
								for (int i = 0;i < 12;i++)
									if (me3[i].alive && me3[i].x == xchoose && me3[i].y == ychoose && (!doska[me3[i].y - 1][me3[i].x + 1] && me3[i].x < 7 || !doska[me3[i].y - 1][me3[i].x - 1] && me3[i].x > 0 || me3[i].name == me2[1] && (!doska[me3[i].y + 1][me3[i].x + 1] && me3[i].x < 7 || !doska[me3[i].y + 1][me3[i].x - 1] && me3[i].x > 0))) {
										indxod = i;
										prav_choose = true;
										break;
									}
							}
							write1 = (prav_choose) ? 3 : 4;
							sm = 3;
							RECT r;
							r.left = 300;
							r.right = 716;
							r.bottom = 531;
							r.top = 431;
							InvalidateRect(hWnd, &r, true);
						}
					}
				}
			}
			else if (hmen == 7 || hmen == 5) {
				hmen = 1;
				InvalidateRect(hWnd, 0, true);
			}
			else if (hmen == 2) {
				if (!pred_xod[0]) {
					if (nuzno_li_est) {
						for (int i = 0;i < kol_vo_ed_shah_polz;i++)
							if (xchoose == eat_shah_polz[0][i] && ychoose == eat_shah_polz[1][i]) {
								indxod = 0;
								while (!enemy3[indxod].alive || enemy3[indxod].x != eat_shah_polz[0][i] || enemy3[indxod].y != eat_shah_polz[1][i])indxod++;
								pred_xod[0] = true;
								break;
							}
					}
					else {
						for (int i = 0;i < 12;i++)
							if (enemy3[i].alive && enemy3[i].x == xchoose && enemy3[i].y == ychoose && (!doska[enemy3[i].y + 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y + 1][enemy3[i].x - 1] && enemy3[i].x > 0 || enemy3[i].name == enemy2[1] && (!doska[enemy3[i].y - 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y - 1][enemy3[i].x - 1] && enemy3[i].x > 0))) {
								indxod = i;
								pred_xod[0] = true;
								break;
							}
					}
					write1 = (pred_xod[0]) ? 3 : 2;
					sm = 3;
					RECT r;
					r.left = 300;
					r.right = 716;
					r.bottom = 531;
					r.top = 431;
					InvalidateRect(hWnd, &r, true);
				}
				else {
					xold = enemy3[indxod].x;
					yold = enemy3[indxod].y;
					if (xod_enemy(me2, enemy2, me3, enemy3, nuzno_li_est, enemy3[indxod].x, enemy3[indxod].y, xchoose, ychoose, eat_shah_polz, &kol_vo_eat_sh)) {
						if (nuzno_li_est) {
							if (!proverka_eat(me2, enemy2,  enemy3, indxod, eat_shah_polz, kol_vo_eat_sh)) {
								if (enemy3[indxod].name == enemy2[0] && enemy3[indxod].y == enemy2[2])
									enemy3[indxod].name += 2;
								if (proverka_win(enemy2, me2,me3)) {
									write1 = 1;
									xod = (xod + 1) % 2;
									pred_xod[0] = false;
									for (int i = 0;i < kol_vo_eat_sh;i++)
										doska[eat_shah_polz[1][i]][eat_shah_polz[0][i]] = 0;
									kol_vo_eat_sh = 0;
									kol_vo_ed_shah_polz = -1;
									nuzno_li_est = proverka_eat_i_vibor_ediashix(enemy2, me2,  me3, eat_shah_polz, &kol_vo_ed_shah_polz);
									int arr[2][18];
									arr[0][0] = 1;
									arr[1][0] = 1;
									c = 0;
									pred_xod[0] = nuzno_li_est;
									pred_xod[2] = true;
									xod_bot(enemy2, me2, enemy3, me3, arr, nuzno_li_est, 1, eat_shah_polz, kol_vo_ed_shah_polz);
									sm = 1;
									InvalidateRect(hWnd, 0, true);
									xod = (xod + 1) % 2;
									kol_vo_ed_shah_polz = -1;
									nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2,  enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
									kol_vo_ed_shah_polz++;

									if (!proverka_win(me2, enemy2,  enemy3)) {
										nuzno_li_est = (me2[0] == 1) ? true : false;
										hmen = 7;
									}
								}
								else {
									nuzno_li_est = (me2[0] == 1) ? false : true;
									hmen = 7;
									InvalidateRect(hWnd, 0, true);
								}
							}
							else {
								sm = 4;
								RECT r;
								r.left = xold * 52 + 300;
								r.right = xold * 52 + 52 + 300;
								r.bottom = 416 - yold * 52 + 15;
								r.top = 364 - yold * 52 + 15;
								InvalidateRect(hWnd, &r, true);
								r.left = enemy3[indxod].x * 52 + 300;
								r.right = enemy3[indxod].x * 52 + 52 + 300;
								r.bottom = 416 - enemy3[indxod].y * 52 + 15;
								r.top = 364 - enemy3[indxod].y * 52 + 15;
								InvalidateRect(hWnd, &r, true);
							}
						}
						else {
							if (enemy3[indxod].name == enemy2[0] && enemy3[indxod].y == enemy2[2])
								enemy3[indxod].name += 2;
							write1 = 1;
							pred_xod[0] = false;
							xod = (xod + 1) % 2;
							kol_vo_ed_shah_polz = -1;
							kol_vo_eat_sh = 0;
							nuzno_li_est = proverka_eat_i_vibor_ediashix(enemy2, me2,  me3, eat_shah_polz, &kol_vo_ed_shah_polz);
							int arr[2][18];
							arr[0][0] = 1;
							arr[1][0] = 1;
							c = 0;
							pred_xod[0] = nuzno_li_est;
							pred_xod[2] = true;
							xod_bot(enemy2, me2, enemy3, me3, arr, nuzno_li_est, 1, eat_shah_polz, kol_vo_ed_shah_polz);
							xod = (xod + 1) % 2;
							kol_vo_ed_shah_polz = -1;
							nuzno_li_est = proverka_eat_i_vibor_ediashix(me2, enemy2, enemy3, eat_shah_polz, &kol_vo_ed_shah_polz);
							kol_vo_ed_shah_polz++;
							if (!proverka_win(me2, enemy2,  enemy3)) {
								nuzno_li_est = (me2[0] == 1) ? true : false;
								hmen = 7;
							}
							sm = 1;
							InvalidateRect(hWnd, 0, true);
						}
						kol_vo_ed_shah_polz++;
					}
					else {
						bool prav_choose = false;
						if (nuzno_li_est) {
							for (int i = 0;i < kol_vo_ed_shah_polz;i++)
								if (xchoose == eat_shah_polz[0][i] && ychoose == eat_shah_polz[1][i]) {
									indxod = 0;
									while (!enemy3[indxod].alive || enemy3[indxod].x != eat_shah_polz[0][i] || enemy3[indxod].y != eat_shah_polz[1][i])indxod++;
									prav_choose = true;
									break;
								}
						}
						else {
							for (int i = 0;i < 12;i++)
								if (enemy3[i].alive && enemy3[i].x == xchoose && enemy3[i].y == ychoose && (!doska[enemy3[i].y + 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y + 1][enemy3[i].x - 1] && enemy3[i].x > 0 || enemy3[i].name == enemy2[1] && (!doska[enemy3[i].y - 1][enemy3[i].x + 1] && enemy3[i].x < 7 || !doska[enemy3[i].y - 1][enemy3[i].x - 1] && enemy3[i].x > 0))) {
									indxod = i;
									prav_choose = true;
									break;
								}
						}
						write1 = (prav_choose) ? 3 : 4;
						sm = 3;
						RECT r;
						r.left = 300;
						r.right = 716;
						r.bottom = 531;
						r.top = 431;
						InvalidateRect(hWnd, &r, true);
					}
				}
			}
		}break;
		case VK_BACK: {
			hmen = 1;
			sm = 1;
			InvalidateRect(hWnd, 0, true);
		}break;
		}
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
bool proverka_eat_i_vibor_ediashix(int me1[], int enemy1[], shaka enemy[], int may_xod[][12], int* kol_vo) {
	bool h = false;
	for (int i = 0;i < 12;i++) {
		if (enemy[i].alive) {
			if (enemy[i].name == enemy1[0]) {
				if ((doska[enemy[i].y + 1][enemy[i].x + 1] == me1[0] || doska[enemy[i].y + 1][enemy[i].x + 1] == me1[1]) && !doska[enemy[i].y + 2][enemy[i].x + 2] && enemy[i].y < 6 && enemy[i].x < 6 || (doska[enemy[i].y - 1][enemy[i].x + 1] == me1[0] || doska[enemy[i].y - 1][enemy[i].x + 1] == me1[1]) && !doska[enemy[i].y - 2][enemy[i].x + 2] && enemy[i].y > 1 && enemy[i].x < 6 || (doska[enemy[i].y + 1][enemy[i].x - 1] == me1[0] || doska[enemy[i].y + 1][enemy[i].x - 1] == me1[1]) && !doska[enemy[i].y + 2][enemy[i].x - 2] && enemy[i].y < 6 && enemy[i].x > 1 || (doska[enemy[i].y - 1][enemy[i].x - 1] == me1[0] || doska[enemy[i].y - 1][enemy[i].x - 1] == me1[1]) && !doska[enemy[i].y - 2][enemy[i].x - 2] && enemy[i].y > 1 && enemy[i].x > 1) {
					h = true;
					may_xod[0][++(*kol_vo)] = enemy[i].x;
					may_xod[1][*kol_vo] = enemy[i].y;
				}
			}
			else {
				bool m = true;
				for (int xprib = 1; xprib > -2; xprib -= 2)
					for (int yprib = 1; yprib > -2; yprib -= 2)
						if (m) {
							for (int i1 = enemy[i].y + yprib, k = enemy[i].x + xprib;k > -1 && i1 < 8 && k < 8 && i1 > -1 && doska[i1][k] != enemy1[0] && doska[i1][k] != enemy1[1];i1+=yprib, k += xprib)
								if ((doska[i1][k] == me1[0] || doska[i1][k] == me1[1]) && (i1 + yprib > -1 && yprib == -1 || i1 + yprib < 8 && yprib == 1) && (k + xprib < 8 && xprib == 1 || k + xprib > -1 && xprib == -1) && !doska[i1 + yprib][k + xprib]) {
									m = false;
									break;
								}
								else if (doska[i1][k] == me1[0] || doska[i1][k] == me1[1])
									break;
						}
				if (!m) {
					h = true;
					may_xod[0][++(*kol_vo)] = enemy[i].x;
					may_xod[1][*kol_vo] = enemy[i].y;
				}
			}
		}
	}
	return h;
}
bool proverka_eat(int me1[], int enemy1[], shaka enemy[], int i, int may_xod[][12], int kol_vo) {
	bool h = false;
	if (enemy[i].name == enemy1[0]) {
		for (int xprib = 1; xprib > -2; xprib -= 2)
			for (int yprib = 1; yprib > -2; yprib -= 2)
				if (!h && (doska[enemy[i].y + yprib][enemy[i].x + xprib] == me1[0] || doska[enemy[i].y + yprib][enemy[i].x + xprib] == me1[1]) && !doska[enemy[i].y + 2 * yprib][enemy[i].x + 2 * xprib] && (enemy[i].y < 6 && yprib == 1 || enemy[i].y > 1 && yprib == -1) && (enemy[i].x < 6 && xprib == 1 || enemy[i].x > 1 && xprib == -1)) {
					h = true;
					for (int re = 0;re < kol_vo;re++)
						if (may_xod[0][re] == enemy[i].x + xprib && may_xod[1][re] == enemy[i].y + yprib) {
							h = false;
							break;
						}
				}
	}
	else {
		bool m = true;
		for (int xprib = 1; xprib > -2; xprib -= 2)
			for (int yprib = 1; yprib > -2; yprib -= 2)
				if (m) {
					for (int i1 = enemy[i].y + yprib, k = enemy[i].x + xprib;i1 < 8 && k > -1 && k < 8 && i1 > -1 && doska[i1][k] != enemy1[0] && doska[i1][k] != enemy1[1];i1+= yprib, k+= xprib) {
						if ((doska[i1][k] == me1[0] || doska[i1][k] == me1[1]) && (i1 + yprib > -1 && yprib == -1 || i1 + yprib < 8 && yprib == 1)&& (k + xprib < 8 && xprib == 1 || k + xprib > -1 && xprib == -1) && !doska[i1 + yprib][k + xprib]) {
							m = false;
							for (int re = 0;re < kol_vo;re++)
								if (may_xod[0][re] == k && may_xod[1][re] == i1) {
									m = true;
									break;
								}
							break;
						}
						else if (doska[i1][k])
							break;
					}
				}
		if (!m) {
			h = true;
		}
	}
	return h;
}
bool proverka_win(int me1[], int enemy1[], shaka enemy[]) {
	bool h = false;
	for (int i = 0;i < 12;i++)
		if (enemy[i].alive && ((enemy[i].y + enemy1[3] < 8 && enemy[i].y + enemy1[3] > -1 && (enemy[i].x + 1 < 8 && !doska[enemy[i].y + enemy1[3]][enemy[i].x + 1] || enemy[i].x - 1 > -1 && !doska[enemy[i].y + enemy1[3]][enemy[i].x - 1]) || ((doska[enemy[i].y + 1][enemy[i].x + 1] == me1[0] || doska[enemy[i].y + 1][enemy[i].x + 1] == me1[1]) && !doska[enemy[i].y + 2][enemy[i].x + 2] && enemy[i].y < 6 && enemy[i].x < 6 || (doska[enemy[i].y - 1][enemy[i].x + 1] == me1[0] || doska[enemy[i].y - 1][enemy[i].x + 1] == me1[1]) && !doska[enemy[i].y - 2][enemy[i].x + 2] && enemy[i].y > 1 && enemy[i].x < 6 || (doska[enemy[i].y + 1][enemy[i].x - 1] == me1[0] || doska[enemy[i].y + 1][enemy[i].x - 1] == me1[1]) && !doska[enemy[i].y + 2][enemy[i].x - 2] && enemy[i].y < 6 && enemy[i].x > 1 || (doska[enemy[i].y - 1][enemy[i].x - 1] == me1[0] || doska[enemy[i].y - 1][enemy[i].x - 1] == me1[1]) && !doska[enemy[i].y - 2][enemy[i].x - 2] && enemy[i].y > 1 && enemy[i].x > 1)) || enemy[i].name == enemy1[1] && enemy[i].y - enemy1[3] < 8 && enemy[i].y - enemy1[3] > -1 && (enemy[i].x + 1 < 8 && !doska[enemy[i].y - enemy1[3]][enemy[i].x + 1] || enemy[i].x - 1 > -1 && !doska[enemy[i].y - enemy1[3]][enemy[i].x - 1]))) {
			h = true;
			break;
		}
	return h;
}
void xod_bot(int enemy1[], int me1[], shaka enemy[], shaka me[], int arr[2][18], bool obaz_eat, int counter, int may_xod[2][12], int kol_vo, int* eaten, int kol_vo1, int xp, int yp, int xe, int ye) {
	int a[2][18];
	for (int i = 5; i < 18;i++) {
		a[0][i] = arr[0][i];
		a[1][i] = arr[1][i];
	}
	bool pred_xod1 = pred_xod[2];
	int c1 = c;
	if (obaz_eat) {
		if (xe == -1) {
			eaten = new int[12];
			kol_vo++;
			for (int m = 0;m < kol_vo;m++) {
				int i = 0;
				while (i < 12 && (!me[i].alive || me[i].x != may_xod[0][m] || me[i].y != may_xod[1][m])) i++;
				if (me[i].name == me1[0]) {
					for (int xprib = 1; xprib > -2; xprib -= 2)
						for (int yprib = 1; yprib > -2; yprib -= 2)
							if ((doska[me[i].y + yprib][me[i].x + xprib] == enemy1[0] || doska[me[i].y + yprib][me[i].x + xprib] == enemy1[1]) && (me[i].y < 6 && yprib == 1 || me[i].y > 1 && yprib == -1) && (me[i].x < 6 && xprib == 1 || me[i].x > 1 && xprib == -1) && !doska[me[i].y + 2 * yprib][me[i].x + 2 * xprib]) {
								int j = 0;
								while (j < 12 && (!enemy[j].alive || enemy[j].x != me[i].x + xprib || enemy[j].y != me[i].y + yprib)) j++;
								eaten[0] = j;
								enemy[j].alive = false;
								doska[me[i].y][me[i].x] = 0;
								if (counter == 1) {
									a[0][5] = me[i].x;
									a[1][5] = me[i].y;
								}
								me[i].y += 2 * yprib;
								me[i].x += 2 * xprib;
								if (counter == 1) {
									a[0][6] = me[i].x;
									a[1][6] = me[i].y;
								}
								doska[me[i].y][me[i].x] = me[i].name;
								a[0][0] = 1;
								a[1][0] = 1;
								xod_bot(enemy1, me1, enemy, me, a, true, counter, 0, 0, eaten, 1, -xprib, -yprib, me[i].x, me[i].y);
								sravn(arr, a, counter);
								doska[me[i].y][me[i].x] = 0;
								me[i].y -= 2 * yprib;
								me[i].x -= 2 * xprib;
								doska[me[i].y][me[i].x] = me[i].name;
								enemy[j].alive = true;
								doska[enemy[j].y][enemy[j].x] = enemy[j].name;
							}
				}
				else {

					kol_vo1 = 0;
					doska[me[i].y][me[i].x] = 0;
					int yb = me[i].y, xb = me[i].x;
					int x, y;
					bool find;
					if (counter == 1) {
						a[0][5] = me[i].x;
						a[1][5] = me[i].y;
					}
					for (int xprib = 1; xprib > -2; xprib -= 2)
						for (int yprib = 1; yprib > -2; yprib -= 2) {
							x = me[i].x + xprib;
							y = me[i].y + yprib;
							find = false;
							while ((x < 7 && xprib == 1 || x > 0 && xprib == -1) && (y < 7 && yprib == 1 || y > 0 && yprib == -1) && doska[y][x] != me1[0] && doska[y][x] != me1[1]) {
								if (doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) {
									if (!doska[y + yprib][x + xprib])
										find = true;
									break;
								}
								x+= xprib;
								y+= yprib;
							}
							if (find) {
								int j = 0, x1 = x, y1 = y, x2, y2;
								while (j < 12 && (!enemy[j].alive || enemy[j].x != x || enemy[j].y != y)) j++;
								x += xprib;
								y += yprib;
								eaten[kol_vo1] = j;
								kol_vo1++;
								enemy[j].alive = false;
								bool find1 = true;
								while (x > -1 && y > -1 && x < 8 && y < 8 && doska[y][x] != me1[0] && doska[y][x] != me1[1]) {
									x2 = x + xprib;
									y2 = y - yprib;
									bool t = false;
									while ((x2 < 7 && xprib == 1 || x2 > 0 && xprib == -1) && (y2 < 7 && yprib == -1 || y2 > 0 && yprib == 1) && doska[y2][x2] != me1[0] && doska[y2][x2] != me1[1]) {
										if (doska[y2][x2] == enemy1[0] || doska[y2][x2] == enemy1[1]) {
											t = true;
											break;
										}
										x2+= xprib;
										y2-= yprib;
									}
									if (t && !doska[y2 - yprib][x2 + xprib] && !doska[y][x]) {
										me[i].x = x;
										me[i].y = y;
										find1 = false;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = me[i].x;
											a[1][kol_vo1 + 5] = me[i].y;
										}
										a[0][0] = 1;
										a[1][0] = 1;
										xod_bot(enemy1, me1, enemy, me, a, true, counter, 0, 0, eaten, kol_vo1, -xprib, -yprib, me[i].x, me[i].y);
										sravn(arr, a, counter);
									}
									t = false;
									x2 = x - xprib;
									y2 = y + yprib;
									while ((x2 < 7 && xprib == -1 || x2 > 0 && xprib == 1) && (y2 < 7 && yprib == 1 || y2 > 0 && yprib == -1) && doska[y2][x2] != me1[0] && doska[y2][x2] != me1[1]) {
										if (doska[y2][x2] == enemy1[0] || doska[y2][x2] == enemy1[1]) {
											t = true;
											break;
										}
										x2-= xprib;
										y2+= yprib;
									}
									if (t && !doska[y2 + yprib][x2 - xprib] && !doska[y][x]) {
										me[i].x = x;
										me[i].y = y;
										find1 = false;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = me[i].x;
											a[1][kol_vo1 + 5] = me[i].y;
										}
										a[0][0] = 1;
										a[1][0] = 1;
										xod_bot(enemy1, me1, enemy, me, a, true, counter, 0, 0, eaten, kol_vo1, -xprib, -yprib, me[i].x, me[i].y);
										sravn(arr, a, counter);
									}
									if ((doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) && (y + yprib < 8 && yprib == 1 || y + yprib > -1 && yprib == -1) && (x + xprib < 8 && xprib == 1 || x + xprib > -1 && xprib == -1)) {
										if (doska[y + yprib][x + xprib])
											break;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = x - xprib;
											a[1][kol_vo1 + 5] = y - yprib;
										}
										j = 0;
										while (j < 12 && (!enemy[j].alive || enemy[j].x != x || enemy[j].y != y)) j++;
										eaten[kol_vo1++] = j;
										find1 = true;
										x1 = enemy[j].x;
										y1 = enemy[j].y;
										enemy[j].alive = false;
										x += 2 * xprib;
										y += 2 * yprib;
										continue;
									}
									x+= xprib;
									y+= yprib;
								}
								if (find1) {
									for (int re = 0;re < kol_vo1;re++) {
										doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = 0;
									}
									x1 += xprib;
									y1 += yprib;
									while (x1 > -1 && y1 > -1 && x1 < 8 && y1 < 8 && !doska[y1][x1]) {
										me[i].x = x1;
										me[i].y = y1;
										doska[y1][x1] = me[i].name;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = me[i].x;
											a[1][kol_vo1 + 5] = me[i].y;
										}
										int may_xod1[2][12];
										int kol_vo2 = -1;
										bool h;
										if (next(enemy1, me1, enemy, me, &h, may_xod1/*, counter*/, &kol_vo2)) {
											a[0][0] = 1;
											a[1][0] = 1;
											xod_bot(me1, enemy1, me, enemy, a, h, counter + 1, may_xod1, kol_vo2);
											if (c1 != c)
												c--;
											sravn(arr, a, counter);
										}
										else {
											zopoln_arr(enemy1, me1, enemy, me, a, counter);
											sravn(arr, a, counter);
										}
										if (pred_xod1 != pred_xod[2])
											pred_xod[2] = pred_xod1;
										doska[y1][x1] = 0;
										x1 += xprib;
										y1 += yprib;
									}
								}
								for (int re = 0;re < kol_vo1;re++) {
									enemy[eaten[re]].alive = true;
									doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = enemy[eaten[re]].name;
								}
								kol_vo1 = 0;
								me[i].x = xb;
								me[i].y = yb;
							}
						}
					doska[me[i].y][me[i].x] = me[i].name;
				}

			}
			if (counter == 1) {
				kol_vo1 = 6;
				kol_vo = 0;
				int i = 0;
				while (i < 12 && (!me[i].alive || me[i].x != arr[0][5] || me[i].y != arr[1][5])) i++;
				do {
					int xpr, ypr;
					if (arr[0][kol_vo1] - me[i].x > 0) {
						xpr = 1;
						ypr = (arr[1][kol_vo1] - me[i].y > 0) ? 1 : -1;
					}
					else {
						xpr = -1;
						ypr = (arr[1][kol_vo1] - me[i].y > 0) ? 1 : -1;
					}
					if (me[i].name == me1[0]) {
						doska[me[i].y][me[i].x] = 0;
						int j = 0;
						while (j < 12 && (!enemy[j].alive || enemy[j].x != me[i].x + xpr || enemy[j].y != me[i].y + ypr)) j++;
						enemy[j].alive = false;
						may_xod[0][kol_vo] = enemy[j].x;
						may_xod[1][kol_vo++] = enemy[j].y;
						me[i].y += 2 * ypr;
						me[i].x += 2 * xpr;
						doska[me[i].y][me[i].x] = me[i].name;
					}
					else {
						int x = me[i].x + xpr, y = me[i].y + ypr;
						while (doska[y][x] != enemy1[0] && doska[y][x] != enemy1[1]) {
							x += xpr;
							y += ypr;
						}
						int j = 0;
						while (j < 12 && (!enemy[j].alive || enemy[j].x != x || enemy[j].y != y)) j++;
						enemy[j].alive = false;
						may_xod[0][kol_vo] = enemy[j].x;
						may_xod[1][kol_vo++] = enemy[j].y;
						doska[me[i].y][me[i].x] = 0;
						me[i].y = arr[1][kol_vo1];
						me[i].x = arr[0][kol_vo1];
						doska[me[i].y][me[i].x] = me[i].name;
					}
					kol_vo1++;
				} while (proverka_eat(enemy1, me1, /*enemy,*/ me, i, may_xod, kol_vo));

				if (me[i].y == me1[2] && me[i].name == me1[0])
					me[i].name += 2;
				for (i = 0;i < kol_vo;i++) {
					doska[may_xod[1][i]][may_xod[0][i]] = 0;
				}
			}
		}
		else {
			int i = 0;
			while (i < 12 && (!me[i].alive || me[i].x != xe || me[i].y != ye)) i++;
			if (me[i].name == me1[0]) {
				bool h = true;
				for (int xprib = 1; xprib > -2; xprib -= 2)
					for (int yprib = 1; yprib > -2; yprib -= 2)
						if ((xp != xprib || yp != yprib) && (doska[me[i].y + yprib][me[i].x + xprib] == enemy1[0] || doska[me[i].y + yprib][me[i].x + xprib] == enemy1[1]) && (me[i].y < 6 && yprib == 1 || me[i].y > 1 && yprib == -1) && (me[i].x < 6 && xprib == 1 || me[i].x > 1 && xprib == -1) && !doska[me[i].y + 2 * yprib][me[i].x + 2 * xprib]) {
							bool pr = true;
							for (int re = 0;re < kol_vo1;re++)
								if (enemy[eaten[re]].x == me[i].x + xprib && enemy[eaten[re]].y == me[i].y + yprib) {
									pr = false;
									break;
								}
							if (pr) {
								h = false;
								int j = 0;
								while (j < 12 && (!enemy[j].alive || enemy[j].x != me[i].x + xprib || enemy[j].y != me[i].y + yprib)) j++;
								eaten[kol_vo1] = j;
								enemy[j].alive = false;
								doska[me[i].y][me[i].x] = 0;
								me[i].y += 2 * yprib;
								me[i].x += 2 * xprib;
								if (counter == 1) {
									a[0][kol_vo1 + 6] = me[i].x;
									a[1][kol_vo1 + 6] = me[i].y;
								}
								doska[me[i].y][me[i].x] = me[i].name;
								a[0][0] = 1;
								a[1][0] = 1;
								xod_bot(enemy1, me1, enemy, me, a, true, counter, 0, 0, eaten, kol_vo1 + 1, -xprib, -yprib, me[i].x, me[i].y);
								sravn(arr, a, counter);
								doska[me[i].y][me[i].x] = 0;
								me[i].y -= 2 * yprib;
								me[i].x -= 2 * xprib;
								doska[me[i].y][me[i].x] = me[i].name;
								enemy[j].alive = true;
							}
						}
				if (h) {
					for (int re = 0;re < kol_vo1;re++) {
						doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = 0;
					}
					int may_xod1[2][12];
					int kol_vo2 = -1;
					bool h;
					if (me[i].y == me1[2]) {
						me[i].name += 2;
						doska[me[i].y][me[i].x] = me[i].name;
					}
					if (next(enemy1, me1, enemy, me, &h, may_xod1/*, counter*/, &kol_vo2)) {
						a[0][0] = 1;
						a[1][0] = 1;
						xod_bot(me1, enemy1, me, enemy, a, h, counter + 1, may_xod1, kol_vo2);
						if (c1 != c)
							c--;
						sravn(arr, a, counter);
					}
					else {
						zopoln_arr(enemy1, me1, enemy, me, a, counter);
						sravn(arr, a, counter);
					}
					if (pred_xod1 != pred_xod[2])
						pred_xod[2] = pred_xod1;
					if (me[i].y == me1[2]) {
						me[i].name -= 2;
						doska[me[i].y][me[i].x] = me[i].name;
					}
					for (int re = 0;re < kol_vo1;re++) {
						doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = enemy[eaten[re]].name;
					}
				}
			}
			else {

				int x, y;
				int yb = me[i].y, xb = me[i].x;
				bool find, ytr = true;
				int ind = kol_vo1;
				for (int xprib = 1; xprib > -2; xprib -= 2)
					for (int yprib = 1; yprib > -2; yprib -= 2) {
						x = me[i].x + xprib, y = me[i].y + yprib;
						find = false;
						if ((xp != xprib || yp != yprib) && (xp != -xprib || yp != -yprib)) {
							while ((x < 7 && xprib == 1 || x > 0 && xprib == -1) && (y < 7 && yprib == 1 || y > 0 && yprib == -1) && doska[y][x] != me1[0] && doska[y][x] != me1[1]) {
								if (doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) {
									if (!doska[y + yprib][x + xprib])
										find = true;
									for (int re = 0;re < kol_vo1;re++) {
										if (enemy[eaten[re]].x == x && enemy[eaten[re]].y == y) {
											find = false;
											break;
										}
									}
									break;
								}
								x += xprib;
								y += yprib;
							}
							if (find) {
								ytr = false;
								int j = 0, x1 = x, y1 = y, x2, y2;
								while (j < 12 && (!enemy[j].alive || enemy[j].x != x || enemy[j].y != y)) j++;
								x += xprib;
								y += yprib;
								eaten[kol_vo1] = j;
								kol_vo1++;
								enemy[j].alive = false;
								bool find1 = true;
								while (x > -1 && y > -1 && x < 8 && y < 8 && doska[y][x] != me1[0] && doska[y][x] != me1[1]) {
									x2 = x + xprib;
									y2 = y - yprib;
									bool t = false;
									while ((x2 < 7 && xprib == 1 || x2 > 0 && xprib == -1) && (y2 < 7 && yprib == -1 || y2 > 0 && yprib == 1) && doska[y2][x2] != me1[0] && doska[y2][x2] != me1[1]) {
										if (doska[y2][x2] == enemy1[0] || doska[y2][x2] == enemy1[1]) {
											t = true;
											for (int re = 0;re < kol_vo1;re++) {
												if (enemy[eaten[re]].x == x2 && enemy[eaten[re]].y == y2) {
													t = false;
													break;
												}
											}
											break;
										}
										x2+= xprib;
										y2-= yprib;
									}
									if (t && !doska[y2 - yprib][x2 + xprib] && !doska[y][x]) {
										me[i].x = x;
										me[i].y = y;
										find1 = false;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = me[i].x;
											a[1][kol_vo1 + 5] = me[i].y;
										}
										a[0][0] = 1;
										a[1][0] = 1;
										xod_bot(enemy1, me1, enemy, me, a, true, counter, 0, 0, eaten, kol_vo1, -xprib, -yprib, me[i].x, me[i].y);
										sravn(arr, a, counter);
									}
									t = false;
									x2 = x - xprib;
									y2 = y + yprib;
									while ((x2 < 7 && xprib == -1 || x2 > 0 && xprib == 1) && (y2 < 7 && yprib == 1 || y2 > 0 && yprib == -1) && doska[y2][x2] != me1[0] && doska[y2][x2] != me1[1]) {
										if (doska[y2][x2] == enemy1[0] || doska[y2][x2] == enemy1[1]) {
											t = true;
											for (int re = 0;re < kol_vo1;re++) {
												if (enemy[eaten[re]].x == x2 && enemy[eaten[re]].y == y2) {
													t = false;
													break;
												}
											}
											break;
										}
										x2-= xprib;
										y2+= yprib;
									}
									if (t && !doska[y2 + yprib][x2 - xprib] && !doska[y][x]) {
										me[i].x = x;
										me[i].y = y;
										find1 = false;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = me[i].x;
											a[1][kol_vo1 + 5] = me[i].y;
										}
										a[0][0] = 1;
										a[1][0] = 1;
										xod_bot(enemy1, me1, enemy, me, a, true, counter, 0, 0, eaten, kol_vo1, -xprib, -yprib, me[i].x, me[i].y);
										sravn(arr, a, counter);
									}
									if ((doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) && (y + yprib < 8 && yprib == 1 || y + yprib > -1 && yprib == -1) && (x + xprib < 8 && xprib == 1 || x + xprib > -1 && xprib == -1)) {
										t = false;
										for (int re = 0;re < kol_vo1;re++) {
											if (enemy[eaten[re]].x == x && enemy[eaten[re]].y == y) {
												t = true;
												break;
											}
										}
										if (doska[y + yprib][x + xprib] || t)
											break;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = x - xprib;
											a[1][kol_vo1 + 5] = y - yprib;
										}
										j = 0;
										while (j < 12 && (!enemy[j].alive || enemy[j].x != x || enemy[j].y != y)) j++;
										eaten[kol_vo1++] = j;
										find1 = true;
										x1 = enemy[j].x;
										y1 = enemy[j].y;
										enemy[j].alive = false;
										x += 2 * xprib;
										y += 2 * yprib;
										continue;
									}
									x+= xprib;
									y+= yprib;
								}
								if (find1) {
									for (int re = 0;re < kol_vo1;re++) {
										doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = 0;
									}
									x1+= xprib;
									y1+= yprib;
									while (x1 > -1 && y1 > -1 && x1 < 8 && y1 < 8 && !doska[y1][x1]) {
										bool t = false;
										for (int re = 0;re < kol_vo1;re++) {
											if (enemy[eaten[re]].x == x1 && enemy[eaten[re]].y == y1) {
												t = true;
												break;
											}
										}
										if (t)
											break;
										me[i].x = x1;
										me[i].y = y1;
										if (counter == 1) {
											a[0][kol_vo1 + 5] = me[i].x;
											a[1][kol_vo1 + 5] = me[i].y;
										}
										doska[y1][x1] = me[i].name;
										int may_xod1[2][12];
										int kol_vo2 = -1;
										bool h;
										if (next(enemy1, me1, enemy, me, &h, may_xod1/*, counter*/, &kol_vo2)) {
											a[0][0] = 1;
											a[1][0] = 1;
											xod_bot(me1, enemy1, me, enemy, a, h, counter + 1, may_xod1, kol_vo2);
											if (c1 != c)
												c--;
											sravn(arr, a, counter);
										}
										else {
											zopoln_arr(enemy1, me1, enemy, me, a, counter);
											sravn(arr, a, counter);
										}
										if (pred_xod1 != pred_xod[2])
											pred_xod[2] = pred_xod1;
										doska[y1][x1] = 0;
										y1+= yprib;
										x1+= xprib;
									}
								}
								for (int re = 0;re < ind;re++) {
									doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = enemy[eaten[re]].name;
								}
								for (int re = ind;re < kol_vo1;re++) {
									enemy[eaten[re]].alive = true;
									doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = enemy[eaten[re]].name;
								}
								kol_vo1 = ind;
								me[i].x = xb;
								me[i].y = yb;
							}
						}
					}
				if (ytr) {
					x = xb;
					y = yb;
					for (int re = 0;re < kol_vo1;re++) {
						doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = 0;
					}
					while (x > -1 && y > -1 && x < 8 && y < 8 && (!doska[y][x] || doska[y][x] == me1[1])) {
						bool t = false;
						for (int re = 0;re < kol_vo1;re++) {
							if (enemy[eaten[re]].x == x && enemy[eaten[re]].x == y) {
								t = true;
								break;
							}
						}
						if (t)
							break;
						me[i].x = x;
						me[i].y = y;
						if (counter == 1) {
							a[0][kol_vo1 + 5] = me[i].x;
							a[1][kol_vo1 + 5] = me[i].y;
						}
						doska[y][x] = me[i].name;
						int may_xod1[2][12];
						int kol_vo2 = -1;
						bool h;
						if (next(enemy1, me1, enemy, me, &h, may_xod1/*, counter*/, &kol_vo2)) {
							a[0][0] = 1;
							a[1][0] = 1;
							xod_bot(me1, enemy1, me, enemy, a, h, counter + 1, may_xod1, kol_vo2);
							if (c1 != c)
								c--;
							sravn(arr, a, counter);
						}
						else {
							zopoln_arr(enemy1, me1, enemy, me, a, counter);
							sravn(arr, a, counter);
						}
						if (pred_xod1 != pred_xod[2])
							pred_xod[2] = pred_xod1;
						doska[y][x] = 0;
						y -= yp;
						x -= xp;
					}
					me[i].x = xb;
					me[i].y = yb;
					for (int re = 0;re < kol_vo1;re++) {
						doska[enemy[eaten[re]].y][enemy[eaten[re]].x] = enemy[eaten[re]].name;
					}
				}
			}
		}
	}
	else {
		for (int i = 0;i < 12;i++) {
			if (me[i].alive) {
				if (me[i].name == me1[0]) {
					for (int xprib = 1; xprib > -2; xprib -= 2)
						if (me[i].x + xprib > -1 && me[i].x + xprib < 8 && !doska[me[i].y + me1[3]][me[i].x + xprib]) {
							doska[me[i].y][me[i].x] = 0;
							me[i].x += xprib;
							me[i].y += me1[3];
							if (me[i].y == me1[2]) {
								me[i].name += 2;
							}
							doska[me[i].y][me[i].x] = me[i].name;
							int may_xod1[2][12];
							kol_vo = -1;
							bool h;
							if (next(enemy1, me1, enemy, me, &h, may_xod1/*, counter*/, &kol_vo)) {
								if (counter == 1) {
									a[0][5] = me[i].x - xprib;
									a[1][5] = me[i].y - me1[3];
									a[0][6] = me[i].x;
									a[1][6] = me[i].y;
								}
								a[0][0] = 1;
								a[1][0] = 1;

								xod_bot(me1, enemy1, me, enemy, a, h, counter + 1, may_xod1, kol_vo);
								if (c1 != c)
									c--;
								sravn(arr, a, counter);
							}
							else {
								if (counter == 1) {
									a[0][5] = me[i].x - xprib;
									a[1][5] = me[i].y - me1[3];
									a[0][6] = me[i].x;
									a[1][6] = me[i].y;
								}
								zopoln_arr(enemy1, me1, enemy, me, a, counter);
								sravn(arr, a, counter);
							}
							if (pred_xod1 != pred_xod[2])
								pred_xod[2] = pred_xod1;

							if (me[i].y == me1[2]) {
								me[i].name -= 2;
							}
							doska[me[i].y][me[i].x] = 0;
							me[i].x -= xprib;
							me[i].y -= me1[3];
							doska[me[i].y][me[i].x] = me[i].name;
						}
				}
				else {
					int xb = me[i].x, yb = me[i].y;
					if (counter == 1) {
						a[0][5] = xb;
						a[1][5] = yb;
					}
					for (int xprib = 1; xprib > -2; xprib -= 2)
						for (int yprib = 1; yprib > -2; yprib -= 2) {
							while (!doska[me[i].y + yprib][me[i].x + xprib] && (me[i].y < 7 && yprib == 1 || me[i].y >0 && yprib == -1) && (me[i].x < 7 && xprib == 1 || me[i].x >0 && xprib == -1)) {
								doska[me[i].y][me[i].x] = 0;
								me[i].x += xprib;
								me[i].y += yprib;
								doska[me[i].y][me[i].x] = me[i].name;
								int may_xod1[2][12];
								kol_vo = -1;
								bool h;
								if (next(enemy1, me1, enemy, me, &h, may_xod1/*, counter*/, &kol_vo)) {
									if (counter == 1) {
										a[0][6] = me[i].x;
										a[1][6] = me[i].y;
									}
									a[0][0] = 1;
									a[1][0] = 1;
									xod_bot(me1, enemy1, me, enemy, a, h, counter + 1, may_xod1, kol_vo);
									if (c1 != c)
										c--;
									sravn(arr, a, counter);
								}
								else {
									if (counter == 1) {
										a[0][6] = me[i].x;
										a[1][6] = me[i].y;
									}
									zopoln_arr(enemy1, me1, enemy, me, a, counter);
									sravn(arr, a, counter);
								}
								if (pred_xod1 != pred_xod[2])
									pred_xod[2] = pred_xod1;
							}
							doska[me[i].y][me[i].x] = 0;
							me[i].y = yb;
							me[i].x = xb;
							doska[me[i].y][me[i].x] = me[i].name;
						}
				}
			}
		}
		if (counter == 1) {
			int i = 0;
			while (i < 12 && (!me[i].alive || me[i].x != arr[0][5] || me[i].y != arr[1][5])) i++;
			doska[me[i].y][me[i].x] = 0;
			me[i].x = arr[0][6];
			me[i].y = arr[1][6];
			if (arr[1][6] == me1[2] && me[i].name == me1[0])
				me[i].name += 2;
			doska[arr[1][6]][arr[0][6]] = me[i].name;
		}
	}
}
void sravn(int arr[2][18], int a[2][18], int counter) {
	bool h = false;
	srand(static_cast<unsigned int>(time(0)));
	if (counter % 2 == 1) {
		if (arr[0][0] == 0 && a[0][0] == 1 || arr[1][0] == 1 && a[1][0] == 0 || arr[0][0] == 1 && a[0][0] == 1 || arr[1][0] == 1 && a[1][0] == 1) {
			h = true;
		}
		else if (arr[0][0] == 0 && a[0][0] == 0 && arr[1][0] == 0 && a[1][0] == 0) {
			if (arr[1][1] > a[1][1] && (-arr[0][3] + arr[1][3] >= -a[0][3] + a[1][3] || a[0][3] > 4 && -arr[0][3] + arr[1][3] < -a[0][3] + a[1][3])) {
				h = true;
			}
			else if (arr[1][1] == a[1][1] || a[0][3] < 5 || arr[0][3] < 5) {
				if (arr[0][1] < a[0][1]) {
					h = true;
				}
				else if (arr[0][1] == a[0][1]) {

					if (arr[1][2] > a[1][2] && (a[0][3] > 4 || a[0][3] < 5 && -arr[0][3] + arr[1][3] >= -a[0][3] + a[1][3])) {
						h = true;
					}
					else if (arr[1][2] == a[1][2] || a[0][3] < 5 || arr[0][3] < 5) {
						if (arr[0][2] < a[0][2]) {
							h = true;
						}
						else if (arr[0][2] == a[0][2]) {

							if (-arr[0][3] + arr[1][3] > -a[0][3] + a[1][3]) {
								h = true;
							}
							else if (-arr[0][3] + arr[1][3] == -a[0][3] + a[1][3]) {
								if (arr[0][4] < a[0][4]) {
									h = true;
								}
								else if (arr[0][4] == a[0][4] && counter == 1) {
									h = rand() % 2;
								}
							}

						}
					}

				}
			}
		}
	}
	else {
		if (arr[1][0] == 0 && a[1][0] == 1 || arr[0][0] == 1 && a[0][0] == 0 || arr[1][0] == 1 && a[1][0] == 1 || arr[0][0] == 1 && a[0][0] == 1) {
			h = true;
		}
		else if (arr[1][0] == 0 && a[1][0] == 0 && arr[0][0] == 0 && a[0][0] == 0) {
			if (arr[0][1] > a[0][1] && (-arr[1][3] + arr[0][3] >= -a[1][3] + a[0][3] || a[1][3] > 4 && -arr[1][3] + arr[0][3] < -a[1][3] + a[0][3])) {
				h = true;
			}
			else if (arr[0][1] == a[0][1] || a[1][3] < 5 || arr[1][3] < 5) {
				if (arr[1][1] < a[1][1]) {
					h = true;
				}
				else if (arr[1][1] == a[1][1]) {

					if (arr[0][2] > a[0][2] && (a[1][3] > 4 || a[1][3] < 5 && -arr[1][3] + arr[0][3] >= -a[1][3] + a[0][3])) {
						h = true;
					}
					else if (arr[0][2] == a[0][2] || a[1][3] < 5 || arr[1][3] < 5) {
						if (arr[1][2] < a[1][2]) {
							h = true;
						}
						else if (arr[1][2] == a[1][2]) {

							if (-arr[1][3] + arr[0][3] > -a[1][3] + a[0][3]) {
								h = true;
							}
							else if (-arr[1][3] + arr[0][3] == -a[1][3] + a[0][3]) {
								if (arr[1][4] < a[1][4]) {
									h = true;
								}
								else if (arr[1][4] == a[1][4]) {
									h = rand() % 2;
								}
							}

						}
					}

				}
			}
		}
	}
	if (h)
		for (int i = 0;i < 18;i++) {
			arr[0][i] = a[0][i];
			arr[1][i] = a[1][i];
		}
}
void men(HDC hdc) {
	static LOGFONT il;
	il.lfHeight = 50;
	il.lfCharSet = DEFAULT_CHARSET;
	il.lfPitchAndFamily = DEFAULT_PITCH;
	il.lfWidth = 20;
	il.lfItalic = 0;
	il.lfUnderline = 0;
	strcpy_s(il.lfFaceName, "Times New Roman");
	HFONT hFont;
	hFont = CreateFontIndirect(&il);
	SelectObject(hdc, hFont);
	TextOut(hdc, clientRect.left + 425, clientRect.top, (LPCSTR)text1, 5);
	il.lfHeight = 25;
	il.lfWidth = 15;
	hFont = CreateFontIndirect(&il);
	SelectObject(hdc, hFont);
	if (cour == 3) {
		TextOut(hdc, clientRect.left + 438, clientRect.top + 250, (LPCSTR)text4, 7);
		TextOut(hdc, clientRect.left + 449, clientRect.top + 400, (LPCSTR)text5, 5);
		TextOut(hdc, clientRect.left + 365, clientRect.top + 100, (LPCSTR)text2, 16);
		char text6[] = "О программе";
		TextOut(hdc, clientRect.left + 400, clientRect.top + 325, (LPCSTR)text6, 11);
		COLORREF oldcol = SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)text3, 20);
		SetTextColor(hdc, oldcol);
	}
	else if (cour == 4) {
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)text3, 20);
		TextOut(hdc, clientRect.left + 449, clientRect.top + 400, (LPCSTR)text5, 5);
		TextOut(hdc, clientRect.left + 365, clientRect.top + 100, (LPCSTR)text2, 16);
		char text6[] = "О программе";
		TextOut(hdc, clientRect.left + 400, clientRect.top + 325, (LPCSTR)text6, 11);
		COLORREF oldcol = SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, clientRect.left + 438, clientRect.top + 250, (LPCSTR)text4, 7);
		SetTextColor(hdc, oldcol);
	}
	else if (cour == 5) {
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)text3, 20);
		TextOut(hdc, clientRect.left + 438, clientRect.top + 250, (LPCSTR)text4, 7);
		TextOut(hdc, clientRect.left + 365, clientRect.top + 100, (LPCSTR)text2, 16);
		TextOut(hdc, clientRect.left + 449, clientRect.top + 400, (LPCSTR)text5, 5);
        COLORREF oldcol = SetTextColor(hdc, RGB(255, 0, 0));
		char text6[] = "О программе";
		TextOut(hdc, clientRect.left + 400, clientRect.top + 325, (LPCSTR)text6, 11);
		SetTextColor(hdc, oldcol);
	}
	else if (cour == 2) {
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)text3, 20);
		TextOut(hdc, clientRect.left + 438, clientRect.top + 250, (LPCSTR)text4, 7);
		TextOut(hdc, clientRect.left + 449, clientRect.top + 400, (LPCSTR)text5, 5);
		char text6[] = "О программе";
		TextOut(hdc, clientRect.left + 400, clientRect.top + 325, (LPCSTR)text6, 11);
		COLORREF oldcol = SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, clientRect.left + 365, clientRect.top + 100, (LPCSTR)text2, 16);
		SetTextColor(hdc, oldcol);
	}
	else {
		TextOut(hdc, clientRect.left + 365, clientRect.top + 100, (LPCSTR)text2, 16);
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)text3, 20);
		TextOut(hdc, clientRect.left + 438, clientRect.top + 250, (LPCSTR)text4, 7);
		char text6[] = "О программе";
		TextOut(hdc, clientRect.left + 400, clientRect.top + 325, (LPCSTR)text6, 11);
		COLORREF oldcol = SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, clientRect.left + 449, clientRect.top + 400, (LPCSTR)text5, 5);
		SetTextColor(hdc, oldcol);
	}
	DeleteObject(hFont);
}
void pravila(HDC hdc) {
	static LOGFONT il;
	il.lfHeight = 50;
	il.lfCharSet = DEFAULT_CHARSET;
	il.lfPitchAndFamily = DEFAULT_PITCH;
	il.lfWidth = 20;
	il.lfItalic = 0;
	il.lfUnderline = 0;
	strcpy_s(il.lfFaceName, "Times New Roman");
	HFONT hFont;
	il.lfHeight = 25;
	il.lfWidth = 10;
	hFont = CreateFontIndirect(&il);
	SelectObject(hdc, hFont);
	char pr[90] = "1.Перемещение шашки происходит на одно поле вперед по диагонали.";
	TextOut(hdc, clientRect.left + 70, clientRect.top, (LPCSTR)pr, 64);
	strcpy_s(pr, "2.Бить шашки можно как вперёд, так и назад.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 25, (LPCSTR)pr, 43);
	strcpy_s(pr, "3.Бить шашки обязательно.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 50, (LPCSTR)pr, 25);
	strcpy_s(pr, "4.Правило турецкого удара. Одну шашку можно бить только один раз за ход,");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 75, (LPCSTR)pr, 71);
	strcpy_s(pr, "но через свободное поле можно ходить несколько раз.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 100, (LPCSTR)pr, 51);
	strcpy_s(pr, "5.Шашка, дойдя до последнего ряда доски, становится дамкой");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 125, (LPCSTR)pr, 58);
	strcpy_s(pr, "6.При наличии нескольких возможных комбинаций ударных ходов, можно");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 150, (LPCSTR)pr, 66);
	strcpy_s(pr, "выбрать любую, но обязательно захватить все шашки в этой комбинации.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 175, (LPCSTR)pr, 68);
	strcpy_s(pr, "7.Дамка, после съедения шашки противника, может встать на любое");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 200, (LPCSTR)pr, 63);
	strcpy_s(pr, "свободное поле за ней.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 225, (LPCSTR)pr, 22);
	strcpy_s(pr, "8.Цель игры – уничтожить все шашки противника или добиться такой");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 250, (LPCSTR)pr, 62);
	strcpy_s(pr, "ситуации, что они не смогут совершить ход.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 275, (LPCSTR)pr, 42);
	strcpy_s(pr, "9.Красный квадрат можно перемещать используя стрелки вправо, влево, вверх, вниз.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 300, (LPCSTR)pr, 80);
	strcpy_s(pr, "10.Чтобы сходиться нужно выбрать шашку, переместив на неё квадрат");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 325, (LPCSTR)pr, 65);
	strcpy_s(pr, "и нажав Enter, а потом точно также выбрать место куда нужно сходиться.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 350, (LPCSTR)pr, 70);
	strcpy_s(pr, "11.Можно выбрать другую шашку, переместив на неё квадрат и нажав Enter.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 375, (LPCSTR)pr, 71);
	strcpy_s(pr, "12.Если ход заключается в взятии нескольких вражеских шашек,");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 400, (LPCSTR)pr, 59);
	strcpy_s(pr, "нужно поочерёдно указывать все промежуточные клетки.");
	TextOut(hdc, clientRect.left + 70, clientRect.top + 425, (LPCSTR)pr, 52);
	DeleteObject(hFont);
}
void create_desk(HDC hdc) {
	HBRUSH newbrushrkl = CreateSolidBrush(RGB(255, 0, 0)), newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH), newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH), newbrushdkl = CreateSolidBrush(RGB(110, 110, 110)), newbrushwkl = CreateSolidBrush(RGB(222, 232, 130));
	HPEN newpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)), newpenno = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	RECT r;
	SelectObject(hdc, newpen);
	for (int i = 7;i > -1;i--)
		for (int k = 0;k < 8;k++) {
			r.left = k * 52 + 300;
			r.right = k * 52 + 52 + 300;
			r.bottom = 416 - i * 52 + 15;
			r.top = 364 - i * 52 + 15;
			if (i % 2 == 1 && k % 2 == 0 || i % 2 == 0 && k % 2 == 1)
				FillRect(hdc, &r, newbrushwkl);
			else if (ychoose == i && xchoose == k)
				FillRect(hdc, &r, newbrushrkl);
			else
				FillRect(hdc, &r, newbrushdkl);
			SelectObject(hdc, newpenno);
			if (doska[i][k] == 1) {
				HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
				Ellipse(hdc, k * 52 + 300, 364 - i * 52 + 15, k * 52 + 52 + 300, 416 - i * 52 + 15);
				SelectObject(hdc, oldbrush);
				DeleteObject(oldbrush);
			}
			else if (doska[i][k] == 2) {
				HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
				Ellipse(hdc, k * 52 + 300, 364 - i * 52 + 15, k * 52 + 52 + 300, 416 - i * 52 + 15);
				SelectObject(hdc, oldbrush);
				DeleteObject(oldbrush);
			}
			else if (doska[i][k] == 3) {
				HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
				Ellipse(hdc, k * 52 + 300, 364 - i * 52 + 15, 52 + k * 52 + 300, 416 - i * 52 + 15);
				SelectObject(hdc, newbrushdsh);
				Ellipse(hdc, k * 52 + 13 + 300, 377 - i * 52 + 15, 39 + k * 52 + 300, 403 - i * 52 + 15);
				SelectObject(hdc, oldbrush);
				DeleteObject(oldbrush);
			}
			else if (doska[i][k] == 4) {
				HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
				Ellipse(hdc, k * 52 + 300, 364 - i * 52 + 15, 52 + k * 52 + 300, 416 - i * 52 + 15);
				SelectObject(hdc, newbrushwsh);
				Ellipse(hdc, 13 + k * 52 + 300, 377 - i * 52 + 15, 39 + k * 52 + 300, 403 - i * 52 + 15);
				SelectObject(hdc, oldbrush);
				DeleteObject(oldbrush);
			}
			SelectObject(hdc, newpen);
			static LOGFONT il;
			il.lfHeight = 25;
			il.lfCharSet = DEFAULT_CHARSET;
			il.lfPitchAndFamily = DEFAULT_PITCH;
			il.lfWidth = 10;
			il.lfItalic = 0;
			il.lfUnderline = 0;
			HFONT hFont;
			hFont = CreateFontIndirect(&il);
			SelectObject(hdc, hFont);
			if (write1 == 1) {
				char pr[] = "Выберите шашку";
				TextOut(hdc, clientRect.left + 425, clientRect.top + 436, (LPCSTR)pr, 14);
			}
			else if (write1 == 2) {
				char pr[] = "Выберите другую шашку";
				TextOut(hdc, clientRect.left + 390, clientRect.top + 436, (LPCSTR)pr, 21);
			}
			else if (write1 == 3) {
				char pr[] = "Выберите позицию, куда вы сходитесь";
				TextOut(hdc, clientRect.left + 310, clientRect.top + 436, (LPCSTR)pr, 35);
			}
			else {
				char pr[] = "Такой ход невозможен";
				TextOut(hdc, clientRect.left + 395, clientRect.top + 436, (LPCSTR)pr, 20);
			}
			DeleteObject(hFont);
		}
	DeleteObject(newbrushrkl);
	DeleteObject(newpen);
	DeleteObject(newpenno);
	DeleteObject(newbrushwsh);
	DeleteObject(newbrushdsh);
	DeleteObject(newbrushdkl);
	DeleteObject(newbrushwkl);
}
void pokaz_win(HDC hdc, bool win) {
	static LOGFONT il;
	il.lfHeight = 50;
	il.lfCharSet = DEFAULT_CHARSET;
	il.lfPitchAndFamily = DEFAULT_PITCH;
	il.lfWidth = 20;
	il.lfItalic = 0;
	il.lfUnderline = 0;
	strcpy_s(il.lfFaceName, "Times New Roman");
	HFONT hFont;
	hFont = CreateFontIndirect(&il);
	SelectObject(hdc, hFont);
	if (win) {
		char ch_win[] = "Победа белых";
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)ch_win, 12);
	}
	else {
		char ch_win[] = "Победа чёрных";
		TextOut(hdc, clientRect.left + 335, clientRect.top + 175, (LPCSTR)ch_win, 13);
	}
	DeleteObject(hFont);
}
bool next(int enemy1[], int me1[], shaka enemy[], shaka me[], bool* h, int may_eat[2][12], int* kol_vo) {
	if (c == 0) {
		if (proverka_win(me1, enemy1,/* me,*/ enemy)) {
			*h = proverka_eat_i_vibor_ediashix(me1, enemy1, /*me,*/ enemy, may_eat, kol_vo);
			if (pred_xod[0]) {
				if (!(*h)) {
					pred_xod[1] = *h;
					c++;
				}
			}
			else {
				pred_xod[1] = *h;
				c++;
			}
			return true;
		}
		else
			return false;
	}
	else if (c == 1) {
		if (proverka_win(me1, enemy1,/* me,*/ enemy)) {
			*h = proverka_eat_i_vibor_ediashix(me1, enemy1, /*me,*/ enemy, may_eat, kol_vo);

			if (pred_xod[0]) {
				if ((*h)) {
					c++;
					return true;
				}
				return false;
			}
			else {
				if (!pred_xod[1]) {
					if ((*h)) {
						c++;
						return true;
					}
					int may_eat1[2][12], kol_vo_pr = -1;
					if (proverka_eat_i_vibor_ediashix(enemy1, me1, /*enemy,*/ me, may_eat1, &kol_vo_pr) && pred_xod[2]) {
						pred_xod[2] = false;
						return true;
					}
					return false;
				}
				else {
					if (!(*h)) {
						c++;
					}
					return true;
				}
			}
		}
		else
			return false;
	}
	else if (c == 2) {
		if (proverka_win(me1, enemy1, /*me,*/ enemy)) {
			*h = proverka_eat_i_vibor_ediashix(me1, enemy1, /*me,*/ enemy, may_eat, kol_vo);
			if (pred_xod[0]) {
				if ((*h))
					return true;
				return false;
			}
			else {
				if (pred_xod[1]) {
					if ((*h)) {
						c++;
						return true;
					}
					return false;
				}
				else {
					if (*h)
						return true;
					else {
						c++;
						return true;
					}
				}
			}
		}
		else
			return false;
	}
	else if (c == 3) {
		if (proverka_win(me1, enemy1, /*me,*/ enemy)) {
			*h = proverka_eat_i_vibor_ediashix(me1, enemy1, /*me,*/ enemy, may_eat, kol_vo);
			if (pred_xod[1]) {
				if ((*h))
					return true;
				return false;
			}
			else {
				if ((*h)) {
					c++;
					return true;
				}
				return false;
			}
		}
		else
			return false;
	}
	else if (c == 4) {
		if (proverka_win(me1, enemy1, /*me, */enemy)) {
			*h = proverka_eat_i_vibor_ediashix(me1, enemy1,/* me,*/ enemy, may_eat, kol_vo);
			if (*h)
				return true;
			return false;
		}
		else
			return false;
	}
}
void smena(HDC hdc) {
	RECT r;
	r.left = xoldchoose * 52 + 300;
	r.right = xoldchoose * 52 + 52 + 300;
	r.bottom = 416 - yoldchoose * 52 + 15;
	r.top = 364 - yoldchoose * 52 + 15;
	HBRUSH newbrushdkl = CreateSolidBrush(RGB(110, 110, 110)), newbrushrkl = CreateSolidBrush(RGB(255, 0, 0));
	FillRect(hdc, &r, newbrushrkl);
	if (doska[yoldchoose][xoldchoose] == 1) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, xoldchoose * 52 + 300, 364 - yoldchoose * 52 + 15, xoldchoose * 52 + 52 + 300, 416 - yoldchoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	else if (doska[yoldchoose][xoldchoose] == 2) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, xoldchoose * 52 + 300, 364 - yoldchoose * 52 + 15, xoldchoose * 52 + 52 + 300, 416 - yoldchoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[yoldchoose][xoldchoose] == 3) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, xoldchoose * 52 + 300, 364 - yoldchoose * 52 + 15, 52 + xoldchoose * 52 + 300, 416 - yoldchoose * 52 + 15);
		DeleteObject(newbrushwsh);
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, xoldchoose * 52 + 13 + 300, 377 - yoldchoose * 52 + 15, 39 + xoldchoose * 52 + 300, 403 - yoldchoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[yoldchoose][xoldchoose] == 4) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, xoldchoose * 52 + 300, 364 - yoldchoose * 52 + 15, 52 + xoldchoose * 52 + 300, 416 - yoldchoose * 52 + 15);
		DeleteObject(newbrushdsh);
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, 13 + xoldchoose * 52 + 300, 377 - yoldchoose * 52 + 15, 39 + xoldchoose * 52 + 300, 403 - yoldchoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	r.left = xchoose * 52 + 300;
	r.right = xchoose * 52 + 52 + 300;
	r.bottom = 416 - ychoose * 52 + 15;
	r.top = 364 - ychoose * 52 + 15;
	FillRect(hdc, &r, newbrushdkl);
	if (doska[ychoose][xchoose] == 1) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, xchoose * 52 + 300, 364 - ychoose * 52 + 15, xchoose * 52 + 52 + 300, 416 - ychoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	else if (doska[ychoose][xchoose] == 2) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, xchoose * 52 + 300, 364 - ychoose * 52 + 15, xchoose * 52 + 52 + 300, 416 - ychoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[ychoose][xchoose] == 3) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, xchoose * 52 + 300, 364 - ychoose * 52 + 15, 52 + xchoose * 52 + 300, 416 - ychoose * 52 + 15);
		DeleteObject(newbrushwsh);
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, xchoose * 52 + 13 + 300, 377 - ychoose * 52 + 15, 39 + xchoose * 52 + 300, 403 - ychoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[ychoose][xchoose] == 4) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, xchoose * 52 + 300, 364 - ychoose * 52 + 15, 52 + xchoose * 52 + 300, 416 - ychoose * 52 + 15);
		DeleteObject(newbrushdsh);
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, 13 + xchoose * 52 + 300, 377 - ychoose * 52 + 15, 39 + xchoose * 52 + 300, 403 - ychoose * 52 + 15);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	static LOGFONT il;
	il.lfHeight = 25;
	il.lfCharSet = DEFAULT_CHARSET;
	il.lfPitchAndFamily = DEFAULT_PITCH;
	il.lfWidth = 10;
	il.lfItalic = 0;
	il.lfUnderline = 0;
	HFONT hFont;
	hFont = CreateFontIndirect(&il);
	if (write1 == 1) {
		char pr[] = "Выберете шашку";
		TextOut(hdc, clientRect.left + 425, clientRect.top + 436, (LPCSTR)pr, 14);
	}
	else if (write1 == 2) {
		char pr[] = "Выберете другую шашку";
		TextOut(hdc, clientRect.left + 390, clientRect.top + 436, (LPCSTR)pr, 21);
	}
	else if (write1 == 3) {
		char pr[] = "Выберете позицию, куда вы сходитесь";
		TextOut(hdc, clientRect.left + 310, clientRect.top + 436, (LPCSTR)pr, 35);
	}
	else {
		char pr[] = "Такой ход невозможен";
		TextOut(hdc, clientRect.left + 395, clientRect.top + 436, (LPCSTR)pr, 20);
	}
	DeleteObject(hFont);
	DeleteObject(newbrushrkl);
	DeleteObject(newbrushdkl);
}
void red(HDC hdc) {
	HBRUSH newbrushrkl = CreateSolidBrush(RGB(255, 0, 0));
	HPEN newpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)), newpenno = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	SelectObject(hdc, newpen);
	RECT r;
	SelectObject(hdc, newbrushrkl);
	r.left = xchoose * 52 + 300;
	r.right = xchoose * 52 + 52 + 300;
	r.bottom = 416 - ychoose * 52 + 15;
	r.top = 364 - ychoose * 52 + 15;
	FillRect(hdc, &r, newbrushrkl);
	SelectObject(hdc, newpenno);
	if (doska[ychoose][xchoose] == 1) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	else if (doska[ychoose][xchoose] == 2) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[ychoose][xchoose] == 3) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		DeleteObject(newbrushwsh);
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, r.left + 13, r.top + 13, r.right - 13, r.bottom - 13);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[ychoose][xchoose] == 4) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		DeleteObject(newbrushdsh);
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, r.left + 13, r.top + 13, r.right - 13, r.bottom - 13);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	DeleteObject(newbrushrkl);
}
void dark(HDC hdc, int x, int y) {
	HBRUSH newbrushrkl = CreateSolidBrush(RGB(110, 110, 110));
	RECT r;
	HPEN newpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)), newpenno = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	SelectObject(hdc, newpen);
	SelectObject(hdc, newbrushrkl);
	r.left = x * 52 + 300;
	r.right = x * 52 + 52 + 300;
	r.bottom = 416 - y * 52 + 15;
	r.top = 364 - y * 52 + 15;
	FillRect(hdc, &r, newbrushrkl);
	SelectObject(hdc, newpenno);
	if (doska[y][x] == 1) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	else if (doska[y][x] == 2) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[y][x] == 3) {
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		DeleteObject(newbrushwsh);
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, r.left + 13, r.top + 13, r.right - 13, r.bottom - 13);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushdsh);
	}
	else if (doska[y][x] == 4) {
		HBRUSH newbrushdsh = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HGDIOBJ oldbrush = SelectObject(hdc, newbrushdsh);
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		DeleteObject(newbrushdsh);
		HBRUSH newbrushwsh = (HBRUSH)GetStockObject(WHITE_BRUSH);
		SelectObject(hdc, newbrushwsh);
		Ellipse(hdc, r.left + 13, r.top + 13, r.right - 13, r.bottom - 13);
		SelectObject(hdc, oldbrush);
		DeleteObject(oldbrush);
		DeleteObject(newbrushwsh);
	}
	DeleteObject(newbrushrkl);
}
void textwr(HDC hdc) {
	static LOGFONT il;
	il.lfHeight = 25;
	il.lfCharSet = DEFAULT_CHARSET;
	il.lfPitchAndFamily = DEFAULT_PITCH;
	il.lfWidth = 10;
	il.lfItalic = 0;
	il.lfUnderline = 0;
	HFONT hFont;
	hFont = CreateFontIndirect(&il);
	SelectObject(hdc, hFont);
	if (write1 == 1) {
		char pr[] = "Выберите шашку";
		TextOut(hdc, clientRect.left + 425, clientRect.top + 436, (LPCSTR)pr, 14);
	}
	else if (write1 == 2) {
		char pr[] = "Выберите другую шашку";
		TextOut(hdc, clientRect.left + 390, clientRect.top + 436, (LPCSTR)pr, 21);
	}
	else if (write1 == 3) {
		char pr[] = "Выберите позицию, куда вы сходитесь";
		TextOut(hdc, clientRect.left + 310, clientRect.top + 436, (LPCSTR)pr, 35);
	}
	else {
		char pr[] = "Такой ход невозможен";
		TextOut(hdc, clientRect.left + 395, clientRect.top + 436, (LPCSTR)pr, 20);
	}
	DeleteObject(hFont);
}
bool xod_enemy(int me1[], int enemy1[], shaka me[], shaka enemy[], bool obaz_eat, int x, int y, int x1, int y1, int  may_xod[2][12], int* kol_vo) {
	int i = 0;
	bool neprav_vibor = true;
	for (;i < 12 && (enemy[i].x != x || enemy[i].y != y || !enemy[i].alive);) i++;
	if (!obaz_eat && abs(x1 - x) == 1 && y1 - y == enemy1[3] && !doska[y1][x1] && enemy[i].name == enemy1[0]) {
		neprav_vibor = false;
		enemy[i].x = x1;
		enemy[i].y = y1;
		doska[y][x] = 0;
		doska[y1][x1] = enemy[i].name;
	}
	else if (enemy[i].name == enemy1[0] && obaz_eat && !doska[y1][x1] && ((doska[y + 1][x + 1] == me1[0] || doska[y + 1][x + 1] == me1[1]) && y1 - y == 2 && x1 - x == 2 || (doska[y - 1][x + 1] == me1[0] || doska[y - 1][x + 1] == me1[1]) && y1 - y == -2 && x1 - x == 2 || (doska[y + 1][x - 1] == me1[0] || doska[y + 1][x - 1] == me1[1]) && y1 - y == 2 && x1 - x == -2 || (doska[y - 1][x - 1] == me1[0] || doska[y - 1][x - 1] == me1[1]) && y1 - y == -2 && x1 - x == -2)) {
		if (y1 - y == 2 && x1 - x == 2) {
			bool may = true;
			for (int re = 0;re < (*kol_vo);re++)
				if (may_xod[0][re] == x + 1 && may_xod[1][re] == y + 1) {
					may = false;
					break;
				}
			if (may) {
				neprav_vibor = false;
				enemy[i].x = x1;
				enemy[i].y = y1;
				doska[y][x] = 0;
				doska[y1][x1] = enemy[i].name;
				int k = 0;
				for (;k < 12 && (me[k].x != x + 1 || me[k].y != y + 1 || !me[k].alive);)k++;
				may_xod[0][(*kol_vo)] = me[k].x;
				may_xod[1][(*kol_vo)++] = me[k].y;
				me[k].alive = false;
			}
		}
		else if (y1 - y == -2 && x1 - x == 2) {
			bool may = true;
			for (int re = 0;re < (*kol_vo);re++)
				if (may_xod[0][re] == x + 1 && may_xod[1][re] == y - 1) {
					may = false;
					break;
				}
			if (may) {
				neprav_vibor = false;
				enemy[i].x = x1;
				enemy[i].y = y1;
				doska[y][x] = 0;
				doska[y1][x1] = enemy[i].name;
				int k = 0;
				for (;k < 12 && (me[k].x != x + 1 || me[k].y != y - 1 || !me[k].alive);)k++;
				may_xod[0][(*kol_vo)] = me[k].x;
				may_xod[1][(*kol_vo)++] = me[k].y;
				me[k].alive = false;
			}
		}
		else if (y1 - y == 2 && x1 - x == -2) {
			bool may = true;
			for (int re = 0;re < (*kol_vo);re++)
				if (may_xod[0][re] == x - 1 && may_xod[1][re] == y + 1) {
					may = false;
					break;
				}
			if (may) {
				neprav_vibor = false;
				enemy[i].x = x1;
				enemy[i].y = y1;
				doska[y][x] = 0;
				doska[y1][x1] = enemy[i].name;
				int k = 0;
				for (;k < 12 && (me[k].x != x - 1 || me[k].y != y + 1 || !me[k].alive);)k++;
				may_xod[0][(*kol_vo)] = me[k].x;
				may_xod[1][(*kol_vo)++] = me[k].y;
				me[k].alive = false;
			}
		}
		else {
			bool may = true;
			for (int re = 0;re < (*kol_vo);re++)
				if (may_xod[0][re] == x - 1 && may_xod[1][re] == y - 1) {
					may = false;
					break;
				}
			if (may) {
				neprav_vibor = false;
				enemy[i].x = x1;
				enemy[i].y = y1;
				doska[y][x] = 0;
				doska[y1][x1] = enemy[i].name;
				int k = 0;
				for (;k < 12 && (me[k].x != x - 1 || me[k].y != y - 1 || !me[k].alive);)k++;
				may_xod[0][(*kol_vo)] = me[k].x;
				may_xod[1][(*kol_vo)++] = me[k].y;
				me[k].alive = false;
			}
		}
	}
	else if (enemy[i].name == enemy1[1] && !doska[y1][x1] && abs(x1 - x) == abs(y1 - y)) {
		int xprib, yprib;
		if (x1 > x) {
			if (y1 > y) {
				xprib = 1;
				yprib = 1;
			}
			else {
				xprib = 1;
				yprib = -1;
			}
		}
		else {
			if (y1 > y) {
				xprib = -1;
				yprib = 1;
			}
			else {
				xprib = -1;
				yprib = -1;
			}
		}
		if (obaz_eat) {
			int i2 = y + yprib, k2 = x + xprib;
			bool find = false;
			while (i2 != y1 && doska[i2][k2] != enemy1[0] && doska[i2][k2] != enemy1[1]) {
				if (doska[i2][k2] == me1[0] || doska[i2][k2] == me1[1]) {
					find = true;
					for (int re = 0;re < (*kol_vo);re++)
						if (may_xod[0][re] == k2 && may_xod[1][re] == i2)
							find = false;
					break;
				}
				i2 += yprib;
				k2 += xprib;
			}
			if (find) {
				int j = 0;
				for (;j < 12 && (me[j].x != k2 || me[j].y != i2 || !me[j].alive);) j++;
				i2 += yprib;
				k2 += xprib;
				bool m = false;
				int i1, k1;
				bool t1 = false;
				for (i1 = y1 + yprib, k1 = x1 - xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 += yprib, k1 -= xprib) {
					if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
						t1 = true;
						for (int re = 0;re < (*kol_vo);re++)
							if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
								t1 = false;
						break;
					}
				}
				if (t1 && i1 + yprib < 8 && i1 + yprib > -1 && k1 - xprib < 8 && k1 - xprib > -1 && doska[i1 + yprib][k1 - xprib] == 0)
					m = true;
				else
					t1 = false;
				if (!t1) {
					for (i1 = y1 - yprib, k1 = x1 + xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 -= yprib, k1 += xprib) {
						if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
							t1 = true;
							for (int re = 0;re < (*kol_vo);re++)
								if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
									t1 = false;
							break;
						}
					}
					if (t1 && i1 - yprib < 8 && i1 - yprib > -1 && k1 + xprib < 8 && k1 + xprib > -1 && doska[i1 - yprib][k1 + xprib] == 0)
						m = true;
					else
						t1 = false;
					if (!t1) {
						for (i1 = y1 + yprib, k1 = x1 + xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 += yprib, k1 += xprib) {
							if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
								t1 = true;
								for (int re = 0;re < (*kol_vo);re++)
									if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
										t1 = false;
								break;
							}
						}
						if (t1 && i1 + yprib < 8 && i1 + yprib > -1 && k1 + xprib < 8 && k1 + xprib > -1 && doska[i1 + yprib][k1 + xprib] == 0)
							m = true;
						else
							t1 = false;
					}
				}
				bool ost = true;
				while (i2 != y1) {
					if (ost) {
						int i1, k1;
						bool t = false;;
						for (i1 = i2 + yprib, k1 = k2 - xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 += yprib, k1 -= xprib) {
							if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
								t = true;
								for (int re = 0;re < (*kol_vo);re++)
									if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
										t = false;
								break;
							}
						}
						if (t && i1 + yprib < 8 && i1 + yprib > -1 && k1 - xprib < 8 && k1 - xprib > -1 && doska[i1 + yprib][k1 - xprib] == 0)
							ost = false;
						else
							t = false;
						if (!t) {
							for (i1 = i2 - yprib, k1 = k2 + xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 -= yprib, k1 += xprib) {
								if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
									t = true;
									for (int re = 0;re < (*kol_vo);re++)
										if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
											t = false;
									break;
								}
							}
							if (t && i1 - yprib < 8 && i1 - yprib > -1 && k1 + xprib < 8 && k1 + xprib > -1 && doska[i1 - yprib][k1 + xprib] == 0)
								ost = false;
							else
								t = false;
						}
					}
					if (doska[i2][k2]) {
						find = false;
						break;
					}
					i2 += yprib;
					k2 += xprib;
				}
				if (!ost && !m && find) {
					find = false;
				}
				else if (find && !m) {
					i2 = y1 + yprib; k2 = x1 + xprib;
					while (i2 < 8 && i2 >-1 && k2 <8 && k2 >-1 && !doska[i2][k2] && ost) {
						if (ost) {
							int i1, k1;
							bool t = false;
							for (i1 = i2 + yprib, k1 = k2 - xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 += yprib, k1 -= xprib) {
								if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
									t = true;
									for (int re = 0;re < (*kol_vo);re++)
										if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
											t = false;
									break;
								}
							}
							if (t && i1 + yprib < 8 && i1 + yprib > -1 && k1 - xprib < 8 && k1 - xprib > -1 && doska[i1 + yprib][k1 - xprib] == 0)
								ost = false;
							else
								t = false;
							if (!t) {
								for (i1 = i2 - yprib, k1 = k2 + xprib; k1 > -1 && k1 < 8 && i1 < 8 && i1 > -1 && doska[i1][k1] != enemy1[0] && doska[i1][k1] != enemy1[1];i1 -= yprib, k1 += xprib) {
									if (doska[i1][k1] == me1[0] || doska[i1][k1] == me1[1]) {
										t = true;
										for (int re = 0;re < (*kol_vo);re++)
											if (may_xod[0][re] == k1 && may_xod[1][re] == i1)
												t = false;
										break;
									}
								}
								if (t && i1 - yprib < 8 && i1 - yprib > -1 && k1 + xprib < 8 && k1 + xprib > -1 && doska[i1 - yprib][k1 + xprib] == 0)
									ost = false;
								else
									t = false;
							}
						}
						if (doska[i2][k2] || !ost)
							break;
						i2 += yprib;
						k2 += xprib;
					}
					if (!ost)
						find = false;

				}
				if (find) {
					neprav_vibor = false;
					enemy[i].x = x1;
					enemy[i].y = y1;
					me[j].alive = false;
					may_xod[0][(*kol_vo)] = me[j].x;
					may_xod[1][(*kol_vo)++] = me[j].y;
					doska[y1][x1] = enemy[i].name;
					doska[y][x] = 0;
					y = y1;
					x = x1;
				}
			}
		}
		else {
			bool vozmo_xoda = true;
			int i1 = y + yprib, k = x + xprib;
			while (i1 != y1) {
				if (doska[i1][k]) {
					vozmo_xoda = false;
					break;
				}
				i1 += yprib;
				k += xprib;
			}
			if (vozmo_xoda) {
				neprav_vibor = false;
				enemy[i].x = x1;
				enemy[i].y = y1;
				doska[y1][x1] = enemy1[1];
				doska[y][x] = 0;
			}
		}
	}
	return !neprav_vibor;
}
void zopoln_arr(int me1[], int enemy1[], shaka me[], shaka enemy[], int arr[2][18], int counter) {
	int n = (counter % 2 == 0) ? 0 : 1, n1 = (counter % 2 == 0) ? 1 : 0;
	if (proverka_win(me1, enemy1,enemy)) {
		arr[n][0] = 0;
		if (proverka_win(enemy1, me1, me)) {
			for (int i = 0;i < 5;i++) {
				arr[0][i] = 0;
				arr[1][i] = 0;
			}
			for (int i = 0;i < 12;i++)
				if (me[i].alive) {
					arr[n][3]++;
					if (me[i].name == me1[1])
						arr[n][1]++;
				}
			for (int i = 0;i < 12;i++)
				if (enemy[i].alive) {
					arr[n1][3]++;
					if (enemy[i].name == enemy1[1])
						arr[n1][1]++;
				}
			for (int i = 0;i < 12;i++)
				if (enemy[i].name == enemy1[0] && enemy[i].alive) {
					if (enemy[i].y == enemy1[2] - enemy1[3])
						arr[n1][2]++;
					else if (enemy[i].y == enemy1[2] - 2 * enemy1[3]) {
						bool h = true;
						if (h) {
							int x = enemy[i].x, y = enemy[i].y + 2 * enemy1[3];
							while (y < 8 && y > -1) {
								if (doska[y][x] == me1[0] || doska[y][x] == me1[1]) {
									h = false;
									break;
								}
								y += 2 * enemy1[3];
							}
						}
						for (int dx = 1;dx > -2;dx -= 2)
							if (h && (enemy[i].x > 0 && dx == -1 || enemy[i].x < 7 && dx == 1)) {
								bool h1 = true;
								int x, y;
								int dli = (dx == 1) ? 7 : 0;
								if (enemy[i].x == dli - dx) {
									if (doska[enemy[i].y + enemy1[3]][enemy[i].x + dx] == me1[0] || doska[enemy[i].y + enemy1[3]][enemy[i].x + dx] == me1[1])
										h1 = false;
								}
								else
									for (int dx1 = dx;dx1 <= dx * 3 && dx == 1 || dx1 >= dx * 3 && dx == -1;dx1 += dx) {
										x = enemy[i].x + dx1;
										y = (dx1 % 2)? enemy[i].y + enemy1[3]: enemy[i].y;
										while (y < 8 && y > -1 && h1) {
											if (doska[y][x] == me1[1] || doska[y][x] == me1[0]) {
												h1 = false;
												break;
											}
											y += 2 * enemy1[3];
										}
									}
								if (h1) {
									arr[n1][2]++;
									h = false;
								}
							}
					}
					else {
						bool h = true;
						if (h) {
							int x = enemy[i].x, y = enemy[i].y + 2 * enemy1[3];
							while (y < 8 && y > -1) {
								if (doska[y][x] == me1[0] || doska[y][x] == me1[1]) {
									h = false;
									break;
								}
								y += 2 * enemy1[3];
							}
						}
						for (int dx1 = 1;dx1 > -2;dx1 -= 2) {
							if (h && (enemy[i].x > 0 && dx1 == -1 || enemy[i].x < 7 && dx1 == 1)) {
								bool h1 = true;
								int y = enemy[i].y, x;
								while (y < 8 && y > -1 && h1) {
									x = ((enemy[i].y - y) % 2) ? enemy[i].x + dx1 : enemy[i].x + 2 * dx1;
									while (x < 8 && x > -1) {
										if (doska[y][x] == me1[1] || doska[y][x] == me1[0]) {
											h1 = false;
											break;
										}
										x += 2 * dx1;
									}
									y += enemy1[3];
								}
								if (h1) {
									int dx = (enemy1[2] == 7) ? -1 : 1;
									int* prib[2];
									int max_elem;
									if (enemy1[2] == 7 && dx == 1 || dx == -1 && enemy1[2] != 7) {
										if (enemy[i].x == enemy1[2] + dx) {
											prib[0] = new int[5] { 1, 2, 3, 4, 5 };
											prib[1] = new int[5] {1, 2, 3, 6, 7};
											max_elem = 5;
										}
										else if (enemy[i].x == enemy1[2] + 3 * dx) {
											prib[0] = new int[3]{ 1, 2, 3};
											prib[1] = new int[3]{ 3, 6, 7};
											max_elem = 3;
										}
										else if (enemy[i].x == enemy1[2] + 5 * dx) {
											prib[0] = new int[1]{1};
											prib[1] = new int[1]{3};
											max_elem = 1;
										}
										else if (enemy[i].x == enemy1[2] + 2 * dx) {
											prib[0] = new int[3]{ 1, 2, 3};
											prib[1] = new int[3]{ 1, 2, 5};
											max_elem = 3;
										}
										else if (enemy[i].x == enemy1[2] + 4 * dx) {
											max_elem = 1000;
											if (doska[enemy[i].y + 5 * enemy1[3]][enemy[i].x + dx] != me1[0] && doska[enemy[i].y + 5 * enemy1[3]][enemy[i].x + dx] != me1[1]) {
												h = false;
												arr[n1][2]++;
											}
										}
										else if (enemy[i].x == enemy1[2] + 6 * dx) {
											max_elem = 1000;
											if (doska[enemy[i].y + 5 * enemy1[3]][enemy[i].x + dx] != me1[0] && doska[enemy[i].y + 5 * enemy1[3]][enemy[i].x + dx] != me1[1]) {
												h = false;
												arr[n1][2]++;
											}
										}
										else {
											max_elem = 1000;
											h = false;
											arr[n1][2]++;
										}
									}
									else {
										if (enemy[i].x == enemy1[2] + dx || enemy[i].x == enemy1[2] + 3 * dx || enemy[i].x == enemy1[2] + 2 * dx) {
											prib[0] = new int[1]{-1};
											prib[1] = new int[1]{3};
											max_elem = 1;
										}
										else if (enemy[i].x == enemy1[2] + 5 * dx) {
											prib[0] = new int[3]{ -1, -2, -3 };
											prib[1] = new int[3]{ 3, 6, 7 };
											max_elem = 3;
										}
										else if (enemy[i].x == enemy1[2] + 4 * dx) {
											prib[0] = new int[2]{ -1, -2};
											prib[1] = new int[2]{ 3, 6};
											max_elem = 2;
										}
										else if (enemy[i].x == enemy1[2] + 6 * dx) {
											prib[0] = new int[4]{ -1, -2, -3, -4};
											prib[1] = new int[4]{ 1, 2, 3, 6};
											max_elem = 4;
										}
										else {
											max_elem = 1000;
											h = false;
											arr[n1][2]++;
										}
									}
									for (int j = 0; j < max_elem && max_elem !=1000; j++) {
										x = enemy[i].x + dx * prib[0][j];
										y = enemy[i].y + enemy1[3] * prib[1][j];
										while (y < 8 && y > -1 && h1) {
											if (doska[y][x] == me1[1] || doska[y][x] == me1[0]) {
												h1 = false;
												break;
											}
											y += 2 * enemy1[3];
										}
									}
									if (h1 && max_elem != 1000) {
										h = false;
										arr[n1][2]++;
									}
								}
							}
						}
					}
				}

			for (int i = 0;i < 12;i++)
				if (me[i].name == me1[0] && me[i].alive) {
					if (me[i].y == me1[2] - me1[3])
						arr[n][2]++;
					else if (me[i].y == me1[2] - 2 * me1[3]) {
						bool h = true;
						if (h) {
							int x = me[i].x, y = me[i].y + 2 * me1[3];
							while (y < 8 && y > -1) {
								if (doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) {
									h = false;
									break;
								}
								y += 2 * me1[3];
							}
						}
						for (int dx = 1;dx > -2;dx -= 2)
						if (h && (me[i].x < 7 && dx == 1|| me[i].x > 0 && dx == -1) ) {
							bool h1 = true;
							int x, y;
							int dli = (dx == 1) ? 7 : 0;
							if (me[i].x == dli - dx) {
								if (doska[me[i].y + me1[3]][me[i].x + dx] == enemy1[0] || doska[me[i].y + me1[3]][me[i].x + dx] == enemy1[1])
									h1 = false;
							}
							else {
								for (int dx1 = dx;dx1 <= dx * 2 && dx == 1 || dx1 >= dx * 2 && dx == -1;dx1 += dx) {
									x = me[i].x + dx1;
									y = (dx1 % 2)? me[i].y + me1[3]:me[i].y;
									while (y < 8 && y > -1 && h1) {
										if (doska[y][x] == enemy1[1] || doska[y][x] == enemy1[0]) {
											h1 = false;
											break;
										}
										y += 2 * me1[3];
									}
								}
							}
							if (h1) {
								arr[n][2]++;
								h = false;
							}
						}
					}
					else if (me[i].y == me1[2] - 3 * me1[3]) {
						bool h = true;
						if (h) {
							int x = me[i].x, y = me[i].y + 2 * me1[3];
							while (y < 8 && y > -1) {
								if (doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) {
									h = false;
									break;
								}
								y += 2 * me1[3];
							}
						}
						for (int dx = 1;dx > -2;dx -= 2)
							if (h && (me[i].x > 0 && dx == -1 || me[i].x < 7 && dx == 1)) {
								int dli = (dx == 1) ? 7 : 0;
								bool h1 = true;
								int y, x;
								if (me[i].x == dli - dx) {
									int prib[2][3] = { {dx, -dx, -2 * dx}, {1, 1, 0} };
									for (int j = 0;j < 3;j++) {
										x = me[i].x + prib[0][j];
										y = me[i].y + me1[3] * prib[1][j];
										while (y < 8 && y > -1 && h1) {
											if (doska[y][x] == enemy1[1] || doska[y][x] == enemy1[0]) {
												h1 = false;
												break;
											}
											y += 2 * me1[3];
										}
									}
								}
								else if (me[i].x == dli - 7 * dx) {
									int prib[2][2] = { {dx, 2 * dx}, {1, 0} };
									for (int j = 0;j < 2;j++) {
										x = me[i].x + prib[0][j];
										y = me[i].y + me1[3] * prib[1][j];
										while (y < 8 && y > -1 && h1) {
											if (doska[y][x] == enemy1[1] || doska[y][x] == enemy1[0]) {
												h1 = false;
												break;
											}
											y += 2 * me1[3];
										}
									}
								}
								else {
									y = me[i].y;
									while (y < 8 && y > -1 && h1) {
										x = ((me[i].y - y) % 2) ? me[i].x + dx : me[i].x + 2 * dx;
										while (x < 8 && x - me[i].x < 5 && dx == 1 || -x + me[i].x < 5 && dx == -1 && x > -1) {
											if (doska[y][x] == enemy1[1] || doska[y][x] == enemy1[0]) {
												h1 = false;
												break;
											}
											x += 2 * dx;
										}
										y += me1[3];
									}
								}
								if (h1) {
									arr[n][2]++;
									h = false;
								}
							}
					}
					else {
						bool h = true;
						if (h) {
							int x = me[i].x, y = me[i].y + 2 * me1[3];
							while (y < 8 && y > -1) {
								if (doska[y][x] == enemy1[0] || doska[y][x] == enemy1[1]) {
									h = false;
									break;
								}
								y += 2 * me1[3];
							}
						}
						for (int dx1 = 1;dx1 > -2;dx1 -= 2)
							if (h && (me[i].x > 0 && dx1 == -1 || me[i].x < 7 && dx1 == 1)) {
								bool h1 = true;
								int y = me[i].y, x;
								while (y < 8 && y > -1 && h1) {
									x = ((me[i].y - y) % 2) ? me[i].x + dx1 : me[i].x + 2 * dx1;
									while (x < 8 && x > -1) {
										if (doska[y][x] == enemy1[1] || doska[y][x] == enemy1[0]) {
											h1 = false;
											break;
										}
										x += 2 * dx1;
									}
									y += me1[3];
								}
								if (h1) {
									int* prib[2];
									int max_elem;
									int dx = (me1[2] == 7) ? -1 : 1;
									if (me1[2] == 7 && dx == 1 || dx == -1 && me1[2] != 7) {
										if (me[i].x == me1[2] + dx) {
											prib[0] = new int[4]{ 1, 2, 3, 4 };
											prib[1] = new int[4]{ 1, 2, 5, 6};
											max_elem = 4;
										}
										else if (me[i].x == me1[2] + 3 * dx) {
											prib[0] = new int[2]{ 1, 2};
											prib[1] = new int[2]{ 5, 6};
											max_elem = 2;
										}
										else if (me[i].x == me1[2] + 2 * dx) {
											prib[0] = new int[2]{ 1, 2};
											prib[1] = new int[2]{ 3, 4};
											max_elem = 2;
										}
										else {
											max_elem = 1000;
											h = false;
											arr[n][2]++;
										}
									}
									else {
										if (me[i].x == me1[2] + 3 * dx) {
											max_elem = 1000;
											if (doska[me[i].y + 7 * me1[3]][me[i].x - dx] != enemy1[0] && doska[me[i].y + 7 * me1[3]][me[i].x - dx] != enemy1[1]) {
												h = false;
												arr[n][2]++;
											}
										}
										else if (me[i].x == me1[2] + 5 * dx) {
											prib[0] = new int[3]{ -1, -2, -3};
											prib[1] = new int[3]{ 3, 4, 7};
											max_elem = 3;
										}
										else if (me[i].x == me1[2] + 4 * dx) {
											prib[0] = new int[2]{ -1, -2 };
											prib[1] = new int[2]{ 5, 6 };
											max_elem = 2;
										}
										else if (me[i].x == me1[2] + 6 * dx) {
											prib[0] = new int[4]{ -1, -2, -3, -4 };
											prib[1] = new int[4]{ 3, 4, 5, 6 };
											max_elem = 4;
										}
										else {
											max_elem = 1000;
											h = false;
											arr[n][2]++;
										}
									}
									for (int j = 0; j < max_elem && max_elem != 1000; j++) {
										x = me[i].x + dx * prib[0][j];
										y = me[i].y + me1[3] * prib[1][j];
										while (y < 8 && y > -1 && h1) {
											if (doska[y][x] == enemy1[1] || doska[y][x] == enemy1[0]) {
												h1 = false;
												break;
											}
											y += 2 * me1[3];
										}
									}
									if (h1 && max_elem != 1000) {
										h = false;
										arr[n][2]++;
									}

								}
							}
					}
				}
			if (doska[4][2]) {
				if (doska[4][2] == me1[0])
					arr[n][4]++;
				else if (doska[4][2] == enemy1[0])
					arr[n1][4]++;
			}
			if (doska[3][3]) {
				if (doska[3][3] == me1[0])
					arr[n][4]++;
				else if (doska[3][3] == enemy1[0])
					arr[n1][4]++;
			}
			if (doska[4][4]) {
				if (doska[4][4] == me1[0])
					arr[n][4]++;
				else if (doska[4][4] == enemy1[0])
					arr[n1][4]++;
			}
			if (doska[3][5]) {
				if (doska[3][5] == me1[0])
					arr[n][4]++;
				else if (doska[3][5] == enemy1[0])
					arr[n1][4]++;
			}
		}
		else
			arr[n1][0] = 1;
	}
	else
		arr[n][0] = 1;
}
void about(HDC hdc) {
	static LOGFONT il;
	il.lfHeight = 50;
	il.lfCharSet = DEFAULT_CHARSET;
	il.lfPitchAndFamily = DEFAULT_PITCH;
	il.lfWidth = 20;
	il.lfItalic = 0;
	il.lfUnderline = 0;
	strcpy_s(il.lfFaceName, "Times New Roman");
	HFONT hFont;
	hFont = CreateFontIndirect(&il);
	SelectObject(hdc, hFont);
	char pr[25] = "Игра в шашки";
	TextOut(hdc, clientRect.left + 25, clientRect.top + 75, (LPCSTR)pr, 12);
	strcpy_s(pr, "Версия 1.7.6");
	TextOut(hdc, clientRect.left + 25, clientRect.top + 150, (LPCSTR)pr, 12);
	strcpy_s(pr, "Кочнев Виктор Алексеевич");
	TextOut(hdc, clientRect.left + 25, clientRect.top + 225, (LPCSTR)pr, 24);
	strcpy_s(pr, "Все права защищены.");
	TextOut(hdc, clientRect.left + 25, clientRect.top + 300, (LPCSTR)pr, 19);
	DeleteObject(hFont);
}