/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
// *********************************************************************/

// A l�gica principal do jogo � feita com uma matriz, que pode ser customizada entre (8x8)(10x10) e (12x12), apenas alterando a variavel de mesmo nome
// Esta matriz usa n�meros para definir o que ocupa cada posi��o, 0 para casa vazia, 1 para pe�a azul e 2 para pe�a vermelha.
// A sele��o de pe�as basicamente soma 10 ao n�mero da casa para selecion�-la e subtrai 10 caso a pe�a ja havia sido selecionada previamente
// A impress�o na tela simplesmente traduz o que cada n�mero da matriz significa e desenha a figura em tamanho proporcional ao canvas

// Foram implementados os seguintes quesitos:
//  - Matriz de aloca��o din�mica e configur�vel
//  - Tabuleiro e pe�as desenhados com as fun��es da API
//  - Controle do jogo pelo teclado, com seletor travado dentro do tabuleiro
//  - Sistema alternado de turnos
//  - Detec��o de pe�a capturada ap�s jogada
//  - Indica��o visual de pe�a capturada
//  - Sistema de game over
//  - Registro de vit�rias e derrotas ilustrado na tela
//  - Bot�o 'reset' para recome�ar partida sem alterar o placar
//  - Lista de instru��es na tela

// Trabalho de Logica e Algoritmo - Gustavo Machado de Freitas - 09/11/2020


#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "gl_canvas2d.h"

// Variaveis globais:

// Cena 0 = Tela de In�cio
// Cena 1 = Tela de jogo
int cena = 0;

// Coordenadas do seletor
int posQuadradoX = 200;
int posQuadradoY = 200;

// Coordenadas da pe�a selecionada
int pecaSelecionadaX;
int pecaSelecionadaY;

// Coordenadas da pe�a que ser� removida
int pecaVitimaX;
int pecaVitimaY;

// Vetor para guardar as elimina��es e vit�rias de cada jogador
int pontos[2];
int vitorias[2];
char vitoriasFormatado[100];

// Turno 1 = Jogador Azul
// Turno 2 = Jogador Vermelho
int turno = 1;

// Armazena a sele��o
int existePecaSelecionada = 0;

// Vari�veis de apoio para renderiza��o
int eixoX = 0;
int eixoY = 0;

float raioDasPecas = 25;
int comparacao = 0;

// Tabuleiro do jogo, matriz pode ser alterada para 8, 10 e 12
const int matriz = 8;
int tabuleiro[matriz][matriz];

// Dimens�es iniciais da tela
int screenWidth = 800, screenHeight = 600;

// Prepara o setup do jogo
void PreparaJogo(){
    // Preenche a tabela
    for(eixoY = 0; eixoY < matriz; eixoY ++) {
        for(eixoX = 0; eixoX < matriz; eixoX ++) {
            tabuleiro[eixoY][eixoX] = 0;
        }
    }

    // Posiciona as pe�as
    for(eixoY = 0; eixoY < matriz; eixoY ++) {
        if(eixoY % 2 == 0) {
            comparacao = 1;
        } else {
            comparacao = 0;
        }
        for(eixoX = 0; eixoX < matriz; eixoX ++) {
            if (eixoX % 2 == comparacao) {
                if (eixoY < 3) {
                    tabuleiro[eixoY][eixoX] = 1;
                }
                if (eixoY > matriz - 4) {
                    tabuleiro[eixoY][eixoX] = 2;
                }
            }
        }
    }
}

// Volta o jogo para a tela inicial ap�s o fim da partida
void GameOver()
{
    cena = 0;
    turno = 1;

    pontos[0] = 0;
    pontos[1] = 0;

}

