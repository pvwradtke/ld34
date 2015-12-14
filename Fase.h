#ifndef FASE_HPP
#define FASE_HPP

#include <c2d3/chien2d.h>
#include <stdio.h>
#include "headers.h"

#define     MAX_TRIGGERS    5
#define     MAX_MAPA        32


class Fase
{
    public:
        Fase();
        virtual ~Fase();
        bool carrega(const char *nomeArquivo);
        void imprime();
        void desenha(int x_desl, int y_desl, int angulo, bool debug);
        bool procuraPrimeiraMarca(const int marca, int *x, int *y);
        bool colideCenario(int *xatual, int *yatual, const int xanterior, const int yanterior, const int largura, const int altura);
        bool colideMarca(const int marca, const int x, const int y, const int largura, const int altura);
        bool ativaTrigger(int triggerNumber);
        const char *pegaMensagemTrigger(int triggerNumber);
    protected:
        bool init;
        char    title[MAX_TEXTO];
        int     num_trigger;
        char    trigger_messages[MAX_TRIGGERS][MAX_TEXTO];
        bool    trigger_activated[MAX_TRIGGERS];
        int     largura, altura;
        int     mapa[MAX_MAPA][MAX_MAPA];
        int     marca;
        int     geometria[MAX_MAPA][MAX_MAPA];
        unsigned int tileset;
    private:
};

#endif // FASE_HPP
