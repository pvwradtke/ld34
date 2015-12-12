#ifndef FASE_HPP
#define FASE_HPP

#include <c2d3/chien2d.h>
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
