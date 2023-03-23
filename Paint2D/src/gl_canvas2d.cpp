#include "gl_canvas2d.h"
#include <GL/glut.h>

int *scrWidth, *scrHeight; //guarda referencia para as variaveis de altura e largura da main()

//conjunto de cores predefinidas. Pode-se adicionar mais cores.
float Colors[14][3]=
{
    {0, 0, 0}, //Black
    {0.5, 0.5, 0.5}, //Gray
    {1, 0, 0}, //Red
    {0, 1, 0}, //Green
    {0, 0, 1}, //Blue
    {0, 1, 1}, //Cyan
    {1, 0, 1}, //Magenta
    {1, 1, 0}, //Yellow
    {1, 0.5, 0}, //Orange
    {0.5, 0, 0}, //Brown
    {0.5, 0.5, 0}, //Olive
    {0, 0.5, 0.5}, //
    {0.5, 0, 0.5}, //
    {1, 1, 1}, //white
};

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y);

//funcoes de CALLBACK da biblioteca Glut
void keyboard(int key);
void keyboardUp(int key);
void specialUp(int key);
void mouse(int bt, int st, int wheel, int direction, int x, int y);
void mouseWheelCB(int wheel, int direction, int x, int y);
void render();


void point(float x, float y)
{
   glBegin(GL_POINTS);
      glVertex2d(x, y);
   glEnd();
}

void line( float x1, float y1, float x2, float y2 )
{
   glBegin(GL_LINES);
      glVertex2d(x1, y1);
      glVertex2d(x2, y2);
   glEnd();
}

void rect( float x1, float y1, float x2, float y2 )
{
   glBegin(GL_LINE_LOOP);
      glVertex2d(x1, y1);
      glVertex2d(x1, y2);
      glVertex2d(x2, y2);
      glVertex2d(x2, y1);
   glEnd();
}

void rectFill( float x1, float y1, float x2, float y2 )
{
   glBegin(GL_QUADS);
      glVertex2d(x1, y1);
      glVertex2d(x1, y2);
      glVertex2d(x2, y2);
      glVertex2d(x2, y1);
   glEnd();
}

void polygon(float vx[], float vy[], int elems)
{
   int cont;
   glBegin(GL_LINE_LOOP);
      for(cont=0; cont<elems; cont++)
      {
         glVertex2d(vx[cont], vy[cont]);
      }
   glEnd();
}

void polygonFill(float vx[], float vy[], int elems)
{
   int cont;
   glBegin(GL_POLYGON);
      for(cont=0; cont<elems; cont++)
      {
         glVertex2d(vx[cont], vy[cont]);
      }
   glEnd();

}

/*opcoes de fontes
GLUT_STROKE_ROMAN
GLUT_STROKE_MONO_ROMAN
GLUT_BITMAP_9_BY_15
GLUT_BITMAP_8_BY_13
GLUT_BITMAP_TIMES_ROMAN_10
GLUT_BITMAP_TIMES_ROMAN_24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18
*/

void text(float x, float y, const char c)
{
   glRasterPos2i(x, y);
   glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}

void text(float x, float y, const char *t)
{
    int tam = (int)strlen(t);
    for(int c=0; c < tam; c++)
    {
      glRasterPos2i(x + c*10, y);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, t[c]);
    }
}

void text(float x, float y, const int i)
{
    char str[100];
    sprintf(str, "%d", i);
    int tam = (int)strlen(str);
    for(int c=0; c < tam; c++)
    {
      glRasterPos2i(x + c*10, y);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[c]);
    }
}

void text(float x, float y, const double d)
{
    char str[100];
    sprintf(str, "%.3f", d);
    int tam = (int)strlen(str);
    for(int c=0; c < tam; c++)
    {
      glRasterPos2i(x + c*10, y);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[c]);
    }
}

void clear(float r, float g, float b)
{
   glClearColor( r, g, b, 1 );
}

void circle( float x, float y, float radius, int div )
{
   float ang = 0, x1, y1;
   float inc = PI_2/div;
   glBegin(GL_LINE_LOOP);
      for(int lado = 1; lado <= div; lado++) //GL_LINE_LOOP desenha um poligono fechado. Liga automaticamente o primeiro e ultimio vertices.
      {
         x1 = (cos(ang)*radius);
         y1 = (sin(ang)*radius);
         glVertex2d(x1+x, y1+y);
         ang+=inc;
      }
   glEnd();
}

