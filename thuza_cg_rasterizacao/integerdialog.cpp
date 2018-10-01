#include "integerdialog.h"
#include <wx/sizer.h>

IntegerDialog::IntegerDialog(const wxString& prompt)
    : wxDialog(NULL, -1, prompt, wxDefaultPosition)
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    spinCtrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("7"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 19);
    topSizer->Add(spinCtrl);
    topSizer->Add(CreateButtonSizer(wxOK|wxCANCEL));
    SetSizerAndFit(topSizer);
}

//virtual
int IntegerDialog::ShowModal()
{
    int result = wxDialog::ShowModal();
    value = spinCtrl->GetValue();
    return result;
}

int IntegerDialog::Get()
{
    return value;
}

