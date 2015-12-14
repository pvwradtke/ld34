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
    cor = (rand())%4;
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

int Jogador::atualiza(Fase &mapa, int angulo)
{
    if(estado == JOGADOR_VITORIA || estado == JOGADOR_MORTO)
        return estado;
    rotacao+=dirRotacao/600.0;
    double dx, dy;
    int xanterior;
    int yanterior;
    int xatual;
    int yatual;

    // Calcula o deslocamento horizontal, baseado na direção do jogador
    dx = (VJOGADOR*cosseno(direcao))/600.0;
    dy = (VJOGADOR*seno(direcao))/600.0;
    // Ajusta colisao com o cenário
    xanterior=(int)x;
    yanterior=(int)y;
    x+=dx;
    y-=dy;
    xatual = (int)x;
    yatual = (int)y;
    if(mapa.colideCenario(&xatual, &yatual, xanterior, yanterior, 32, 32)){
        direcao=(direcao+180)%360;
        dirRotacao*=-1;
        if((int)x != xatual)
            x=xatual;
        if((int)y != yatual)
            y=yatual;
    }

    int dgravidade = angulo-90;
    if(dgravidade<0)
        dgravidade+=360;
    // Desce um pixel na direção da gravidade para testar se cai numa armadilha
    dx=cosseno(dgravidade);
    dy=-seno(dgravidade);
    xanterior=(int)x;
    yanterior=(int)y;
    xatual = (int)x+dx;
    yatual = (int)y+dy;
    if(!mapa.colideCenario(&xatual, &yatual, xanterior, yanterior, 32, 32))
        if(estado==JOGADOR_NORMAL && mapa.colideMarca(JOGO_MORTE, xatual, yatual, 32, 32))
            estado=JOGADOR_MORTO;
    // Aplica a gravidade (é mais fácil aqui, para separar da colisão com o movimento normal)
    tqueda++;
    double tempo=(float)tqueda/100.0;
    double aceleracao=20*tempo*tempo;
    if(aceleracao>500)
        aceleracao=500;
    dx = (aceleracao*cosseno(dgravidade))/600.0;
    dy = (aceleracao*seno(dgravidade))/600.0;
    xanterior=(int)x;
    yanterior=(int)y;
    x+=dx;
    y-=dy;
    xatual = (int)x;
    yatual = (int)y;
    if(mapa.colideCenario(&xatual, &yatual, xanterior, yanterior, 32, 32)){
        if(aceleracao>400){
            this->estado=JOGADOR_MORTO;
            printf("Aceleracao: %f\n", aceleracao);
        }
        tqueda=0;
        if((int)x != xatual)
            x=xatual;
        if((int)y != yatual)
            y=yatual;
    }
    // Achou uma armadilha?
    if(estado==JOGADOR_NORMAL && mapa.colideMarca(JOGO_MORTE, x, y, 32, 32))
        estado=JOGADOR_MORTO;
    // Achou o fim da fase?
    if(estado==JOGADOR_NORMAL && aceleracao<30 && mapa.colideMarca(JOGO_FIM, x+14, y+14, 4, 4)){
        estado=JOGADOR_VITORIA;
        mapa.imprime();
    }
    return estado;
}

void Jogador::desenha(const int xref, const int yref, const int angulo)
{
    int cor=this->cor;
    if(estado == JOGADOR_MORTO)
        cor+=8;
    else if(estado == JOGADOR_VITORIA)
        cor+=4;
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

void Jogador::rotaciona(const int difangulo)
{
    direcao+=difangulo;
    if(direcao>=360)
        direcao-=360;
    else if(direcao<0)
        direcao+=360;
    rotacao+=difangulo;
    if(rotacao>=360)
        rotacao-=360;
    else if(rotacao<0)
        rotacao+=360;

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

void Jogador::pegaBoundingBox(int *x, int *y, int *largura, int *altura)
{
    *x=this->x;
    *y=this->y;
    *largura=32;
    *altura=32;
}

int Jogador::pegaEstado()
{
    return estado;
}
