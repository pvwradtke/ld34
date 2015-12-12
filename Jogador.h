#ifndef JOGADOR_H
#define JOGADOR_H

#include "Game.hpp"

#define     DIM_JOGADOR     32
#define     VJOGADOR        1.0

class Jogador
{
    protected:
        static unsigned int sprite;
        int cor;
        float x;
        float y;
        int estado;
        int direcao;
        double vqueda;
        double rotacao;
        double dirRotacao;
    public:
        enum estados_jogador{JOGADOR_NORMAL, JOGADOR_VITORIA, JOGADOR_MORTO};
        Jogador();
        virtual ~Jogador();
        bool inicializa(const Fase *mapa);
        int atualiza(const Fase *mapa);
        void desenha(const int angulo, const Fase *mapa, const int xref, const int yref);
        void rotaciona(int angulo);
    protected:
    private:
};

#endif // JOGADOR_H
