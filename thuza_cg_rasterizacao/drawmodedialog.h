#ifndef DRAWMODE_DIALOG_H
#define DRAWMODE_DIALOG_H

#include "wx/dialog.h"
#include "wx/radiobox.h"

class DrawModeDialog : public wxDialog
{
    public:
        DrawModeDialog(const wxString& prompt);
        virtual int ShowModal();
        int Get();
        int value;
    protected:
        wxRadioBox* mRadio;
};

#endif

