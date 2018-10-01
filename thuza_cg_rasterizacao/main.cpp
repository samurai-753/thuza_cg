#include "wx/wx.h"
#include "integerdialog.h"
#include "drawmodedialog.h"
#include "bigpixelcanvas.h"

// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
#endif

typedef enum {
    LINES,
    CIRCLES,
    ELIPSES,
    TRIANGLES,
    COMANDOS
} DrawMode;

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp {
    public:
        // override base class virtuals
        // ----------------------------

        // this one is called on application startup and is a good place for the app
        // initialization (doing it here and not in the ctor allows to have an error
        // return: if OnInit() returns false, the application terminates)
        virtual bool OnInit();

        virtual int OnExit() { return 0; }

    protected:
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame {
    public:
        // ctor(s)
        MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

        // event handlers (these functions should _not_ be virtual)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOption(wxCommandEvent &event);
        void OnClick(wxMouseEvent &event);

        void ReadPixelSize();
        void SetDrawMode();

        BigPixelCanvas* mCanvasPtr;
        wxMenu* mMenuFilePtr;
        DrawMode mDrawMode;
    protected:
        int mContadorCliques;

    private:
        // any class wishing to process wxWidgets events must use this macro
        DECLARE_EVENT_TABLE()
}; // MyFrame

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum {
    // menu items
    File_Quit = wxID_EXIT,
    File_About = wxID_ABOUT,

    MenuShow_First = wxID_HIGHEST,

    MenuOption_First = MenuShow_First,
    DRAW_MODE = MenuOption_First,
    PIXEL_SIZE,

    MenuOption_Last = PIXEL_SIZE
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------


// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // Create the main application window
    MyFrame *frame = new MyFrame(_T("Rasterização"),
                                 wxPoint(50, 50), wxSize(800, 600));

    // Show it and tell the application that it's our main window
    frame->Show(true);
    SetTopWindow(frame);

    // ok, continue
    return true;
}

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU      (File_Quit,     MyFrame::OnQuit)
    EVT_MENU      (File_About,    MyFrame::OnAbout)
    EVT_MENU_RANGE(MenuOption_First, MenuOption_Last, MyFrame::OnOption)
    EVT_LEFT_UP   (MyFrame::OnClick)
END_EVENT_TABLE()

using namespace std;

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE),
         mDrawMode(LINES), mContadorCliques(0)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    mMenuFilePtr = new wxMenu;
    mMenuFilePtr->Append(DRAW_MODE, _T("Modo de &Desenho...\tCtrl-D"),
                         _T("Permite a escolha da primitiva a ser desenhada."));
    mMenuFilePtr->Append(PIXEL_SIZE, _T("Tamanho &Pixel...\tCtrl-P"), _T("sets the pixel size"));
    mMenuFilePtr->AppendSeparator();
    mMenuFilePtr->Append(File_About, _T("Sobre...\tCtrl-A"), _T("Show about dialog"));
    mMenuFilePtr->AppendSeparator();
    mMenuFilePtr->Append(File_Quit, _T("Sair\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(mMenuFilePtr, _T("&Opções"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText(_T("Clique em dois pontos para desenhar um segmento de reta."));

    mCanvasPtr = new BigPixelCanvas( this );
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxString msg;
    msg.Printf( wxT("Interface para BigPixelCanvas.\n")
                wxT("Feito por Bruno Schneider, Jun 2014.\n")
                wxT("Baseado no exemplo 'drawing' do wxWidgets, por Robert Roebling.")
              );

    wxMessageBox(msg, _T("Sobre este programa"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::ReadPixelSize() {
    IntegerDialog dialog(wxT("Escolha o tamanho do pixel"));
    if (dialog.ShowModal() == wxID_OK) {
        mCanvasPtr->mPixelSize = dialog.Get();
        wxString str;
        str << _T("Tamanho do pixel alterado para ") << mCanvasPtr->mPixelSize;
        SetStatusText(str);
    }
}

void MyFrame::SetDrawMode() {
    DrawModeDialog dialog(wxT("Escolha o modo de desenho"));
    if (dialog.ShowModal() == wxID_OK) {
        mDrawMode = static_cast<DrawMode>(dialog.Get());
        mCanvasPtr->mUsandoComandos = mDrawMode == COMANDOS;
        mCanvasPtr->Refresh();
    }
    mContadorCliques = 0;
}

void MyFrame::OnOption(wxCommandEvent& event) {
    switch (event.GetId()) {
        case DRAW_MODE: {
            SetDrawMode();
            wxString feedback;
            switch (mDrawMode) {
                case LINES:
                    feedback = wxT("Clique em dois pontos para desenhar um segmento de reta.");
                    break;
                case CIRCLES:
                    feedback = wxT("Clique no centro e num ponto da borda para desenhar um círculo.");
                    break;
                case TRIANGLES:
                    feedback = wxT("Clique em três pontos para desenhar um triângulo.");
                    break;
                default:
                    break;
            }
            SetStatusText(feedback);
            break;
        }
        case PIXEL_SIZE:
            ReadPixelSize();
            mCanvasPtr->Refresh();
            break;
        default:
            // skip Refresh()
            return;
    }
}

void MyFrame::OnClick(wxMouseEvent &event) {
    static wxPoint p1;
    static wxPoint p2;

    wxPoint pos = event.GetPosition();
    wxPoint logicalPos = mCanvasPtr->ConvertDeviceToLogical(pos);
    wxString str;
    str.Printf(wxT("Clicou em: %d,%d"), logicalPos.x, logicalPos.y);
    SetStatusText( str );
    ++mContadorCliques;
    switch (mDrawMode)
    {
        case LINES:
            if (mContadorCliques == 1)
                p1 = pos;
            else {
                mCanvasPtr->DrawLine(p1, pos);
                mContadorCliques = 0;
            }
            break;
        case CIRCLES:
            if (mContadorCliques == 1)
                p1 = pos;
            else {
                int deltaX = p1.x - pos.x;
                int deltaY = p1.y - pos.y;
                int radius = static_cast<int>(sqrt(deltaX*deltaX + deltaY*deltaY));
                mCanvasPtr->DrawCircle(p1, radius);
                mContadorCliques = 0;
            }
            break;
        case TRIANGLES:
            if (mContadorCliques == 1)
                p1 = logicalPos;
            else {
                if (mContadorCliques == 2)
                    p2 = logicalPos;
                else {
                    BigPixelCanvas::Triang2D triangulo(
                        BigPixelCanvas::Ponto(p1.x, p1.y, 0),
                        BigPixelCanvas::Ponto(p2.x, p2.y, 0),
                        BigPixelCanvas::Ponto(logicalPos.x, logicalPos.y, 0));
                    mCanvasPtr->DesenharTriangulo2D(triangulo);
                    mContadorCliques = 0;
                }
            }
            break;
        default:
            break;
    }
}

