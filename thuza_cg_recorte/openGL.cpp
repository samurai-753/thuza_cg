// Este programa desenha linhas com o mouse.

// Historico de alteracoes:
// Maio de 2014 - Bruno Schneider - criado a partido do basicoDesenho.cpp

#include "line.h"
#include "clip-rectangle.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

vector<Line*> linePtrVec;
vector<Line*> clippedLinePtrVec;
ClipRectangle clipRectangle;
int windowLength = 800;
int windowHeight = 600;

Point WorldCoordinates(int x, int y)
{
    double wX = static_cast<double>(2*x) / windowLength - 1;
    double wY = static_cast<double>(windowHeight-y) * 2 / windowHeight - 1;
    return Point(wX, wY);
}

// Os dois primeiros pontos definem a area de recorte. Cada par a seguir define um
// segmento de reta.
void OnClick(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        static Point p0;
        static Point p1;
        static int clickCount = 0;
        ++clickCount;
        if (clickCount % 2 == 1)
            p0 = WorldCoordinates(x, y);
        else
        {
            p1 = WorldCoordinates(x, y);
            if (clipRectangle.Initialized())
            {
                Line* linePtr = new Line(p0, p1);
                linePtrVec.push_back(linePtr);
                Line* clippedLinePtr = clipRectangle.Clip(*linePtr);
                if (clippedLinePtr)
                    clippedLinePtrVec.push_back(clippedLinePtr);
                //linePtr->calculaRecorte();
            }
            else
            {   // usuario terminou de definir o retangulo de recorte
                if (p0.mX < p1.mX)
                {
                    if (p0.mY < p1.mY)
                        clipRectangle.Set(p0.mX, p1.mX, p0.mY, p1.mY);
                    else
                        clipRectangle.Set(p0.mX, p1.mX, p1.mY, p0.mY);
                }
                else
                {
                    if (p0.mY < p1.mY)
                        clipRectangle.Set(p1.mX, p0.mX, p0.mY, p1.mY);
                    else
                        clipRectangle.Set(p1.mX, p0.mX, p1.mY, p0.mY);
                }
            }
            glutPostRedisplay();
        }
    }
}

void CleanUp()
{
    // desalocar a memoria de todas as linhas
    vector<Line*>::iterator iter = linePtrVec.begin();
    for (; iter != linePtrVec.end(); ++iter)
        delete *iter;
    iter = clippedLinePtrVec.begin();
    for (; iter != clippedLinePtrVec.end(); ++iter)
        delete *iter;
}

// Chamada pela GLUT quando a janela e' redimensionada.
void ChangeSize(GLsizei w, GLsizei h)
{
    windowLength = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

// Funcao de callback de desenho
// Executada sempre que e' necessario re-exibir a imagem
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (clipRectangle.Initialized())
    {
        glBegin(GL_LINES);
        // Desenhar o retangulo de recorte
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2d(clipRectangle.XMin(), clipRectangle.YMin());
        glVertex2d(clipRectangle.XMax(), clipRectangle.YMin());
        glVertex2d(clipRectangle.XMax(), clipRectangle.YMin());
        glVertex2d(clipRectangle.XMax(), clipRectangle.YMax());
        glVertex2d(clipRectangle.XMax(), clipRectangle.YMax());
        glVertex2d(clipRectangle.XMin(), clipRectangle.YMax());
        glVertex2d(clipRectangle.XMin(), clipRectangle.YMax());
        glVertex2d(clipRectangle.XMin(), clipRectangle.YMin());
        // Desenhar cada linha
        glColor3f(1.0f, 1.0f, 1.0f);
        vector<Line*>::iterator iter = linePtrVec.begin();
        for (; iter != linePtrVec.end(); ++iter)
        {
              glVertex2d((*iter)->mP0.mX, (*iter)->mP0.mY);
              glVertex2d((*iter)->mP1.mX, (*iter)->mP1.mY);
        }
        // Desenhar cada linha recortada
        glColor3f(0.0f, 1.0f, 0.0f);
        iter = clippedLinePtrVec.begin();
        for (; iter != clippedLinePtrVec.end(); ++iter)
        {
              glVertex2d((*iter)->mP0.mX, (*iter)->mP0.mY);
              glVertex2d((*iter)->mP1.mX, (*iter)->mP1.mY);
        }
        glEnd();
    }
    glFlush();
}

// Inicializa aspectos do rendering
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // cor de fundo da janela
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    if ((key == 27) or (key == 'q')) // se for a tecla ESC ou 'q'...
        exit(0);                     // ...termina o programa
}

// Parte principal - ponto de inicio de execucao
// Cria janela
// Inicializa aspectos relacionados a janela e a geracao da imagem
// Especifica a funcao de callback de desenho
int main(int argc, char* argv[])
{
    // Procedimento de inicializacao da GLUT
    glutInit(&argc, argv);

    // Indica que deve ser usado um unico frame buffer e representacao de cores RGB
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Cria uma janela com o titulo especificado
    glutInitWindowSize(windowLength, windowHeight);
    glutCreateWindow("Teste Recorte");

    // Especifica para a OpenGL que funcao deve ser chamada para geracao da imagem
    glutDisplayFunc(RenderScene);

    // Especifica a funcao que vai tratar teclas comuns (letras, numeros)
    glutKeyboardFunc(KeyboardFunc);

    // Determina o callback de mouse
    glutMouseFunc(OnClick);

    // Determina o callback de redimensionamento da janela.
    glutReshapeFunc(ChangeSize);

    // Executa a inicializacao de parametros de exibicao
    clippedLinePtrVec.reserve(25);
    linePtrVec.reserve(25);
    SetupRC();

    // Registrar a funcao de liberar recursos
    atexit(CleanUp);

    cout << "Entre com o retangulo de recorte, clicando no canto inferior esquerdo "
         << "e depois no canto superior direito.\n"
         << "Tecle [ESC] para terminar a execucao.\n";
    // Entra no loop de tratamento de eventos da GLUT
    glutMainLoop();
    return 0;
}

