#ifndef BIG_PIXEL_CANVAS_H
#define BIG_PIXEL_CANVAS_H

#include "triangulo3d.h"
#include "zbuffer.h"
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/brush.h>
#include <wx/dcclient.h>
#include <vector>

//class Aresta;
//class IntervaloHorizontal;

class BigPixelCanvas: public wxPanel
{
    public:
        typedef Ponto3D<wxColour, double> P3D;
        typedef Triangulo3D<wxColour, double>  Triang3D;
        typedef Triangulo3D<wxColour, int>     Triang2D;
        typedef Triangulo3D<wxColour, int>::Ponto      Ponto;
        typedef IntervaloHorizontal<wxColour, double>  Interv3D;
        typedef IntervaloHorizontal<wxColour, int>     Interv2D;

        BigPixelCanvas(wxFrame *parent);
        void OnPaint(wxPaintEvent &event);
        void OnClick(wxMouseEvent &event);
        wxPoint ConvertDeviceToLogical(const wxPoint& p);
        wxPoint ConvertDeviceToLogical(const Triang2D& t);

        // Draw pixel using window coordinates.
        void DrawLine(wxPoint p0, wxPoint p1);
        // Draw circle using window coordinates.
        void DrawCircle(wxPoint center, int radius);
        // Draw triangle using window coordinates.
        void DesenharTriangulo2D(const Triang2D& t);
        void DesenharTriangulo3D(const Triang3D& triangulo, wxDC& dc);
        // Interpreta comandos de desenho do arquivo comandos.txt.
        void InterpretarComandos();

        int mPixelSize;
        bool mUsandoComandos;

    protected:
        // Draw pixel in local (big pixel) coordinates.
        void DrawPixel(int x, int y, wxDC& dc);
        void DrawPixel(int x, int y, double z, wxDC& dc);
        // Draw line in local (big pixel) coordinates.
        void DrawLine(const wxPoint& p0, const wxPoint& p1, wxDC& dc);
        // Draw circle in local (big pixel) coordinates.
        void DrawCircle(const wxPoint& center, int radius, wxDC& dc);
        // Draw triangle in local (big pixel) coordinates.
        void DesenharTriangulo2D(const Triang2D& triangulo, wxDC& dc);
        void PrepareDC(wxDC& dc);
//        void DrawSpansBetweenEdges(const Aresta<int>& e1, const Aresta<int>& e2, wxDC& dc);
        void DesenharIntervaloHorizontal(const Interv2D& intervalo, wxDC& dc);
        void DesenharIntervaloHorizontal(const Interv3D& intervalo, wxDC& dc);
        void DrawSpan(const Interv2D& intervalo, int y, wxDC& dc);

        int mBackgroundMode;
        wxBrush mBackgroundBrush;
        wxColour mColourForeground;
        wxColour mColourBackground;
        wxPen mPen;
        ZBuffer mZBuffer;

    private:
        wxFrame* mOwnerPtr;
        bool m_clip;

        DECLARE_EVENT_TABLE()
};

#endif

