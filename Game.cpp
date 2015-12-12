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
                Jogador jogador=Jogador();
                bool resultado = gamescreen(jogador, 1, "./levels/testlevel.dat");
/*                if(!carregaListaFases("maps/level_list.dat", &numFases, listaFases))
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
                }*/
                currentstate = Game::mainmenu;
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
    while(!end){
        C2D_LimpaTela();
        C2D_DesenhaTexto(fonteSistema, 960, 600, "Press a Gamepad shoulder button to play with the gamepad", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 640, "Press Space to play with Keyboard", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 800, "Press ESC to quit", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
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
        else if(teclado[C2D_TESPACO].pressionou)
        {
            end=true;
            choice = Game::game_keyplusmouse;
        }
        else if(gamepads[0].botoes[C2D_GBOTAO_LS].pressionou || gamepads[0].botoes[C2D_GBOTAO_RS].pressionou)
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

bool Game::carregaFase(char *nome, Fase *fase)
{
    FILE *arquivo = fopen(nome, "r");
    bool title=false, triggers=false, mapa=false, geometria=false, marker=false;
    char tipo[MAX_TEXTO];
    while(!feof(arquivo) && (!title || !triggers || !mapa || !geometria || !marker))
    {
        // Lê o tipo do dado que vem agora
        if(fscanf(arquivo, "%s", tipo) == 0){
            fclose(arquivo);
            return false;
        }
        // Processa o tipo do dado
        if(strcmp(tipo, "stage_title")==0 && !title){
            if(fscanf(arquivo, "%*[ \n\t]%[^\t\n]", fase->title)==0){
                fclose(arquivo);
                return(false);
            }
            title=true;
        }
        else if(strcmp(tipo, "trigged_messages")==0 && !triggers){
            if(fscanf(arquivo, "%d", &fase->num_trigger)==0){
                fclose(arquivo);
                return(false);
            }
            for(int i=0;i<fase->num_trigger;i++){
                if(fscanf(arquivo, "%*[ \n\t]%[^\t\n]", fase->trigger_messages[i])==0){
                    fclose(arquivo);
                    return false;
                }
                fase->trigger_activated[i]=false;
            }
            // Para garantir, inicializa os triggers não utilizados
            for(int i=fase->num_trigger;i<MAX_TRIGGERS;i++)
                fase->trigger_activated[i]=true;
            triggers=true;
        }
        else if(strcmp(tipo, "map")==0){
            if(fscanf(arquivo, "%d %d", &fase->largura, &fase->altura)<2){
                fclose(arquivo);
                return false;
            }
            for(int i=0;i<fase->altura;i++)
                for(int j=0;j<fase->largura;j++)
                    if(fscanf(arquivo, "%d", &fase->mapa[i][j])==0){
                        fclose(arquivo);
                        return false;
                    }
            mapa=true;
        }
        else if(strcmp(tipo, "marker")==0){
            if(fscanf(arquivo, "%d", &fase->marca)==0){
                fclose(arquivo);
                return(false);
            }
            marker=true;
        }
        else if(strcmp(tipo, "geometry")==0){
            int largura, altura;
            if(fscanf(arquivo, "%d %d", &largura, &altura)<2){
                fclose(arquivo);
                return false;
            }
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++)
                    if(fscanf(arquivo, "%d", &fase->geometria[i][j])==0){
                        fclose(arquivo);
                        return false;
                    }
            geometria=true;
        }
    }
    // Verifica que leu tudo
    if(!title || !triggers || !mapa || !geometria || !marker)
        return false;
    // Processa a camada de geometria
    for(int i=0;i<fase->altura;i++)
        for(int j=0;j<fase->largura;j++)
            if(fase->geometria[i][j]!=0)
                fase->geometria[i][j]-=fase->marca-1;
    return true;
}

void Game::imprimeFase(Fase *fase)
{
    printf("Titulo: %s\n", fase->title);
    printf("Mapa:\n");
    for(int i=0;i<fase->altura;i++)
    {
        for(int j=0;j<fase->largura;j++)
            printf("%d\t", fase->mapa[i][j]);
        printf("\n");
    }
    printf("Geometria:\n");
    for(int i=0;i<fase->altura;i++)
    {
        for(int j=0;j<fase->largura;j++)
            printf("%d\t", fase->geometria[i][j]);
        printf("\n");
    }
    printf("Marca: %d\n", fase->marca);
    for(int i=0;i<fase->num_trigger;i++)
        printf("Trigger %d: %s\n", i+1, fase->trigger_messages[i]);
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
    char *listaMusicas[4]= {
        "audio/02-tutorial.ogg",
        "audio/03-tutorial2.ogg",
        "audio/05-youknowthosekillright.ogg",
        "audio/06-thinkfast.ogg"
    };
    Fase    fase;
    int angulo=0;
    bool debug=true;
    if(!carregaFase(arquivoFase, &fase))
    {
        printf("Failed loading stage %s.\n", arquivoFase);
        return false;
    }
    imprimeFase(&fase);
    jogador.inicializa(&fase);

/*    if(!carregaFase(arquivoFase, mensagem, mapa, arquivoMusica))
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
    int musicaClear = CA_CarregaMusica("audio/04-stage_clear.ogg");*/

    // Inicializa a fase com os dados do mapa
    char texto[128];

    C2D_TrocaCorLimpezaTela(0,0,0);
    bool fim=false;
    bool sai=false;
    while(!fim && !sai)
    {
        // roda a lógica
        if(teclado[C2D_TDIREITA].pressionou || teclado[C2D_TD].pressionou || gamepads[0].botoes[C2D_GBOTAO_R].pressionou)
            angulo=(angulo+90)%360;
        else if(teclado[C2D_TESQUERDA].pressionou || teclado[C2D_TA].pressionou || gamepads[0].botoes[C2D_GBOTAO_L].pressionou){
            angulo=(angulo-90);
            if(angulo<0)
                angulo=270;
        }
        if(teclado[C2D_TF12].pressionou)
            debug=!debug;
        // Desenha a tela
        C2D_LimpaTela();
        if(angulo==0){
            int x_desl=(1920-fase.largura*32)/2;
            int y_desl=(1080-fase.altura*32)/2;
            for(int i=0;i<fase.altura;i++)
                for(int j=0;j<fase.largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, fase.mapa[i][j]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && fase.geometria[i][j])
                        C2D_DesenhaSpriteEspecial(tileset, fase.geometria[i][j]+fase.marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                }
        }else if(angulo==90){
            int x_desl=(1920-fase.altura*32)/2;
            int y_desl=(1080-fase.largura*32)/2;
            for(int i=0;i<fase.altura;i++)
                for(int j=0;j<fase.largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, fase.mapa[fase.altura-1-j][i]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && fase.geometria[fase.altura-1-j][i])
                        C2D_DesenhaSpriteEspecial(tileset, fase.geometria[fase.altura-1-j][i]+fase.marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                }
        }else if(angulo==180){
            int x_desl=(1920-fase.largura*32)/2;
            int y_desl=(1080-fase.altura*32)/2;
            for(int i=0;i<fase.altura;i++)
                for(int j=0;j<fase.largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, fase.mapa[fase.largura-1-i][fase.altura-1-j]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && fase.geometria[fase.largura-1-i][fase.altura-1-j])
                        C2D_DesenhaSpriteEspecial(tileset, fase.geometria[fase.largura-1-i][fase.altura-1-j]+fase.marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                }
        }else if(angulo==270){
            int x_desl=(1920-fase.altura*32)/2;
            int y_desl=(1080-fase.largura*32)/2;
            for(int i=0;i<fase.altura;i++)
                for(int j=0;j<fase.largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, fase.mapa[j][fase.largura-1-i]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && fase.geometria[j][fase.largura-1-i])
                        C2D_DesenhaSpriteEspecial(tileset, fase.geometria[j][fase.largura-1-i]+fase.marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);

                }
        }
        C2D_Sincroniza(C2D_FPS_PADRAO);
        if(teclado[C2D_TESC].pressionou)
            fim=true;
    }
    CA_FadeMusica(0);
    C2D_RemoveSpriteSet(tileset);
    C2D_RemoveSpriteSet(spriteJogador);
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


