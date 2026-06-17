#include "Setting.h"
#include <stdio.h>
#include "resource.h"
#include "DefOrg.h"
#include "OrgData.h"
#include "rxoFunction.h"
#include <string>

RECT CmnDialogWnd;
int count_of_INIT_DONE;
int iDlgRepeat; //ダイアログから取得した繰り返し回数
extern void TitlebarRefresh(void);

char GetFileNameSave(HWND hwnd, char* title)
{//ファイル名を取得(セーブ)
	OPENFILENAME ofn;
	//FILE *fp;
//	char res;//ファイルオープンの結果
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = hInst;
	ofn.lpstrFile = mus_file;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	ofn.Flags = OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT | OFN_HIDEREADONLY;

	if (!org_data.TrackFlag())
	{
		ofn.lpstrFilter = "OrganyaData[*.org]\0*.org\0\0";
		ofn.lpstrDefExt = "org";
	}
	else
	{
		ofn.lpstrFilter = "Organya16 Data[*.org16]\0*.org16\0\0";
		ofn.lpstrDefExt = "org16";
	}

	//ファイル名取得を試みる。
	if (GetSaveFileName(&ofn));//InvalidateRect(hwnd,NULL,TRUE);
	else return MSGCANCEL;//キャンセルで0が返る
	return MSGSAVEOK;
}

// グラフィックス描画 
int DrawGr(HWND hWnd, HDC hdc) {
	return 0; //もう何もしないよ。
	HPEN hPen, hOldPen;
	RECT rt;
	GetClientRect(hWnd, &rt);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	hOldPen = (HPEN )SelectObject(hdc, hPen);
	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, rt.right, rt.bottom);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 255));
	hOldPen = (HPEN )SelectObject(hdc, hPen);
	MoveToEx(hdc, rt.right, 0, NULL);
	LineTo(hdc, 0, rt.bottom);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	return 0;
} 

//フックプロシージャ

char GetFileNameLoad(HWND hwnd,char *title, char *filename)
{//ファイル名を取得(ロード)
	OPENFILENAME ofn;
	//FILE *fp;
	char mfile[MAX_PATH];

	memset(&ofn, 0, sizeof(OPENFILENAME));
	memset(&mfile, '\0', MAX_PATH);
//	strcpy(GetName,"*.pmd"); WOAH PMD??
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner   = hwnd;
	ofn.hInstance   = hInst;
	ofn.lpstrFilter = "Organya Files[*.org & *.org16]\0*.org; *.org16\0All Files[*.*]\0*.*\0\0";
	ofn.lpstrFile   = mfile;
	ofn.nMaxFile    = MAX_PATH;
	ofn.lpstrTitle  = title;
	ofn.Flags       = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	ofn.lpstrDefExt = "org16", "org";
	
	count_of_INIT_DONE = 0;
	//ファイル名取得を試みる。
	if(GetOpenFileName(&ofn));//InvalidateRect(hwnd,NULL,TRUE);
	else return MSGCANCEL;//キャンセルで0が返る
	if(org_data.FileCheckBeforeLoad(mfile)){
		return MSGCANCEL;//指定ファイルが存在しない
	}


	strcpy(mus_file,mfile);
	

	return MSGLOADOK;
}

char GetFileNameExportWav(HWND hwnd, char* title, char* filename)
{
	OPENFILENAME ofn;
	//FILE* fp;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	strcpy(filename, mus_file);
	char* p;
	if ((p = strstr(filename, ".org")) != NULL) {
		strcpy(p, ".wav");
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = hInst;
	//ofn.lpstrFilter = "OrganyaData[*.org]\0*.org\0全ての形式 [*.*]\0*.*\0\0";	// 2014.10.19 D
	ofn.lpstrFilter = MessageString[IDS_STRING119];	// 2014.10.19 A
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	ofn.Flags = OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "wav";

	//ファイル名取得を試みる。
	if (GetSaveFileName(&ofn));//InvalidateRect(hwnd,NULL,TRUE);
	else return MSGCANCEL;//キャンセルで0が返る
	//fp = fopen(filename, "rb");
	//既存ファイルが存在する？ OFN_OVERWRITEPROMPT 指定で不要とした。
	//if(fp != NULL){
	//	fclose(fp);
	//	return MSGEXISFILE;//既存ファイル
	//}
	return MSGSAVEOK;
}