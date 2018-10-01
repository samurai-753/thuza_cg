#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>
#include <cmath>

using namespace std;
using namespace VART;

#define NUMBER_OF_CHAIRS 10
#define ANGLE_BETWEEN_CHAIRS ((2.0 * M_PI) / NUMBER_OF_CHAIRS)
#define RADIUS_OF_WHEEL 70

class MyIHClass : public ViewerGlutOGL::IdleHandler {
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        vector<Transform*> translationChair;
        Transform* rotationWheel;

        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            // rotacao da roda no eixo X
            rotationWheel->MakeRotation(Point4D::X(), rotationWheelAngle);
            
            // translada as cadeiras para sua posicao especifica
            double angleTranslationChair = 0;
            // para cada uma das transformacoes de cada cadeira
            for(unsigned int i = 0; i < NUMBER_OF_CHAIRS; i++) {
                // calculo das novas coordenadas
                double y = RADIUS_OF_WHEEL * sin(angleTranslationChair + rotationWheelAngle);
                double z = RADIUS_OF_WHEEL * -cos(angleTranslationChair + rotationWheelAngle);
                // translacao para as novas coordenadas
                translationChair[i]->MakeTranslation(0, y, z);
                // varia o angulo para calcular a translacao da proxima cadeira
                angleTranslationChair += ANGLE_BETWEEN_CHAIRS;
            }

            // varia o angulo de rotacao da roda
            rotationWheelAngle += 0.03;
            viewerPtr->PostRedisplay();
        }
    private:
        // angulo de rotacao da roda gigente
        double rotationWheelAngle = 0;
};

// The application itself:
int main(int argc, char* argv[]) {
    ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static Scene scene; // create a scene
    static ViewerGlutOGL viewer; // create a viewer (application window)
    // create a camera (scene observer)
    Camera camera(Point4D(200,60,0),Point4D::ORIGIN(),Point4D::Y());

    camera.SetFarPlaneDistance(300);
    // leitura dos objetos
    list<MeshObject*> listObjs;
    MeshObject::ReadFromOBJ("ferris-wheel.obj", &listObjs);

    vector<MeshObject*> objects = {begin(listObjs), end(listObjs)};

    MeshObject* chair;
    MeshObject* support;
    MeshObject* wheel;

    // reconhecimento dos obejtos
    for(unsigned int i = 0; i < objects.size(); i++) {
        if(objects[i]->GetDescription() == "chair") {
            chair = objects[i];
        }
        if(objects[i]->GetDescription() == "support") {
            support = objects[i];
        }
        if(objects[i]->GetDescription() == "wheel") {
            wheel = objects[i];
        }
    }


    // criacao das transformacoes da cadeira
    vector<Transform*> tChairs;
    // para cada cadeira
    for(int i = 0; i < NUMBER_OF_CHAIRS; i++) {
        Transform* tChair = new Transform();
        tChair->MakeIdentity();
        // adiciona a cadeira como filha da transformacao
        tChair->AddChild(*chair);
        tChairs.push_back(tChair);
        // adiciona a transformacao na cena
        scene.AddObject(tChairs[i]);
    }

    // criacao da transformacao da roda
    Transform* tWheel = new Transform();
    tWheel->MakeIdentity();
    // adiciona a roda como filha da transformacao
    tWheel->AddChild(*wheel);
    // adiciona a roda na cena
    scene.AddObject(tWheel);
    
    // adiciona o suporte na cena
    scene.AddObject(support);

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // criacao e atribuicao dos valores do idle
    MyIHClass idle;
    idle.rotationWheel = tWheel;
    idle.translationChair = tChairs;

    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene
    // setta o objeto idle como idlehandler
    viewer.SetIdleHandler(&idle);
    
    scene.DrawLightsOGL();
    ViewerGlutOGL::MainLoop();
    return 0;
}