void circleFill( float x, float y, float radius, int div )
{
   float ang = 0, x1, y1;
   float inc = PI_2/div;
   glBegin(GL_POLYGON);
      for(int lado = 1; lado <= div; lado++) //GL_POLYGON desenha um poligono CONVEXO preenchido.
      {
         x1 = (cos(ang)*radius);
         y1 = (sin(ang)*radius);
         glVertex2d(x1+x, y1+y);
         ang+=inc;
      }
   glEnd();
}

//coordenada de offset para desenho de objetos.
//nao armazena translacoes cumulativas.
void translate(float offsetX, float offsetY)
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslated(offsetX, offsetY, 0);
}

void color(float r, float g, float b)
{
   glColor3d(r, g, b);
}

void color(int idx)
{
    glColor3fv(Colors[idx]);
}

void color(float r, float g, float b, float alpha)
{
   glColor4d(r, g, b, alpha);
}

void special(int key, int , int )
{
   keyboard(key+100);
}

void specialUp(int key, int , int )
{
   keyboardUp(key+100);
}

void keyb(unsigned char key, int , int )
{
   keyboard(key);
}

void keybUp(unsigned char key, int , int )
{
   keyboardUp(key);
}

void mouseClick(int button, int state, int x, int y)
{
   ConvertMouseCoord(button, state, -2, -2, x, y);
}

void mouseWheelCB(int wheel, int direction, int x, int y)
{
   ConvertMouseCoord(-2, -2, wheel, direction, x, y);
}

void motion(int x, int y)
{
   ConvertMouseCoord(-2, -2, -2, -2, x, y);
}

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y)
{
#if Y_CANVAS_CRESCE_PARA_CIMA == TRUE
   y = *scrHeight - y; //deve-se inverter a coordenada y do mouse se o y da canvas crescer para cima. O y do mouse sempre cresce para baixo.
#else
   //nao faz nada.
#endif
   mouse(button, state, wheel, direction, x, y);
}

//funcao chamada sempre que a tela for redimensionada.
void reshape (int w, int h)
{
   *scrHeight = h; //atualiza as variaveis da main() com a nova dimensao da tela.
   *scrWidth = w;

   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   //cria uma projecao ortografica com z entre (-1, 1).
#if Y_CANVAS_CRESCE_PARA_CIMA == TRUE
   //parametros: left, right, bottom, top
   gluOrtho2D (0.0, w, 0.0, h); //o eixo y cresce para cima.
#else
   //parametros: left, right, bottom, top
   gluOrtho2D (0.0, w, h, 0.0); //o eixo y cresce para baixo
#endif

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();
}

//definicao de valores para limpar buffers
void inicializa()
{
   glClearColor(1,1,1,1);
   glPolygonMode(GL_FRONT, GL_FILL);
}

void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT );

   //clear(0,0,0);  //tela preta por default. Escolha a cor desejada
   clear(1, 1, 1); //tela branca por default


   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   render();

   glFlush();
   glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////////////
//  inicializa o OpenGL
////////////////////////////////////////////////////////////////////////////////////////
void init(int *w, int *h, const char *title)
{
   int argc = 0;
   glutInit(&argc, NULL);

   scrHeight = h;
   scrWidth = w;

   //habilita MSAA
   //glutSetOption(GLUT_MULTISAMPLE, 4);
   //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

   glutInitWindowSize (*w, *h);
   glutInitWindowPosition (50, 50);
   glutCreateWindow (title);

   inicializa();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyb);
   glutKeyboardUpFunc(keybUp);
   glutSpecialUpFunc(specialUp);
   glutSpecialFunc(special);

   glutIdleFunc(display);
   glutMouseFunc(mouseClick);
   glutPassiveMotionFunc(motion);
   glutMotionFunc(motion);
   glutMouseWheelFunc(mouseWheelCB);

   printf("GL Version: %s", glGetString(GL_VERSION));
}

void run()
{
   glutMainLoop();
}

