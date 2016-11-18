/*------------------------------------------------------------------------
maiku.cpp マイクを動かす
---------------------------------------------------------------------------*/

#include <windows.h>
#include <tchar.h>

#define	ID_MYTIMER	(32767)
#define	ID_MYCHILD	(100)
#define	CHD_WIDTH	(87)	//ピクセル
#define	CHD_HEIGHT	(110)

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChdProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK ChdProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL InitApp(HINSTANCE, WNDPROC, LPCTSTR);
BOOL InitInstance(HINSTANCE, int, LPCTSTR);

static TCHAR szClassName[] = _T("maiku");
static TCHAR szchClassName[] = _T("child");
static TCHAR szTitle[] = _T("maiku");

int img_start_x = 83;
int img_start_y = 111;
int img_end_x = 482;
int img_end_y = 193;
int min_y = 29;
int center_screen = 283;

int count = 1;      //動作を指定

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow) {
	MSG msg;

	if (!hPrevInst) {
		if (!InitApp(hCurInst, WndProc, szClassName)) {
			return FALSE;
		}
	}
	if (!InitInstance(hCurInst, nCmdShow, szClassName)) {
		return FALSE;
	}
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

BOOL InitApp(HINSTANCE hInst, WNDPROC WndProc, LPCTSTR szClassName) {
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	return (RegisterClass(&wc));
}


BOOL InitInstance(HINSTANCE hInst, int nCmdShow, LPCTSTR szClassName) {
	HWND hWnd;

	hWnd = CreateWindow(
		szClassName,
		szTitle,				//タイトルバーにこの名前が表示されます
		WS_OVERLAPPEDWINDOW,	//ウィンドウの種類
		10,						//Ｘ座標　適宜指定する
		10,						//Ｙ座標　適宜指定する
		700,					//幅	　適宜指定する
		400,					//高さ	　適宜指定する
		NULL,					//親ウィンドウのハンドル、親を作るときはNULL
		NULL,					//メニューハンドル、クラスメニューを使うときはNULL
		hInst,					//インスタンスハンドル
		NULL);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT		rc;
	static int 	x = img_start_x;
	static int 	y = img_start_y;
	static int 	direction;
	static HWND		hChdWnd;
	HINSTANCE		hInst;
	enum { right, left };
	int height = 102;
	int width = 200;
	bool window_end = false;
	HDC hDC;
	HBRUSH  hBrushBlack;
	HBRUSH  hBrushYellow;
	HBRUSH  hBrushWhite;
	PAINTSTRUCT ps;

	int go_x = width / 64;
	int go_y = height / 128+1;

	switch (message) {

	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps);		// GDI関数による描画を開始する

		
		//iPhoneを横向きにする
		hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrushBlack);
		RoundRect(hDC, 10, 10, 643, 322, 70, 80);    //iPhone7の外枠
		Rectangle(hDC, 120, 7, 170, 20);               //iPhone7の電源ボタン

		hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hDC, hBrushWhite);
		RoundRect(hDC, 39, 129, 46, 207, 80, 10);
		Ellipse(hDC, 579, 139, 633, 193);            //iPhone7の中央ボタン外枠

		hBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
		SelectObject(hDC, hBrushYellow);
		Rectangle(hDC, 83, 29, 571, 303);            //iPhone7の画面サイズ

		SelectObject(hDC, hBrushBlack);
		Ellipse(hDC, 584, 144, 628, 188);            //iPhone7の中央ボタンの内枠

		EndPaint(hWnd, &ps);				// GDI関数による描画を終了する

	}

	switch (message) {

	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		InitApp(hInst, ChdProc1, szchClassName);
		hChdWnd = CreateWindow(
			szchClassName,		// ウィンドウクラス名
			NULL,				// タイトルバーに表示する文字列
			WS_CHILD,			// ウィンドウの種類
			0,					// ウィンドウを表示する位置(X座標）
			0,					// ウィンドウを表示する位置(Y座標）
			CHD_WIDTH,			// ウィンドウの幅
			CHD_HEIGHT,			// ウィンドウの高さ
			hWnd,				// 親ウィンドウのウィンドウハンドル
			(HMENU)ID_MYCHILD,	// メニューハンドル
			hInst,				// インスタンスハンドル
			NULL				// その他の作成データ
			);

		ShowWindow(hChdWnd, SW_SHOW);
		UpdateWindow(hChdWnd);

		SetTimer(hWnd, ID_MYTIMER, 10, NULL);
		break;

	case WM_TIMER:
		GetClientRect(hWnd, &rc);
		MoveWindow(hChdWnd, x, y, CHD_WIDTH, CHD_HEIGHT, TRUE);

		switch (direction) {


			//右向きに移動する時の処理	
		case right:
			if ((x >= img_start_x && y <= img_start_y) && y >= min_y && x < center_screen){		//右上の中央スクリーンに向かって進む
				x += go_x;
				y -= go_y;
			}
			if (y >= min_y && x >= center_screen && y <= img_start_y){		//中央スクリーンから右下に向かって進む
				x += go_x;
				y += go_y;
			}
			if (x >= img_end_x){		//右から左に変換
				direction = left;
			}
			break;


			//左向きに移動する時の処理	
		case left:
			if (x <= img_end_x + 1 && x >= center_screen && y <= img_end_y){	//左下の中央スクリーンに向かって進む
				x -= go_x;
				y += go_y;
			}
			if (x <= center_screen && y <= img_end_y && x >= img_start_x){		///中央スクリーンから左上に向かって進む
				x -= go_x;
				y -= go_y;
			}
			if (x <= img_start_x){	//左から右に変換
				direction = right;
				count += 1;
			}
			break;
		}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, ID_MYTIMER);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

LRESULT CALLBACK ChdProc1(HWND hChdWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HBITMAP	hBitmap1;
	static HBITMAP	hPrevBitmap;
	HINSTANCE		hInst;
	PAINTSTRUCT	ps;
	HDC			hDC;
	HDC			hCompatDC;


	
		switch (message) {
		case WM_PAINT:
			hInst = (HINSTANCE)GetWindowLong(hChdWnd, GWL_HINSTANCE);

			hBitmap1 = (HBITMAP)LoadImage(
				hInst,
				_T("maiku.bmp"),
				IMAGE_BITMAP,
				0,
				0,
				LR_LOADFROMFILE);

			if (hBitmap1 == NULL) {
				MessageBox(
					hChdWnd,
					_T("ビットマップのロードに失敗しました"),
					_T("エラー"),
					MB_OK | MB_ICONWARNING
					);
				return 0;
			}

			hDC = BeginPaint(hChdWnd, &ps);
			hCompatDC = CreateCompatibleDC(hDC);
			SelectObject(hCompatDC, hBitmap1);

			BitBlt(hDC, 0, 0, CHD_WIDTH, CHD_HEIGHT, hCompatDC, 0, 0, SRCCOPY);

			DeleteDC(hCompatDC);
			DeleteObject(hBitmap1);
			EndPaint(hChdWnd, &ps);

			break;

		default:
			return(DefWindowProc(hChdWnd, message, wParam, lParam));
		}
	
	return 0;
}
