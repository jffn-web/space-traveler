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

    int pontos;

    struct Inimigo *prox;

} Inimigo;

typedef struct DeathStar{
    
    float largura;
    float altura;

    float velocidade;

    float x;
    float y;

    int pontos;
    int vivo;

    float tempoSpawn;
    
} DeathStar;


void inicializarEstrelaDaMorte(DeathStar *estrela) {
    
    if (estrela == NULL) {
        return;
    }
        
    estrela->largura = 160;
    estrela->altura = 80;
        
    estrela->velocidade = 150;
        
    estrela->y = 35;
    estrela->x = -60;
        
    estrela->pontos = 100;
    estrela->vivo = 0;

    estrela->tempoSpawn = 0.0f;

}


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

                novo->pontos = 30;

            }else{


                anterior->prox = novo;//ex: se for o segundo, o anterior ta apontado pro começo da lista, o prox passa apontar pra o segundo

                novo->y = 80;
                novo->x = anterior->x + constante;//sempre soma 50//define o x enquanto anterior ainda é o primeiro
        
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                novo->pontos = 30;


                anterior = novo;//anterior vira o novo pra quando criarem outro novo ele virar o anterior novamente

                novo->prox = NULL; //como to adicionando no final da lista cada novo, todo prox do novo vai ser NULL;

                
            }
        }

        if(i > 10 && i < 22){

            if (i == 11){
                
                anterior->prox = novo;

                novo->y = 130;
                novo->x = 130;//toda vez tenho que iniciar o primeiro da filieira pra declarar o x e usar ele pra somar a constante
            
                novo->vivo = 1;
                novo->velocidade = 50;

                novo->altura = 45;
                novo->largura = 45;

                novo->pontos = 20;


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

                novo->pontos = 20;

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

                novo->pontos = 20;

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

                novo->pontos = 20;

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

                novo->pontos = 10;

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

                novo->pontos = 10;

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

                novo->pontos = 10;

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

                novo->pontos = 10;

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

TiroJogador *adicionarTiro(TiroJogador *bullet, Jogador *um,int *deathContador) {////////

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

        (*deathContador)++;
    }

    

    return bullet;
}

void movimentacaoEstrelaDaMorte(DeathStar *estrela, int *deathContador) {

    float deslocamento;

    static int direcao;//se vai pra esquerda ou pra direita

    if (estrela->vivo == 0) {

        estrela->tempoSpawn += GetFrameTime();

        if (estrela->tempoSpawn >= 15.0f) {//espera 15 segundo pra spawnar

            estrela->vivo = 1;//quando passa 15 segundos, a estrela muda o estado para vivo
            estrela->tempoSpawn = 0.0f;// e o contador volta

            if(*deathContador % 2 == 0){

                estrela->x = -160;// se a quantidade de tiros for par ele vai da esquerda pra direita

                direcao = 1;
            }else{

                estrela->x = 960;// se for impar da direita pra esquerda, feature do space invaders

                direcao = -1;
            }
        }

    } else {

        //ele vai checar antes se e par ou impar pra deicidir se e esquerda ou direita
        if(direcao == 1){

            deslocamento = estrela->velocidade * GetFrameTime();

            estrela->x += deslocamento;

            if(estrela->x >= 800 + estrela->largura){//pra quando ele sumir totalmente da tela

                estrela->vivo = 0;//se ele sair da tela ele vai ficar desativado por um tempo
            }
        }

        if(direcao == -1){

            deslocamento = estrela->velocidade * GetFrameTime();

            estrela->x -= deslocamento;//mesma logica so que pra esquerda

            if(estrela->x <= 0 - estrela->largura){//pra quando ele sumir totalmente da tela

                estrela->vivo = 0;
            }
        }
        

    }
}

Inimigo *movimentacaoInimigos(Inimigo *stromtrooper, Jogador *um) {
        
    static float tempoMovimento = 0.0f;
    static int direcao = 1;
        
    tempoMovimento += GetFrameTime();
        
    Inimigo *auxiliar = stromtrooper;
        
        
    if (um->vidas != 0) {
        
        if (tempoMovimento >= 0.80f) {// a cada 0.80
            
            if (direcao == 1) {
                
                for (int i = 0; i < 55; i++) {
                    
                    if (auxiliar->x >= 750) {//checa se bateu antes de mover pra ele nao atravessar a parede q q é 800 mas tem que contar com a largura dele
                        
                        direcao = -1;//faz ele ir pra esquerda na proxima chamada da funcao
                        
                        auxiliar = stromtrooper;//auxiliar aponta pro comeco da lista
                        
                        for (int j = 0; j < 55; j++) {//se o if acontecer ele vai em todos os inimigos vivos e desce 20 (aumenta o y)
                            
                            auxiliar->y += 20;//quando bate na parede todos descem
                            
                            if (auxiliar->y + auxiliar->altura >= um->y) {//se chegar na altura da nave é gameover
                                um->vidas = 0;
                            }
                            
                            auxiliar = auxiliar->prox;//percorre todos
                        }
                        
                        tempoMovimento = 0.0f;//reseta o tempo de movimento
                        
                        return stromtrooper;//retorna a posição que vao vificar
                    }
                    
                    auxiliar = auxiliar->prox;//checa um por um pra ver se bateu na direita
                }
            }
            
            
            auxiliar = stromtrooper;//se nao tiver retornado nada o auxiliar volta pra o comeco
            
            
            if (direcao == -1) {//direcao pra esquerda
                
                for (int i = 0; i < 55; i++) {
                    
                    if (auxiliar->x <= 0) {//checa se bateu antes de mover pra ele nao atravessar a parede q é 0 so que no railib nao precisa diminuir da largura se so for pra direita
                        
                        direcao = 1;//se bateu troca pra direita
                        
                        auxiliar = stromtrooper;//faz auxiliar voltar pro comeco pra aumentar o y de todos
                        
                        for (int j = 0; j < 55; j++) {
                            
                            auxiliar->y += 20;//aumenta o y
                            
                            if (auxiliar->y + auxiliar->altura >= um->y) {
                                um->vidas = 0;//verifica se chegou na area da nave
                            }
                            
                            auxiliar = auxiliar->prox;//percorre todos da lista
                        }
                        
                        tempoMovimento = 0.0f;//o "relogio" reseta
                        
                        return stromtrooper;//retorna a nova posicao
                    }
                    
                    auxiliar = auxiliar->prox;//percorre todos pra ver se, se enquadra no if de bater na parede
                }
            }
            
            
            auxiliar = stromtrooper;//volta auxiliar pro comeco
            
            
            for (int i = 0; i < 55; i++) {//se retornou nada é pq nao bateu na parede
                
                if (direcao == 1) {
                    auxiliar->x += 10;// move pra direita
                }
                
                if (direcao == -1) {
                    auxiliar->x -= 10;//move pra esquerda
                }
                
                auxiliar = auxiliar->prox;//percorre todos
            }
            
            tempoMovimento = 0.0f;//resta o timer
        }
    }
        
    return stromtrooper;//retorna a nova posição
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

    Inimigo *auxiliar = naveInimiga;//aponta pra o começo da lista

    while (auxiliar != NULL) {//vai percorrer a lista toda

        if (auxiliar->vivo == 1) {//se o inimigo tiver vivo

            Rectangle origem = {0, 0, texturaInimigo.width, texturaInimigo.height};//significa que voce quer a imagem inteira começando do pixel 0x0

            Rectangle destino = {auxiliar->x, auxiliar->y, auxiliar->largura, auxiliar->altura};//adequa a imagem para os parametros ja definidos

            DrawTexturePro(texturaInimigo, origem, destino, (Vector2){0, 0}, 0, WHITE);//pega a textura, a imagem, a localizaçao e o tamanho da imagem, e vector é ponto de referencia
        }

        auxiliar = auxiliar->prox;//move o ponteiro
    }
}

void desenharEstrelaDaMorte(DeathStar *estrela, Texture2D texturaDeath_Star) {

    if (estrela == NULL || estrela->vivo == 0) {//se ainda nao tiver no tempo da estrela ela nao é desenhada
        return;
    }

    Rectangle origem = {0, 0, texturaDeath_Star.width, texturaDeath_Star.height};

    Rectangle destino = {estrela->x, estrela->y, estrela->largura, estrela->altura};

    DrawTexturePro(texturaDeath_Star, origem, destino, (Vector2){0, 0}, 0, WHITE);
}

int main(void) {

    InitWindow(800, 600, "Space Traveler");

    Texture2D texturaNave = LoadTexture("assets/nave.png");
    Texture2D texturaTiro = LoadTexture("assets/tiro.png");
    Texture2D texturaInimigo = LoadTexture("assets/inimigo1.png");
    Texture2D texturaDeath_Star = LoadTexture("assets/Death_Star.png");

    SetTargetFPS(60);
    
    Jogador *primeiro = malloc(sizeof(Jogador));

    
    
    if (primeiro == NULL) {//nao tem free primeiro pq ele ja é null entao nao tem oq liberar

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);
        UnloadTexture(texturaDeath_Star);

        CloseWindow();

        return 1;
    }
    
    inicializarJogador(primeiro);

    TiroJogador *bullet = NULL;

    Inimigo *naveInimiga = NULL;

    int deathContador1 = 0;

    naveInimiga = inicializarInimigo(naveInimiga);

    if (naveInimiga == NULL) {//aqui tem free primeiro pq ele ja funcionou mas nao tem free nave pq a função ja libera

        free(primeiro);

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);
        UnloadTexture(texturaDeath_Star);

        CloseWindow();

        return 1;
    }

    DeathStar *estrela = malloc(sizeof(DeathStar));

    if (estrela == NULL) {//aqui tem q ter os dois free menos o da estrela que ja é null

        free(primeiro);

        while (naveInimiga != NULL) {

            Inimigo *auxiliar = naveInimiga;

            naveInimiga = naveInimiga->prox;

            free(auxiliar);
        }

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);
        UnloadTexture(texturaDeath_Star);

        CloseWindow();

        return 1;
    }

    inicializarEstrelaDaMorte(estrela);
    
    
    

    while (!WindowShouldClose()) {

        movimentacaoJogador(primeiro);

        bullet = adicionarTiro(bullet, primeiro, &deathContador1);

        bullet = movimentacaoTiro(bullet);

        naveInimiga = movimentacaoInimigos(naveInimiga, primeiro);

        movimentacaoEstrelaDaMorte(estrela, &deathContador1);

        BeginDrawing();

        ClearBackground(BLACK);

        desenharNave(primeiro, texturaNave);

        desenharInimigos(naveInimiga, texturaInimigo);

        desenharEstrelaDaMorte(estrela, texturaDeath_Star);

        desenharTiro(bullet, texturaTiro);

        EndDrawing();
    }

    UnloadTexture(texturaNave);//libera a textura
    UnloadTexture(texturaTiro);
    UnloadTexture(texturaInimigo);
    UnloadTexture(texturaDeath_Star);

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

    
    free(estrela);
    free(primeiro);

    return 0;
} 