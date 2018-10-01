#include "zbuffer.h"
#include <limits>
#include <algorithm>
#include <iostream> //apagar

using namespace std;

ZBuffer::ZBuffer()
    : mData(NULL)
{
}

ZBuffer::ZBuffer(unsigned int linhas, unsigned int colunas)
    : mLinhas(linhas), mColunas(colunas)
{
    unsigned int limite = linhas*colunas;
    mData = new double[limite];
    // inicializar todos os valores com o maior numero possivel
    fill(mData, mData+limite, numeric_limits<double>::max());
}

void ZBuffer::AlterarCapacidade(unsigned int linhas, unsigned int colunas)
{
    if (mData != NULL)
        delete[] mData;
    unsigned int limite = linhas*colunas;
    mData = new double[limite];
    mColunas = colunas;
    mLinhas = linhas;
    // inicializar todos os valores com o maior numero possivel
    fill(mData, mData+limite, numeric_limits<double>::max());
}

bool ZBuffer::IsVisible(int y, int x, double z)
{
    // adequar as coordenadas
    y += mLinhas/2;
    x += mColunas/2;
    // atualizar zbuffer e retornar valor
    #warning ZBuffer::IsVisible não foi completamente implementado.
}

ZBuffer::~ZBuffer()
{
    delete[] mData;
}

