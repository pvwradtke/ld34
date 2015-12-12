#ifndef HEADERS_H_INCLUDED
#define HEADERS_H_INCLUDED

#define     MAX_TRIGGERS    5
#define     MAX_TEXTO       512
#define     MAX_MAPA        32

enum marcas{JOGO_NADA=0, JOGO_JOGADOR, JOGO_SOLIDO, JOGO_FIM, JOGO_MORTE,
            JOGO_TRIGGER_0, JOGO_TRIGGER_1, JOGO_TRIGGER_2, JOGO_TRIGGER_3, JOGO_TRIGGER_4};

typedef struct TagFase
{
    char    title[MAX_TEXTO];
    int     num_trigger;
    char    trigger_messages[MAX_TRIGGERS][MAX_TEXTO];
    bool    trigger_activated[MAX_TRIGGERS];
    int     largura, altura;
    int     mapa[MAX_MAPA][MAX_MAPA];
    int     marca;
    int     geometria[MAX_MAPA][MAX_MAPA];
}Fase;

#endif // HEADERS_H_INCLUDED
