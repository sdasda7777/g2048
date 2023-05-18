#include "g2048.h"

void PegAppInitialize(PegPresentationManager *pPresentation)
{   
	PegRect Rect;
    Rect.Set(MAINFRAME_LEFT, MAINFRAME_TOP, MAINFRAME_RIGHT, MAINFRAME_BOTTOM);
    pPresentation->Add(new g2048(Rect, FF_NONE));
}

extern "C" char *ExtensionGetLang(ID_MESSAGE MessageNo)
{
	return "";
}
