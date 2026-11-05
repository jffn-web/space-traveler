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


void desenharNave(struct Jogador *um, Texture2D textura){


    DrawTexture(textura, um->x, um->y, WHITE);//pega o comando de textura e bota como o rectangles

}


int main(void) {

    InitWindow(800, 600, "Space Traveler");

    Texture2D texturaNave = LoadTexture("assets/nave.png");

    SetTargetFPS(60);
    
    Jogador *primeiro = malloc(sizeof(Jogador));
    
    if (primeiro == NULL) {

        UnloadTexture(texturaNave);

        CloseWindow();

        return 1;
    }
    
    inicializarJogador(primeiro);
    
    

    while (!WindowShouldClose()) {

        movimentacaoJogador(primeiro);

        BeginDrawing();
        ClearBackground(BLACK);

        desenharNave(primeiro, texturaNave);

        DrawText("SPACE TRAVELER", 250, 250, 30, WHITE);

        EndDrawing();
    }

    UnloadTexture(texturaNave);

    CloseWindow();
    
    free(primeiro);

    return 0;
}