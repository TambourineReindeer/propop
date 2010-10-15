// ProjectP.cpp : definisce il punto di ingresso dell'applicazione.
//

#include "stdafx.h"
#include "commdlg.h"
#include "ProjectP.h"
#include "World.h"
#include "BasePath.h"
#include "RenderWindow.h" // Ogre-based Render Window

#define MAX_LOADSTRING 100

// Variabili globali:
HINSTANCE hInst;								// istanza corrente
HWND hWnd;								        // finestra main
HWND hWndLegend;                                // finestra della legenda
TCHAR szTitle[MAX_LOADSTRING];					// Testo della barra del titolo
TCHAR szWindowClass[MAX_LOADSTRING];			// nome della classe di finestre principale

// Dichiarazioni con prototipo delle funzioni incluse in questo modulo di codice:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


CWorld *myWorld;
bool m_bPlaceEditMode = true;
bool m_bSelCellChanged = false;
bool m_bCellDeleted = false;
void RedrawWorld();
string CurrentFile;
CBasePath *path;
CPlace Start(-1,-1), Goal(-1,-1);

POINT GridSelCell;
POINT GridDeletedCell;
POINT GridOldSelCell;

int iGridNumOfRows = 20;
int iGridNumOfCols = 20;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*** INIZIALIZZAZIONE WORLD ***/
	myWorld = new CWorld();
	/******************************/

	MSG msg;
	HACCEL hAccelTable;

	// Inizializzare le stringhe globali
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJECTP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Eseguire l'inizializzazione dall'applicazione:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Visualizzo la legenda
	hWndLegend = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, NULL);
	SetWindowPos(hWnd, NULL, 30, 30, 394, 394, SWP_NOZORDER);
	SetWindowPos(hWndLegend, NULL, 720, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowWindow(hWndLegend, SW_SHOW);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTP));

	// Ciclo di messaggi principale:
	DWORD ticks;
	ticks = GetTickCount();
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if ((GetTickCount() - ticks) > 1000)
		{
			RedrawWorld();
			ticks = GetTickCount();
		}

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete myWorld;
	return (int) msg.wParam;
}


