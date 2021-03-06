/*
 * File:   Game.cpp
 * Author: paulo
 *
 * Created on 26 de Abril de 2013, 23:05
 */

#include "Game.hpp"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

Game::Game() {
    initialized=false;
}

Game::~Game() {
}

bool Game::init(bool fullscreen){
    bool status = false;
    if(fullscreen)
        status = C2D_Inicia(1920, 1080, C2D_TELA_CHEIA, "Ludum Dare 32", true);
    else
        status = C2D_Inicia(1920, 1080, C2D_JANELA, "Ludum Dare 32", true);
    if(status)
    {
        fonteSistema = C2D_CarregaFonte("gfx/TerminusBold.ttf", "sistema", 24);
        fonteTitulo = C2D_CarregaFonte("gfx/ASS.TTF", "titulo", 250);
        fonteMensagem = C2D_CarregaFonte("gfx/Domdiagb.ttf", "titulo", 80);
        fonteURL = C2D_CarregaFonte("gfx/TerminusBold.ttf", "URL", 60);
        teclado = C2D_PegaTeclas();
        gamepads = C2D_PegaGamepads();
        mouse = C2D_PegaMouse();
        initialized=true;
    }
    else
        return false;
//    if(!loadhighscore())
//        highScore = 3;
    return true;
}

bool Game::run(){
    if(!initialized)
        return false;
    int currentstate = Game::splash;
    int exit=false;
    std::string levelname="./resources/tutorial.dat";

    while(!exit){
        switch(currentstate)
        {
            case Game::splash:
                if(!splashscreen())
                    return false;
                currentstate = Game::mainmenu;
                break;
            case Game::mainmenu:
                currentstate = mainmenuscreen();
                break;
            case Game::credits:
                creditsscreen();
                currentstate = Game::mainmenuscreen();
                break;
            case Game::game_gamepad:
            case Game::game_keyplusmouse:{
                int numFases;
                char *musicas[5]={"./sfx/song_01.ogg", "./sfx/song_02.ogg", "./sfx/song_03.ogg", "./sfx/song_04.ogg", "./sfx/song_05.ogg"};
                unsigned int musica = CA_CarregaMusica(musicas[rand()%5]);
                CA_TocaMusica(musica, -1);
                bool trocouFase=false;
                if(!carregaListaFases("./levels/levels_list.dat", &numFases, listaFases))
                {
                    printf("Erro ao carregar as fases!\n");
                    currentstate = Game::mainmenu;
                }
                else
                {
                    srand(time(NULL));
                    Jogador jogador=Jogador();
                    int mortes=0;
                    int faseAtual=0;
                    bool resultado=false;
                    while(faseAtual<numFases)
                    {
                        if(trocouFase){
                            trocouFase=false;
                            CA_FadeMusica(0);
                            musica = CA_CarregaMusica(musicas[rand()%5]);
                            CA_TocaMusica(musica, -1);
                        }
                        resultado = gamescreen(jogador, faseAtual,listaFases[faseAtual]);
                        if(resultado)
                        {
                            if(jogador.pegaEstado()==Jogador::JOGADOR_VITORIA){
                                faseAtual++;
                                trocouFase=true;
                            }
                            else
                                mortes++;
                        }
                        else
                        {
                            currentstate = Game::mainmenu;
                            break;
                        }
                    }
                    if(resultado)
                        currentstate = Game::credits;
                    else
                        currentstate = Game::mainmenu;
                }
                CA_FadeMusica(0);
                CA_RemoveMusica(musica);
                break;
            }
            case Game::tutorial:
                currentstate = Game::mainmenu;
                break;
            case Game::quit:
                exit=true;
            default:
                currentstate = Game::splash;
                break;

        }
        if(teclado[C2D_TENCERRA].pressionou)
            exit=true;
    }
    return true;
}

