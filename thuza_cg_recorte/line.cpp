#include "line.h"
#include <iostream>
using namespace std;

Line::Line(const Point& p0, const Point& p1)
    : mP0(p0), mP1(p1)
{}

// Dado um valor t entre 0 e 1, retorna um ponto no segmento.
// Exemplos: Para t = 0, retorna P0;
//           para t = 1, retorna P1;
//           para t = 0.5, retorna o ponto no meio do segmento.
Point Line::Coordinates(double t) const
{
    return mP0 + (mP1 - mP0) * t;
}

void Line::Read(const string& prompt)
{
    cout << prompt;
    mP0.Read("Digite as duas coordenadas do ponto inicial: ");
    mP1.Read("Digite as duas coordenadas do ponto final: ");
}

ostream& operator << (ostream& os, const Line& l)
{
    return os << l.mP0 << "=>" << l.mP1;
}
