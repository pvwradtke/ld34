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
        fonteURL = C2D_CarregaFonte("gfx/TerminusBold.ttf", "URL", 60);
        fonteTituloPequena = C2D_CarregaFonte("gfx/Stac555n.ttf", "titulo_pequena", 120);
        fonteTituloGrande = C2D_CarregaFonte("gfx/Abduction.ttf", "titulo_grande", 130);
        fonteMensagem = C2D_CarregaFonte("gfx/Stac555n.ttf", "titulo_pequena", 60);
        fontePlacar = C2D_CarregaFonte("gfx/DS-DIGIB.TTF", "titulo_placar", 32);
        efeitoInimigo = CA_CarregaEfeito("audio/enemyhit.ogg");
        efeitoJogador = CA_CarregaEfeito("audio/playerhit.ogg");
        efeitoRepele = CA_CarregaEfeito("audio/repel.ogg");
        teclado = C2D_PegaTeclas();
        gamepads = C2D_PegaGamepads();
        mouse = C2D_PegaMouse();
        initialized=true;
    }
    else
        return false;
    if(!loadhighscore())
        highScore = 3;
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
                if(!carregaListaFases("maps/level_list.dat", &numFases, listaFases))
                {
                    printf("Erro ao carregar as fases!\n");
                    currentstate = Game::mainmenu;
                }
                else
                {
                    srand(time(NULL));
                    Jogador jogador;
                    jogador.vivo=true;
                    int placarFinal=0;
                    int placar;
                    int previousHighScore = highScore;
                    int faseAtual=0;
                    bool resultado=false;
                    while(faseAtual<numFases)
                    {
                        placar=placarFinal;
                        resultado = gamescreen(&jogador, currentstate, faseAtual, &placar, listaFases[faseAtual]);
                        if(resultado)
                        {
                            if(jogador.vivo)
                            {
                                faseAtual++;
                                placarFinal+=placar;
                            }

                        }
                        else
                        {
                            currentstate = Game::mainmenu;
                            break;
                        }
                    }
                    if(highScore > previousHighScore)
                        savehighscore();
                    if(resultado)
                        currentstate = Game::credits;
                    else
                        currentstate = Game::mainmenu;
                }
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
    int musica = CA_CarregaMusica("audio/01-menu.ogg");
    CA_TocaMusica(musica, 1);
    while(!end){
        C2D_LimpaTela();
        C2D_DesenhaTexto(fonteTituloGrande, 960, 350, "Magnetic  Force", C2D_TEXTO_CENTRALIZADO, 230, 230, 230, 255);
        C2D_DesenhaTexto(fonteTituloPequena, 960, 200, "Mighty", C2D_TEXTO_CENTRALIZADO, 255, 0, 0, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 600, "Press Gamepad button A button to play with the Gamepad (BEST!)", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 640, "Press Space to play with Keyboard+Mouse (or go out and buy a Gamepad)", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 800, "Press ESC to quit", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 900, "(c) 2013 Paulo V W Radtke - follow me at @pvwradtke or http://chienloco.com/wp/", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
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
        else if(teclado[C2D_TESPACO].pressionou)
        {
            end=true;
            choice = Game::game_keyplusmouse;
        }
        else if(gamepads[0].botoes[C2D_GBOTAO_A].pressionou)
        {
            end = true;
            choice = Game::game_gamepad;
        }
    }
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

bool Game::gamescreen(Jogador *jogador, int controle, int numFase, int *placar, char *arquivoFase)
{
    // The current map
    Mapa mapa;
    //memset(mapa, JOGO_CHAO, 33*59*sizeof(int));
    // Load resources
    int cenario = C2D_CarregaSpriteSet("gfx/map.png", 32, 32);
    int spriteJogador = C2D_CarregaSpriteSet("gfx/jogador.png", 32, 32);
    int spriteIma = C2D_CarregaSpriteSet("gfx/imas.png", 20, 20);
    int spritemouse = C2D_CarregaSpriteSet("gfx/mouse.png", 0, 0);
    int spriteEstrela =C2D_CarregaSpriteSet("gfx/inimigo.png", 32, 32);
    int spriteFaixa = C2D_CarregaSpriteSet("gfx/faixa.png", 0, 0);
    char mensagem[512];
    char arquivoMusica[512];

    bool acabouFase = false;
    int tempoMensagem=180;
    char *listaMusicas[4]= {
        "audio/02-tutorial.ogg",
        "audio/03-tutorial2.ogg",
        "audio/05-youknowthosekillright.ogg",
        "audio/06-thinkfast.ogg"
    };


    if(!carregaFase(arquivoFase, mensagem, mapa, arquivoMusica))
    {
        printf("Erro ao carregar a fase %s\n", arquivoFase);
        return false;
    }
    printf("Fase %d: Mensagem: %s\nMúsica: %s\n", numFase+1, mensagem, arquivoMusica);

    // Se a música for aleatória
    if(strcmp(arquivoMusica, "random")==0)
    {
        int escolha = rand()%4;
        strcpy(arquivoMusica, listaMusicas[escolha]);
    }

    int musica = CA_CarregaMusica(arquivoMusica);
    int musicaClear = CA_CarregaMusica("audio/04-stage_clear.ogg");

    // Inicializa a fase com os dados do mapa
    Personagem imas[MAX_IMAS];
    Personagem estrelas[MAX_ESTRELAS];
    processaFase(mapa, jogador, imas, estrelas);

    char texto[128];

    C2D_TrocaCorLimpezaTela(0,0,0);
    bool fim=false;
    bool sai=false;
    bool acabouEstrelas=false;
    CA_TocaMusica(musica, -1);
    while(!fim && !sai)
    {
        // Lógica dos personagens
        // Verifica se deve desenhar a mensagem (pára a lógica)
        if(tempoMensagem>0)
        {
            tempoMensagem--;
            if(tempoMensagem==0)
                if(acabouEstrelas)
                    fim=true;
        }
        else
        {
            atualizaJogador(mapa, jogador, controle);
            for(int i=0;i<MAX_IMAS;i++)
                    if(imas[i].tipo!=JOGO_MORTO)
                        atualizaIma(mapa, &imas[i], jogador);
            for(int i=0;i<MAX_ESTRELAS;i++)
                if(estrelas[i].tipo != JOGO_MORTO)
                    atualizaEstrela(mapa, &estrelas[i]);
            // Algum ímã atingiu uma estrela
            int parcial=colisoesImasEstrelas(imas, estrelas);
            if(*placar<highScore && *placar+parcial>highScore)
                printf("quebrou o recorde");
            *placar+=parcial;
            if(*placar>highScore)
                highScore=*placar;
            // Acabaram as estrelas?
            acabouEstrelas=true;
            for(int i=0;i<MAX_ESTRELAS;i++)
            {
                if(estrelas[i].tipo!=JOGO_MORTO)
                    acabouEstrelas=false;
            }
            if(teclado[C2D_TF1].pressionou)
                acabouEstrelas=true;
            if(acabouEstrelas)
            {
                tempoMensagem=180;
                sprintf(mensagem, "Stage %d clear", numFase+1);
                CA_FadeMusica(0);
                CA_TocaMusica(musicaClear,1);
            }
            // O jogador colidiu com uma estrela?
            if(colisaoJogadorEstrelas(jogador, estrelas))
            {
                jogador->vivo=false;
                jogador->tempoMorte=180;
            }
            if(!jogador->vivo && jogador->tempoMorte==0)
                fim=true;
        }

        if(teclado[C2D_TESC].pressionou || teclado[C2D_TENCERRA].pressionou)
            sai=true;

        C2D_LimpaTela();
        // Desenha o mapa
        for(int i=0;i<33;i++)
            for(int j=0;j<59;j++)
                if(mapa[i][j]==JOGO_CHAO)
                    C2D_DesenhaSprite(cenario, 0, DESLX+32*j, DESLY+32*i);
                else if(mapa[i][j]==JOGO_PAREDE)
                    C2D_DesenhaSprite(cenario, 1, DESLX+32*j, DESLY+32*i);
        // Desenha os personagens
        for(int i=0;i<MAX_ESTRELAS;i++)
            if(estrelas[i].tipo!=JOGO_MORTO)
            {
                if(estrelas[i].tipo == JOGO_ESTRELA_MORRENDO)
                {
                    C2D_AlteraAlphaDoSprite(spriteEstrela, estrelas[i].timer);
                    if((estrelas[i].timer/4)%2)
                        C2D_DesenhaSpriteEspecial(spriteEstrela, 1, DESLX+estrelas[i].x, DESLY+estrelas[i].y, C2D_FLIP_NENHUM, 1.0, 1.0, estrelas[i].anguloRotacao);
                    C2D_RestauraAlphaDoSprite(spriteEstrela);
                }
                else
                    C2D_DesenhaSpriteEspecial(spriteEstrela, 0, DESLX+estrelas[i].x, DESLY+estrelas[i].y, C2D_FLIP_NENHUM, 1.0, 1.0, estrelas[i].anguloRotacao);
            }
        if(jogador->vivo)
            C2D_DesenhaSpriteEspecial(spriteJogador, 0, DESLX+(int)jogador->x, DESLY+(int)jogador->y, C2D_FLIP_NENHUM, 1.0, 1.0, jogador->angulo);
        else
        {
            C2D_AlteraAlphaDoSprite(spriteJogador, jogador->tempoMorte);
            if((jogador->tempoMorte/4)%2)
                C2D_DesenhaSpriteEspecial(spriteJogador, 1, DESLX+(int)jogador->x, DESLY+(int)jogador->y, C2D_FLIP_NENHUM, 1.0, 1.0, jogador->angulo);
            C2D_RestauraAlphaDoSprite(spriteJogador);
        }
        for(int i=0;i<MAX_IMAS;i++)
            if(imas[i].tipo!=JOGO_MORTO)
                C2D_DesenhaSprite(spriteIma, imas[i].tipo-JOGO_NEGATIVO, DESLX+(int)imas[i].x, DESLY+(int)imas[i].y);
        if(controle == game_keyplusmouse)
            C2D_DesenhaSprite(spritemouse, 0, mouse->x-TAM_MOUSE/2, mouse->y-TAM_MOUSE/2);
        // Desenha o placar e demais informações
        sprintf(texto, "Stage %02d", numFase+1);
        C2D_DesenhaTexto(fontePlacar, 50, 20, texto, C2D_TEXTO_ESQUERDA, 0, 255, 0, 255);
        sprintf(texto, "Score: %03d00", *placar);
        C2D_DesenhaTexto(fontePlacar, 960, 20, texto, C2D_TEXTO_CENTRALIZADO, 0, 255, 0, 255);
        sprintf(texto, "Record: %03d00", highScore);
        C2D_DesenhaTexto(fontePlacar, 1870, 20, texto, C2D_TEXTO_DIREITA, 0, 255, 0, 255);
        if(tempoMensagem>0)
        {
            C2D_DesenhaSprite(spriteFaixa, 0, 0, 400);
            C2D_DesenhaTexto(fonteMensagem, 960, 410, mensagem, C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        }
        C2D_Sincroniza(C2D_FPS_PADRAO);

    }
    CA_FadeMusica(0);
    if(sai)
        return false;
    return true;
}

void Game::processaFase(Mapa mapa, Jogador *jogador, Personagem imas[], Personagem estrelas[])
{
    // Procura os elementos
    int contaImas=0;
    int contaEstrelas=0;
    for(int i=0;i<33;i++)
        for(int j=0;j<59;j++)
        {
            switch(mapa[i][j])
            {
            case JOGO_JOGADOR:
                jogador->x=32*j;
                jogador->y=32*i;
                jogador->angulo=0;
                mapa[i][j]=JOGO_CHAO;
                jogador->vivo=true;
                break;
            case JOGO_NEGATIVO:
            case JOGO_POSITIVO:
                if(contaImas<MAX_IMAS)
                {
                    imas[contaImas].tipo=mapa[i][j];
                    imas[contaImas].angulo=0;
                    imas[contaImas].velocidade=0;
                    imas[contaImas].x=32*j+(32-TAM_IMA)/2;
                    imas[contaImas].y=32*i+(32-TAM_IMA)/2;
                    mapa[i][j]=JOGO_CHAO;
                    contaImas++;
                }
                break;
            case JOGO_ESTRELA:
                if(contaEstrelas<MAX_ESTRELAS)
                {
                    estrelas[contaEstrelas].tipo=mapa[i][j];
                    estrelas[contaEstrelas].angulo=0;
                    estrelas[contaEstrelas].velocidade=0;
                    estrelas[contaEstrelas].x=32*j;
                    estrelas[contaEstrelas].y=32*i;
                    estrelas[contaEstrelas].anguloRotacao=rand()%360;
                    mapa[i][j]=JOGO_CHAO;
                    contaEstrelas++;
                }
                break;
            case JOGO_ESTRELA0:
            case JOGO_ESTRELA45:
            case JOGO_ESTRELA90:
            case JOGO_ESTRELA135:
            case JOGO_ESTRELA180:
            case JOGO_ESTRELA225:
            case JOGO_ESTRELA270:
            case JOGO_ESTRELA315:
                if(contaEstrelas<MAX_ESTRELAS)
                {
                    estrelas[contaEstrelas].tipo=mapa[i][j];
                    estrelas[contaEstrelas].angulo=45*(mapa[i][j]-JOGO_ESTRELA0);
                    estrelas[contaEstrelas].velocidade=VELOCIDADE_ESTRELA;
                    estrelas[contaEstrelas].x=32*j;
                    estrelas[contaEstrelas].y=32*i;
                    estrelas[contaEstrelas].anguloRotacao=rand()%360;
                    mapa[i][j]=JOGO_CHAO;
                    contaEstrelas++;
                }
                break;
            }
        }
    // Reseta os imas restantes
    for(int i=contaImas;i<MAX_IMAS;i++)
        imas[i].tipo=JOGO_MORTO;
    for(int i=contaEstrelas;i<MAX_ESTRELAS;i++)
        estrelas[i].tipo=JOGO_MORTO;
}

bool Game::carregaFase(char *name, char *mensagem, Mapa mapa, char *song)
{
    FILE *arquivo = fopen(name, "r");
    if(arquivo==0)
        return false;
    if(fgets(mensagem, 256, arquivo))
        // Removes the new line in the end of this string
        mensagem[strlen(mensagem)-1]=0;
    else{
        fclose(arquivo);
        return false;
    }
    if(fgets(song, 256, arquivo))
        // Removes the new line in the end of this string
        song[strlen(song)-1]=0;
    else{
        fclose(arquivo);
        return false;
    }

    // reds the map
    for(int i=0;i<MAPA_LINHAS;i++)
        for(int j=0;j<MAPA_COLUNAS;j++)
            if(fscanf(arquivo, "%d", &mapa[i][j])==0)
            {
                fclose(arquivo);
                return false;
            }
    return true;
}

void Game::atualizaJogador(Mapa mapa, Jogador *jogador, int controle)
{
    static int counter =0;
    if(jogador->vivo)
    {
        // as variáveis para guardar as coordenadas do jogador
        int x=jogador->x, y=jogador->y;
        // Escolhe se o controle é pelo gamepad ou pelo teclado
        if(controle==Game::game_gamepad)
        {
            if(gamepads[0].eixos[C2D_GLEIXOX]!=0 || gamepads[0].eixos[C2D_GLEIXOY]!=0)
            {
                // Calcula o ângulo do deslocamento
                float angulo = calculaAngulo(gamepads[0].eixos[C2D_GLEIXOX], gamepads[0].eixos[C2D_GLEIXOY]);
                // Calcula a nova posição
                x = jogador->x+cos(angulo*PI/180)*VELOCIDADE_JOGADOR;
                y = jogador->y+sin(angulo*PI/180)*VELOCIDADE_JOGADOR;

            }
            // Calcula o angulo em que desenha o elemento
            if(gamepads[0].eixos[C2D_GREIXOX]!=0 || gamepads[0].eixos[C2D_GREIXOY]!=0)
                jogador->angulo=calculaAngulo(gamepads[0].eixos[C2D_GREIXOX], gamepads[0].eixos[C2D_GREIXOY]);
        }
        else
        {
            if((teclado[C2D_TESQUERDA].pressionando && !teclado[C2D_TCIMA].pressionando && !teclado[C2D_TBAIXO].pressionando) ||
               (teclado[C2D_TA].pressionando && !teclado[C2D_TW].pressionando && !teclado[C2D_TS].pressionando))
                x=jogador->x-VELOCIDADE_JOGADOR;
            else if((teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TCIMA].pressionando && !teclado[C2D_TBAIXO].pressionando) ||
                    (teclado[C2D_TD].pressionando && !teclado[C2D_TW].pressionando && !teclado[C2D_TS].pressionando))
                x=jogador->x+VELOCIDADE_JOGADOR;
            else if((teclado[C2D_TCIMA].pressionando && !teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TESQUERDA].pressionando) ||
                    (teclado[C2D_TW].pressionando && !teclado[C2D_TD].pressionando && !teclado[C2D_TA].pressionando))
                y=jogador->y-VELOCIDADE_JOGADOR;
            else if((teclado[C2D_TBAIXO].pressionando && !teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TESQUERDA].pressionando) ||
                    (teclado[C2D_TS].pressionando && !teclado[C2D_TD].pressionando && !teclado[C2D_TA].pressionando))
                y=jogador->y+VELOCIDADE_JOGADOR;
            else if((teclado[C2D_TCIMA].pressionando && teclado[C2D_TDIREITA].pressionando) || (teclado[C2D_TW].pressionando && teclado[C2D_TD].pressionando))
            {
                x=jogador->x+VELOCIDADE_JOGADOR*0.707106781;
                y=jogador->y-VELOCIDADE_JOGADOR*0.707106781;
            }
            else if((teclado[C2D_TCIMA].pressionando && teclado[C2D_TESQUERDA].pressionando) || (teclado[C2D_TW].pressionando && teclado[C2D_TA].pressionando))
            {
                x=jogador->x-VELOCIDADE_JOGADOR*0.707106781;
                y=jogador->y-VELOCIDADE_JOGADOR*0.707106781;
            }
            else if((teclado[C2D_TBAIXO].pressionando && teclado[C2D_TESQUERDA].pressionando) || (teclado[C2D_TS].pressionando && teclado[C2D_TA].pressionando))
            {
                x=jogador->x-VELOCIDADE_JOGADOR*0.707106781;
                y=jogador->y+VELOCIDADE_JOGADOR*0.707106781;
            }
            else if((teclado[C2D_TBAIXO].pressionando && teclado[C2D_TESQUERDA].pressionando) || (teclado[C2D_TS].pressionando && teclado[C2D_TA].pressionando))
            {
                x=jogador->x-VELOCIDADE_JOGADOR*0.707106781;
                y=jogador->y+VELOCIDADE_JOGADOR*0.707106781;
            }
            else if((teclado[C2D_TBAIXO].pressionando && teclado[C2D_TDIREITA].pressionando) || (teclado[C2D_TS].pressionando && teclado[C2D_TD].pressionando))
            {
                x=jogador->x+VELOCIDADE_JOGADOR*0.707106781;
                y=jogador->y+VELOCIDADE_JOGADOR*0.707106781;
            }
            jogador->angulo=calculaAngulo(mouse->x-jogador->x+(TAM_JOGADOR/2), mouse->y-jogador->y+(TAM_JOGADOR/2));
        }
        // Verifica se o jogador está entrando dentro de um bloco
        int xesq = (int)x /32;
        int xdir = ((int)x +32)/32;
        int ycima = (int)y/32;
        int ybaixo = ((int)y+ 32)/32;
        int xmeio = ((int)x +16)/32;
        int ymeio = ((int)y + 16)/32;
        // Está batendo em cima?
        if(mapa[ycima][xmeio] != JOGO_CHAO)
            y=((ycima+1)*32);
        else if(mapa[ybaixo][xmeio] != JOGO_CHAO)
            y=((ycima)*32);
        // Está batendo à esquerda
        if(mapa[ymeio][xesq] != JOGO_CHAO)
            x=(xesq+1)*32 ;
        else if(mapa[ymeio][xdir] != JOGO_CHAO)
            x=(xesq)*32;
        // Atualiza a posição do jogador
        jogador->x=x;
        jogador->y=y;
    }
    else
    {
        jogador->tempoMorte-=2;
        if(jogador->tempoMorte<0)
            jogador->tempoMorte=0;

    }
}