// Fun��o respons�vel por desenhar a tela constantemente
void render()
{
    CV::clear(0,0,0);

    // Checa se o jogo j� come�ou
    if (cena == 0) {
        CV::color(13);
        CV::text(screenWidth / 4, screenHeight / 2,"Aperte 'Enter' para iniciar a partida");
        PreparaJogo();
    } else {
        // Variaveis de dimens�o
        int altura = matriz * 50;
        int largura = matriz * 50;

        // Desenha a moldura
        CV::color(1);
        CV::rect(0, 0, largura, altura);

        // Desenha o tabuleiro
        for(eixoY = 0; eixoY < altura; eixoY += 50) {
            if(eixoY % 100 == 0) {
                comparacao = 0;
            } else {
                comparacao = 50;
            }
            for(eixoX = 0; eixoX < largura; eixoX += 50) {
                if (eixoX % 100 == comparacao) {
                    CV::color(1);
                    CV::rectFill(eixoX, eixoY, eixoX + 50, eixoY + 50);
                } else {
                    CV::color(0);
                    CV::rectFill(eixoX, eixoY, eixoX + 50, eixoY + 50);
                }
            }
        }

        // Desenha o seletor
        CV::color(3);

        CV::rect(posQuadradoX, posQuadradoY, posQuadradoX + 50, posQuadradoY + 50);

        // Desenha selecionado
        for(eixoY = 0; eixoY < matriz; eixoY ++) {
            for(eixoX = 0; eixoX < matriz; eixoX ++) {
                if (tabuleiro[eixoY][eixoX] > 10) {
                    CV::color(3);
                    CV::rectFill(eixoX * 50, eixoY * 50, eixoX * 50 + 50, eixoY * 50 + 50);
                }
            }
        }

        // Desenha as pe�as
        for(eixoY = 0; eixoY < matriz; eixoY ++) {
            for(eixoX = 0; eixoX < matriz; eixoX ++) {
                if(tabuleiro[eixoY][eixoX] % 2 != 0){
                    CV::color(4);
                    CV::circleFill( eixoX * 50 + raioDasPecas, eixoY * 50 + raioDasPecas, raioDasPecas, 30 );
                }
                if(tabuleiro[eixoY][eixoX] % 2 == 0 && tabuleiro[eixoY][eixoX] != 0 && tabuleiro[eixoY][eixoX] != 10){
                    CV::color(2);
                    CV::circleFill( eixoX * 50 + raioDasPecas, eixoY * 50 + raioDasPecas, raioDasPecas, 30 );
                }
            }
        }

        // Desenha o "cemit�rio" de pe�as na tela
        for(eixoY = 0; eixoY < pontos[0] * 50 / 4; eixoY += 50){
            for(eixoX = matriz * 50; eixoX < matriz * 50 + pontos[0] * 50; eixoX += 50){
                CV::color(2);
                CV::circleFill( eixoX + raioDasPecas, eixoY + raioDasPecas, raioDasPecas, 30 );
            }
        }
        for(eixoY = matriz * 50 - 150; eixoY < matriz * 50 - 150 + pontos[1] * 50 / 4; eixoY += 50){
            for(eixoX = matriz * 50; eixoX < matriz * 50 + pontos[1] * 50; eixoX += 50){
                CV::color(4);
                CV::circleFill( eixoX + raioDasPecas, eixoY + raioDasPecas, raioDasPecas, 30 );
            }
        }

        // Escreve o n�mero de vitorias na tela
        if (vitorias[0] > vitorias[1]){
            CV::color(4);
        } else if (vitorias[1] > vitorias[0]){
            CV::color(2);
        } else {
            CV::color(13);
        }

        sprintf(vitoriasFormatado, "O placar de vitorias esta: %d X %d", vitorias[0], vitorias[1]);
        CV::text(matriz * 50 + 50, matriz * 26, vitoriasFormatado);

        // Escreve o turno na tela
        switch(turno){
            case 1:
                CV::color(4);
                CV::text(matriz * 50 + 100, matriz * 24,"Turno do jogador 1");
            break;
            case 2:
                CV::color(2);
                CV::text(matriz * 50 + 100, matriz * 24,"Turno do jogador 2");
            break;
        }

        // Desenha instru��es na tela
        CV::color(13);
        CV::text(10, matriz * 50 + 20,"Instrucoes:");
        CV::text(10, matriz * 50 + 50,"Use as setas para mover o quadrado seletor");
        CV::text(10, matriz * 50 + 70,"Use a barra de espaco para selecionar uma peca");
        CV::text(10, matriz * 50 + 90,"Use a tecla 'R' para reiniciar a partida");
        CV::text(10, matriz * 50 + 110,"Use a tecla 'Esc' para encerrar o jogo");

        // Checa constantemente se as condi��es de vit�ria foram atingidas
        if(pontos[0] == matriz * 3 / 2){
            vitorias[0] += 1;
            GameOver();
        } else if(pontos[1] == matriz * 3 / 2){
            vitorias[1] += 1;
            GameOver();
        }
    }

}

// Cuida da troca de turno
void PassaTurno(){
    switch(turno){
        case 1:
            turno = 2;
        break;
        case 2:
            turno = 1;
        break;
    }
}

// Cuida da sele��o das pe�as
void SelecionaPeca(){
    // Verifica se n�o existem pe�as selecionadas e define a coordenada para testes posteriores
    if (existePecaSelecionada == 0){
        pecaSelecionadaX = posQuadradoX / 50;
        pecaSelecionadaY = posQuadradoY / 50;
    }
    // Seleciona pe�a
    if (existePecaSelecionada == 0 && tabuleiro[pecaSelecionadaY][pecaSelecionadaX] == turno){
        tabuleiro[pecaSelecionadaY][pecaSelecionadaX] += 10;
        existePecaSelecionada = 1;
    // De-Seleciona pe�a
    } else if (tabuleiro[posQuadradoY / 50][posQuadradoX / 50] >= 10 && existePecaSelecionada == 1) {
        tabuleiro[pecaSelecionadaY][pecaSelecionadaX] -= 10;
        existePecaSelecionada = 0;
    }
}
// Cuida da remo��o de uma pe�a movida
void LimpaCasa(){
    tabuleiro[pecaSelecionadaY][pecaSelecionadaX] = 0;
    existePecaSelecionada = 0;
    PassaTurno();
}

