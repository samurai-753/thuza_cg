#ifndef TRIANGULO3D_H
#define TRIANGULO3D_H

#include "ponto3d.h"
#include "intervalohorizontal.h"
#include "aresta.h"
#include <algorithm>
#include <iostream> // apagar

inline int Arredondar(double n) {
    return (n > 0.0) ? (n + 0.5) : (n - 0.5);
}

template <class Cor, class Coord>
class Triangulo3D
{
    public:
        // tipos =========================================================================
        typedef Ponto3D<Cor,Coord> Ponto;
        typedef Ponto3D<Cor,Coord> Vetor;
        // metodos =======================================================================
        Triangulo3D(const Ponto& p1, const Ponto& p2, const Ponto& p3);

        // Cada vez que for ativado, este metodo retorna o proximo intervalo para ser
        // desenhado.
#warning Triangulo3D::AtualizarIntervaloHorizontal não foi completamente implementado (necessário para o z-buffer, procure por FixMe, retire os pragmas quando for implementar).
        bool AtualizarIntervaloHorizontal(IntervaloHorizontal<Cor,Coord>* ptIntervalo) const {
            static bool naoInicializado = true;
            static bool primeiraParte = true; // primeira ou segunda parte?
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
            static int y; // coordenada y corrente
            static double variacaoZX; // quanto o z varia para cada x
            static double variacaoZY; // quanto o z varia para cada y
            static double incrementoXMin; // quanto o xMin varia para cada y
            static double incrementoXMax; // quanto o xMax varia para cada y
            static double xMin;
            static double xMax;
            static double z; // coordenada z em xMin
            // se acabou o triangulo, preparar para comecar de novo e finalizar
#pragma GCC diagnostic pop
            if (naoInicializado) {
                // Calcular o controle necessario para desenhar a primeira parte.
                y = mAresta2.Y1(); // menor y da aresta2
                variacaoZX = ptIntervalo->mVariacaoZ = -mNormal.mX / mNormal.mZ;
                variacaoZY = -mNormal.mY / mNormal.mZ;
                double fator1 = static_cast<double>(mAresta2.Y1()-mArestaLonga.Y1()) / mArestaLonga.DeltaY();
                xMin = mArestaLonga.X1() + mArestaLonga.DeltaX() * fator1;
                xMax = mAresta2.X1();
                if ((xMin > xMax)or(mArestaLonga.X2() > mAresta2.X2())) { // aresta curta esta 'a esquerda?
                    std::swap(xMin, xMax);
                    z = mAresta2.Z1();
                    incrementoXMin = mAresta2.IncrementoX();
                    incrementoXMax = mArestaLonga.IncrementoX();
                }
                else {
                    z = mArestaLonga.Z1() + mArestaLonga.DeltaZ() * fator1;
                    incrementoXMin = mArestaLonga.IncrementoX();
                    incrementoXMax = mAresta2.IncrementoX();
                }
                naoInicializado = false;
            }
            else {
                if (primeiraParte) {
                    // primeira parte: desenha entre a aresta longa e a aresta2
                    if (y >= mAresta2.Y2()) {
                        // Acabou a primeira parte. Calcular o controle necessario para
                        // desenhar a segunda parte.
                        primeiraParte = false;
                        y = mAresta3.Y1();
                        double fator1 = static_cast<double>(mAresta3.Y1()-mArestaLonga.Y1())
                                        / mArestaLonga.DeltaY();
                        xMin = mArestaLonga.X1() + mArestaLonga.DeltaX() * fator1;
                        xMax = mAresta3.X1();
                        // aresta curta esta 'a esquerda?
                        if ((xMin > xMax)or(mArestaLonga.X2() > mAresta3.X2())) {
                            std::swap(xMin, xMax);
                            z = mAresta3.Z1();
                            incrementoXMin = mAresta3.IncrementoX();
                            incrementoXMax = mArestaLonga.IncrementoX();
                        }
                        else {
                            z = mArestaLonga.Z1() + mArestaLonga.DeltaZ() * fator1;
                            incrementoXMin = mArestaLonga.IncrementoX();
                            incrementoXMax = mAresta3.IncrementoX();
                        }
                    }
                }
                else {
                    // segunda parte: desenha entre a aresta longa e a aresta3
                    if (y >= mAresta3.Y2()) {
                        naoInicializado = true;
                        primeiraParte = true;
                        return false;
                    }
                }
            }
            // atualizar mY, mXMin, mXMax e mZ no intervalo
            // FixMe
            // atualizar y, xMin, xMax e z locais (para proxima ativacao da funcao)
            // FixMe
            return true;
        }
        Ponto P1() const { return mP1; }
        Ponto P2() const { return mP2; }
        Ponto P3() const { return mP3; }
    private:
        Ponto mP1;
        Ponto mP2;
        Ponto mP3;
        Vetor mNormal;
        Aresta<Cor, Coord> mArestaLonga;
        Aresta<Cor, Coord> mAresta2;
        Aresta<Cor, Coord> mAresta3;
};

template <class Cor, class Coord>
Triangulo3D<Cor,Coord>::Triangulo3D(const Ponto& p1, const Ponto& p2, const Ponto& p3)
    : mP1(p1), mP2(p2), mP3(p3),
      mNormal(ProdutoVetorial(p2 - p1, p3 - p2)),
      mArestaLonga(p1, p2), mAresta2(p2, p3), mAresta3(p3, p1)
{
    // determinar qual e' a aresta longa
    if (mArestaLonga.VariacaoY() < mAresta2.VariacaoY()) {
        if (mAresta2.VariacaoY() < mAresta3.VariacaoY())
            std::swap(mArestaLonga, mAresta3);
        else
            std::swap(mArestaLonga, mAresta2);
    }
    else
        if (mArestaLonga.VariacaoY() < mAresta3.VariacaoY()) {
            std::swap(mArestaLonga, mAresta3);
        }
}

#endif
