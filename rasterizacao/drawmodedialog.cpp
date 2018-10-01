#include "drawmodedialog.h"
#include <wx/sizer.h>

DrawModeDialog::DrawModeDialog(const wxString& prompt)
    : wxDialog(NULL, -1, prompt, wxDefaultPosition)
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxString opcoes[] = { wxT("Linhas"), wxT("Círculos"), wxT("Elipses"),
                          wxT("Triângulos"), wxT("Comandos do arquivo")};
    mRadio = new wxRadioBox(this, wxID_ANY, wxT("Tipo de desenho"), wxDefaultPosition,
                            wxDefaultSize, wxArrayString(WXSIZEOF(opcoes), opcoes), 0,
                            wxRA_SPECIFY_ROWS);
    topSizer->Add(mRadio);
    topSizer->Add(CreateButtonSizer(wxOK|wxCANCEL));
    SetSizerAndFit(topSizer);
}

//virtual
int DrawModeDialog::ShowModal()
{
    int result = wxDialog::ShowModal();
    value = mRadio->GetSelection();
    return result;
}

int DrawModeDialog::Get()
{
    return value;
}

