#ifndef INTEGER_DIALOG_H
#define INTEGER_DIALOG_H

#include "wx/dialog.h"
#include "wx/spinctrl.h"

class IntegerDialog : public wxDialog
{
    public:
        IntegerDialog(const wxString& prompt);
        virtual int ShowModal();
        int Get();
        int value;
    protected:
        wxSpinCtrl* spinCtrl;
};

#endif

