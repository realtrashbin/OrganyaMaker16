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
extern char strMIDI_TITLE[256];
extern char strMIDI_AUTHOR[256];
extern void TitlebarRefresh(void);
LPCTSTR  MIDIPC[]={
	"000 Acoustic Grand Piano","001 Bright Acoustic Piano","002 Electric Grand Piano","003 Honky-tonk Piano","004 Electric Piano 1","005 Electric Piano 2","006 Harpsichord","007 Clavi",
	"008 Celesta","009 Glockenspiel","010 Music Box","011 Vibraphone","012 Marimba","013 Xylophone","014 Tubular Bells","015 Dulcimer",
	"016 Drawbar Organ","017 Percussive Organ","018 Rock Organ","019 Church Organ","020 Reed Organ","021 Accordion","022 Harmonica","023 Tango Accordion",
	"024 Acoustic Guitar (nylon)","025 Acoustic Guitar (steel)","026 Electric Guitar (jazz)","027 Electric Guitar (clean)","028 Electric Guitar (muted)","029 Overdriven Guitar","030 Distortion Guitar","031 Guitar Harmonics",
	"032 Acoustic Bass","033 Electric Bass (finger)","034 Electric Bass (pick)","035 Fretless Bass","036 Slap Bass 1","037 Slap Bass 2","038 Synth Bass 1","039 Synth Bass 2",
	"040 Violin","041 Viola","042 Cello","043 Contrabass","044 Tremolo Strings","045 Pizzicato Strings","046 Orchestral Harp","047 Timpani",
	"048 String Ensemble 1","049 String Ensemble 2","050 Synth Strings 1","051 Synth Strings 2","052 Choir Aahs","053 Voice Oohs","054 Synth Voice","055 Orchestra Hit",
	"056 Trumpet","057 Trombone","058 Tuba","059 Muted Trumpet","060 French Horn","061 Brass Section","062 Synth Brass 1","063 Synth Brass 2",
	"064 Soprano Sax","065 Alto Sax","066 Tenor Sax","067 Baritone Sax","068 Oboe","069 English Horn","070 Bassoon","071 Clarinet",
	"072 Piccolo","073 Flute","074 Recorder","075 Pan Flute","076 Blown Bottle","077 Shakuhachi","078 Whistle","079 Ocarina",
	"080 Lead 1 (square)","081 Lead 2 (sawtooth)","082 Lead 3 (calliope)","083 Lead 4 (chiff)","084 Lead 5 (charang)","085 Lead 6 (voice)","086 Lead 7 (fifths)","087 Lead 8 (bass+lead)",
	"088 Pad 1 (new age)","089 Pad 2 (warm)","090 Pad 3 (polysynth)","091 Pad 4 (choir)","092 Pad 5 (bowed)","093 Pad 6 (metallic)","094 Pad 7 (halo)","095 Pad 8 (sweep)",
	"096 FX 1 (ice rain)","097 FX 2 (soundtrack)","098 FX 3 (crystal)","099 FX 4 (atmosphere)","100 FX 5 (brightness)","101 FX 6 (goblins)","102 FX 7 (echoes)","103 FX 8 (sci-fi)",
	"104 Sitar","105 Banjo","106 Shamisen","107 Koto","108 Kalimba","109 Bag Pipe","110 Fiddle","111 Shanai",
	"112 Tinkle Bell","113 Agogo","114 Steel Drums","115 Woodblock","116 Taiko Drum","117 Melodic Tom","118 Synth Drum","119 Reverse Cymbal",
	"120 Guitar Fret Noise","121 Breath Noise","122 Seashore","123 Bird Tweet","124 Telephone Ring","125 Helicopter","126 Applause","127 Gunshot"
};

