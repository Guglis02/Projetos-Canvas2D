//********************************************************
// Cesar Tadeu Pozzer
// 09/2009
// UFSM
// pozzer@inf.ufsm.br
//********************************************************

#ifndef _FRAMES_H___
#define _FRAMES_H___

#include <stdio.h>
#include <time.h>

#define UPDATE_RATE 30   //milisegundos



////////////////////////////////////////////////////////////////////////////////////////
class Frames{
   clock_t t1, t2;
   long    cont_frames;
   float   fps, fps_old;
public:
   Frames()
   {
      t1 = clock();
	  cont_frames = 0;
      fps_old = 20; //valor inicial
   }

   //*********************************************************************
   // Este metodo deve ser chamado uma unica vez a cada atualizacao de tela
   // Retorna o fps da aplicacao. Altere o valor de UPDATE_RATE para diferentes
   // tempos de atualizacao.
   //*********************************************************************
   float getFrames()
   {
      double tempo;

      t2 = clock();
      tempo  = (double)(t2 - t1);  //milisegundos
      cont_frames++;
      //apos UPDATE_RATE milisegundos, atualiza o framerate
      if( tempo > UPDATE_RATE )
      {
         t1 = t2;
         fps = cont_frames / (tempo/CLOCKS_PER_SEC);
         cont_frames = 0;
         fps_old = fps;

         return fps;
      }
      //se nao foi decorrido UPDATE_RATE milisegundos, retorna a taxa antiga
      return fps_old;
   }
};

#endif
