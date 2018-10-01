#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/cylinder.h>
#include <vart/contrib/viewerglutogl.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace VART;


class Assento : public MeshObject {
    public:
        Assento(double larguraX, double larguraZ, double altura, double alturaAssento, double origem[]);
};

Assento::Assento(double larguraX, double larguraZ, double altura, double alturaAssento, double origem[]) {
    vector<double> coordenadas({origem[0], origem[1] + altura + alturaAssento, origem[2], // quadrado de cima
                                origem[0], origem[1] + altura + alturaAssento, origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1] + altura + alturaAssento, origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1] + altura + alturaAssento, origem[2],
                                origem[0], origem[1] + altura, origem[2], // quadrado de baixo
                                origem[0], origem[1] + altura, origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1] + altura, origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1] + altura, origem[2]
                            });

    vertCoordVec = coordenadas;
    vector<unsigned int> faces({0, 1, 2, 3,
                                7, 6, 5, 4,
                                0, 4, 5, 1,
                                1, 5, 6, 2,
                                2, 6, 7, 3,
                                3, 7, 4, 0
                                });
    Mesh meshQuads;
    meshQuads.type = Mesh::QUADS;
    meshQuads.material = Material::PLASTIC_WHITE();
    meshQuads.indexVec = faces;
    meshList.push_back(meshQuads);

    ComputeVertexNormals();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();

}

class Encosto : public MeshObject {
    public:
        Encosto(double larguraX, double larguraZ, double alturaAssento, double origem[]);
};

Encosto::Encosto(double larguraX, double larguraZ, double alturaAssento, double origem[]) {
    vector<double> coordenadas({origem[0], origem[1] + alturaAssento, origem[2], // quadrado de cima
                                origem[0], origem[1] + alturaAssento, origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1] + alturaAssento, origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1] + alturaAssento, origem[2],
                                origem[0], origem[1], origem[2], // quadrado de baixo
                                origem[0], origem[1], origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1], origem[2] + larguraZ,
                                origem[0] + larguraX, origem[1], origem[2]
                            });

    vertCoordVec = coordenadas;
    vector<unsigned int> faces({0, 1, 2, 3,
                                7, 6, 5, 4,
                                0, 4, 5, 1,
                                1, 5, 6, 2,
                                2, 6, 7, 3,
                                3, 7, 4, 0
                                });
    Mesh meshQuads;
    meshQuads.type = Mesh::QUADS;
    meshQuads.material = Material::PLASTIC_WHITE();
    meshQuads.indexVec = faces;
    meshList.push_back(meshQuads);

    ComputeVertexNormals();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();

}



class Pe : public MeshObject {
    private:
        Cylinder* pe;
        double tamanhoPe;
        double raioPe;
    public:

        Transform* translationPe;
        Transform* rotationPe;

        Pe(double tPe, double rPe);
        ~Pe();
        double GetTamanho() { return tamanhoPe; }
        double GetRaio() { return raioPe; }
};

Pe::Pe(double tPe = 1, double rPe = 0.1) {
    tamanhoPe = tPe;
    raioPe = rPe;

    pe = new Cylinder();
    pe->SetRadius(raioPe);
    pe->SetHeight(tamanhoPe);
    pe->SetMaterial(Material::PLASTIC_WHITE());
    pe->SetPartsVisibility(Cylinder::ALL);

    rotationPe = new Transform();
    rotationPe->AddChild(*pe);

    translationPe = new Transform();
    translationPe->AddChild(*rotationPe);
}


Pe::~Pe() {
    delete pe;
    delete translationPe;
    delete rotationPe;
}

class Cadeira : public MeshObject {
    private:
        Pe* pes;
        double larguraX;
        double larguraZ;
        double alturaPe;
        double origemPe[3];

        Assento* assento;
        double tamAssento = 0.1;

        Encosto* encosto;
        Transform* rotationEncosto;
        Transform* translationEncosto;

        void MakePes();
        void MakeAssento();
        void MakeEncosto();
    public:
        Transform* transformCadeira;
        Cadeira();
        ~Cadeira();
};

Cadeira::Cadeira() {
    transformCadeira = new Transform();
    transformCadeira->MakeIdentity();

    MakePes();
    MakeAssento();
    MakeEncosto();

}

Cadeira::~Cadeira() {
    delete transformCadeira;
    delete assento;
    delete encosto;
    delete rotationEncosto;
    delete translationEncosto;
    delete[] pes;
}

void Cadeira::MakePes() {
    pes = new Pe[4];


    for(int i = 0; i < 4; i++) {
        pes[i].rotationPe->MakeRotation(Point4D::X(), M_PI/2);
    }

    pes[0].translationPe->MakeTranslation(0, pes[0].GetTamanho(), 0);
    pes[1].translationPe->MakeTranslation(pes[0].GetTamanho()/2, pes[1].GetTamanho(), 0);
    pes[2].translationPe->MakeTranslation(0, pes[2].GetTamanho(), pes[2].GetTamanho());
    pes[3].translationPe->MakeTranslation(pes[0].GetTamanho()/2, pes[3].GetTamanho(), pes[3].GetTamanho());

    larguraX = pes[0].GetTamanho() / 2 + 2 * pes[0].GetRaio();
    larguraZ = pes[0].GetTamanho() + 2 * pes[0].GetRaio();
    origemPe[0] = -pes[0].GetRaio();
    origemPe[1] = 0;
    origemPe[2] = -pes[0].GetRaio();
    alturaPe = pes[0].GetTamanho();

    for(int i = 0; i < 4; i++) {
        this->AddChild(*(pes[i].translationPe));
    }

}

void Cadeira::MakeAssento() {
    assento = new Assento(larguraX, larguraZ, alturaPe, tamAssento, origemPe);
    this->AddChild(*assento);
}

void Cadeira::MakeEncosto() {
    encosto = new Encosto(larguraX, larguraZ, 0.1, origemPe);
    rotationEncosto = new Transform();
    rotationEncosto->MakeRotation(Point4D::X(), M_PI/3);
    rotationEncosto->AddChild(*encosto);
    translationEncosto = new Transform();
    translationEncosto->MakeTranslation(Point4D(0, alturaPe + (larguraZ * sin(M_PI/3)), larguraZ / 2 * cos(M_PI)));
    translationEncosto->AddChild(*rotationEncosto);
    this->AddChild(*translationEncosto);

}


int main(int argc, char* argv[]) {
    ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static Scene scene; // create a scene
    static ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    Camera camera(Point4D(0,0,6),Point4D::ORIGIN(),Point4D::Y());

    // Create some objects
    Arrow arrowX(Point4D::ORIGIN(), Point4D::X()*2);
    Arrow arrowY(Point4D::ORIGIN(), Point4D::Y()*2);
    Arrow arrowZ(Point4D::ORIGIN(), Point4D::Z()*2);

    // Initialize scene objects
    arrowX.SetMaterial(Material::PLASTIC_RED());
    arrowY.SetMaterial(Material::PLASTIC_GREEN());
    arrowZ.SetMaterial(Material::PLASTIC_BLUE());

    // Build the scene graph

    Cadeira cad;

    scene.AddObject(&cad);


    scene.AddObject(&arrowX);
    scene.AddObject(&arrowY);
    scene.AddObject(&arrowZ);

    // Add lights and cameras to the scene
    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the viewer
    viewer.SetTitle("CADEIRA");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
