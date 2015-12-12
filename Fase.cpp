#include "Fase.h"

Fase::Fase()
{
    //ctor
    init = false;
}

Fase::~Fase()
{
    //dtor
    C2D_RemoveSpriteSet(tileset);
}

bool Fase::carrega(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    bool ltitle=false, ltriggers=false, lmapa=false, lgeometria=false, lmarker=false, ltile=false;
    char tipo[MAX_TEXTO];
    while(!feof(arquivo) && (!ltitle || !ltriggers || !lmapa || !lgeometria || !lmarker || !ltile))
    {
        // Lê o tipo do dado que vem agora
        if(fscanf(arquivo, "%s", tipo) == 0){
            fclose(arquivo);
            return false;
        }
        // Processa o tipo do dado
        if(strcmp(tipo, "stage_title")==0 && !title){
            if(fscanf(arquivo, "%*[ \n\t]%[^\t\n]", title)==0){
                fclose(arquivo);
                return(false);
            }
            ltitle=true;
        }
        // Processa o tipo do dado
        if(strcmp(tipo, "tileset")==0 && !tileset){
            if(fscanf(arquivo, "%*[ \n\t]%[^\t\n]", tipo)==0){
                fclose(arquivo);
                return(false);
            }
            tileset = C2D_CarregaSpriteSet(tipo, 32, 32);
            if(tileset)
                ltile=true;
            else{
                fclose(arquivo);
                return(false);
            }
        }

        else if(strcmp(tipo, "trigged_messages")==0 && !ltriggers){
            if(fscanf(arquivo, "%d", &num_trigger)==0){
                fclose(arquivo);
                return(false);
            }
            for(int i=0;i<num_trigger;i++){
                if(fscanf(arquivo, "%*[ \n\t]%[^\t\n]", trigger_messages[i])==0){
                    fclose(arquivo);
                    return false;
                }
                trigger_activated[i]=false;
            }
            // Para garantir, inicializa os triggers não utilizados
            for(int i=num_trigger;i<MAX_TRIGGERS;i++)
                trigger_activated[i]=true;
            ltriggers=true;
        }
        else if(strcmp(tipo, "map")==0 && !lmapa){
            if(fscanf(arquivo, "%d %d", &largura, &altura)<2){
                fclose(arquivo);
                return false;
            }
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++)
                    if(fscanf(arquivo, "%d", &mapa[i][j])==0){
                        fclose(arquivo);
                        return false;
                    }
            lmapa=true;
        }
        else if(strcmp(tipo, "marker")==0 && !lmarker){
            if(fscanf(arquivo, "%d", &marca)==0){
                fclose(arquivo);
                return(false);
            }
            lmarker=true;
        }
        else if(strcmp(tipo, "geometry")==0 && !lgeometria){
            int largura, altura;
            if(fscanf(arquivo, "%d %d", &largura, &altura)<2){
                fclose(arquivo);
                return false;
            }
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++)
                    if(fscanf(arquivo, "%d", &geometria[i][j])==0){
                        fclose(arquivo);
                        return false;
                    }
            lgeometria=true;
        }
    }
    // Verifica que leu tudo
    if(!ltitle || !ltriggers || !lmapa || !lgeometria || !lmarker || !ltile)
        return false;
    // Processa a camada de geometria
    for(int i=0;i<altura;i++)
        for(int j=0;j<largura;j++)
            if(geometria[i][j]!=0)
                geometria[i][j]-=marca-1;
    return init=true;
}

void Fase::imprime()
{
    printf("Titulo: %s\n", title);
    printf("Tileset: %d\n", tileset);
    printf("Mapa:\n");
    for(int i=0;i<altura;i++)
    {
        for(int j=0;j<largura;j++)
            printf("%d\t", mapa[i][j]);
        printf("\n");
    }
    printf("Geometria:\n");
    for(int i=0;i<altura;i++)
    {
        for(int j=0;j<largura;j++)
            printf("%d\t", geometria[i][j]);
        printf("\n");
    }
    printf("Marca: %d\n", marca);
    for(int i=0;i<num_trigger;i++)
        printf("Trigger %d: %s\n", i+1, trigger_messages[i]);
}

void Fase::desenha(int x_desl, int y_desl, int angulo, bool debug)
{
    if(init){
        if(angulo==0){
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, mapa[i][j]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && geometria[i][j])
                        C2D_DesenhaSpriteEspecial(tileset, geometria[i][j]+marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                }
        }else if(angulo==90){
            x_desl=(1920-altura*32)/2;
            y_desl=(1080-largura*32)/2;
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, mapa[altura-1-j][i]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && geometria[altura-1-j][i])
                        C2D_DesenhaSpriteEspecial(tileset, geometria[altura-1-j][i]+marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                }
        }else if(angulo==180){
            x_desl=(1920-largura*32)/2;
            y_desl=(1080-altura*32)/2;
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, mapa[largura-1-i][altura-1-j]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && geometria[largura-1-i][altura-1-j])
                        C2D_DesenhaSpriteEspecial(tileset, geometria[largura-1-i][altura-1-j]+marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                }
        }else if(angulo==270){
            x_desl=(1920-altura*32)/2;
            y_desl=(1080-largura*32)/2;
            for(int i=0;i<altura;i++)
                for(int j=0;j<largura;j++){
                    C2D_DesenhaSpriteEspecial(tileset, mapa[j][largura-1-i]-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);
                    if(debug && geometria[j][largura-1-i])
                        C2D_DesenhaSpriteEspecial(tileset, geometria[j][largura-1-i]+marca-1, x_desl+32*j, y_desl+32*i, C2D_FLIP_NENHUM, 1.0, 1.0, angulo);

                }
        }
    }
}

bool Fase::procuraPrimeiraMarca(const int marca, int *x, int *y)
{
    bool achou=false;
    for(int i=0;i<altura && !achou;i++)
        for(int j=0;j<largura && !achou;j++)
            if(geometria[i][j]==marca){
                *x=j*32;
                *y=i*32;
                achou=true;
            }
    if(!achou)
        return false;
    return true;
}
