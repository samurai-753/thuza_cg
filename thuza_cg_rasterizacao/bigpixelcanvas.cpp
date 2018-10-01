#include "bigpixelcanvas.h"
#include <wx/dcclient.h>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

BEGIN_EVENT_TABLE(BigPixelCanvas, wxPanel)
    EVT_PAINT    (BigPixelCanvas::OnPaint)
    EVT_LEFT_UP  (BigPixelCanvas::OnClick)
END_EVENT_TABLE()


inline wxColour operator-(const wxColour& c1, const wxColour& c2) {
    unsigned char red = c1.Red() - c2.Red();
    unsigned char green = c1.Green() - c2.Green();
    unsigned char blue = c1.Blue() - c2.Blue();
    return wxColour(red, green, blue);
}

inline wxColour operator*(const wxColour& c, float n) {
    unsigned char red = c.Red() * n;
    unsigned char green = c.Green() * n;
    unsigned char blue = c.Blue() * n;
    return wxColour(red, green, blue);
}

inline wxColour operator+(const wxColour& c1, const wxColour& c2) {
    unsigned char red = c1.Red() + c2.Red();
    unsigned char green = c1.Green() + c2.Green();
    unsigned char blue = c1.Blue() + c2.Blue();
    return wxColour(red, green, blue);
}

BigPixelCanvas::BigPixelCanvas(wxFrame *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
      mPixelSize(5),
      mUsandoComandos(false),
      mBackgroundMode(wxSOLID),
      mColourForeground(*wxGREEN),
      mColourBackground(*wxWHITE),
      mPen(*wxBLACK_PEN)
{
    mOwnerPtr = parent;
    m_clip = false;
}

void BigPixelCanvas::DrawPixel(int x, int y, wxDC& dc)
{
    x *= mPixelSize;
    y *= mPixelSize;

    int halfPixelSize = mPixelSize / 2;
    int xStart = x - halfPixelSize;
    int xEnd = x + halfPixelSize;
    int yStart = y - halfPixelSize;
    int yEnd = y + halfPixelSize;
    for (int x = xStart; x <= xEnd; ++x)
        for (int y = yStart; y <= yEnd; ++y)
            dc.DrawPoint(x,y);
}

void BigPixelCanvas::DrawPixel(int x, int y, double z, wxDC& dc)
{
    if (mZBuffer.IsVisible(y, x, z)) {
        x *= mPixelSize;
        y *= mPixelSize;
        int halfPixelSize = mPixelSize / 2;
        int xStart = x - halfPixelSize;
        int xEnd = x + halfPixelSize;
        int yStart = y - halfPixelSize;
        int yEnd = y + halfPixelSize;
        for (int x = xStart; x <= xEnd; ++x)
            for (int y = yStart; y <= yEnd; ++y)
                dc.DrawPoint(x,y);
    }
}

void BigPixelCanvas::DrawLine(wxPoint p0, wxPoint p1)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    p0 = ConvertDeviceToLogical(p0);
    p1 = ConvertDeviceToLogical(p1);
    DrawLine(p0, p1, dc);
}

void BigPixelCanvas::DrawLine(const wxPoint& p0,const wxPoint& p1, wxDC& dc) {
    // Aqui o codigo para desenhar uma linha.
    int dy = (p1.y - p0.y);
    int dx = (p1.x - p0.x);
    
    int sinal = 1;
    
    int x = p0.x;
    int y = p0.y;
    int xFinal = p1.x;
    int yFinal = p1.y;

    // se estiver nos octantes da esquerda, invertemos os pontos

    if(dx < 0){
        x = p1.x;
        y = p1.y;
        xFinal = p0.x;
        yFinal = p0.y;
        dy = -dy;
        dx = -dx;
    }

    if(dy < 0){
        sinal = -1;
        dy = -dy;
    }


    if(dy < dx) {

        int d = 2 * dy - dx;
        int variacaoLeste = 2 * dy;
        int variacaoNordeste = (dy - dx) * 2;

        DrawPixel(x, y, dc);
        

        while(x < xFinal) {
            if(d > 0) {
                d += variacaoNordeste;
                y += sinal;
            } else {
                d += variacaoLeste;
            }
            x++;
            DrawPixel(x, y, dc);
        }

    } else {
        int d = 2 * dx - dy;
        int variacaoLeste = 2 * dx;
        int variacaoNordeste = (dx - dy) * 2;

        DrawPixel(x, y, dc);
        while(y*sinal < yFinal*sinal) {
            if(d > 0) {
                d += variacaoNordeste;
                x++;
            } else {
                d += variacaoLeste;
            }
            y += sinal;
            DrawPixel(x, y, dc);
        }
    }
}

