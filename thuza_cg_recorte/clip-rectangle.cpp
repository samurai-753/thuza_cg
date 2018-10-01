#include "clip-rectangle.h"
#include <iostream>
#include <cmath>

using namespace std;

ClipRectangle::ClipRectangle (double xmin, double xmax, double ymin, double ymax)
    : mXmin(xmin), mXmax(xmax), mYmin(ymin), mYmax(ymax), mInitialized(true)
{}

void ClipRectangle::Set(double xmin, double xmax, double ymin, double ymax)
{
    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;
    mInitialized = true;
}

// Retorna um ponteiro para uma linha recem alocada inteiramente dentro da area de recorte
// ou NULL se l for completamente descartada.
// A linha alocada aqui será desalocada na função CleanUp().
Line* ClipRectangle::Clip(const Line& l)
{
    // t = Ni * (P0 - PLi) / -Ni * D
    // Se denominador == 0, entao l é paralelo ao lado de recorte
    // Se denominador == 0 e numeradores > 0, entao l está fora da area de recorte
    // Se denominador < 0, entao l está potencialmente saindo
    
    // inicializa inicio e fim
    double inicio = 0;
    double fim = 1;

    // calcula os 4 numeradores
    double numeradores[4] = {
                             this->mXmin - l.mP0.mX, 
                             l.mP0.mY - this->mYmax, 
                             l.mP0.mX - this->mXmax, 
                             this->mYmin - l.mP0.mY
                            };

    // calcula os 4 denominadores
    double denominadores[4] = {
                               l.mP1.mX - l.mP0.mX, 
                               l.mP0.mY - l.mP1.mY, 
                               l.mP0.mX - l.mP1.mX, 
                               l.mP1.mY - l.mP0.mY
                            };


    for(int i = 0; i < 4; i++) {
        // se for paralelo e o numerador > 0, esta fora
        if(denominadores[i] == 0.0) {
            if(numeradores[i] > 0) {
                return NULL;
            }
        } else {

            double t = numeradores[i] / denominadores[i];

            // potencialmente saindo
            if(denominadores[i] < 0) {
                // se esta saindo antes de entrar, esta fora
                if(t < inicio) {
                    return NULL;
                }
                // se esta saindo antes do fim do segmento, entao o menor valor é o fim
                if(t < fim) {
                    fim = t;
                }
            } else {
                // potencialmente entrando

                // se esta entrando antes de sair, esta fora
                if(t > fim) {
                    return NULL;
                }
                // se esta saindo antes do fim do segmento, entao o maior valor é o inicio
                if(t > inicio) {
                    inicio = t;
                }
            }
        }
    }

    Point p0 = l.mP0;
    // se o inicio foi recortado
    if(inicio != 0) {
        // calcula as novas coordenadas de inicio do segmento
        p0 = l.Coordinates(inicio);
    }
    
    Point p1 = l.mP1;
    // se o fim foi recortado
    if(fim != 1) {
        // calcula as novas coordenadas de fim
        p1 = l.Coordinates(fim);
    }
    
    // retorna o novo segmento
    return (new Line(p0, p1));
}

void ClipRectangle::Read(const string& prompt)
{
    cout << prompt;
    cin >> mXmin >> mXmax >> mYmin >> mYmax;
    if (not mInitialized)
        mInitialized = true;
}

ostream& operator << (ostream& os, const ClipRectangle& r)
{
    return os << "(Clipping Rectangle: " << r.mXmin << ", " << r.mXmax << ", " 
              << r.mYmin << ", " << r.mYmax << ")";
}


