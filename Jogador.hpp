#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include "headers.h"
#include "Fase.h"
#include <stdio.h>

#define     DIM_JOGADOR     32
#define     VJOGADOR        80.0

class Jogador
{
    protected:
        static unsigned int sprite;
        int cor;
        float x;
        float y;
        int estado;
        int direcao;
        int tqueda;
        double rotacao;
        double dirRotacao;
    public:
        enum estados_jogador{JOGADOR_NORMAL, JOGADOR_VITORIA, JOGADOR_MORTO};
        Jogador();
        virtual ~Jogador();
        bool inicializa(Fase &mapa);
        int atualiza(Fase &fase, const int angulo);
        void desenha(const int xref, const int yref, const int angulo);
        void rotaciona(const int angulo);
        void pegaBoundingBox(int *x, int *y, int *largura, int *altura);
        int pegaEstado();
    protected:
        int seno(int angulo);
        int cosseno(int angulo);
    private:
};

#endif // JOGADOR_HPP
