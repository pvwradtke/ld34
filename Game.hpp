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

#define MAX_IMAS        64
#define MAX_ESTRELAS    64

#define DESLX       16
#define DESLY       12

#define PI          3.14159265

#define VELOCIDADE_JOGADOR  5
#define VELOCIDADE_REPULSAO_IMA         10
#define VELOCIDADE_ATRACAO_IMA          3
#define VELOCIDADE_ESTRELA              6
#define DISTANCIA_JOGADOR   150

#define TAM_MOUSE       11
#define TAM_IMA         20
#define TAM_JOGADOR     32
#define TAM_ESTRELA     32

#define MAPA_LINHAS     33
#define MAPA_COLUNAS    59

#define MAX_FASES       64
#define MAX_TEXTO       256

typedef int Mapa[MAPA_LINHAS][MAPA_COLUNAS];

// All values are multiplied by 256 for fixed point precision
typedef struct TagEventDescriptor
{
    int frame;
    int type;
    std::string text;
    int textduration;
    int width, height, y, speed;
    int target;
    int repeat;
}EventDescriptor;

typedef struct TagJogador
{
    float x;
    float y;
    float angulo;
    bool vivo;
    int tempoMorte;
}Jogador;

typedef struct TagPersonagem
{
    int tipo;
    float  x;
    float  y;
    float  angulo;
    float  velocidade;
    int anguloRotacao;
    int timer;
}Personagem;

class Game {
public:
    enum States{splash, mainmenu, help, game_keyplusmouse, game_gamepad, tutorial, credits, gameover, quit};
    enum marcas{JOGO_NADA=0, JOGO_CHAO, JOGO_PAREDE, JOGO_JOGADOR, JOGO_NEGATIVO, JOGO_POSITIVO, JOGO_MORTO, JOGO_ESTRELA, JOGO_ESTRELA0, JOGO_ESTRELA45, JOGO_ESTRELA90,
            JOGO_ESTRELA135, JOGO_ESTRELA180, JOGO_ESTRELA225, JOGO_ESTRELA270, JOGO_ESTRELA315, JOGO_ESTRELA_MORRENDO, JOGO_ARMADILHA};
    enum EventDescriptorType {eventtext, eventenemy, eventjump, eventend, eventdisablescore, eventmusic, eventcomment};
    Game();
    virtual ~Game();

    bool init(bool fullscreen);
    bool run();
    bool splashscreen();
    int mainmenuscreen();
    bool carregaListaFases(char *nome, int *numFases, char lista[MAX_FASES][MAX_TEXTO]);
    bool gamescreen(Jogador *jogador, int controle, int numFase, int *placar, char *arquivoFase);
    void processaFase(Mapa mapa, Jogador *jogador, Personagem imas[], Personagem estrelas[]);
    bool carregaFase(char *name, char *mensagem, Mapa mapa, char *song);
    void atualizaJogador(Mapa mapa, Jogador *jogador, int controle);
    void atualizaIma(Mapa mapa, Personagem *ima, Jogador *jogador);
    void atualizaEstrela(Mapa mapa, Personagem *estrela);
    int colisoesImasEstrelas(Personagem imas[], Personagem estrelas[]);
    bool colisaoJogadorEstrelas(Jogador *jogador, Personagem estrelas[]);
    float calculaAngulo(const float dx, const float dy);
    bool loadhighscore();
    bool savehighscore();
    bool creditsscreen();
private:
    bool    initialized;
    const C2D_Botao *teclado;
    const C2D_Gamepad *gamepads;
    const C2D_Mouse *mouse;
    int     fonteTituloPequena;
    int     fonteTituloGrande;
    int     fonteMensagem;
    int     fonteSistema;
    int     fontePlacar;
    int     fonteURL;
    int     highScore;
    char    listaFases[MAX_FASES][MAX_TEXTO];
    int efeitoRepele, efeitoJogador, efeitoInimigo;
};

#endif	/* GAME_HPP */

