#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>



typedef struct Jogador{
    
    float largura;
    float altura;
    
    float velocidade;
    int vidas;
    
    float x;
    float y;

    int pontos;
    float tempoDano;

    char nome[30];
    
}Jogador;

typedef struct Log {

    char nome[30];
    int pontos;

} Log;

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

typedef struct TiroInimigo{
    
    float largura;
    float altura;
    
    float velocidade;
    
    float x;
    float y;
    
    
    struct TiroInimigo *prox;
    
}TiroInimigo;

typedef struct Escudo {

    int quadrado[5][8];
    float x;
    float y;

} Escudo;

void inicializarEscudos(Escudo escudos[4]) {

    int formacao[5][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0}
    };//define aonde vai desenhar e onde nao

    int h = 0;//comeca o contador pra mover o while

    float constanteX = 64;

    while (h < 4) {

        escudos[h].x = constanteX;//onde o escudo comeca
        escudos[h].y = 400;

        for (int i = 0; i < 5; i++) {//linha

            for (int j = 0; j < 8; j++) {//coluna

                escudos[h].quadrado[i][j] = formacao[i][j];//iguala de cada item da lista que vai servir para desenhar e colisao
                //vai ser a posicao de cada quadrado
            }

        }

        h++;//troca de vetor de escudo

        constanteX += 176;//move o x pra os escudos nao ficarem um em cima do outro
    }
}

void desenharEscudos(Escudo escudos[4], Texture2D texturaEscudo){

    int h = 0;//comeca o contador
    
    while(h < 4){

        for(int i = 0; i < 5; i++){

            for(int j = 0;j < 8; j++){

                if (escudos[h].quadrado[i][j] == 1) {

                    Rectangle origem = {0, 0, texturaEscudo.width, texturaEscudo.height};//pega tudo da imagem,

                    Rectangle destino = {escudos[h].x + j * 16, escudos[h].y + i * 16, 16, 16};//16 pq é a largura e altura do quadrado
                    //pega o x de cada escudo, e fica adicionando ao lado e abaixo, multiplica com j, e y pra nao ficar colado

                    DrawTexturePro(texturaEscudo, origem, destino, (Vector2){0, 0}, 0, WHITE);
                }
            }

        }

        h++;//troca de vetor de escudo
    }

}

TiroInimigo *colisaoTiroDoInimigoNosEscudos(TiroInimigo *laser, Escudo escudos[4]) {

    TiroInimigo *auxiliarTiroInimigo = laser;
    TiroInimigo *anterior = NULL;

    int h = 0;

    while (auxiliarTiroInimigo != NULL) {//percorre os tiros

        Rectangle hitboxTiroInimigo = {auxiliarTiroInimigo->x, auxiliarTiroInimigo->y, auxiliarTiroInimigo->largura, auxiliarTiroInimigo->altura};
        //hitbox do tiro
        while(h < 4){//percorre o escudo

            for (int i = 0; i < 5; i++){//ja que os tiros vem de cima pra baixo prioriza os de cima

                for(int j = 0; j < 8; j++) {//colunas

                    if (escudos[h].quadrado[i][j] == 1){//se o quadrado tiver vivo

                        Rectangle hitboxQuadrado = {escudos[h].x + j * 16, escudos[h].y + i * 16, 16, 16};//hitbox quadrado

                        if (CheckCollisionRecs(hitboxQuadrado, hitboxTiroInimigo)) {//se acertou

                            escudos[h].quadrado[i][j] = 0;//o quadrado morre

                            if(auxiliarTiroInimigo == laser) {//libera o tiro inimigo

                                laser = auxiliarTiroInimigo->prox;

                                free(auxiliarTiroInimigo);
                            } else{

                                anterior->prox = auxiliarTiroInimigo->prox;

                                free(auxiliarTiroInimigo);
                            }

                            return laser;
                        }
                    }
                }
            }

            h++;//perocrre escudo
        }

        h = 0;

        anterior = auxiliarTiroInimigo;//percorre tiro inimigo
        auxiliarTiroInimigo = auxiliarTiroInimigo->prox;
    }

    return laser;
}

TiroJogador *colisaoTiroDoJogadorNosEscudos(TiroJogador *bullet, Escudo escudos[4]) {

    TiroJogador *auxiliarTiro = bullet;
    TiroJogador *anterior = NULL;

    int h = 0;

    while (auxiliarTiro != NULL) {// percorre os tiros do jogador

        Rectangle hitboxTiro = {auxiliarTiro->x, auxiliarTiro->y, auxiliarTiro->largura, auxiliarTiro->altura};

        while(h < 4) {//percorre os 4 escudos

            for(int i = 4; i >= 0; i--){//prioriza os quadrados inferiores, as linhas

                for(int j = 0; j < 8; j++) {//percorre as colunas da esquerda pra direita

                    if (escudos[h].quadrado[i][j] == 1){//se o bloco ainda existir != 0

                        Rectangle hitboxQuadrado = {escudos[h].x + j * 16, escudos[h].y + i * 16, 16, 16};//pega o hitbox do bloco

                        if (CheckCollisionRecs(hitboxTiro, hitboxQuadrado)){//checa colisao

                            escudos[h].quadrado[i][j] = 0;//se colidiu, ele morre

                            if(auxiliarTiro == bullet) {//da free na bala que atinjiu

                                bullet = auxiliarTiro->prox;

                                free(auxiliarTiro);

                            } else {

                                anterior->prox = auxiliarTiro->prox;

                                free(auxiliarTiro);

                            }

                            return bullet;

                        }
                    }
                }
            }

            h++;//percorre o while dos escudos
        }

        h = 0;//percorre o while dos escudos

        anterior = auxiliarTiro;//percorre as balas
        auxiliarTiro = auxiliarTiro->prox;//percorre as balas
    }

    return bullet;
}


