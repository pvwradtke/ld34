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

bool Jogador::inicializa(const Fase *mapa)
{
    // Procura a posicao do jogador
    for(int i=0;i<mapa->altura;i++)
        for(int j=0;j<mapa->largura;j++)
            if(mapa->mapa[i][j]==JOGO_JOGADOR){
                x=j*DIM_JOGADOR;
                y=i*DIM_JOGADOR;
            }
    estado=JOGADOR_NORMAL;
    direcao = 0;
    vqueda=0;
    rotacao=0;
    dirRotacao = 2;
}

int Jogador::atualiza(const Fase *mapa)
{
    rotacao+=dirRotacao;
}

void Jogador::desenha(const int angulo, const Fase *mapa, const int xref, const int yref)
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

void rotaciona(int angulo)
{

}
