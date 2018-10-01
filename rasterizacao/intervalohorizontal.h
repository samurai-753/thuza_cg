#ifndef INTERVALO_HORIZONTAL_H
#define INTERVALO_HORIZONTAL_H

template <class Cor, class Coord>
class IntervaloHorizontal {
    public:
        bool Valido() {
            return mXMin <= mXMax;
        }

        int mXMin;
        int mXMax;
        int mY;
        double mZ; // coordenada z para x minimo
        double mVariacaoZ; // quanto o z varia ao incrementar o x
        Cor mCor1; // cor em x minimo
        Cor mCor2; // cor em x maximo
};

#endif