//
//  FUNZIONE: RedrawWorld()
//
//  SCOPO: ridisegna la finestra leggendo il contenuto di myWorld.
//
//  COMMENTI: Ok, per la gestione del rettangolo di selezione e della cancellazione,
//            per evitare l'orribile ritracciamento da GDI, mantengo due variabili di
//            stato e due variabili POINT (che indicano la vecchia posizione del puntatore
//            e la posizione della cella cancellata).
//
void RedrawWorld()
{
	HDC hdc;

	int rowHeight, colWidth;
	RECT rcWindow;

	//hdc = BeginPaint(hWnd, &ps);
	hdc = GetDC(hWnd);

	GetClientRect(hWnd,&rcWindow);
	rowHeight = (rcWindow.bottom-rcWindow.top) / iGridNumOfRows;
	colWidth  = (rcWindow.right-rcWindow.left) / iGridNumOfCols;

	HBRUSH brush;
	RECT cellrect;

	// CANCELLO IL VECCHIO RETTANGOLO DI SELEZIONE
	if (m_bSelCellChanged)
	{
		brush = CreateSolidBrush(RGB(255,255,255));
		cellrect.top = ((GridOldSelCell.x) * rowHeight) - 1;
		cellrect.bottom = cellrect.top + rowHeight + 3;
		cellrect.left = ((GridOldSelCell.y)*colWidth) - 1;
		cellrect.right = cellrect.left + colWidth + 3;
		FrameRect(hdc, &cellrect, brush);
		DeleteObject(brush);
		m_bSelCellChanged = false;
	}

	// CANCELLO LA CELLA ELIMINATA
	if (m_bCellDeleted)
	{
		brush = CreateSolidBrush(RGB(255,255,255));
		cellrect.top = (GridDeletedCell.x * rowHeight) + 1;
		cellrect.bottom = cellrect.top + rowHeight - 1;
		cellrect.left = (GridDeletedCell.y * colWidth) + 1;
		cellrect.right = cellrect.left + colWidth - 1;
		FillRect(hdc, &cellrect, brush);
		DeleteObject(brush);
		m_bCellDeleted = false;
	}

	// RIDISEGNO LA GRIGLIA
	for (int r=0; r<iGridNumOfRows-1; r++)
	{
		MoveToEx(hdc, 0, (r+1)*rowHeight, NULL);
		LineTo(hdc, rcWindow.right, (r+1)*rowHeight);
	}
	for (int c=0; c<iGridNumOfCols-1; c++)
	{
		MoveToEx(hdc, (c+1)*colWidth, 0, NULL);
		LineTo(hdc, (c+1)*colWidth, rcWindow.bottom);
	}

	// LEGGO I PLACE GIA' PRESENTI E LI RAPPRESENTO
	for (int r=0; r<iGridNumOfRows; r++)
	{
		for (int c=0; c<iGridNumOfCols; c++)
		{
			CPlace *currPlace = myWorld->GetPlaceAtPos((float)r,(float)c);
			if (currPlace != NULL) 
			{
				if (((Start.m_fPosX == -1) || (*currPlace != Start)) && ((Goal.m_fPosX == -1) || (*currPlace != Goal)))
				{
					HBRUSH brush = NULL;
					RECT cellrect;
					if (currPlace->GetType() == CPlace::PTFloor)
					{
						brush = CreateSolidBrush(RGB(0,180,0));
					}
					float alt = currPlace->GetAltitude();
					
					if (alt > 0)
					{
						int r = 200-(int)alt > 0 ? 200-(int)alt : 0;
						int g = 150-(int)alt > 0 ? 150-(int)alt : 0;
						int b =  30-(int)alt > 0 ?  30-(int)alt : 0;

						brush = CreateSolidBrush(RGB(r,g,b));
					}
					if (alt < 0)
					{
						int r = 0;
						int g = 0;
						int b = 255+(int)alt > 0 ? 255+(int)alt : 0;

						brush = CreateSolidBrush(RGB(r,g,b));
					}

					cellrect.top = ((r) * rowHeight) + 1;
					cellrect.bottom = cellrect.top + rowHeight - 1;
					cellrect.left = ((c)*colWidth) + 1;
					cellrect.right = cellrect.left + colWidth - 1;
					FillRect(hdc, &cellrect, brush);
					DeleteObject(brush);
				}
			}
		}
	}

	// DISEGNO, SE DEFINITI START E GOAL DEL PATH
	HBRUSH redbrush = CreateSolidBrush(RGB(255,0,0));

	cellrect.top = (((long)Start.m_fPosX) * rowHeight) - 1;
	cellrect.bottom = cellrect.top + rowHeight + 3;
	cellrect.left = (((long)Start.m_fPosY)*colWidth) - 1;
	cellrect.right = cellrect.left + colWidth + 3;
	FillRect(hdc, &cellrect, redbrush);

	cellrect.top = (((long)Goal.m_fPosX) * rowHeight) - 1;
	cellrect.bottom = cellrect.top + rowHeight + 3;
	cellrect.left = (((long)Goal.m_fPosY)*colWidth) - 1;
	cellrect.right = cellrect.left + colWidth + 3;
	FillRect(hdc, &cellrect, redbrush);
	DeleteObject(redbrush);

	// INFINE DISEGNO IL PATH, SE GIA' CALCOLATO
	if (path != NULL)
	{
		// UN Place alla volta
		CPlace *Step = path->GetNextPlace();
		if (Step != NULL)
		{
			cellrect.top = ((((long)Step->m_fPosX) * rowHeight) - 1) + ((rowHeight-10)/2);
			cellrect.bottom = cellrect.top + 10 + 3;
			cellrect.left = ((((long)Step->m_fPosY)*colWidth) - 1) + ((colWidth-10)/2);
			cellrect.right = cellrect.left + 10 + 3;
		
			HBRUSH whitebrush = CreateSolidBrush(RGB(255,255,255));
			SelectObject(hdc, whitebrush);
			Ellipse(hdc, cellrect.left, cellrect.top, cellrect.right, cellrect.bottom);
			DeleteObject(whitebrush);
		}
	}

	// DISEGNO IL BORDO ATTORNO ALLA CELLA SELEZIONATA
	HBRUSH brush2 = CreateSolidBrush(RGB(255,0,0));

	cellrect.top = ((GridSelCell.x) * rowHeight) - 1;
	cellrect.bottom = cellrect.top + rowHeight + 3;
	cellrect.left = ((GridSelCell.y)*colWidth) - 1;
	cellrect.right = cellrect.left + colWidth + 3;
	FrameRect(hdc, &cellrect, brush2);
	DeleteObject(brush2);

	ReleaseDC(hWnd, hdc);
	//EndPaint(hWnd, &ps);
}