void Game::atualizaIma(Mapa mapa, Personagem *ima, Jogador *jogador)
{
    //  se o jogador está perto para ganhar velocidade e direção
    int xcentroj = jogador->x + (TAM_JOGADOR/2);
    int ycentroj = jogador->y + (TAM_JOGADOR/2);
    int xcentroi = ima->x + TAM_IMA/2;
    int ycentroi = ima->y + TAM_IMA/2;
    // Calcula o quadrado da distância
    float distancia = sqrt((xcentroj-xcentroi)*(xcentroj-xcentroi)+ (ycentroj-ycentroi)*(ycentroj-ycentroi));
    // Se o quadrado da distância for menor que o quadrado da distância mínima
    if(distancia < DISTANCIA_JOGADOR)
    {
        // Calcula o angulo entre o jogador e o íma

        float anguloJogadorIma = calculaAngulo(xcentroi-xcentroj, ycentroi-ycentroj);
        float anguloImaJogador = calculaAngulo(xcentroj-xcentroi, ycentroj-ycentroi);
        // Calcula a diferença dos ângulos do jo
        int diferenca=abs(anguloJogadorIma-(int)jogador->angulo);
        // Caso seja maior que 270, normaliza o quadrante (casos especiais)
        if(diferenca>270)
            diferenca=360-diferenca;
        //printf("Diferenca vale: %d\n", diferenca);
        // Se for menor que 90, está apontando para o pólo positivo, se for maior que 90, aponta o põlo negativo
        // Aqui, repele o ímã
        if((diferenca < 45 && ima->tipo==JOGO_POSITIVO) || (diferenca>135 && ima->tipo==JOGO_NEGATIVO))
        {
            if(diferenca>90)
                diferenca = 180-diferenca;
            ima->angulo = anguloJogadorIma;
            //ima->velocidade=VELOCIDADE_REPULSAO_IMA*(45.0-diferenca)/45.0;
            // Se a velocidade é zero, toca o efeito
            CA_TocaEfeito(efeitoRepele, xcentroi);
            ima->velocidade=VELOCIDADE_REPULSAO_IMA;
            ////printf("Deu velocidade de repulsao %f\n", ima->velocidade);
        }
        // Aqui faz o contrário, afasta os ímãs
        else if((diferenca < 45 && ima->tipo==JOGO_NEGATIVO) || (diferenca>135 && ima->tipo==JOGO_POSITIVO))
        {
            // normaliza em zero
            if(diferenca>90)
                diferenca = 180-diferenca;
            ima->angulo = anguloImaJogador;
            //ima->velocidade=(VELOCIDADE_ATRACAO_IMA)*(45.0-diferenca)/45.0;
            ima->velocidade=VELOCIDADE_ATRACAO_IMA;
            if(distancia < TAM_JOGADOR/2 + TAM_IMA +5)
                ima->velocidade=0;
            //printf("Deu velocidade de atracao %f\n", ima->velocidade);
        }

    }
    // Futura posição do ímã na tela
    float x=ima->x+ima->velocidade*cos(ima->angulo*PI/180);
    float y=ima->y+ima->velocidade*sin(ima->angulo*PI/180);

    // Verifica se o jogador está entrando dentro de um bloco
    int xesq = (int)x/32;
    int xdir = ((int)x+TAM_IMA)/32;
    int ycima = (int)y/32;
    int ybaixo = ((int)y + TAM_IMA)/32;
    int xmeio = ((int)x + TAM_IMA/2)/32;
    int ymeio = ((int)y + TAM_IMA/2)/32;

    bool bateu=false;
    // Está batendo em cima?
    if(mapa[ycima][xmeio] != JOGO_CHAO)
    {
        y=((ycima+1)*32);
        ima->angulo=360-ima->angulo;
    }
    else if(mapa[ybaixo][xmeio] != JOGO_CHAO)
    {
        y=((ycima)*32+32-TAM_IMA);
        ima->angulo=360-ima->angulo;
    }
    // Está batendo à esquerda
    if(mapa[ymeio][xesq] != JOGO_CHAO)
    {
        x=(xesq+1)*32;
        ima->angulo=180-ima->angulo;
    }
    else if(mapa[ymeio][xdir] != JOGO_CHAO)
    {
        x=((xesq)*32+32-TAM_IMA);
        ima->angulo=180-ima->angulo;
    }
    if(ima->angulo<0)
        ima->angulo+=360;
    else
        ima->angulo= (int)ima->angulo%360;
    // Simula o atrito
    if(ima->velocidade>0)
    {
        ima->velocidade-=0.1;
        if(ima->velocidade<0)
            ima->velocidade=0;
    }

    // Atualiza a posição do ímã
    ima->x = x;
    ima->y=y;
}

