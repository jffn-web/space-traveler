#include "raylib.h"
#include <stdlib.h>

typedef struct Jogador{
    
    float largura;
    float altura;
    
    float velocidade;
    int vidas;
    
    float x;
    float y;
    
}Jogador;

typedef struct TiroJogador{
    
    float largura;
    float altura;
    
    float velocidade;
    
    float x;
    float y;
    
    
    struct TiroJogador *prox;
    
}TiroJogador;

typedef struct Inimigo {

    float largura;
    float altura;

    float velocidade;

    float x;
    float y;

    int vivo;

    struct Inimigo *prox;

} Inimigo;

Inimigo* inicializarInimigo(Inimigo *naveInimiga) {

    Inimigo *anterior = naveInimiga;

    int const constante = 50;

    

    for(int i = 0; i < 55; i++){


        Inimigo *novo = malloc(sizeof(Inimigo));

            if (novo == NULL) {

            Inimigo *auxiliar = naveInimiga;

            while (auxiliar != NULL) {

                Inimigo *proximo = auxiliar->prox;

                free(auxiliar);

                auxiliar = proximo;
            }

            return NULL;
        }

        if(i > -1 && i < 11){
            if(i == 0){

                naveInimiga = novo;
                novo->prox = NULL;//quando é o primeiro ele aponta pra null e faz a nave inimiga apontar pra o comeco da lista

                anterior = novo;// e o anterior vai ser o primeiro pq ainda nao tem anterior


                novo->largura = 45;
                novo->altura = 45;

                novo->y = 80;
                novo->x = 130;//começa com esse e vai aumentando;

                novo->vivo = 1;

                novo->velocidade = 50;

            }else{


                anterior->prox = novo;//ex: se for o segundo, o anterior ta apontado pro começo da lista, o prox passa apontar pra o segundo

                novo->y = 80;
                novo->x = anterior->x + constante;//sempre soma 50//define o x enquanto anterior ainda é o primeiro
        
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;//anterior vira o novo pra quando criarem outro novo ele virar o anterior novamente

                novo->prox = NULL; //como to adicionando no final da lista cada novo, todo prox do novo vai ser NULL;

                
            }
        }

        if(i > 10 && i < 22){

            if (i == 11){
                
                anterior->prox = novo;

                novo->y = 130;
                novo->x = 130;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;

            }else{        


                anterior->prox = novo;

                novo->y = 130;
                novo->x = anterior->x + constante;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;
            }


        }

        if(i > 21 && i < 33){


           if (i == 22){
                
                anterior->prox = novo;

                novo->y = 180;
                novo->x = 130;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;

            }else{        


                anterior->prox = novo;

                novo->y = 180;
                novo->x = anterior->x + constante;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;
            }



        }

        if(i > 32 && i < 44){


            if (i == 33){
                
                anterior->prox = novo;

                novo->y = 230;
                novo->x = 130;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;

            }else{        


                anterior->prox = novo;

                novo->y = 230;
                novo->x = anterior->x + constante;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;
            }



        }

        if(i > 43 && i < 55){


            if (i == 44){
                
                anterior->prox = novo;

                novo->y = 280;
                novo->x = 130;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;

            }else{        


                anterior->prox = novo;

                novo->y = 280;
                novo->x = anterior->x + constante;
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                anterior = novo;

                novo->prox = NULL;;
            }



        }


    }

    return naveInimiga;

}

TiroJogador *criarTiro(struct Jogador *um) {

    TiroJogador *novo = malloc(sizeof(TiroJogador));

    if (novo == NULL) {
        return NULL;
    }

    novo->largura = 6;
    novo->altura = 16;
    
    novo->velocidade = 550;
    
    novo->x = um->x + (um->largura - novo->largura) / 2; // Centraliza o tiro na nave
    novo->y = um->y - novo->altura; // Faz o tiro nascer acima da nave


    novo->prox = NULL;

    return novo;
}

TiroJogador *adicionarTiro(TiroJogador *bullet, Jogador *um) {

    static float tempoDisparo = 0.20f;

    tempoDisparo += GetFrameTime();

    int contador = 0;

    TiroJogador *cont = bullet;

    while (cont != NULL) {

        contador++;

        cont = cont->prox;
    }

    if (IsKeyDown(KEY_SPACE) && tempoDisparo >= 0.20f && contador < 2) {

        TiroJogador *novo = criarTiro(um);

        if (novo == NULL) {
            return bullet;
        }

        if (contador == 0) {

            bullet = novo; // Primeiro tiro vira o início da lista

        } else if (contador == 1) {

            bullet->prox = novo; // O único tiro existente aponta para o novo
        }

        tempoDisparo = 0.0f;
    }

    return bullet;
}




