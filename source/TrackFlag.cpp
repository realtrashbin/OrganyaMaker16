#include "TrackFlag.h"

#include <string>
#include <string.h>
#include "Setting.h"
#include "OrgData.h"
#include "DefOrg.h"
#include "rxoFunction.h"

extern unsigned short OrgFlag[ALLOCFLAG][5];
extern void SetTitlebarText();


BOOL OrgData::TrackFlag(void) //Used for ORG-16
{
	char i;
	MUSICINFO mi;

	for (i = 8; i < 16; i++)
	{
		mi.tdata[i].note_num = GetNoteNumber(i, NULL);
		mi.tdata[i+16].note_num = GetNoteNumber(i+16, NULL);
		if (mi.tdata[i].note_num != NULL || mi.tdata[i+16].note_num != NULL)
		{
			return TRUE;
		}
	}

	for (i = 0; i < ALLOCFLAG; i++)
	{
		if (OrgFlag[i][0] != NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int binTrackCode(char* str)
{
	int i=0;
	i = ReverseTrackCode((char) *str); //Should work now...
	if (!i)i = ReverseTrackCode(atoi(str));
	return i;
}

void TitlebarRefresh(void)
{
	std::string Ext(mus_file);
	if (org_data.TrackFlag() && strstr(mus_file, "org16") == NULL)
	{
		strcpy(mus_file, Ext.replace(Ext.find_last_of("o"), Ext.find_last_of("g"), "org16").c_str());
	}
	else if (!org_data.TrackFlag())
	{
		strcpy(mus_file, Ext.replace(Ext.find_last_of("o"), Ext.find_last_of("6"), "org\0\0").c_str());
	}
	SetTitlebarText();
}

