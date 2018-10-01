
#include<iostream>
// bibliotecas para criar cena
#include<vart/scene.h>
#include<vart/light.h>
#include <vart/contrib/viewerglutogl.h>
// bibliotecas para criar a piramide
#include<vart/meshobject.h>

using namespace std;

class Casa : public VART::MeshObject {
    public:
        Casa();

};

Casa::Casa() {
    double coordenadas[] = {-1, 0, 1,   //primeiro ponto da base
                            -1, 0, -1,
                            1, 0, -1,
                            1, 0, 1,    //ultimo ponto da base
                            0, 1, 0,     //topo da piramide
                            -1, -1, 1,  //primeiro ponto do cubo
                            -1, -1, -1,
                            1, -1, -1,
                            1, -1, 1    //ultimo ponto do cubo
                        };
    // calcula a posicao final do array
    double* fimDoArrayCoordenadas = coordenadas + sizeof(coordenadas)/sizeof(double);

    // insere as coordenadas do mesh
    vertCoordVec.assign(coordenadas, fimDoArrayCoordenadas);

    // seleciona os vertices que formam as paredes e a base
    unsigned int verticesCubo[] = {5, 6, 7, 8,
                                   5, 8, 3, 0,
                                   6, 5, 0, 1,
                                   7, 6, 1, 2,
                                   8, 7, 2, 3
                                };
    unsigned int* fimDoArrayVerticesCubo = verticesCubo + sizeof(verticesCubo)/sizeof(unsigned int);

    // cria os quadrados para formar as paredes e a base 
    VART::Mesh meshBase;
    meshBase.type = VART::Mesh::QUADS;
    meshBase.indexVec.assign(verticesCubo, fimDoArrayVerticesCubo);
    meshBase.material = VART::Material::PLASTIC_GREEN();
    meshList.push_back(meshBase);

    // seleciona os vertices que formam o telhado
    unsigned int verticesTriangulos[] = {4, 0, 3,
                                         4, 1, 0,
                                         4, 2, 1,
                                         4, 3, 2
                                    };
    unsigned int* fimDoArrayVerticesTriangulos = verticesTriangulos + sizeof(verticesTriangulos)/sizeof(unsigned int);

    // cria os triangulos para formar o telhado
    VART::Mesh meshTriangulos;
    meshTriangulos.type = VART::Mesh::TRIANGLES;
    meshTriangulos.indexVec.assign(verticesTriangulos, fimDoArrayVerticesTriangulos);
    meshTriangulos.material = VART::Material::PLASTIC_BLUE();
    meshList.push_back(meshTriangulos);

    ComputeVertexNormals();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();

}

int main(int argv, char** argc) {
    // cria cena, camera e vizualizador
    VART::ViewerGlutOGL::Init(&argv, argc);
    static VART::Scene scene;
    static VART::ViewerGlutOGL viwer;

    VART::Camera camera(VART::Point4D(5,0,10), VART::Point4D::ORIGIN(), VART::Point4D::Y());

    Casa c;

    scene.AddObject(&c);

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);


    viwer.SetTitle("MEU FIRST CG TREM");
    viwer.SetScene(scene);

    scene.DrawLightsOGL();
    VART::ViewerGlutOGL::MainLoop();
    return 0;
}
