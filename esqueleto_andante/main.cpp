#include <vart/scene.h>
#include <vart/box.h>
#include <vart/cylinder.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/point4d.h>
#include <vart/camera.h>
#include <vart/transform.h>
#include <vart/arrow.h>
#include <vart/material.h>

#include <iostream>
#include <cmath>

using namespace std;
using namespace VART;

// auxiliares para o controle do teclado
const unsigned int MAX_KEYS = 10; // quantidade maxima de teclas que podem ser usadas
enum Key { UP, DOWN, RIGHT, LEFT }; // teclas uteis
// vetor que indica as teclas pressionadas
bool keyPressed[MAX_KEYS];

// controlador do teclado
class KeyboardHandler : public ViewerGlutOGL::KbHandler {
    public:
        KeyboardHandler() {
            // inicializa o vetor de teclas pressionadas
            for (unsigned int i = 0; i < MAX_KEYS; ++i)
                keyPressed[i] = false;
        }
        virtual void OnKeyDown(int key) {
            // quando uma tecla é pressionada, verifica se é uma das uteis e a marca
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = true;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = true;
                    break;
                case KEY_UP:
                    keyPressed[UP] = true;
                    break;
                case KEY_DOWN:
                    keyPressed[DOWN] = true;
                    break;
            }
        }
        virtual void OnKeyUp(int key) {
            // quando uma tecla é solta, verifica se é uma das uteis e a desmarca
            switch (key) {
                case KEY_LEFT:
                    keyPressed[LEFT] = false;
                    break;
                case KEY_RIGHT:
                    keyPressed[RIGHT] = false;
                    break;
                case KEY_UP:
                    keyPressed[UP] = false;
                    break;
                case KEY_DOWN:
                    keyPressed[DOWN] = false;
                    break;
            }
        }
    private:
};

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler {
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        Transform* movimentTranslation;
        Transform* rotation;
        // ponteiro para a camera da cena para que ela possa ser modificada
        Camera* camera;

        virtual ~MyIHClass() {}
        virtual void OnIdle()  {
            if(keyPressed[LEFT]) {
                // se a seta para a esquerda foi pressionada, aumenta o angulo e rotaciona
                angleYRotation += M_PI/20;
                rotation->MakeRotation(Point4D::Y(), angleYRotation);
            }
            if(keyPressed[RIGHT]) {
                // se a seta para a esquerda foi pressionada, diminui o angulo e rotaciona
                angleYRotation -= M_PI/20;
                rotation->MakeRotation(Point4D::Y(), angleYRotation);
            }
            if(keyPressed[UP]) {
                // se a seta para cima foi aoertada, utiliza o angulo da rotacao para calcular
                // as novas coordenadas de x e z e faz a translacao
                xTranslation += sin(angleYRotation) * speed;
                zTranslation += cos(angleYRotation) * speed;
                movimentTranslation->MakeTranslation(xTranslation, yTranslation, zTranslation);
            }
            if(keyPressed[DOWN]) {
                // se a seta para cima foi aoertada, utiliza o angulo da rotacao para calcular
                // as novas coordenadas de x e z e faz a translacao
                xTranslation += -sin(angleYRotation) * speed;
                zTranslation += -cos(angleYRotation) * speed;
                movimentTranslation->MakeTranslation(xTranslation, yTranslation, zTranslation);
            }
            // altera as coordenadas da camera para que ela se mantenha proxima do esqueleto
            xCamera = sin(angleYRotation) * distCamera + xTranslation;
            zCamera = cos(angleYRotation) * distCamera + zTranslation;
            camera->SetLocation(Point4D(xCamera, yCamera, zCamera));
            // altera o objeto observado da camera para que elea olhe para o esqueleto
            camera->SetTarget(Point4D(xTranslation, yTranslation, zTranslation));

            viewerPtr->PostRedisplay();
        }
        
    private:
        // posicoes do esqueleto
        double xTranslation = 0;
        double yTranslation = 0;
        double zTranslation = 0;
        // angulo de rotacao do esqueleto
        double angleYRotation = 0;
        // velocidade de translacao do esqueleto
        double speed = 2;

        // posicoes da camera
        double xCamera = 0;
        double yCamera = 30;
        double zCamera = 0;
        // distancia da camera em relacao ao esqueleto
        double distCamera = -50;
};




int main(int argc, char* argv[]) {
    ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT


    static Scene scene; // create a scene
    static ViewerGlutOGL viewer; // create a viewer (application window)

    Camera camera(Point4D(0,0,10), Point4D::ORIGIN(), Point4D::Y());
    
    // leitura dos objetos
    list<MeshObject*> objects;
    MeshObject::ReadFromOBJ("skeleton5b/skeleton5b.obj", &objects);
	
    list<MeshObject*>::iterator iter = objects.begin();
    
    // primeira transformacao do esqueleto
    Transform* initialRotation = new Transform();
    initialRotation->MakeRotation(Point4D::X(), -M_PI/2);
    
    // adiciona todos os objetos lidos como filhos da rotacao
    for (iter = objects.begin(); iter != objects.end(); ++iter) {	
        initialRotation->AddChild(**iter);
    }
    
    // translacao do esqueleto para que o pe fique mais proximo da origem
    Transform* initialTranslation = new Transform();
    initialTranslation->MakeTranslation(0, 9.004964, 0);
    // adiciona a rotacao como filha da translacao
    initialTranslation->AddChild(*initialRotation);
    
    
    // cria o chao
    MeshObject box;
    box.MakeBox(-60, 60, 0, 0.1,-60, 60);
    box.SetMaterial(Material::DARK_PLASTIC_GRAY());
    
    scene.AddObject(&box);

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();
    
    // rotacao do usuario
    Transform* rotation = new Transform();
    rotation->MakeIdentity();
    // adiciona a translacao para a origem como filha da rotacao do usuario
    rotation->AddChild(*initialTranslation);
    
    // translacao do usuario
    Transform* movimentTranslation = new Transform();
    movimentTranslation->MakeIdentity();
    // adiciona a rotacao do usuario como filha da translacao
    movimentTranslation->AddChild(*rotation);
    
    // adiciona a translacao na cena
    scene.AddObject(movimentTranslation);
    
    // cria o idle e adiciona os valores
    MyIHClass idle;
    idle.rotation = rotation;
    idle.movimentTranslation = movimentTranslation;
    idle.camera = &camera;
    
    // cria o controlador do teclado
    KeyboardHandler kbh;
    // Set up the viewer
    viewer.SetTitle("V-ART OBJ Viewer");
    viewer.SetScene(scene); // attach the scene
    // setta o controlador do teclado e o idle handler
    viewer.SetKbHandler(&kbh);
    viewer.SetIdleHandler(&idle);
    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}