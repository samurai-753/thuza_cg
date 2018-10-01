#ifndef ZBUFFER_H
#define ZBUFFER_H

class ZBuffer
{
    public:
        ZBuffer();
        ZBuffer(unsigned int linhas, unsigned int colunas);
        virtual ~ZBuffer();
        void AlterarCapacidade(unsigned int linhas, unsigned int colunas);
        // Verifica se um pixel e' visivel. Atualiza o z-buffer se ele for.
        bool IsVisible(int y, int x, double z);
    private:
        unsigned int mLinhas;
        unsigned int mColunas;
        double* mData;
};

#endif