void Game::atualizaEstrela(Mapa mapa, Personagem *estrela)
{
    // Se está morrendo, simplesmente decrementa o contador de morte e volta
    if(estrela->tipo == JOGO_ESTRELA_MORRENDO)
    {
        estrela->timer-=2;
        if(estrela->timer <= 0)
            estrela->tipo = JOGO_MORTO;
        return;
    }

    // Atualiza a rotação
    estrela->anguloRotacao=(estrela->anguloRotacao+2)%360;


    // Futura posição da estrela na tela
    float x=estrela->x+estrela->velocidade*cos(estrela->angulo*PI/180);
    float y=estrela->y+estrela->velocidade*sin(estrela->angulo*PI/180);

    // Verifica se o jogador está entrando dentro de um bloco
    int xesq = (int)x/32;
    int xdir = ((int)x+TAM_ESTRELA)/32;
    int ycima = (int)y/32;
    int ybaixo = ((int)y + TAM_ESTRELA)/32;
    int xmeio = ((int)x + TAM_ESTRELA/2)/32;
    int ymeio = ((int)y + TAM_ESTRELA/2)/32;

    bool bateu=false;
    // Está batendo em cima?
    if(mapa[ycima][xmeio] != JOGO_CHAO)
    {
        y=((ycima+1)*32);
        estrela->angulo=360-estrela->angulo;
    }
    else if(mapa[ybaixo][xmeio] != JOGO_CHAO)
    {
        y=((ycima)*32);
        estrela->angulo=360-estrela->angulo;
    }
    // Está batendo à esquerda
    if(mapa[ymeio][xesq] != JOGO_CHAO)
    {
        x=(xesq+1)*32;
        estrela->angulo=180-estrela->angulo;
    }
    else if(mapa[ymeio][xdir] != JOGO_CHAO)
    {
        x=((xesq)*32);
        estrela->angulo=180-estrela->angulo;
    }
    if(estrela->angulo<0)
        estrela->angulo+=360;
    else
        estrela->angulo= (int)estrela->angulo%360;

    // Atualiza a posição do ímã
    estrela->x = x;
    estrela->y=y;

}