bool Game::splashscreen(){
    int logo = C2D_CarregaSpriteSet("./gfx/chienloco.png", 0,0);
    if(logo == 0)
        return false;
    int timer=0;
    bool end = false;
    C2D_TrocaCorLimpezaTela(0,0,0);
    char url[] = "http://chienloco.com/wp";
    // Pega a largura do texto da URL
    int ltexto, atexto;
    C2D_DimensoesTexto(fonteSistema, url, &ltexto, &atexto);
    while(!end && !teclado[C2D_TENCERRA].pressionou){
        C2D_LimpaTela();
        int alpha;
        if(timer<120)
            alpha = (timer*255)/120;
        if(alpha>255)
            alpha=255;
        C2D_AlteraAlphaDoSprite(logo, alpha);
        C2D_DesenhaSprite(logo, 0, 480, 414);
        if(timer>=255)
            C2D_DesenhaTexto(fonteSistema, 1440-ltexto, 600, url, C2D_TEXTO_ESQUERDA, 255, 255, 255, 255);
        timer++;
        if(timer>=530)
            end=true;
        C2D_Sincroniza(C2D_FPS_PADRAO);
        // se pressionou qualquer tecla, pula fora
        for(int i=0;i<C2D_MAX_TECLAS;i++)
            if(teclado[i].pressionou)
                end=true;
        // Idem para o gamepad
        for(int i=0;i<C2D_GMAX_BOTOES;i++)
            if(gamepads[0].botoes[i].pressionou)
                end=true;
    }
    C2D_RemoveSpriteSet(logo);
    return true;
}

