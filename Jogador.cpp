#include "Jogador.hpp"
#include <cstdlib>
#include <c2d3/chien2d.h>

unsigned int Jogador::sprite=0;

Jogador::Jogador()
{
    //ctor
    if(sprite==0)
        sprite = C2D_CarregaSpriteSet("./gfx/colored.png", DIM_JOGADOR, DIM_JOGADOR);
    // Dá uma posição por default
    x = 0;
    y = 0;
    estado=JOGADOR_NORMAL;
    direcao = 0;
    vqueda=0;
    rotacao=0;
    dirRotacao = 2;
    cor = (random())%4;
}

Jogador::~Jogador()
{
    //dtor
    C2D_RemoveSpriteSet(sprite);
    sprite=0;
}

bool Jogador::inicializa(Fase &mapa)
{
    int xlocal, ylocal;
    // Procura a posicao do jogador
    if(!mapa.procuraPrimeiraMarca(JOGO_JOGADOR, &xlocal, &ylocal))
        return false;
    x=xlocal;
    y=ylocal;
    estado=JOGADOR_NORMAL;
    direcao = 0;
    vqueda=0;
    rotacao=0;
    dirRotacao = 2;
    return true;
}

int Jogador::atualiza(Fase &mapa)
{
    rotacao+=dirRotacao/600.0;
    // Calcula o deslocamento horizontal, baseado na direção do jogador
    double dx, dy;
    dx = (VJOGADOR*cos(PI*direcao))/600.0;
    dy = (VJOGADOR*sin(PI*direcao))/600.0;
    return 0;
}

void Jogador::desenha(const int angulo, const int xref, const int yref)
{
    switch(angulo)
    {
    case 0:
        C2D_DesenhaSprite(sprite, cor, xref+x, yref+y);
        break;
    case 90:
        break;
    case 180:
        break;
    case 270:
        break;
    }
}

void Jogador::rotaciona(const int angulo)
{
    direcao=angulo;
}