int Game::colisoesImasEstrelas(Personagem imas[], Personagem estrelas[])
{
    int placar=0;
    for(int i=0;i<MAX_IMAS;i++)
    {
        // O ímã só mata se está se movendo
        if(imas[i].velocidade==0)
            continue;
        for(int j=0;j<MAX_ESTRELAS;j++)
        {
            if(estrelas[j].tipo == JOGO_MORTO || estrelas[j].tipo==JOGO_ESTRELA_MORRENDO)
                continue;
            float xcentroe = estrelas[j].x + (TAM_ESTRELA/2);
            float ycentroe = estrelas[j].y + (TAM_ESTRELA/2);
            float xcentroi = imas[i].x + TAM_IMA/2;
            float ycentroi = imas[i].y + TAM_IMA/2;
            float distancia = sqrt((xcentroe-xcentroi)*(xcentroe-xcentroi)+ (ycentroe-ycentroi)*(ycentroe-ycentroi));
            if(distancia< TAM_ESTRELA/2 + TAM_IMA/2)
            {
                estrelas[j].tipo=JOGO_ESTRELA_MORRENDO;
                estrelas[j].timer=255;
                placar+=1;
                CA_TocaEfeito(efeitoInimigo, xcentroe);
            }
        }
    }
    return placar;
}