extern unsigned char ucMIDIProgramChangeValue[MAXTRACK];
char GetFileNameSave(HWND hwnd, char* title)
{//ファイル名を取得(セーブ)
	OPENFILENAME ofn;
	int i;
	const char* nExt;
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

	TitlebarRefresh();

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
UINT CALLBACK OFNHookProcMID(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//RECT  rcWindow;
	LPOFNOTIFY lpOfn = (LPOFNOTIFY)lParam;
	char ctmp[12];
	int DlgWidth, DlgHeight, i,j;
	HWND haDlg;
	HDC hdc;
	PAINTSTRUCT ps;
	static MUSICINFO mi;
	switch(msg){
        case WM_INITDIALOG:
			org_data.GetMusicInfo(&mi);
			//iDlgRepeat = 1;
			itoa(iDlgRepeat ,ctmp, 10);
			SetDlgItemText(hdlg, IDC_REPEAT, ctmp);
			SetDlgItemText(hdlg, IDC_AUTHOR, strMIDI_AUTHOR);
			SetDlgItemText(hdlg, IDC_TITLE,  strMIDI_TITLE);
 			for(j=0;j<8;j++){
				SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_RESETCONTENT,0,0);//初期化
				for(i=0;i<128;i++){
					SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_ADDSTRING,0,(LPARAM)MIDIPC[i]);
				}
				if(ucMIDIProgramChangeValue[j] == 255){
					ucMIDIProgramChangeValue[j] = mi.tdata[j].wave_no; //こんなところで初期化
				}
				SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_SETCURSEL,ucMIDIProgramChangeValue[j],0);
			}
           return TRUE;

		case WM_SIZE:
			DlgWidth  = LOWORD(lParam);	//クライアント領域のサイズ
			DlgHeight = HIWORD(lParam);
			haDlg = GetDlgItem(hdlg, IDC_TITLE);
			SetWindowPos(haDlg, HWND_TOP, 1, 1, DlgWidth-150, 18, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
			haDlg = GetDlgItem(hdlg, IDC_AUTHOR);
			SetWindowPos(haDlg, HWND_TOP, 1, 1, DlgWidth-150, 18, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
			return TRUE;

		case WM_PAINT:
			hdc = BeginPaint(hdlg, &ps);
			DrawGr(hdlg, hdc);
			EndPaint(hdlg, &ps);
			return TRUE; 		
		case WM_COMMAND:
			switch(LOWORD(wParam)){
			case IDC_ALL_SQUAREWAVE1:
				i = SendDlgItemMessage(hdlg,IDC_MIDIPC1,CB_GETCURSEL,0,0);
				for(j=1;j<8;j++) SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_SETCURSEL,i,0);
				break;
			case IDC_ALL_SQUAREWAVE2:
				for(j=0;j<8;j++) SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_SETCURSEL,16,0);
				break;
			case IDC_ALL_SQUAREWAVE3:
				for(j=0;j<8;j++) SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_SETCURSEL,80,0);
				break;
			case IDC_ALL_SQUAREWAVE4:
				for(j=0;j<8;j++) SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_SETCURSEL,(rand() & 0x7F),0);
				break;
			}

			return TRUE;
		case WM_NOTIFY:
			if(lpOfn->hdr.code == CDN_INITDONE ){
			}
			if(lpOfn->hdr.code == CDN_FILEOK ){
				GetDlgItemText(hdlg,IDC_REPEAT,ctmp,12); iDlgRepeat = atol(ctmp);
				iDlgRepeat = (iDlgRepeat>0xFFFF) ? 0xFFFF : ((iDlgRepeat<=0) ? 1: iDlgRepeat);
				memset(strMIDI_AUTHOR, 0, 255);
				GetDlgItemText(hdlg, IDC_AUTHOR, strMIDI_AUTHOR, 250);
				memset(strMIDI_TITLE, 0, 255);
				GetDlgItemText(hdlg, IDC_TITLE,  strMIDI_TITLE,  250);
				for(j=0;j<8;j++){
					ucMIDIProgramChangeValue[j] = (unsigned char)SendDlgItemMessage(hdlg,IDC_MIDIPC1+j,CB_GETCURSEL,0,0);
				}

				//MessageBox(NULL,ctmp,"Message",MB_OK);
			}
			//MessageBox(NULL,"初期化しました","Message",MB_OK);
			return TRUE;
    }
    return FALSE;
}
char GetFileNameXM(HWND hwnd,char *title, char *filename)
{//ファイル名を取得(MIDI)
	OPENFILENAME ofn;
	//FILE *fp;
//	char res;//ファイルオープンの結果

	memset(&ofn,0,sizeof(OPENFILENAME));
	strcpy(filename, mus_file);
	char *p,*o;
	if( (p = strstr(filename, ".org")) || ((p = strstr(filename, ".org16")) != NULL)) {
		strcpy(p, ".xm");
	}

//	strcpy(GetName,"*.pmd");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner   = hwnd;
	ofn.hInstance   = hInst;
	ofn.lpstrFilter = "XM Module(*.xm)\0*.xm\0\0";	// 2014.10.19 A
	ofn.lpstrFile   = filename;
	ofn.nMaxFile    = MAX_PATH;
	ofn.lpstrTitle  = title;
    ofn.Flags       = OFN_EXPLORER |
                      OFN_OVERWRITEPROMPT |
                      OFN_CREATEPROMPT |
                      OFN_HIDEREADONLY |
                      OFN_ENABLESIZING |
                      OFN_ENABLEHOOK |     //フックプロシージャを使う
                      OFN_ENABLETEMPLATE;
	ofn.lpfnHook = OFNHookProcMID;
	ofn.lpTemplateName = MAKEINTRESOURCE(IDD_MIDI);
		

	ofn.lpstrDefExt = "xm";

	//ファイル名取得を試みる。
	if(GetSaveFileName(&ofn));//InvalidateRect(hwnd,NULL,TRUE);
	else return MSGCANCEL;//キャンセルで0が返る
	return MSGSAVEOK;
}

//フックプロシージャ

char GetFileNameLoad(HWND hwnd,char *title, char *filename)
{//ファイル名を取得(ロード)
	OPENFILENAME ofn;
	//FILE *fp;
	char mfile[MAX_PATH];
	char* p;

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