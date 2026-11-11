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

int main(void) {

    InitWindow(800, 600, "Space Traveler");

    Texture2D texturaNave = LoadTexture("assets/nave.png");
    Texture2D texturaTiro = LoadTexture("assets/tiro.png");

    SetTargetFPS(60);
    
    Jogador *primeiro = malloc(sizeof(Jogador));

    
    
    if (primeiro == NULL) {

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);

        CloseWindow();

        return 1;
    }
    
    inicializarJogador(primeiro);

    TiroJogador *bullet = NULL;
    
    

    while (!WindowShouldClose()) {

        movimentacaoJogador(primeiro);

        bullet = adicionarTiro(bullet, primeiro);

        bullet = movimentacaoTiro(bullet);

        BeginDrawing();

        ClearBackground(BLACK);

        desenharNave(primeiro, texturaNave);

        desenharTiro(bullet, texturaTiro);

        DrawText("SPACE TRAVELER", 250, 250, 30, WHITE);

        EndDrawing();
    }

    UnloadTexture(texturaNave);//meio que libera a textura
    UnloadTexture(texturaTiro);

    CloseWindow();

    while (bullet != NULL) {

        TiroJogador *auxiliar = bullet;

        bullet = bullet->prox;

        free(auxiliar);
    }
    
    free(primeiro);

    return 0;
} 