TiroInimigo *criarTiroInimigo(Inimigo *naveInimiga){

    TiroInimigo *novo = malloc(sizeof(TiroInimigo));

    if (novo == NULL) {
        return NULL;
    }

    novo->largura = 6;
    novo->altura = 16;
    
    novo->velocidade = 535;
    
    novo->x = naveInimiga->x + (naveInimiga->largura - novo->largura) / 2; //Centraliza o tiro na nave
    novo->y = naveInimiga->y + naveInimiga->altura; //Faz o tiro nascer abaixo da nave inimiga pq ele nasce depois da altura e o y dele


    novo->prox = NULL;

    return novo;

}

Inimigo *escolherStormTrooper(Inimigo *naveInimiga){

    int contador = 0;//contador

    Inimigo *cont = naveInimiga;//ponteiro auxiliar para a conta

    while(cont != NULL){//conta quantos inimigos estao vivos

        if(cont->vivo == 1){//se tiver vivo incrementa 1

            contador++;
        }

        cont = cont->prox;
    }

    if (contador == 0) {
    return NULL;//se os inimigos estiverem mortos, trocanddo pra reviver
    }

    int randomizer = GetRandomValue(1, contador);//funcao de randomizar de 1 ate o valor de inimigos vivos, que é o contador

    int contador2 = 0;//cria outro contador pra usar na selecao do inimigo

    Inimigo *auxiliar = naveInimiga;//cria um ponteiro auxilair

    while(auxiliar != NULL){


        if(auxiliar->vivo == 1){//se tiver vivo o contador2 incrementa

            contador2++;

            if(contador2 == randomizer){//se tiver vivo e o contador 2 chegar ate o randomizer ele retorna o inimigo que vai atirar


                return auxiliar;
            }
        }

        auxiliar = auxiliar->prox;//move o ponteiro
    }

     return NULL;//se nao retornar o aleatorio ele retorna null

}

TiroInimigo *adicionarTiroInimigo(TiroInimigo *laser, Inimigo *naveInimiga){

    static float tempoDisparo = 0.00f;// é static pq acumula, a cada frame ele chama a funcao
    //se nao fosse ela ia voltar pra 0 sempre

    tempoDisparo += GetFrameTime();//espera o tempo de desparo pra atirar de novo, adicionar um tiro


    if (tempoDisparo >= 0.80f) {

        Inimigo *storm = escolherStormTrooper(naveInimiga);//pega a lista da nave pra ver qual ta vivo e retorna o aleatorio pra atirar

        if (storm == NULL) { //se nao tiver atualizando todos os inimigos mortos
            return laser;
        }

        TiroInimigo *novo = criarTiroInimigo(storm);//cria os atributos do tiro no aleatorio

        if (novo == NULL) { //verfica se alocou certo
            return laser;
        }

        novo->prox = laser;//o ponteiro antigo vai ser apontado pelo prox do novo
        laser = novo;//e laser vai virar o novo, novo, assim o ciclo se repete

        tempoDisparo = 0.0f;//zera pra proxima chamada
    }

    

    return laser;//retorna a nova lista

}

TiroInimigo *movimentacaoTiroInimigo(TiroInimigo *laser){

    TiroInimigo *auxiliar = laser;
    TiroInimigo *anterior = NULL;

    float deslocamento;

    while (auxiliar != NULL) {//percorrer

        deslocamento = auxiliar->velocidade * GetFrameTime();//move o tiro pra baixo

        auxiliar->y = auxiliar->y + deslocamento;//move o tiro pra baixo mesma logica do tiro jogador

        if (auxiliar->y >= 600) {//verifica se o tiro saiu da tela

            if (auxiliar == laser) {//se auxiliar for = ao inicio

                laser = auxiliar->prox;//se tiver dado um tiro o inicio vai ser null pelo aux->prox ser null, e se for o primeiro tiro o 2 vira o primeiro
                free(auxiliar);//libera o que saiu da tela

                auxiliar = laser;//e o auxiliar volta a ser o primeiro

            } else {

                anterior->prox = auxiliar->prox;// ja que o primeiro tiro ja ocorreu o anterior apontava pra o 1, aqui ele ta fazendo o anterior prox apontar pra null ou proximo tiro

                free(auxiliar);//libera o do meio/ou ultimo 

                auxiliar = anterior->prox;//se for o ultimo, o anteiror prox aponta pra null, e termina o while com aux null
                
            }

        } else {

            anterior = auxiliar;//se o tiro atual continua na tela, o anterior vira auxi, 

            auxiliar = auxiliar->prox;// e auxiliar para o proximo, se for null ele encerra o while tambem
        }
    }

    return laser;//retorna ou null ou o inicio da lista
}

TiroInimigo *colisaoTiroDOInimigo(TiroInimigo *laser, Jogador *um){

    TiroInimigo *auxiliarTiroInimigo = laser;//pra mover em bullet
    
    TiroInimigo *anterior = NULL;
    
        
    while(auxiliarTiroInimigo != NULL){
            
        Rectangle hitboxTiroInimigo = {auxiliarTiroInimigo->x, auxiliarTiroInimigo->y ,auxiliarTiroInimigo->largura ,auxiliarTiroInimigo->altura};//declara o hitbox onde o tiro ta e qual a area
        
        Rectangle hitboxJogador = {um->x, um->y ,um->largura , um->altura};
        
            
        if(CheckCollisionRecs(hitboxJogador, hitboxTiroInimigo)){//se bateu no jogador
            if (auxiliarTiroInimigo == laser) {//se auxiliar for = ao inicio
                
                if (um->vidas > 0) {
                    
                    um->vidas--;
                    
                }
                            
                laser = auxiliarTiroInimigo->prox;//se tiver dado um tiro o inicio vai ser null pelo aux->prox ser null, e se for o primeiro tiro o 2 vira o primeiro
                free(auxiliarTiroInimigo);//libera o que saiu da tela

                auxiliarTiroInimigo = laser;//e o auxiliar volta a ser o primeiro e se for nul encerra o while

            } else {//ele vai estar no segundo tiro da lista
                
                if (um->vidas > 0) {
                    
                    um->vidas--;
                    
                }
                

                anterior->prox = auxiliarTiroInimigo->prox;// ja que o primeiro tiro ja ocorreu o anterior apontava pra o 1, aqui ele ta fazendo o anterior-prox apontar pra null ou proximo tiro

                free(auxiliarTiroInimigo);//libera o do meio/ou ultimo 

                auxiliarTiroInimigo = anterior->prox;//se for o ultimo, o anteiror prox aponta pra null, e termina o while com aux null
                
            }

        } else {

            anterior = auxiliarTiroInimigo;//se o tiro atual continua na tela, o anterior vira auxi, 

            auxiliarTiroInimigo = auxiliarTiroInimigo->prox;// e auxiliar para o proximo, se for null ele encerra o while tambem
        }
            
    }
        
    return laser;//retorna a lista
        
}
        


