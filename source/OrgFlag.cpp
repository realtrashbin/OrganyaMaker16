#include "TrackFlag.h"

#include "Setting.h"
#include "OrgData.h"
#include "DefOrg.h"
#include "Sound.h"

long OrgFlagX[ALLOCFLAG];
unsigned short OrgFlag[ALLOCFLAG][5];//32 Flags, 5 elements
/*
* 0. Mode
* 1. Track Num
* 2. Wave Num
* 3. Pipi
* 4. Freq.
*/
unsigned short OrgFlagUndo[ALLOCFLAG][3]; //undoes flags
/*
* 0. Wave Num
* 1. Pipi
* 2. Freq.
*/
char FlagFinder(long x,bool clearflag)
{
	//Used for anything relating to clicking and flags
	//Ex. Deleting a flag, it has to get the flag ID to clear it obviously.
	char i;
	for (i = 0; i < ALLOCFLAG; i++)
	{
		if (OrgFlagX[i] == x)
		{
			break;
		}
	}
	if (clearflag == true)
	{
		OrgFlagX[i] = 0;
		for (char j = 0; j < 4; j++)
		{
			OrgFlag[i][j] = 0;
		}
	}
	return i; //returns flag
}

void FlagsMoveActivate(long x)
{
	char i;
	//char str[20];
	MUSICINFO mi;
	
	for (i = 0; i < MAXTRACK; i++)
	{
		org_data.GetMusicInfo(&mi);
		if (i < MAXMELODY) {
			org_data.GetMusicInfo(&mi);
			mi.tdata[i].wave_no = OrgFlagUndo[i][0];
			mi.tdata[i].pipi = OrgFlagUndo[i][1];
			mi.tdata[i].freq = OrgFlagUndo[i][2];
			MakeOrganyaWave(i, OrgFlagUndo[i][0], OrgFlagUndo[i][1]); //Melody
			org_data.SetMusicInfo(&mi, SETWAVE | SETPIPI | SETFREQ);
		}
		else mi.tdata[i].wave_no = OrgFlagUndo[i][0]; InitDramObject(OrgFlagUndo[i][0],i-MAXMELODY); org_data.SetMusicInfo(&mi, SETWAVE);
	}
	
	for (i = 0; i < ALLOCFLAG; i++)
	{
		if ((x >= OrgFlagX[i]) && (x != 0 && OrgFlagX[i] != 0))
		{
			FunctionChange(i);
		}
	}
}

void FunctionChange(unsigned char flag)
{
	MUSICINFO mi;
	switch (OrgFlag[flag][0])
	{
		case 1:
		{
			org_data.GetMusicInfo(&mi);
			Rxo_StopTrackSound(OrgFlag[flag][1]);
			if (OrgFlag[flag][1] < MAXMELODY) {
				mi.tdata[OrgFlag[flag][1]].wave_no = OrgFlag[flag][2];
				mi.tdata[OrgFlag[flag][1]].pipi = OrgFlag[flag][3];
				MakeOrganyaWave(OrgFlag[flag][1], OrgFlag[flag][2], OrgFlag[flag][3]); //Melody
				org_data.SetMusicInfo(&mi, SETWAVE | SETPIPI);
			}
			else
			{
				Rxo_StopTrackSound(OrgFlag[flag][1]);
				mi.tdata[OrgFlag[flag][1]].wave_no = OrgFlag[flag][2];
				InitDramObject(OrgFlag[flag][2], OrgFlag[flag][1] - MAXMELODY);
				org_data.SetMusicInfo(&mi, SETWAVE);
			}
			return;
		}
		case 2:
		{
			Rxo_StopTrackSound(OrgFlag[flag][1]);
			org_data.GetMusicInfo(&mi);
			mi.tdata[OrgFlag[flag][1]].freq = OrgFlag[flag][4];
			org_data.SetMusicInfo(&mi, SETFREQ);
			return;
		}
		default:
		{
			return;
		}
	}
}

void WhyAquaDoesStuff(void)
{
	//Niko Says "hi".

}