//
//  FUNZIONE: MyRegisterClass()
//
//  SCOPO: registra la classe di finestre.
//
//  COMMENTI:
//
//    Questa funzione e il relativo utilizzo sono necessari solo se si desidera che il codice
//    sia compatibile con i sistemi Win32 precedenti alla funzione 'RegisterClassEx'
//    aggiunta a Windows 95. È importante chiamare questa funzione
//    in modo da ottenere piccole icone in formato corretto associate
//    all'applicazione.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROJECTP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNZIONE: InitInstance(HINSTANCE, int)
//
//   SCOPO: salva l'handle di istanza e crea la finestra principale
//
//   COMMENTI:
//
//        In questa funzione l'handle di istanza viene salvato in una variabile globale e
//        la finestra di programma principale viene creata e visualizzata.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Memorizzare l'handle di istanza nella variabile globale

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNZIONE: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  SCOPO:  elabora i messaggi per la finestra principale.
//
//  WM_COMMAND	- elabora il menu dell'applicazione
//  WM_PAINT	- disegna la finestra principale
//  WM_DESTROY	- inserisce un messaggio di uscita e restituisce un risultato
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
	OPENFILENAME ofn;
	char szFile[520];
	static long ticks = 0;

	int rowHeight, colWidth;
	int mouseXPos, mouseYPos;
	RECT rcWindow;
	CPlace *pStart = NULL;
	CPlace *pGoal = NULL;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Analizzare le selezioni di menu:
		switch (wmId)
		{
		case ID_AZIONI_AGGIUNGIPLACE:
			m_bPlaceEditMode = true;
			ShowCursor(true);
			SetCursor(LoadCursor(NULL, IDC_HAND));
			break;

		case ID_FILE_APRI:
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = (LPTSTR)szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = (LPTSTR)_T("All Files (*.*)\0*.*\0Dat File (*.dat)\0*.dat\0");
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn)==TRUE) 
			{
				delete myWorld;
				myWorld = new CWorld;
				char szLoadFile[260];
				string loadfile;
				size_t iConverted;
				wcstombs_s(&iConverted, szLoadFile, ofn.lpstrFile, 260);
				loadfile.append(szLoadFile);
				CurrentFile = loadfile;
				myWorld->Load(loadfile);
			}
			break;

		case ID_FILE_SALVA:
			if (CurrentFile=="")
			{
				OPENFILENAME ofn;
				char szFile[520];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = (LPTSTR)szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = (LPTSTR)_T("All Files (*.*)\0*.*\0Dat File (*.dat)\0*.dat\0");
				ofn.nFilterIndex = 2;
				ofn.lpstrDefExt = _T("dat");
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;
				if (GetSaveFileName(&ofn)==TRUE) 
				{
					char szSaveFile[260];
					string savefile;
					size_t iConverted;
					wcstombs_s(&iConverted, szSaveFile,ofn.lpstrFile,260);
					savefile.append(szSaveFile);
					CurrentFile = savefile;
					myWorld->Save(savefile);
				}
			}
			else
			{
				myWorld->Save(CurrentFile);
			}
			break;

		case ID_FILE_SALVACONNOME:
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = (LPTSTR)szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = (LPTSTR)_T("All Files (*.*)\0*.*\0Dat File (*.dat)\0*.dat\0");
			ofn.nFilterIndex = 2;
			ofn.lpstrDefExt = _T("dat");
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;
			if (GetSaveFileName(&ofn)==TRUE) 
			{
				char szSaveFile[260];
				string savefile;
				size_t iConverted;
				wcstombs_s(&iConverted, szSaveFile,ofn.lpstrFile,260);
				savefile.append(szSaveFile);
				CurrentFile = savefile;
				myWorld->Save(savefile);
			}
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 97: // a = Aggiunge un nuovo Place
			if (m_bPlaceEditMode)
			{
				if (myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y) == NULL)
				{
					CPlace *newplace = new CPlace;
					newplace->m_fPosX = (float)GridSelCell.x; 
					newplace->m_fPosY = (float)GridSelCell.y; 
					newplace->SetType(CPlace::PTFloor);
					myWorld->AddPlace(newplace[0]);
				}
			}
			break;
		case 43:  // + = Aumenta di 10 unità l'altitudine del Place corrente
			if (m_bPlaceEditMode)
			{
				if (myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y) != NULL)
				{
					CPlace *place = myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y);
					place->SetAltitude(place->GetAltitude() + 10.0f);
				}
			}
			break;
		case 45:  // - = Decrementa di 10 unità l'altitudine del Place corrente
			if (m_bPlaceEditMode)
			{
				if (myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y) != NULL)
				{
					CPlace *place = myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y);
					place->SetAltitude(place->GetAltitude() - 10.0f);
				}
			}
			break;
		case 100: // d = Elimina il Place dalla cella corrente
			if (m_bPlaceEditMode)
			{
				if (myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y) != NULL)
				{
					myWorld->DeletePlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y);
					GridDeletedCell = GridSelCell;
					m_bCellDeleted = true;
				}
			}
			break;

		case 112: // p = Build Path
			path = new CBasePath();
			path->BuildPath(myWorld, Start, Goal);
			break;

		case 115: // s = Set Path Start
			pStart = myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y);
			if (pStart != NULL) Start = *pStart;
			break;

		case 103: // g = Set Path Goal
			pGoal = myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y);
			if (pGoal != NULL) Goal = *pGoal;
			break;
		}

		break;

	case WM_LBUTTONUP:
		mouseXPos = LOWORD(lParam);
		mouseYPos = HIWORD(lParam);
		GetClientRect(hWnd,&rcWindow);
		rowHeight = (rcWindow.bottom-rcWindow.top) / iGridNumOfRows;
		colWidth  = (rcWindow.right-rcWindow.left) / iGridNumOfCols;

		GridOldSelCell = GridSelCell;
		GridSelCell.y = mouseXPos / colWidth;
		GridSelCell.x = mouseYPos / rowHeight;
		
		m_bSelCellChanged = TRUE;
		break;

	case WM_LBUTTONDBLCLK:
		mouseXPos = LOWORD(lParam);
		mouseYPos = HIWORD(lParam);
		GetClientRect(hWnd,&rcWindow);
		rowHeight = (rcWindow.bottom-rcWindow.top) / iGridNumOfRows;
		colWidth  = (rcWindow.right-rcWindow.left) / iGridNumOfCols;

		GridOldSelCell = GridSelCell;
		GridSelCell.y = mouseXPos / colWidth;
		GridSelCell.x = mouseYPos / rowHeight;
		
		m_bSelCellChanged = TRUE;
		if (m_bPlaceEditMode)
		{
			if (myWorld->GetPlaceAtPos((float)GridSelCell.x,(float)GridSelCell.y) == NULL)
			{
				CPlace *newplace = new CPlace;
				newplace->m_fPosX = (float)GridSelCell.x; 
				newplace->m_fPosY = (float)GridSelCell.y; 
				newplace->SetType(CPlace::PTFloor);
				myWorld->AddPlace(newplace[0]);
			}
		}
		break;

	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		if ((GetTickCount() - ticks > 1000) || (ticks == 0))
		{
			RedrawWorld();
			ticks = GetTickCount();
		}
		//EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Gestore dei messaggi della finestra Informazioni su.
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
