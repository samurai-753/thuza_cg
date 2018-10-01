#ifndef PONTO3D_H
#define PONTO3D_H

#include <iostream>

template <class Cor, class Coord>
class Ponto3D {
    public:
        Ponto3D() {
        }
        Ponto3D(Coord x, Coord y, Coord z)
            : mX(x), mY(y), mZ(z) {
        }
        Ponto3D operator-(const Ponto3D& p) const {
            return Ponto3D(mX-p.mX, mY-p.mY, mZ-p.mZ);
        }
        Coord mX;
        Coord mY;
        Coord mZ;
        Cor mCor;
    private:
};

template <class Cor, class Coord>
std::istream& operator>>(std::istream& entrada, Ponto3D<Cor,Coord>& p) {
    return entrada >> p.mX >> p.mY >> p.mZ;
}

template <class Cor, class Coord>
std::ostream& operator<<(std::ostream& saida, const Ponto3D<Cor,Coord>& p) {
    return saida << "(" << p.mX << ", " << p.mY <<", " << p.mZ << ")";
}

template <class Cor, class Coord>
Ponto3D<Cor, Coord> ProdutoVetorial(const Ponto3D<Cor,Coord>& p1,
                                    const Ponto3D<Cor,Coord>& p2) {
    return Ponto3D<Cor,Coord>(p1.mY*p2.mZ-p1.mZ*p2.mY,
                              p1.mZ*p2.mX-p1.mX*p2.mZ,
                              p1.mX*p2.mY-p1.mY*p2.mX);
}

#endif

