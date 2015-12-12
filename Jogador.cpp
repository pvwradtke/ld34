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

void Jogador::desenha(const int xref, const int yref, const int angulo)
{
    switch(angulo)
    {
    case 0:
        C2D_DesenhaSpriteEspecial(sprite, cor, xref+x, yref+y, C2D_FLIP_NENHUM, 1.0, 1.0, rotacao);
        break;
    case 90:
        C2D_DesenhaSpriteEspecial(sprite, cor, xref+992-y, yref+x, C2D_FLIP_NENHUM, 1.0, 1.0, rotacao);
        break;
    case 180:
        C2D_DesenhaSpriteEspecial(sprite, cor, xref+992-x, yref+992-y, C2D_FLIP_NENHUM, 1.0, 1.0, rotacao);
        break;
    case 270:
        C2D_DesenhaSpriteEspecial(sprite, cor, xref+y, yref+992-x, C2D_FLIP_NENHUM, 1.0, 1.0, rotacao);
        break;
    }
}

void Jogador::rotaciona(const int angulo)
{
    direcao=angulo;
    rotacao+=angulo;
    if(rotacao>=360)
        rotacao-=360;
}
