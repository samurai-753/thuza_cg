#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/mesh.h>
#include <vart/transform.h>
#include <vart/texture.h>
#include <vart/material.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace VART;


#define TAM_CAMPO 40
#define MAX_ANGLE M_PI/4
#define VARIATION 3
#define BALL_SIZE 4

class Campo : public MeshObject {
    private:

        double MyRandom();
    protected:
    public:
        Campo();

};

Campo::Campo() {
    srand(time(NULL));
    vector<double> coordenadas;
    for(int i = 0; i < TAM_CAMPO; i++) {
        for(int j = 0; j < TAM_CAMPO; j++) {
            coordenadas.push_back(i * VARIATION);
            double y = this->MyRandom();
            coordenadas.push_back(y);
            coordenadas.push_back(j * VARIATION);
        }
    }
    vertCoordVec = coordenadas;

    vector<unsigned int> faces;

    for(int i = 0; i < TAM_CAMPO - 1; i++) {
        for(int j = 0; j < TAM_CAMPO; j++) {
            faces.push_back((i + 1) * TAM_CAMPO + j);
            faces.push_back(i * TAM_CAMPO + j);
        }
        Mesh meshTriangStrip;
        meshTriangStrip.type = Mesh::TRIANGLE_STRIP;
        meshTriangStrip.indexVec = faces;
        meshTriangStrip.material = Material::PLASTIC_GREEN();
        meshList.push_back(meshTriangStrip);
    }
    ComputeVertexNormals();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

double Campo::MyRandom() {
    int vAux = (int) ((VARIATION * tan(MAX_ANGLE)) * 101);
    double y = (rand() % vAux)/100.0;
    return y;
}

class MyIH : public ViewerGlutOGL::IdleHandler {
    private:
        double rotationBolaParameter = -(MAX_ANGLE);
        double translationXParamater = 0.1;
        double translationYParamater = VARIATION + 1.5 * BALL_SIZE + 0.1;
        double translationZParameter = 0.1;
        double variationXTranslation = ((double) VARIATION + BALL_SIZE)/BALL_SIZE;
        double variationYTranslation = TAM_CAMPO/(VARIATION + BALL_SIZE);
        double variationZTranslation = ((double) VARIATION)/BALL_SIZE;

    public:
        Transform* rotationXBola;
        Transform* rotationYBola;
        Transform* translationBola;
        MyIH() {
        }
        virtual ~MyIH(){
        }

        virtual void OnIdle() {
            rotationXBola->MakeRotation(Point4D::X(), rotationBolaParameter);
            rotationYBola->MakeRotation(Point4D::Y(), rotationBolaParameter);
            // cout << *rotationXBola << endl;

            if((translationXParamater >= TAM_CAMPO * VARIATION) or
              (translationXParamater <= TAM_CAMPO * Point4D::ORIGIN().GetX())) {
                variationXTranslation = -variationXTranslation;
                rotationBolaParameter = -rotationBolaParameter;
            }
            if((translationZParameter >= TAM_CAMPO * VARIATION) or
              (translationZParameter <= TAM_CAMPO * Point4D::ORIGIN().GetZ())) {
                variationZTranslation = -variationZTranslation;
            }
            if((translationYParamater >= TAM_CAMPO + VARIATION) or
              (translationYParamater <= VARIATION + BALL_SIZE + (0.5 * BALL_SIZE))) {
                variationYTranslation = -variationYTranslation;
            }
            translationBola->MakeTranslation(translationXParamater, translationYParamater, translationZParameter);

            translationXParamater += variationXTranslation;
            translationYParamater += variationYTranslation;
            translationZParameter += variationZTranslation;
            rotationBolaParameter += 0.1;

            // cout << translationXParamater << " " << translationYParamater << " " << translationZParameter << endl;

            viewerPtr->PostRedisplay();
        }
};

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "AMANDA FOTO" << endl;
        return 0;
    }

    ViewerGlutOGL::Init(&argc, argv);
    static Scene scene;
    static ViewerGlutOGL viewer;
    Camera camera(Point4D(0, 120, 0),
                  Point4D((VARIATION * TAM_CAMPO) / 2.0, 0, (VARIATION * TAM_CAMPO) / 2.0), Point4D::Y());
    camera.SetFarPlaneDistance(300.0);

    Texture text;
    Material mat;
    Sphere bola;
    Transform tBola;
    Transform rotationXBola;
    Transform rotationYBola;
    Transform translationMiddleBola;

    if(text.LoadFromFile(argv[1])) {
        mat.SetTexture(text);
        // Color c(0, 255, 255);
        // mat.SetAmbientColor(c);
        bola.SetMaterial(mat);
        bola.SetDescription("BOLA");

        tBola.MakeScale(BALL_SIZE, BALL_SIZE, BALL_SIZE);
        tBola.AddChild(bola);

        rotationXBola.MakeIdentity();
        rotationYBola.MakeIdentity();

        rotationXBola.AddChild(tBola);
        rotationYBola.AddChild(rotationXBola);

        translationMiddleBola.MakeTranslation(Point4D((VARIATION * TAM_CAMPO) / 2.0,
                                                        VARIATION + BALL_SIZE, (VARIATION * TAM_CAMPO) / 2.0));
        translationMiddleBola.AddChild(rotationYBola);

    } else {
        cout << "FOTO AMANDADA NAO ACHADA TEU" << endl;
        return 0;
    }


    MyIH idle;
    idle.rotationXBola = &rotationXBola;
    idle.rotationYBola = &rotationYBola;
    idle.translationBola = &translationMiddleBola;

    scene.AddObject(&translationMiddleBola);

    Campo c;
    scene.AddObject(&c);

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    viewer.SetTitle("PAUZAUM");
    viewer.SetScene(scene);
    viewer.SetIdleHandler(&idle);
    scene.DrawLightsOGL();
    ViewerGlutOGL::MainLoop();

    return 0;
}