void BigPixelCanvas::DrawCircle(wxPoint center, int radius)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    center = ConvertDeviceToLogical(center);
    DrawCircle(center, radius/mPixelSize, dc);
}

void BigPixelCanvas::DrawCircle(const wxPoint& center, int radiusx, wxDC& dc)
{
    // Aqui o codigo para desenhar um circulo.
    
    for(int radius = 0; radius < radiusx; radius++) { 
        int d = 1 - radius;
        int y = radius;
        int x = 0;
        int limite = 0.7071 * radius;
        DrawPixel(center.x, center.y + y, dc);
        DrawPixel(center.x + y, center.y, dc);
        DrawPixel(center.x - y, center.y, dc);
        DrawPixel(center.x, center.y - y, dc);
        
        while(y > limite) {
            if(d < 0) {
                d += 2 * x + 3;
            } else {
                y--;
                d += 2 * (x - y) + 5;
            }
            x++;
            DrawPixel(center.x + x, center.y + y, dc);
            DrawPixel(center.x + y, center.y + x, dc);
            DrawPixel(center.x + y, center.y - x, dc);
            DrawPixel(center.x + x, center.y - y, dc);
            DrawPixel(center.x - x, center.y - y, dc);
            DrawPixel(center.x - y, center.y - x, dc);
            DrawPixel(center.x - y, center.y + x, dc);
            DrawPixel(center.x - x, center.y + y, dc);
            
        }
    }

}

void BigPixelCanvas::DesenharTriangulo2D(const Triang2D& triangulo) {
    wxClientDC dc(this);
    PrepareDC(dc);
    DesenharTriangulo2D(triangulo, dc);
}

void BigPixelCanvas::DesenharTriangulo2D(const Triang2D& triangulo, wxDC& dc) {
    Ponto p0 = triangulo.P1();
    Ponto p1 = triangulo.P2();
    Ponto p2 = triangulo.P3();
    
    int variacaoP0P1 = p1.mY - p0.mY;
    int variacaoP1P2 = p2.mY - p1.mY;
    int variacaoP2P0 = p0.mY - p2.mY;
    
    int moduloVariacaoP0P1 = (variacaoP0P1 < 0) ? -variacaoP0P1 : variacaoP0P1;
    int moduloVariacaoP1P2 = (variacaoP1P2 < 0) ? -variacaoP1P2 : variacaoP1P2;
    int moduloVariacaoP2P0 = (variacaoP2P0 < 0) ? -variacaoP2P0 : variacaoP2P0;
    
    int variacaoArestaLonga = moduloVariacaoP0P1;
    Ponto arestaLongaP0 = p0;
    Ponto arestaLongaP1 = p1;
    Ponto sobrou = p2;
    cout << p0 << " " << p1 << " " << p2 << endl;
    if(variacaoArestaLonga < moduloVariacaoP1P2) {
        variacaoArestaLonga = moduloVariacaoP1P2;
        arestaLongaP0 = p1;
        arestaLongaP1 = p2;
        sobrou = p0;
    }
    if(variacaoArestaLonga < moduloVariacaoP2P0) {
        variacaoArestaLonga = moduloVariacaoP2P0;
        arestaLongaP0 = p2;
        arestaLongaP1 = p0;
        sobrou = p1;
    }
    
    if(arestaLongaP0.mY > arestaLongaP1.mY) {
        Ponto temp = arestaLongaP0;
        arestaLongaP0 = arestaLongaP1;
        arestaLongaP1 = temp;
    }
    
    float xesq = arestaLongaP0.mX;
    float xdir = xesq;
    int ymin = arestaLongaP0.mY;
    int ymax = sobrou.mY;
    
    float dxEsq;
    float dxDir;
    float dxAresta2;
    float dxAresta3;
    float dxArestaLonga;

    dxAresta2 = ((float) (sobrou.mX - arestaLongaP0.mX))/(sobrou.mY - arestaLongaP0.mY);
    dxAresta3 = ((float) (arestaLongaP1.mX - sobrou.mX))/(arestaLongaP1.mY - sobrou.mY);
    dxArestaLonga = ((float) (arestaLongaP1.mX - arestaLongaP0.mX))/(arestaLongaP1.mY - arestaLongaP0.mY);
    cout << arestaLongaP0.mX << " " << arestaLongaP1.mX << endl << "+++++++" << endl;

    int limitePrimeiroTriangulo = (dxArestaLonga * (ymax - ymin)) + arestaLongaP0.mX;
    cout << limitePrimeiroTriangulo << endl;
    bool sobrouEstaAEsquerda = sobrou.mX < limitePrimeiroTriangulo;

    if(sobrouEstaAEsquerda) {
        cout << "ESQUERDA" << endl;
        dxEsq = dxAresta2;
        dxDir = dxArestaLonga;
    } else {
        cout << "DIREITA" << endl;
        dxDir = dxAresta2;
        dxEsq = dxArestaLonga;
    }

    cout << sobrou << endl;

    int y = ymin;
    while(y < ymax) {
        for(int x = xesq; x < xdir; x++) {
            DrawPixel(x, y, dc);
        }
        xesq += dxEsq;
        xdir += dxDir;
        y++;
    }

    ymin = sobrou.mY;
    ymax = arestaLongaP1.mY;

    if(sobrouEstaAEsquerda) {
        xesq = sobrou.mX;
        dxEsq = dxAresta3;
        dxDir = dxArestaLonga;
    } else {
        xdir = sobrou.mX;
        dxDir = dxAresta3;
        dxEsq = dxArestaLonga;
    }

    y = ymin;
    while(y < ymax) {
        // cout << "ENTROU2" << endl;
        // cout << xesq << " " << xdir << endl;
        for(int x = xesq; x < xdir; x++) {
            DrawPixel((int) x, y, dc);
        }
        xesq += dxEsq;
        xdir += dxDir;
        y++;
    }
    cout << "--" << endl;

}