bool Game::colisaoJogadorEstrelas(Jogador *jogador, Personagem estrelas[])
{
    if(!jogador->vivo)
        return false;
    for(int j=0;j<MAX_ESTRELAS;j++)
    {
        if(estrelas[j].tipo == JOGO_MORTO || estrelas[j].tipo==JOGO_ESTRELA_MORRENDO)
            continue;
        float xcentroe = estrelas[j].x + (TAM_ESTRELA/2);
        float ycentroe = estrelas[j].y + (TAM_ESTRELA/2);
        float xcentroj = jogador->x + TAM_JOGADOR/2;
        float ycentroj = jogador->y + TAM_JOGADOR/2;
        float distancia = sqrt((xcentroe-xcentroj)*(xcentroe-xcentroj)+ (ycentroe-ycentroj)*(ycentroe-ycentroj));
        if(distancia< TAM_ESTRELA/2 + TAM_JOGADOR/2)
        {
            CA_TocaEfeito(efeitoJogador, xcentroj);
            return true;
        }
    }
    return false;
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
    spriteCreditos = C2D_CarregaSpriteSet("gfx/credits.png", 0, 0);
    if(spriteCreditos==0)
        return false;
    int tempo=24*60;
    bool sai=false;
    int altura=1080+4069;
    int contador=0;
    C2D_TrocaCorLimpezaTela(0,0,0);
    int musica = CA_CarregaMusica("audio/07-welldone.ogg");
    CA_TocaMusica(musica, 1);
    while(!sai && contador < tempo)
    {
        C2D_LimpaTela();
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
    C2D_RemoveSpriteSet(spriteCreditos);
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


