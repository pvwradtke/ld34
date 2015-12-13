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
    tqueda=0;
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
    tqueda=0;
    rotacao=0;
    dirRotacao = 90;
    return true;
}

int Jogador::atualiza(Fase &mapa)
{
    rotacao+=dirRotacao/600.0;
    // Calcula o deslocamento horizontal, baseado na direção do jogador
    double dx, dy;
    dx = (VJOGADOR*cosseno(direcao))/600.0;
    dy = (VJOGADOR*seno(direcao))/600.0;

    // Ajusta colisao com o cenário
    int xanterior=(int)x;
    int yanterior=(int)y;
    x+=dx;
    y-=dy;
    int xatual = (int)x;
    int yatual = (int)y;
    if(mapa.colideCenario(&xatual, &yatual, xanterior, yanterior, 32, 32)){
        direcao=(direcao+180)%360;
        if((int)x != xatual)
            x=xatual;
        if((int)y != yatual)
            y=yatual;
    }
    // Aplica a gravidade (é mais fácil aqui, para separar da colisão com o movimento normal)
    /*tqueda++;
    double tempo=(float)tqueda/100.0;
    double aceleracao=20*tempo*tempo;
    int dgravidade = direcao-90;
    if(dgravidade<0)
        dgravidade+=360;
    if(aceleracao>400)
        aceleracao=400;
    dx = (aceleracao*cosseno(dgravidade))/600.0;
    dy = (aceleracao*seno(dgravidade))/600.0;
    x+=dx;
    y-=dy;*/
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

int Jogador::seno(int angulo)
{
    if(angulo == 0)
        return 0;
    else if(angulo == 90)
        return 1;
    else if(angulo == 180)
        return 0;
    else
        return -1;
}

int Jogador::cosseno(int angulo)
{
    if(angulo == 0)
        return 1;
    else if(angulo == 90)
        return 0;
    else if(angulo == 180)
        return -1;
    else
        return 0;

}