void BigPixelCanvas::DesenharTriangulo3D(const Triang3D& triangulo, wxDC& dc)
{
    Interv3D intervalo;
    while (triangulo.AtualizarIntervaloHorizontal(&intervalo))
        if (intervalo.Valido())
            DesenharIntervaloHorizontal(intervalo, dc);
}

void BigPixelCanvas::DesenharIntervaloHorizontal(const Interv2D& intervalo, wxDC& dc)
{
    int x = intervalo.mXMin;
    while (x < intervalo.mXMax) {
        DrawPixel(x, intervalo.mY, dc);
        ++x;
    }
}

void BigPixelCanvas::DesenharIntervaloHorizontal(const Interv3D& intervalo, wxDC& dc)
{
    // Colocar aqui o código para desenhar um intervalo horizontal 3D. Necessário
    // para a implementação do z-buffer.
    // Desenhar um intervalo 3D é como desenhar um intervalo 2D, usando z-buffer.
    #warning BigPixelCanvas::DesenharIntervaloHorizontal não foi implementado (necessário para a rasterização do z-buffer).
}

void BigPixelCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC pdc(this);
    wxDC &dc = pdc;

    PrepareDC(dc);

    mOwnerPtr->PrepareDC(dc);
    dc.SetBackgroundMode( mBackgroundMode );
    if ( mBackgroundBrush.Ok() )
        dc.SetBackground( mBackgroundBrush );
    if ( mColourForeground.Ok() )
        dc.SetTextForeground( mColourForeground );
    if ( mColourBackground.Ok() )
        dc.SetTextBackground( mColourBackground );

    dc.Clear();
    if (mUsandoComandos)
        InterpretarComandos();
}

void BigPixelCanvas::InterpretarComandos()
{
    ifstream arquivo("comandos.txt");
    wxClientDC dc(this);
    PrepareDC(dc);
    string comando;
    while (arquivo >> comando)
    {
        if (comando == "linha")
        {
            int p0x, p0y, p1x, p1y;
            arquivo >> p0x >> p0y >> p1x >> p1y;
            DrawLine(wxPoint(p0x, p0y), wxPoint(p1x, p1y), dc);
        }
        else if (comando == "cor")
        {
            int r, g, b;
            arquivo >> r >> g >> b;
            mPen.SetColour(r, g, b);
            dc.SetPen(mPen);
        }
        else if (comando == "triangulo3d")
        {
            int x, y, z;
            arquivo >> x >> y >> z;
            P3D p1(x,y,z);
            arquivo >> x >> y >> z;
            P3D p2(x,y,z);
            arquivo >> x >> y >> z;
            P3D p3(x,y,z);
            Triang3D tri(p1, p2, p3);
            DesenharTriangulo3D(tri, dc);
        }
    }
}

void BigPixelCanvas::OnClick(wxMouseEvent &event)
{
    wxPostEvent(mOwnerPtr, event);
}

void BigPixelCanvas::PrepareDC(wxDC& dc)
{
    int height, width;
    GetClientSize(&width, &height);
    dc.SetLogicalOrigin(-width/2, height/2);
    dc.SetAxisOrientation(true, true);
    dc.SetMapMode(wxMM_TEXT);
    dc.SetPen(mPen);
    mZBuffer.AlterarCapacidade(static_cast<unsigned int>(height/mPixelSize),
                               static_cast<unsigned int>(width/mPixelSize));
}

wxPoint BigPixelCanvas::ConvertDeviceToLogical(const wxPoint& p)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    wxPoint result;
    result.x = dc.DeviceToLogicalX(p.x) / mPixelSize;
    result.y = dc.DeviceToLogicalY(p.y) / mPixelSize;
    return result;
}

