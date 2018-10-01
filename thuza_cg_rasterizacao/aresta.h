#ifndef ARESTA_H
#define ARESTA_H

#include "ponto3d.h"
#include "wx/colour.h"

template <class Cor, class Coord>
class Aresta {
// Uma aresta e' composta de 2 pontos. O ponto 1 (p1) e' o de menor Y.
    public:
        typedef Ponto3D<Cor, Coord> Ponto;
        Aresta(const Ponto& p1, const Ponto& p2) {
            if (p1.mY < p2.mY) {
                mX1 = p1.mX;
                mY1 = p1.mY;
                mZ1 = p1.mZ;
                mX2 = p2.mX;
                mY2 = p2.mY;
                mZ2 = p2.mZ;
                mDeltaX = p2.mX - p1.mX;
                mDeltaY = p2.mY - p1.mY;
                mDeltaZ = p2.mZ - p1.mZ;
            }
            else {
                mX1 = p2.mX;
                mY1 = p2.mY;
                mZ1 = p2.mZ;
                mX2 = p1.mX;
                mY2 = p1.mY;
                mZ2 = p1.mZ;
                mDeltaX = p1.mX - p2.mX;
                mDeltaY = p1.mY - p2.mY;
                mDeltaZ = p1.mZ - p2.mZ;
            }
            mIncrementoX = double(mDeltaX) / double(mDeltaY);
        }
        Coord VariacaoY() { return mDeltaY; }
        Coord MenorX() const { return mX1 < mX2? mX1 : mX2; }
        Coord MaiorX() const { return mX1 > mX2? mX1 : mX2; }
        Coord X1() const { return mX1; }
        Coord Y1() const { return mY1; }
        Coord Z1() const { return mZ1; }
        Coord X2() const { return mX2; }
        Coord Y2() const { return mY2; }
        Coord Z2() const { return mZ2; }
        double IncrementoX() const { return mIncrementoX; }
        int DeltaX() const { return mDeltaX; }
        int DeltaY() const { return mDeltaY; }
        double DeltaZ() const { return mDeltaZ; }
    protected:
        Coord mX1;
        Coord mY1;
        Coord mX2;
        Coord mY2;
        Coord mZ1;
        Coord mZ2;
        // numerador e denominador indicam a inclinacao da aresta usando somente inteiros
        int mDeltaX;
        int mDeltaY;
        double mDeltaZ;
        double mIncrementoX; // quanto o x varia para cada y
};

#endif

