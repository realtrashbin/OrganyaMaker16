#include "TrackFlag.h"

#include <string>
#include <string.h>
#include <stdio.h>
#include "Setting.h"
#include "OrgData.h"
#include "DefOrg.h"
#include "rxoFunction.h"

MUSICINFO mus;

BOOL OrgData::TrackFlag(void) //Used for ORG-16
{
	char i; //Track

	//Checks Melody
	for (i = 8; i < MAXMELODY; i++) //i = Last STD Track, i < maxmium track
	{
		mus.tdata[i].note_num = GetNoteNumber(i, NULL);
		if (mus.tdata[i].note_num == NULL)
		{
			continue;
		}
		else
		{
			return true;
		}
	}
	for (i = 24; i < MAXTRACK; i++)
	{
		mus.tdata[i].note_num = GetNoteNumber(i, NULL);
		if (mus.tdata[i].note_num == NULL)
		{
			continue;
		}
		else
		{
			return true;
		}
	}
	return false;
}

int binTrackCode(char* str)
{

	int i=0;
	switch ((char) *str) //Credits to Mr. Kryzstof Kudlak; I would've never figured out it should have been (char) *str instead of anything else.
	{
	case 'Q':
	case 'q':
	case 'W':
	case 'w':
	case 'E':
	case 'e':
	case 'R':
	case 'r':
	case 'T':
	case 't':
	case 'Y':
	case 'y':
	case 'U':
	case 'u':
	case 'I':
	case 'i':
	case 'A':
	case 'a':
	case 'S':
	case 's':
	case 'D':
	case 'd':
	case 'F':
	case 'f':
	case 'G':
	case 'g':
	case 'H':
	case 'h':
	case 'J':
	case 'j':
	case 'K':
	case 'k':
		i = ReverseTrackCode((char) *str); //Should work now...
		break;
	}


	if (!i)
	{
		i = ReverseTrackCode(atoi(str));
	}
	
	return i;
}

void TitlebarRefresh(void)
{
	std::string Ext(mus_file);
	if (org_data.TrackFlag() && strstr(mus_file, "org16") == NULL)
	{
		strcpy(mus_file, Ext.replace(Ext.find_last_of("o"), Ext.find_last_of("g"), "org16").c_str());
	}
	else if (!org_data.TrackFlag() && strstr(mus_file, "org16") != NULL)
	{
		strcpy(mus_file, Ext.replace(Ext.find_last_of("o"), Ext.find_last_of("6"), "org\0\0").c_str());
	}
}