TiroJogador *movimentacaoTiro(TiroJogador *bullet) {

    TiroJogador *auxiliar = bullet;
    TiroJogador *anterior = NULL;

    float deslocamento;

    while (auxiliar != NULL) {

        deslocamento = auxiliar->velocidade * GetFrameTime();//move o tiro pra cima

        auxiliar->y = auxiliar->y - deslocamento;//move o tiro pra cima

        if (auxiliar->y + auxiliar->altura < 0) {

            if (auxiliar == bullet) {//se for o do começo

                bullet = auxiliar->prox;//pq mesmo se so tiver dado um tiro ele vai retornar NULL pq prox vai ser null, e se nao for prox vai ser o tiro 2 que vira o tiro 1

                free(auxiliar);

                auxiliar = bullet;

            } else {

                anterior->prox = auxiliar->prox;// ja que o primeiro tiro ja ocorreu o anterior apontava pro auxiliar antigo, aqui ele ta fazendo o anterior prox apontar pra null

                free(auxiliar);

                auxiliar = anterior->prox;//termina o while pq ele vira null
            }

        } else {

            anterior = auxiliar;// esse else sempre acontece pq o segundo tiro nao pdoe sair sem o priemiro, entao anterior vai apontar pra auxiliar

            auxiliar = auxiliar->prox;// e auxiliar para o proximo
        }
    }

    return bullet;//retorna ou null ou o inicio da lista
}

void inicializarJogador(struct Jogador *um){
    
    um->largura = 64;
    um->altura = 64;
    
    um->velocidade = 300;
    um->vidas = 3;
    
    um->x = 368;
    um->y = 520;
}

void movimentacaoJogador(struct Jogador *um){
    
    float deslocamento;
    
    if(IsKeyDown(KEY_D)){
        
        deslocamento = um->velocidade * GetFrameTime();
        
        um->x = deslocamento + um->x;
        
        if(um->x > 800 - um->largura){//troquei por que provavelmente vou mudar o tamanho da nave
            
            um->x = 800 - um->largura;//limite - largura do jogador
            
            //É PQ O LADO ESQUERDO TEM QUE MANTER A MARGEM
        }
        
    }else if(IsKeyDown(KEY_A)){
        
        deslocamento = -(um->velocidade) * GetFrameTime();
        
        um->x = deslocamento + um->x;
        
        if(um->x < 0){
            
            um->x = 0;
            
            
        }
        
    }
    
    // ELE FAZ ATUALIZAÇÕES EM RELACÃO AO FPS DO JOGO;
}


void desenharNave(struct Jogador *um, Texture2D textura){//Solon 


    DrawTexture(textura, um->x, um->y, WHITE);

}

void desenharTiro(TiroJogador *bullet, Texture2D texturaTiro) {//Solon

    TiroJogador *auxiliar = bullet;

    while (auxiliar != NULL) {

        Rectangle origem = {0, 0, texturaTiro.width, texturaTiro.height};

        Rectangle destino = {auxiliar->x, auxiliar->y, auxiliar->largura, auxiliar->altura};

        DrawTexturePro(texturaTiro, origem, destino, (Vector2){0, 0}, 0, WHITE);

        auxiliar = auxiliar->prox;
    }
}

void desenharInimigos(Inimigo *naveInimiga, Texture2D texturaInimigo) {//solon

    Inimigo *auxiliar = naveInimiga;

    while (auxiliar != NULL) {

        if (auxiliar->vivo == 1) {

            Rectangle origem = {0, 0, texturaInimigo.width, texturaInimigo.height};

            Rectangle destino = {
                auxiliar->x, auxiliar->y, auxiliar->largura, auxiliar->altura};

            DrawTexturePro(texturaInimigo, origem, destino, (Vector2){0, 0}, 0, WHITE);
        }

        auxiliar = auxiliar->prox;
    }
}

int main(void) {

    InitWindow(800, 600, "Space Traveler");

    Texture2D texturaNave = LoadTexture("assets/nave.png");
    Texture2D texturaTiro = LoadTexture("assets/tiro.png");
    Texture2D texturaInimigo = LoadTexture("assets/inimigo1.png");

    SetTargetFPS(60);
    
    Jogador *primeiro = malloc(sizeof(Jogador));

    
    
    if (primeiro == NULL) {

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);

        CloseWindow();

        return 1;
    }
    
    inicializarJogador(primeiro);

    TiroJogador *bullet = NULL;

    Inimigo *naveInimiga = NULL;

    naveInimiga = inicializarInimigo(naveInimiga);

    if (naveInimiga == NULL) {

        free(primeiro);

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);

        CloseWindow();

        return 1;
    }
    
    
    

    while (!WindowShouldClose()) {

        movimentacaoJogador(primeiro);

        bullet = adicionarTiro(bullet, primeiro);

        bullet = movimentacaoTiro(bullet);

        BeginDrawing();

        ClearBackground(BLACK);

        desenharNave(primeiro, texturaNave);

        desenharInimigos(naveInimiga, texturaInimigo);

        desenharTiro(bullet, texturaTiro);

        EndDrawing();
    }

    UnloadTexture(texturaNave);//libera a textura
    UnloadTexture(texturaTiro);
    UnloadTexture(texturaInimigo);

    CloseWindow();

    while (bullet != NULL) {

        TiroJogador *auxiliar = bullet;

        bullet = bullet->prox;

        free(auxiliar);
    }

    while (naveInimiga != NULL) {

    Inimigo *auxiliar = naveInimiga;

    naveInimiga = naveInimiga->prox;

    free(auxiliar);

    }
    
    free(primeiro);

    return 0;
} 