int Game::mainmenuscreen(){
    int timer=0;
    bool end = false;

    bool tutorialonly = false;
    int score = 0;

    int choice;
    C2D_TrocaCorLimpezaTela(0,0,0);
    //int musica = CA_CarregaMusica("audio/01-menu.ogg");
    //CA_TocaMusica(musica, 1);
    unsigned int background = C2D_CarregaSpriteSet("./gfx/title.jpg", 0, 0);
    unsigned int musica = CA_CarregaMusica("./sfx/menu.ogg");
    CA_TocaMusica(musica, -1);
    while(!end){
        C2D_LimpaTela();
        C2D_DesenhaSprite(background, 0, 0, 0);
        C2D_DesenhaTexto(fonteTitulo, 960, 250, "Gravity Flip", C2D_TEXTO_CENTRALIZADO, 32, 128, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 600, "Right button: right gamepad shoulder button, D key or Right Key. ", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 640, "Left: left gamepad shoulder button, A key or Left key.", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 800, "Press a control button to play, or ESC to quit anytime.", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 900, "(c) 2015 Paulo V W Radtke - follow me at @pvwradtke or http://chienloco.com/wp/", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_Sincroniza(C2D_FPS_PADRAO);
        if(teclado[C2D_TESC].pressionou)
        {
            choice = Game::quit;
            end=true;
        }
        else if(teclado[C2D_TC].pressionou)
        {
            end = true;
            choice = Game::credits;
        }
        else if(teclado[C2D_TESPACO].pressionou || teclado[C2D_TA].pressionou || teclado[C2D_TD].pressionou || teclado[C2D_TDIREITA].pressionou || teclado[C2D_TESQUERDA].pressionou)
        {
            end=true;
            choice = Game::game_keyplusmouse;
        }
        else if(gamepads[0].botoes[C2D_GBOTAO_L].pressionou || gamepads[0].botoes[C2D_GBOTAO_R].pressionou)
        {
            end = true;
            choice = Game::game_gamepad;
        }
    }
    C2D_RemoveSpriteSet(background);
    CA_FadeMusica(0);
    CA_RemoveMusica(musica);
    return choice;
}

bool Game::carregaListaFases(char *nome, int *numFases, char lista[MAX_FASES][MAX_TEXTO])
{
    FILE *arquivo = fopen(nome, "r");
    if(!arquivo)
        return false;
    if(fscanf(arquivo, "%d", numFases)==0)
    {
        fclose(arquivo);
        return false;
    }
    if(*numFases > MAX_FASES)
    {
        fclose(arquivo);
        return false;
    }
    for(int i=0;i<*numFases;i++)
    {
        fscanf(arquivo, "%s", lista[i]);
        printf("Stage %d of %d - file is %s.\n", i+1, *numFases, lista[i]);
    }
    fclose(arquivo);
    return true;
}

bool Game::gamescreen(Jogador &jogador, int numFase, char *arquivoFase)
{
    // Load resources
    char mensagem[512];
    char arquivoMusica[512];

    bool acabouFase = false;
    int tempoMensagem=180;
    unsigned int tileset = C2D_CarregaSpriteSet("./gfx/tileset.png", 32, 32);
    unsigned int spriteJogador = C2D_CarregaSpriteSet("./gfx/colored.png", 32, 32);
    unsigned int turn = CA_CarregaEfeito("sfx/turn.ogg");
    unsigned int crash = CA_CarregaEfeito("sfx/crash.ogg");
    unsigned int win = CA_CarregaEfeito("sfx/win.ogg");

    char *backgrounds[6]={"./gfx/back_01.jpg", "./gfx/back_02.jpg", "./gfx/back_03.jpg", "./gfx/back_04.jpg", "./gfx/back_05.jpg", "./gfx/back_06.jpg"};


    unsigned int background = C2D_CarregaSpriteSet(backgrounds[rand()%6], 0, 0);

    Fase    fase=Fase();
    int angulo=0;
    bool debug=false;
    if(!fase.carrega(arquivoFase))
    {
        printf("Failed loading stage %s.\n", arquivoFase);
        return false;
    }
    fase.imprime();
    jogador.inicializa(fase);
    unsigned int faixa = C2D_CarregaSpriteSet("./gfx/faixa.png", 0, 0);


    // Inicializa a fase com os dados do mapa
    char texto[128];

    C2D_TrocaCorLimpezaTela(0,0,0);
    bool fim=false;
    bool sai=false;
    // usa essa variável para controlar a lógica rodando 10x mais rápido que o desenho e as entradas
    int contador=-1;
    bool roda=false;
    int contaTrigger=0;
    int trigger=0;
    int contaVitoria=0;
    int contaGameOver=0;
    int estado = Jogador::JOGADOR_NORMAL;
    while(!fim && !sai)
    {
        contador=(contador+1)%10;
        if(contador==0)
            roda=true;
        else
            roda=false;
        // roda a lógica a 60fps
        if(roda && estado==Jogador::JOGADOR_NORMAL){
            if(teclado[C2D_TDIREITA].pressionou || teclado[C2D_TD].pressionou || gamepads[0].botoes[C2D_GBOTAO_R].pressionou){
                angulo=(angulo+90)%360;
                jogador.rotaciona(90);
                CA_TocaEfeito(turn, CA_CENTRO);
            }
            else if(teclado[C2D_TESQUERDA].pressionou || teclado[C2D_TA].pressionou || gamepads[0].botoes[C2D_GBOTAO_L].pressionou){
                angulo=(angulo-90);
                if(angulo<0)
                    angulo=270;
                jogador.rotaciona(-90);
                CA_TocaEfeito(turn, CA_CENTRO);
            }
            if(teclado[C2D_TF12].pressionou)
                debug=!debug;
            // testa se pegou um dos triggers
            int x, y, largura, altura;
            jogador.pegaBoundingBox(&x, &y, &largura, &altura);
            for(int i=JOGO_TRIGGER_0;i<=JOGO_TRIGGER_4;i++)
                if(fase.colideMarca(i, x, y, largura, altura))
                    if(fase.ativaTrigger(i-JOGO_TRIGGER_0)){
                        contaTrigger=180;
                        trigger=i-JOGO_TRIGGER_0;
                        break;
                    }
        }
        if(roda){
            if(estado==Jogador::JOGADOR_MORTO && contaGameOver>0){
                if(--contaGameOver==0)
                    fim=true;
            }
            if(estado==Jogador::JOGADOR_VITORIA && contaVitoria>0){
                if(--contaVitoria==0)
                    fim=true;
            }
            if(contaTrigger>0)
                contaTrigger--;
            if(teclado[C2D_TESC].pressionou)
                sai=true;
        }
        // roda a lógica a 600fps
        estado = jogador.atualiza(fase, angulo);
        // Verifica se ganhou
        if(contaGameOver==0 && estado==Jogador::JOGADOR_MORTO){
            contaGameOver=180;
            CA_TocaEfeito(crash, CA_CENTRO);
        }
        if(contaVitoria==0 && estado==Jogador::JOGADOR_VITORIA){
            contaVitoria=180;
            CA_TocaEfeito(win, CA_CENTRO);
        }



        // Atualizaa tela a 60fps
        if(roda){
            // Desenha a tela
            C2D_LimpaTela();
            C2D_DesenhaSprite(background, 0, 0, 0);
            int x_desl=(1920-32*32)/2;
            int y_desl=(1080-32*32)/2;
            fase.desenha(x_desl, y_desl, angulo, debug);
            jogador.desenha(x_desl, y_desl, angulo);
            if(contaVitoria>0){
                C2D_DesenhaSprite(faixa, 0, 0, 300);
                C2D_DesenhaTexto(fonteMensagem, 960, 310, "You made it!", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
            }else if(contaGameOver>0){
                C2D_DesenhaSprite(faixa, 0, 0, 300);
                C2D_DesenhaTexto(fonteMensagem, 960, 310, "Better luck the next time ...", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
            }else if(contaTrigger>0){
                C2D_DesenhaSprite(faixa, 0, 0, 300);
                C2D_DesenhaTexto(fonteMensagem, 960, 310, fase.pegaMensagemTrigger(trigger), C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
            }


            C2D_Sincroniza(C2D_FPS_PADRAO);
        }
    }
    C2D_RemoveSpriteSet(tileset);
    C2D_RemoveSpriteSet(spriteJogador);
    CA_RemoveEfeito(turn);
    CA_RemoveEfeito(crash);
    CA_RemoveEfeito(win);
    if(sai)
        return false;
    return true;
}

float Game::calculaAngulo(const float dx, const float dy)
{
    float angulo = -1;
    if(dx == 0)
        angulo=90;
    else{
        float tg = (float)abs(dy) / abs(dx);
        angulo=atan(tg)*180/PI;
    }
    // According to the quadrant we are, we should add 90, 180 or 270 degrees
    if(dx<0 && dy >= 0)
        angulo=180-angulo;
    else if(dx<=0 && dy < 0)
        angulo += 180;
    else if(dx>0 && dy <0)
        angulo = 360 - angulo;
    return angulo;
}

bool Game::creditsscreen(){
    int spriteCreditos;
    unsigned int fundosCreditos = C2D_CarregaSpriteSet("gfx/credits.jpg", 0, 0);
    spriteCreditos = C2D_CarregaSpriteSet("gfx/credits.png", 0, 0);
    if(spriteCreditos==0)
        return false;
    int tempo=24*60;
    bool sai=false;
    int altura=1080+4069;
    int contador=0;
    C2D_TrocaCorLimpezaTela(0,0,0);
    int musica = CA_CarregaMusica("./sfx/credits.ogg");
    CA_TocaMusica(musica, 1);
    while(!sai && contador < tempo)
    {
        C2D_LimpaTela();
        C2D_DesenhaSprite(fundosCreditos, 0, 0, 0);
        int y = (int)(((float)contador/tempo)*altura);
        contador++;
        // Determina se sai
        C2D_DesenhaSprite(spriteCreditos, 0, 0, 1080-y);
        C2D_Sincroniza(C2D_FPS_PADRAO);
        // se pressionou qualquer tecla, pula fora
        for(int i=0;i<C2D_MAX_TECLAS;i++)
            if(teclado[i].pressionou)
                sai=true;
        // Idem para o gamepad
        for(int i=0;i<C2D_GMAX_BOTOES;i++)
            if(gamepads[0].botoes[i].pressionou)
                sai=true;
    }
    CA_FadeMusica(0);
    C2D_RemoveSpriteSet(fundosCreditos);
    C2D_RemoveSpriteSet(spriteCreditos);
    CA_RemoveMusica(musica);
    return true;
}

bool Game::loadhighscore(){
    FILE *hs = fopen("./resources/hs.dat", "r");
    if(hs==0)
        return false;
    fread(&highScore, sizeof(int), 1, hs);
    fclose(hs);
    return true;
}

bool Game::savehighscore(){
    FILE *hs = fopen("./resources/hs.dat", "w");
    if(hs==0)
        return false;
    fwrite(&highScore, sizeof(int), 1, hs);
    fclose(hs);
    return true;
}


