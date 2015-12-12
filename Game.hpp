/*
 * File:   Game.hpp
 * Author: paulo
 *
 * Created on 26 de Abril de 2013, 23:05
 */

#ifndef GAME_HPP
#define	GAME_HPP

#include <memory>
#include <string>
#include <c2d3/chien2d.h>
#include <c2d3/chienaudio.h>

#include "headers.h"
#include "Jogador.hpp"

#define     PI              3.14159265
#define     MAX_FASES       64


class Game {
public:
    enum States{splash, mainmenu, help, game_keyplusmouse, game_gamepad, tutorial, credits, gameover, quit};
    enum EventDescriptorType {eventtext};
    Game();
    virtual ~Game();

    bool init(bool fullscreen);
    bool run();
    bool splashscreen();
    int mainmenuscreen();
    bool carregaListaFases(char *nome, int *numFases, char lista[MAX_FASES][MAX_TEXTO]);
    bool carregaFase(char *nome, Fase *fase);
    void imprimeFase(Fase *fase);
    bool gamescreen(Jogador &jogador, int numFase, char *arquivoFase);
    float calculaAngulo(const float dx, const float dy);
    bool loadhighscore();
    bool savehighscore();
    bool creditsscreen();
private:
    bool    initialized;
    const C2D_Botao *teclado;
    const C2D_Gamepad *gamepads;
    const C2D_Mouse *mouse;
    int     fonteSistema;
    int     fonteURL;
    int     highScore;
    char    listaFases[MAX_FASES][MAX_TEXTO];
};

#endif	/* GAME_HPP */