// Cuida da remo��o e contagem de pe�as "comidas"
void ComePeca(){
    tabuleiro[pecaVitimaY][pecaVitimaX] = 0;
    pontos[turno - 1] += 1;
    LimpaCasa();
}

// Cuida da movimenta��o de pe�as, espelhando o comportamento de uma cor na outra de forma inversa
void MovePeca(){
    if(tabuleiro[pecaSelecionadaY][pecaSelecionadaX] == 11){
        // Movimento simples
        if(posQuadradoY / 50 == pecaSelecionadaY + 1){
            tabuleiro[posQuadradoY / 50][posQuadradoX / 50] = 1;
            LimpaCasa();
        }
        // Movimento com captura de pe�a
        else if(posQuadradoY / 50 == pecaSelecionadaY + 2){
            pecaVitimaY = pecaSelecionadaY + 1;
            if(tabuleiro[pecaSelecionadaY + 1][pecaSelecionadaX + 1] == 2 && posQuadradoX / 50 > pecaSelecionadaX){
                pecaVitimaX = pecaSelecionadaX + 1;
                ComePeca();
                tabuleiro[posQuadradoY / 50][posQuadradoX / 50] = 1;
            } else if (tabuleiro[pecaSelecionadaY + 1][pecaSelecionadaX - 1] == 2 && posQuadradoX / 50 < pecaSelecionadaX){
                pecaVitimaX = pecaSelecionadaX - 1;
                ComePeca();
                tabuleiro[posQuadradoY / 50][posQuadradoX / 50] = 1;
            }
        }
    } else if(tabuleiro[pecaSelecionadaY][pecaSelecionadaX] == 12){
        // Movimento simples
        if(posQuadradoY / 50 == pecaSelecionadaY - 1){
            tabuleiro[posQuadradoY / 50][posQuadradoX / 50] = 2;
            LimpaCasa();
        }
        // Movimento com captura de pe�a
        else if(posQuadradoY / 50 == pecaSelecionadaY - 2){
            pecaVitimaY = pecaSelecionadaY - 1;
            if(tabuleiro[pecaSelecionadaY - 1][pecaSelecionadaX + 1] == 1 && posQuadradoX / 50 > pecaSelecionadaX){
                pecaVitimaX = pecaSelecionadaX + 1;
                ComePeca();
                tabuleiro[posQuadradoY / 50][posQuadradoX / 50] = 2;
            } else if (tabuleiro[pecaSelecionadaY - 1][pecaSelecionadaX - 1] == 1 && posQuadradoX / 50 < pecaSelecionadaX){
                pecaVitimaX = pecaSelecionadaX - 1;
                ComePeca();
                tabuleiro[posQuadradoY / 50][posQuadradoX / 50] = 2;
            }
        }
    }
}

// Move o quadrado seletor
void MoveQuadrado(int x, int y)
{
    posQuadradoX += x;
    posQuadradoY += y;
}

// Gerencia as teclas e suas respectivas fun��es
void keyboard(int key)
{
   printf("\nTecla: %d" , key);

   switch(key)
   {

        case 13: // Tecla Enter -> Inicia o jogo
            cena = 1;
        break;

        case 27: // Tecla Esc -> Finaliza o jogo
            exit(0);
        break;

        case 32: // Tecla de espa�o -> Seleciona e move pe�as
            SelecionaPeca();
            if (existePecaSelecionada == 1 && (posQuadradoX / 50 + posQuadradoY / 50) % 2 == 1 &&
                tabuleiro[posQuadradoY / 50][posQuadradoX / 50] == 0 &&
                !(posQuadradoX / 50 - pecaSelecionadaX > 2 || posQuadradoX / 50 - pecaSelecionadaX < -2) ){
                MovePeca();
            }
        break;

        case 114: // Tecla 'R' -> Reinicia o jogo
            GameOver();
        break;

        case 201: // Seta para cima
            if(posQuadradoY > 0){
                MoveQuadrado(0, -50);
            }
        break;

        case 203: // Seta para baixo
            if(posQuadradoY < matriz * 50 - 50){
                MoveQuadrado(0, 50);
            }
        break;

        case 202: // Seta para a direita
            if(posQuadradoX < matriz * 50 - 50){
                MoveQuadrado(50, 0);
            }
        break;

        case 200: // Seta para a esquerda
            if(posQuadradoX > 0){
                MoveQuadrado(-50, 0);
            }
        break;
        }
}

// Fun��o que mostra quando uma tecla � liberada
void keyboardUp(int key)
{
   printf("\nLiberou tecla: %d" , key);
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Jogo de Damas em C");

    CV::run();

}
