#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include "headers.h"
#include "Fase.h"

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
        bool inicializa(Fase &mapa);
        int atualiza(Fase &fase);
        void desenha(const int xref, const int yref, const int angulo);
        void rotaciona(const int angulo);
    protected:
    private:
};

#endif // JOGADOR_HPP