void desenharTiroInimigo(TiroInimigo *laser, Texture2D texturaTiroInimigo) {

    TiroInimigo *auxiliar = laser;

    while (auxiliar != NULL) {//faz um while pra percorrer todos os tiros e desenhar

        Rectangle origem = {0, 0, texturaTiroInimigo.width, texturaTiroInimigo.height};

        Rectangle destino = {auxiliar->x, auxiliar->y, auxiliar->largura, auxiliar->altura};

        DrawTexturePro(texturaTiroInimigo, origem, destino, (Vector2){0, 0}, 0, WHITE);

        auxiliar = auxiliar->prox;
    }
}

Inimigo* inicializarInimigo(Inimigo *naveInimiga) {

    Inimigo *anterior = naveInimiga;

    int const constante = 50;

    

    for(int i = 0; i < 55; i++){


        Inimigo *novo = malloc(sizeof(Inimigo));

            if (novo == NULL) {

            Inimigo *auxiliar = naveInimiga;//se falhar a alocacao

            while (auxiliar != NULL) {

                Inimigo *proximo = auxiliar->prox;//tem que percorrer todos os inimigosque foram feitos 

                free(auxiliar);

                auxiliar = proximo;
            }

            return NULL;//e vai retornar null
        }

        if(i > -1 && i < 11){
            if(i == 0){

                naveInimiga = novo;
                novo->prox = NULL;//quando é o primeiro ele aponta pra null e faz a nave inimiga apontar pra o comeco da lista

                anterior = novo;//e o anterior vai ser o primeiro pq ainda nao tem anterior


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

        if (i > 43 && i < 55){


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

Inimigo *movimentacaoInimigos(Inimigo *stromtrooper, Jogador *um) {
        
    static float tempoMovimento = 0.0f;
    static int direcao = 1;
        
    tempoMovimento += GetFrameTime();
        
    Inimigo *auxiliar = stromtrooper;
        
        
    if (um->vidas != 0) {
        
        if (tempoMovimento >= 0.80f){// a cada 0.80
            
            if (direcao == 1) {
                
                for (int i = 0; i < 55; i++) {
                    
                    if (auxiliar->vivo == 1 && auxiliar->x >= 750) {//checa se bateu antes de mover pra ele nao atravessar a parede q q é 800 mas tem que contar com a largura dele
                        
                        direcao = -1;//faz ele ir pra esquerda na proxima chamada da funcao
                        
                        auxiliar = stromtrooper;//auxiliar aponta pro comeco da lista
                        
                        for (int j = 0; j < 55; j++) {//se o if acontecer ele vai em todos os inimigos vivos e desce 20 (aumenta o y)
                            
                            auxiliar->y += 20;//quando bate na parede todos descem
                            
                            if (auxiliar->vivo == 1 && auxiliar->y + auxiliar->altura >= um->y) {//se chegar na altura da nave do jogador é gameover
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
                    
                    if (auxiliar->vivo == 1 && auxiliar->x <= 0) {//checa se bateu antes de mover pra ele nao atravessar a parede q é 0 so que no railib nao precisa diminuir da largura se so for pra direita
                        
                        direcao = 1;//se bateu troca pra direita
                        
                        auxiliar = stromtrooper;//faz auxiliar voltar pro comeco pra aumentar o y de todos
                        
                        for (int j = 0; j < 55; j++) {
                            
                            auxiliar->y += 20;//aumenta o y
                            
                            if (auxiliar->vivo == 1 && auxiliar->y + auxiliar->altura >= um->y) {
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

void desenharInimigos(Inimigo *naveInimiga, Texture2D texturaInimigo) {//solon

    Inimigo *auxiliar = naveInimiga;//aponta pra o começo da lista

    while (auxiliar != NULL) {//vai percorrer a lista toda

        if (auxiliar->vivo == 1) {//se o inimigo tiver vivo ele desenha

            Rectangle origem = {0, 0, texturaInimigo.width, texturaInimigo.height};//significa que voce quer a imagem inteira começando do pixel 0x0

            Rectangle destino = {auxiliar->x, auxiliar->y, auxiliar->largura, auxiliar->altura};//adequa a imagem para os parametros ja definidos

            DrawTexturePro(texturaInimigo, origem, destino, (Vector2){0, 0}, 0, WHITE);//pega a textura, a imagem, a localizaçao e o tamanho da imagem, e vector é ponto de referencia
        }

        auxiliar = auxiliar->prox;//move o ponteiro
    }
}

TiroJogador *criarTiro(struct Jogador *um) {

    TiroJogador *novo = malloc(sizeof(TiroJogador));

    if (novo == NULL) {
        return NULL;
    }

    novo->largura = 6;
    novo->altura = 16;
    
    novo->velocidade = 550;
    
    novo->x = um->x + (um->largura - novo->largura) / 2; //Centraliza o tiro na nave
    novo->y = um->y - novo->altura; //faz o tiro nascer acima da nave pq ele nasce depois da altura e o y dele


    novo->prox = NULL;

    return novo;
}

TiroJogador *adicionarTiro(TiroJogador *bullet, Jogador *um,int *deathContador) {////////

    static float tempoDisparo = 0.20f;

    tempoDisparo += GetFrameTime();//espera o tempo de desparo pra atirar de novo, adicionar um tiro

    int contador = 0;

    TiroJogador *cont = bullet;//um ponteiro auxilair pra contador

    while (cont != NULL) {//conta quantos tiros tem

        contador++;

        cont = cont->prox;
    }

    if (IsKeyDown(KEY_SPACE) && tempoDisparo >= 0.20f && contador < 2) {//se apertar espaco,tiver passado 0.20, e tiver 2 ou menos tiros

        TiroJogador *novo = criarTiro(um);//cria um novo tiro

        if (novo == NULL) {
            return bullet;//se falhar retorna null
        }

        if (contador == 0) {

            bullet = novo; //primeiro tiro ja que o contador nao mudou

        } else if (contador == 1) {

            bullet->prox = novo;//o inicio da lista aponta pra o novo tiro, o segundo
        }

        tempoDisparo = 0.0f;//zera o tempo pra comecar o processo de novo



        (*deathContador)++;//isso é pra funcao da death star de se for par ir da esquerda pra direita e se for impar da direita pra esquerda
    }

    

    return bullet;//retorna a nova lista
}

TiroJogador *movimentacaoTiro(TiroJogador *bullet) {

    TiroJogador *auxiliar = bullet;
    TiroJogador *anterior = NULL;

    float deslocamento;

    while (auxiliar != NULL) {

        deslocamento = auxiliar->velocidade * GetFrameTime();//move o tiro pra cima

        auxiliar->y = auxiliar->y - deslocamento;//move o tiro pra cima

        if (auxiliar->y + auxiliar->altura < 0) {//verifica se saiu completamente de tela

            if (auxiliar == bullet) {//se auxiliar for = ao inicio

                bullet = auxiliar->prox;//se tiver dado um tiro o inicio vai ser null pelo aux->prox ser null, e se for o primeiro tiro o 2 vira o primeiro 2 que vira o tiro 1

                free(auxiliar);

                auxiliar = bullet;

            } else {

                anterior->prox = auxiliar->prox;// ja que o primeiro tiro ja ocorreu o anterior apontava pro auxiliar antigo, aqui ele ta fazendo o anterior prox apontar pra null

                free(auxiliar);

                auxiliar = anterior->prox;//termina o while pq ele vira null
            }

        } else {

            anterior = auxiliar;// esse acontece pq o segundo tiro nao pdoe sair sem o 1, entao anterior vai apontar pra auxiliar

            auxiliar = auxiliar->prox;// e auxiliar para o proximo
        }
    }

    return bullet;//retorna ou null ou o inicio da lista
}

TiroJogador *colisaoTiroDOJogador(TiroJogador *bullet, Inimigo *naveInimiga, Jogador *um){
    
    
    TiroJogador *auxiliarTiro = bullet;//pra mover em bullet
    
    Inimigo *auxiliarInimigo = naveInimiga;//pra mover em nave

    TiroJogador *anteriorTiro = NULL;//pra dar free em bullet que colidir
    
    while(auxiliarTiro != NULL){
        
        Rectangle hitboxTiro = {auxiliarTiro->x, auxiliarTiro->y ,auxiliarTiro->largura ,auxiliarTiro->altura};//declara o hitbox onde o tiro ta e qual a area
        
        while(auxiliarInimigo != NULL){//percorre todos os inimigos 
            
            
            Rectangle hitboxInimigo = {auxiliarInimigo->x, auxiliarInimigo->y ,auxiliarInimigo->largura ,auxiliarInimigo->altura};//ve se acertou o hitbox criando o hitbox pra cada inimgio
            //conforme o ponteiro se move

            if (auxiliarInimigo->vivo == 1 && CheckCollisionRecs(hitboxTiro, hitboxInimigo)) {//verifica se o acertou um inimigo visivel(vivo)
                
                auxiliarInimigo->vivo = 0;//impede que ele seja desenhado, bata na parede, ou de gameover

                um->pontos += auxiliarInimigo->pontos;//soma os pontos desse inimigo ao jogador

                if (anteriorTiro == NULL) {

                    bullet = auxiliarTiro->prox;//se esse for null quer dizer que é o primeiro tiro, e nao fez um while todo ainda
                    //bullet vai apontar pro segundo tiro que vai virar o "primeiro"
                    //se nao tiver primeiro tiro ele vai ser null ja que nao vai ter tiro sobrando
                } else {

                    anteriorTiro->prox = auxiliarTiro->prox; //se chegar no else foi o segundo tiro, fazendo o primeiro apontar pra null
                    //ja passou por um while
                }

                free(auxiliarTiro); //libera qualquer que for o tiro

                return bullet; //retorna a lista com o tiro colidido liberado
            }
    
            
            
            
            auxiliarInimigo = auxiliarInimigo->prox;//move a lista pra analisar todos os inimigos
        }
        
        auxiliarInimigo = naveInimiga;//volta o auxiliar pro inicio pra conseguir ver todos inimigos de novo em caso de outro tiro acertar

        anteriorTiro = auxiliarTiro;//o anterior vai apontar pro primeiro tiro
        //-> o de baixo vai pro segundo tiro
        auxiliarTiro = auxiliarTiro->prox;//percorre a lista pra checar se alguma das balas acertou e vai pro segundo tiro
        
    }
    
    return bullet;//retorna a lista
}

TiroJogador *colisaoTiroNaDeathStar(TiroJogador *bullet, DeathStar *estrela, Jogador *um){


    TiroJogador *auxiliarTiro = bullet;
    TiroJogador *anterior = NULL;
        
    if(estrela->vivo == 0){

        return bullet;

    }

    while(auxiliarTiro != NULL){
            
        Rectangle hitboxTiro = {auxiliarTiro->x, auxiliarTiro->y ,auxiliarTiro->largura ,auxiliarTiro->altura};//declara o hitbox onde o tiro ta e qual a area
        
        Rectangle hitboxDeathStar = {estrela->x, estrela->y ,estrela->largura , estrela->altura};
        
            
        if(CheckCollisionRecs(hitboxDeathStar, hitboxTiro)){//se bateu no jogador

            um->pontos += estrela->pontos;

            if (auxiliarTiro == bullet) {//se auxiliar for = ao inicio
                  

                estrela->vivo = 0;
                estrela->tempoSpawn = 0.0f;//isso faz o timer voltar pra ele spawnar dnv

                            
                bullet = auxiliarTiro->prox;//se tiver dado um tiro o inicio vai ser null pelo aux->prox ser null, e se for o primeiro tiro o 2 vira o primeiro
                free(auxiliarTiro);//libera o que saiu da tela

                return bullet; //ele retorna pq ja acertou a estrela, nao da pra matar duas vezes ao mesmo tempo

            } else {//ele vai estar no segundo tiro da lista

                
                estrela->vivo = 0;
                estrela->tempoSpawn = 0.0f;//isso faz o timer voltar pra ele spawnar dnv

                anterior->prox = auxiliarTiro->prox;// anterior aponta pra null ja que so pode ter dois tiros

                free(auxiliarTiro);//libera o ultimo

                return bullet; //ele retorna pq ja acertou a estrela, nao da pra matar duas vezes ao mesmo tempo
                
            }

        } else {

            anterior = auxiliarTiro;//se o tiro atual continua na tela, o anterior vira auxi, 

            auxiliarTiro = auxiliarTiro->prox;// e auxiliar para o proximo, se for null ele encerra o while tambem
        }
            
    }
        
    return bullet;//retorna a lista

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

void inicializarEstrelaDaMorte(DeathStar *estrela) {
    
    if (estrela == NULL) {
        return;
    }
        
    estrela->largura = 80;
    estrela->altura = 40;
        
    estrela->velocidade = 150;
        
    estrela->y = 40;
    estrela->x = -60;
        
    estrela->pontos = 100;
    estrela->vivo = 0;

    estrela->tempoSpawn = 0.0f;

}


void movimentacaoEstrelaDaMorte(DeathStar *estrela, int *deathContador) {

    float deslocamento;

    static int direcao;//se vai pra esquerda ou pra direita

    if (estrela->vivo == 0) {

        estrela->tempoSpawn += GetFrameTime();

        if (estrela->tempoSpawn >= 20.0f) {//espera 20 segundo pra spawnar

            estrela->vivo = 1;//quando passa 20 segundos, a estrela muda o estado para vivo
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

void desenharEstrelaDaMorte(DeathStar *estrela, Texture2D texturaDeath_Star) {

    if (estrela == NULL || estrela->vivo == 0) {//se ainda nao tiver no tempo da estrela ela nao é desenhada
        return;
    }

    Rectangle origem = {0, 0, texturaDeath_Star.width, texturaDeath_Star.height};

    Rectangle destino = {estrela->x, estrela->y, estrela->largura, estrela->altura};

    DrawTexturePro(texturaDeath_Star, origem, destino, (Vector2){0, 0}, 0, WHITE);
}


void inicializarJogador(struct Jogador *um){
    
    um->largura = 64;
    um->altura = 64;
    
    um->velocidade = 300;
    um->vidas = 3;
    
    um->x = 368;
    um->y = 520;

    um->pontos = 0;
    um->tempoDano = 0.0f;

    um->nome[0] = '\0';
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

int digitarNome(Jogador *um) {

    int tamanho = TextLength(um->nome);

    char *posicao = um->nome + tamanho;//aponta para o fim do nome

    int letra = GetCharPressed();

    while (letra != 0) {

        if(tamanho < 29 && ((letra >= 'a' && letra <= 'z') || (letra >= 'A' && letra <= 'Z'))){//aceita letra mauiscula e minuscula

            *posicao = letra;//escreve a letra nessa posicao

            posicao++;//avança para a proxima posição
            tamanho++;// tamanho nao pode passar de 28

            *posicao = '\0';//marca o novo fim do nome
        }

        letra = GetCharPressed();//pega a letra q a pessoa digitar
    }

    if(IsKeyPressed(KEY_BACKSPACE) && tamanho > 0){//se tiver escrito algo 

        posicao--;//volta para a ultima letra

        *posicao = '\0';//encerra o texto antes da ultima letra
    }

    if(IsKeyPressed(KEY_ENTER) && *um->nome != '\0') {

        return 1;//vai retornar que criou o nome
    }

    return 0;//nao criou
}

void desenharNave(Jogador *um, Texture2D texturaNaveNormal, Texture2D texturaMachucada, Texture2D texturaNaveLascada, Texture2D texturaExplosao) {

    Texture2D textura = texturaNaveNormal;

    if (um->vidas == 2) {// se tiver com duas vidas ela fica machucada, e muda de imagem

        textura = texturaMachucada;

    } else if (um->vidas == 1) {

        textura = texturaNaveLascada;// se tiver com um ela fica toda lascada e muda

    } else if (um->vidas == 0) {

        textura = texturaExplosao;// se tiver com 0 explode e encerra o jogo
    }

    Rectangle origem = {0, 0, textura.width, textura.height};//imagem em si

    Rectangle destino = {um->x, um->y, um->largura, um->altura};//area que a imagem vai ocupar

    DrawTexturePro(textura, origem, destino, (Vector2){0, 0}, 0, WHITE);
}

///////////////////////////////////////////////
//Loop do jogo

int ImperioEliminado(Inimigo *naveInimiga) {

    Inimigo *auxiliar = naveInimiga;

    while (auxiliar != NULL){//percorre tudo pra ver se tem alguem vivo

        if (auxiliar->vivo == 1) {//se tiver no minimo 1 vivo ja retorna 0
            //signfica que nao precisa reiniciar os inimigos
            return 0;
        }

        auxiliar = auxiliar->prox;
    }

    return 1;
}

void desenharPontuacao(Jogador *um) {

    const char *texto = TextFormat("PONTOS: %d", um->pontos);

    int larguraTexto = MeasureText(texto, 20);

    DrawText(texto, 800 - larguraTexto - 15, 8, 20, YELLOW);//se alinha na direita

}

void desenharGameOver(void) {

    DrawText("GAME OVER", (800 - MeasureText("GAME OVER", 50)) / 2, 230, 50, RED);//desenha game over, no lugar e tamanho da fonte

    DrawRectangle(230, 340, 340, 55, Fade(BLACK, 0.8f));//o fundo do botao

    DrawRectangleLines(230, 340, 340, 55, YELLOW);//fica opcao pra ver o cenario

    DrawText("VOLTAR AO MENU", (800 - MeasureText("VOLTAR AO MENU", 25)) / 2, 355, 25, YELLOW);//esenha voltar ao menu

    DrawText("ENTER: CONFIRMAR", (800 - MeasureText("ENTER: CONFIRMAR", 18)) / 2, 420, 18, WHITE);//desenha as instrucoes
}



void desenharFundo(Texture2D texturaFundo) {//fundo in game

    Rectangle origem = {0, 0, texturaFundo.width, texturaFundo.height};

    Rectangle destino = {0, 0, 800, 600};

    DrawTexturePro(texturaFundo, origem, destino, (Vector2){0, 0}, 0, WHITE);
}

int controlarMenu(int *opcaoMenu) {

    if(IsKeyPressed(KEY_S)){// faz um loop de 0 a 2 pra as opcoes

        (*opcaoMenu)++;

        if (*opcaoMenu > 2){

            *opcaoMenu = 0;
        }
    }

    if (IsKeyPressed(KEY_W)){

        (*opcaoMenu)--;

        if(*opcaoMenu < 0) {// faz um loop de 0 a 2 pra as opcoes

            *opcaoMenu = 2;
        }
    }

    if(IsKeyPressed(KEY_ENTER)) {

        if(*opcaoMenu == 0) {

            return 1;//jogar

        } else if (*opcaoMenu == 1){

            return 2;//ranking

        } else if (*opcaoMenu == 2) {

            return 3;//sair
        }
    }

    return 0;//continua no menu
}

void desenharMenu(int opcaoMenu, Texture2D texturaFundoMenu) {//esenha o fundo do menu

    Rectangle origem = {0, 0, texturaFundoMenu.width, texturaFundoMenu.height};//a imagem em si

    Rectangle destino = {0, 0, 800, 600};// onde ela vai ocupar

    DrawTexturePro(texturaFundoMenu, origem, destino, (Vector2){0, 0}, 0, WHITE);//mantem as cores originais

    DrawText("SPACE", 90, 100, 50, YELLOW);
    DrawText("TRAVELER", 90, 155, 50, YELLOW);

    const char *opcoes[3] = {"JOGAR", "RANKING", "SAIR"};

    for (int i = 0; i < 3; i++){

        Color cor = WHITE;//se a opcao nao tiver selcionada ele comeca com branco

        if (i == opcaoMenu) {// se tiver encima no w / s 

            cor = YELLOW;//troca a cor pra amarelo

            DrawText(">", 65, 290 + i * 60, 30, cor);//setinha pra ficar bonito, se a escolha estiver em cima
        }

        DrawText(opcoes[i], 100, 290 + i * 60, 30, cor);//desenha a opcao em amarelo, ou branco dependendo de onde tiver o w / s
    }

    DrawText("W / S: SELECIONAR", 90, 510, 16, WHITE);
    DrawText("ENTER: CONFIRMAR", 90, 535, 16, WHITE);
}


void inserirNosMelhores(Log *logs, Log atual) {

    Log *posicao = logs;//apotna pra o primeiro log

    while(posicao < logs + 5) {//percorre os 5 logs

        if(atual.pontos > posicao->pontos) {//se tiver mais pontos

            Log troca = *posicao;//guarda o jogador com menso pontos

            *posicao = atual;// substitui pelo com mais

            atual = troca;//guarda o com menos pontos pra ver se ele continua na lista
        }

        posicao++;//vai para o próximo Log
    }
}

void salvarPontuacao(Jogador *um) {

    Log logs[5] = {0};//cria cinco registros zerados

    Log *posicao = logs;//aponta para o primeiro

    while(posicao < logs + 5) {

        posicao->pontos = -1;//marca a vaga como vazia

        posicao++;
    }

    FILE *arquivo = fopen("ranking.txt", "r");//abre pra ler os resultados antigos

    if(arquivo != NULL) {//so le se conseguiu abrir

        Log atual;//guarda um resultado por vez

        
        while (fscanf(arquivo, "%29s %d", atual.nome, &atual.pontos) == 2){//continua enquanto conseguir ler o nome e os pontos, os dois

            inserirNosMelhores(logs, atual);//encaixa o resultado no ranking
        }

        fclose(arquivo);//terminou de ler
    }

    Log novo;

    snprintf(novo.nome, sizeof(novo.nome), "%s", um->nome);//guarda o novo nome

    novo.pontos = um->pontos;//guarda novos pontos

    inserirNosMelhores(logs, novo);//tenta colocar a pontuacao nos melhores

    arquivo = fopen("ranking.txt", "w");//cria o arquivo ou apaga o conteudo antigo

    if (arquivo == NULL) {

        return;//nao conseguiu abrir pra salvar
    }

    posicao = logs;//volta ao primeiro registro

    while(posicao < logs + 5) {

        if(posicao->pontos >= 0) {//so salva as vagas preenchidas

            fprintf(arquivo, "%s %d\n", posicao->nome, posicao->pontos);//escreve nome e pontos, um jogador por linha
        }

        posicao++;
    }

    fclose(arquivo);//fecha o arquivo
}

int carregarRanking(Log *logs) {

    FILE *arquivo = fopen("ranking.txt", "r");

    if(arquivo == NULL) {

        return 0;//nao tem nada
    }

    int quantidade = 0;
    Log *posicao = logs;

    while(quantidade < 5 && fscanf(arquivo, "%29s %d", posicao->nome, &posicao->pontos) == 2) {

        quantidade++;
        posicao++;//avança para o próximo Log
    }

    fclose(arquivo);

    return quantidade;
}


int main(void) {

    InitWindow(800, 600, "Space Traveler");

    Texture2D texturaNave = LoadTexture("assets/nave.png");
    Texture2D texturaTiro = LoadTexture("assets/tiro.png");
    Texture2D texturaInimigo = LoadTexture("assets/inimigo1.png");
    Texture2D texturaDeath_Star = LoadTexture("assets/Death_Star.png");
    Texture2D texturaTiroInimigo = LoadTexture("assets/tiroInimigo.png");// carrega as texturas
    Texture2D texturaEscudo = LoadTexture("assets/escudo.png");
    Texture2D texturaNave2 = LoadTexture("assets/nave2.png");
    Texture2D texturaNave3 = LoadTexture("assets/nave3.png");
    Texture2D texturaExplosao = LoadTexture("assets/explosao.png");
    Texture2D texturaFundo = LoadTexture("assets/fundo.png");
    Texture2D texturaFundoMenu = LoadTexture("assets/fundoMenu.png");

    SetTargetFPS(60);
    
    Jogador *primeiro = malloc(sizeof(Jogador));// aloca o jogador

    
    
    
    if (primeiro == NULL) {//nao tem free primeiro pq ele ja é null entao nao tem oq liberar

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);
        UnloadTexture(texturaDeath_Star);
        UnloadTexture(texturaTiroInimigo);//libera as texturas
        UnloadTexture(texturaEscudo);
        UnloadTexture(texturaNave2);
        UnloadTexture(texturaNave3);
        UnloadTexture(texturaExplosao);
        UnloadTexture(texturaFundo);
        UnloadTexture(texturaFundoMenu);

        CloseWindow();

        return 1;
    }
    
    inicializarJogador(primeiro);//incializa as especificacoes do jogador

    TiroJogador *bullet = NULL;

    Inimigo *naveInimiga = NULL;//cria os ponteiros dos tiros e inimigos

    TiroInimigo *tirosInimigos = NULL;

    int deathContador1 = 0;//mecanica de sair pela esquerda ou pela direita

    naveInimiga = inicializarInimigo(naveInimiga);//incializa os 55 inimgios

    if (naveInimiga == NULL) {//aqui tem free primeiro pq ele ja funcionou mas nao tem free nave pq a função ja libera

        free(primeiro);//libera primeiro pq ja ta alocado

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);
        UnloadTexture(texturaDeath_Star);
        UnloadTexture(texturaTiroInimigo);
        UnloadTexture(texturaEscudo);//libera as texturas
        UnloadTexture(texturaNave2);
        UnloadTexture(texturaNave3);
        UnloadTexture(texturaExplosao);
        UnloadTexture(texturaFundo);
        UnloadTexture(texturaFundoMenu);

        CloseWindow();

        return 1;
    }

    DeathStar *estrela = malloc(sizeof(DeathStar));//aloca a death star


    Escudo escudos[4];//vetor dos escudos
    inicializarEscudos(escudos);//inicia a matriz dos escudos

    float tempoNovaOnda = 0.0f;//espera um tempo pra spawnar os novos inimigos
    float tempoMorte = 0.0f;//quando explode ela espera um tempo antes de dar game over

    if (estrela == NULL) {//aqui tem q ter os dois free menos o da estrela que ja é null

        free(primeiro);//liberar o jogador

        while (naveInimiga != NULL) {//tem que liberar os inimigos

            Inimigo *auxiliar = naveInimiga;

            naveInimiga = naveInimiga->prox;

            free(auxiliar);
        }

        UnloadTexture(texturaNave);
        UnloadTexture(texturaTiro);
        UnloadTexture(texturaInimigo);
        UnloadTexture(texturaDeath_Star);
        UnloadTexture(texturaTiroInimigo);
        UnloadTexture(texturaEscudo);//liberar a textura
        UnloadTexture(texturaNave2);
        UnloadTexture(texturaNave3);
        UnloadTexture(texturaExplosao);
        UnloadTexture(texturaFundo);
        UnloadTexture(texturaFundoMenu);

        CloseWindow();

        return 1;
    }

    inicializarEstrelaDaMorte(estrela);//especificacoes da estrela da morte

    int opcao = 0;//qual tela ta aberta
    int opcaoMenu = 0;//tela q o w / s ta em cima

    int pontuacaoSalva = 0;//impede salvar a mesma partida em todo frame

    Log logs[5];
    int quantidadeLogs = 0;

    while(!WindowShouldClose()) {//enquando opcao for != 3

        if(opcao == 0) {

            opcao = controlarMenu(&opcaoMenu);//fica no menu

            if(opcao == 2) {

                quantidadeLogs = carregarRanking(logs);//entrou no ranking

            }

            if(opcao == 3) {

                break;//fecha o jogo
            } if(opcao == 1) {

                
                while(naveInimiga != NULL) {//libera os inimigos da ultima partida

                    Inimigo *auxiliar = naveInimiga;

                    naveInimiga = naveInimiga->prox;

                    free(auxiliar);
                }

                
                while(bullet != NULL) {//libera os tiros da ultima partida

                    TiroJogador *auxiliar = bullet;

                    bullet = bullet->prox;

                    free(auxiliar);
                }

                
                while(tirosInimigos != NULL) {//libera os tiros da ultima partida

                    TiroInimigo *auxiliar = tirosInimigos;

                    tirosInimigos = tirosInimigos->prox;

                    free(auxiliar);
                }

                
                naveInimiga = inicializarInimigo(naveInimiga);// Cria os inimigos para a nova partida

                if(naveInimiga == NULL) {

                    break;
                }

                
                inicializarJogador(primeiro);//volta para 3 vidas, 0 pontos e sem nome

                
                inicializarEscudos(escudos);
                inicializarEstrelaDaMorte(estrela);//resutaura escudos e a death

                
                tempoNovaOnda = 0.0f;
                tempoMorte = 0.0f;//zera os cotnadores
                deathContador1 = 0;
                pontuacaoSalva = 0;

                opcao = 4;//pede o nome da nova partida
            }

        }else if (opcao == 1) {//starta o jogo

            if(primeiro->vidas == 0){//verifica se o jogador morreu

                if(tempoMorte < 2.0f){// se ele morreu

                    tempoMorte += GetFrameTime();// pra quando ele morrer pausar o tempo ja que o o proximo else nao vai funcionar
                    //entao nenhuma funcao de colisao etc funciona

                }else{

                if(pontuacaoSalva == 0) {

                        salvarPontuacao(primeiro);//quando morrer salva a pontuacao

                        pontuacaoSalva = 1;
                    }

                    if(IsKeyPressed(KEY_ENTER)) {

                        opcao = 0;//volta para o menu

                        opcaoMenu = 0;//deixa JOGAR selecionado
                    }
                }

            }else if(ImperioEliminado(naveInimiga) == 1){//se todas as naves estiverem mortas

                tempoNovaOnda += GetFrameTime();//so quando todos morrem o timer comeca

                if(tempoNovaOnda >= 2.0f){

                    while(naveInimiga != NULL){//libera os inimigos da onda anterior

                        Inimigo *auxiliar = naveInimiga;

                        naveInimiga = naveInimiga->prox;

                        free(auxiliar);
                    }

                    while(bullet != NULL){//libera os tiros do jogador

                        TiroJogador *auxiliar = bullet;

                        bullet = bullet->prox;

                        free(auxiliar);
                    }

                    while(tirosInimigos != NULL){//libera os tiros dos inimigos

                        TiroInimigo *auxiliar = tirosInimigos;

                        tirosInimigos = tirosInimigos->prox;

                        free(auxiliar);
                    }

                    naveInimiga = inicializarInimigo(naveInimiga);//os inimigos voltam

                    if(naveInimiga == NULL){

                        break;
                    }

                    inicializarEscudos(escudos);//os escudos voltam

                    tempoNovaOnda = 0.0f;
                }

            } else {

                movimentacaoJogador(primeiro);
                //mover o jogador antes de atirar pra saber a posicao
                bullet = adicionarTiro(bullet, primeiro, &deathContador1);

                bullet = movimentacaoTiro(bullet);
                //move o tiro antes de ver se colidiu
                bullet = colisaoTiroDoJogadorNosEscudos(bullet, escudos);
                //verifica o escudo antes dos inimigos pra o jogador nao atriar no mesmo tiro escudo e inimigos
                naveInimiga = movimentacaoInimigos(naveInimiga, primeiro);
                //move os inimigos antes de ver se colidiram que nem jogador
                bullet = colisaoTiroDOJogador(bullet, naveInimiga, primeiro);

                movimentacaoEstrelaDaMorte(estrela, &deathContador1);
                //move death star antes de ver se colidiu que nem inimigo
                bullet = colisaoTiroNaDeathStar(bullet, estrela, primeiro);


                tirosInimigos = adicionarTiroInimigo(tirosInimigos, naveInimiga);
                //escolhe quem vai atirar antes da movimentacao
                tirosInimigos = movimentacaoTiroInimigo(tirosInimigos);

                tirosInimigos = colisaoTiroDoInimigoNosEscudos(tirosInimigos, escudos);//ve se o tiro do inimigo bateu

                tirosInimigos = colisaoTiroDOInimigo(tirosInimigos, primeiro);//verifica se bateu no inimigo, depois do escudo pra nao abter
                //neele e no escudo
            }

        } else if(opcao == 2) {//ranking


            if (IsKeyPressed(KEY_BACKSPACE)) {

                opcao = 0;
            }
        }else if (opcao == 4){//recebe o nome digitado

            if(digitarNome(primeiro) == 1) {

                opcao = 1;//ele volta pro jogo, pq ele sai do if de 0 e vai pro de 1
            }
        }

        ////////////////////////////////////////////////

        BeginDrawing();//desenha

        ClearBackground(BLACK);

        desenharFundo(texturaFundo);

        if (opcao == 0){

            desenharMenu(opcaoMenu, texturaFundoMenu);//desenha o menu

        }else if (opcao == 1) {

            if (primeiro->vidas == 0 && tempoMorte >= 2.0f) {

                desenharGameOver();

            } else {

                desenharNave(primeiro, texturaNave, texturaNave2, texturaNave3, texturaExplosao);

                desenharInimigos(naveInimiga, texturaInimigo);

                desenharEscudos(escudos, texturaEscudo);

                desenharTiroInimigo(tirosInimigos, texturaTiroInimigo);

                desenharEstrelaDaMorte(estrela, texturaDeath_Star);

                desenharTiro(bullet, texturaTiro);

                desenharPontuacao(primeiro);
            }

        }else if (opcao == 2) {

            DrawText("RANKING", (800 - MeasureText("RANKING", 40)) / 2, 130, 40, YELLOW);

            DrawText("BACKSPACE: VOLTAR", 20, 560, 20, WHITE);

            if(quantidadeLogs == 0) {

                DrawText("NENHUMA PONTUACAO", 190, 280, 20, WHITE);

            } else {

                for(int i = 0; i < quantidadeLogs; i++) {

                    DrawText(TextFormat("%d. %s - %d", i + 1, (logs + i)->nome, (logs + i)->pontos), 220, 220 + i * 50, 25, WHITE);
                    //escreve o nome de cada pessoa no ranking
                }
            }

        }else if(opcao == 4){//desenha a tela escolher o nome

            DrawText("DIGITE SEU NOME", (800 - MeasureText("DIGITE SEU NOME", 30)) / 2, 180, 30, YELLOW);

            DrawRectangle(60, 260, 680, 60, Fade(BLACK, 0.8f));

            DrawRectangleLines(60, 260, 680, 60, YELLOW);

            DrawText(primeiro->nome, 75, 280, 20, WHITE);

            DrawText("ENTER: CONFIRMAR", 260, 370, 20, WHITE);

            DrawText("BACKSPACE: APAGAR", 260, 410, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(texturaNave);//libera a textura
    UnloadTexture(texturaTiro);
    UnloadTexture(texturaInimigo);
    UnloadTexture(texturaDeath_Star);
    UnloadTexture(texturaTiroInimigo);
    UnloadTexture(texturaEscudo);
    UnloadTexture(texturaNave2);
    UnloadTexture(texturaNave3);
    UnloadTexture(texturaExplosao);
    UnloadTexture(texturaFundo);
    UnloadTexture(texturaFundoMenu);

    CloseWindow();

    while(bullet != NULL) {

        TiroJogador *auxiliar = bullet;

        bullet = bullet->prox;

        free(auxiliar);
    }

    while (naveInimiga != NULL) {

    Inimigo *auxiliar = naveInimiga;

    naveInimiga = naveInimiga->prox;

    free(auxiliar);

    }
    
    while (tirosInimigos != NULL){

        TiroInimigo *auxiliar = tirosInimigos;

        tirosInimigos = tirosInimigos->prox;

        free(auxiliar);
    }

    
    free(estrela);
    free(primeiro);

    return 0;
} 