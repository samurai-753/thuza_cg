#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>

#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/mesh.h>
#include <vart/transform.h>
#include <vart/cylinder.h>
#include <vart/sphere.h>
#include <vart/material.h>
#include <vart/contrib/viewerglutogl.h>

using namespace std;
using namespace VART;

class Pau : public MeshObject {
    private:
        Cylinder cylinder;
        Transform* tCylinder;
        Sphere s1;
        Sphere s2;
        Sphere s3;
        Transform* tS1;
        Transform* tS2;
        Transform* tS3;

        void MakeSpheres();
        void MakeCylinder();

    public:
        Pau();
        Transform* GetTCylinder() { return tCylinder; }
        Transform* GetTS1() { return tS1; }
        Transform* GetTS2() { return tS2; }
        Transform* GetTS3() { return tS3; }


};

Pau::Pau() {
    MakeSpheres();
    MakeCylinder();

    Transform* rotationCylinder = new Transform();
    rotationCylinder->MakeRotation(Point4D::X(), M_PI/2);
    tCylinder = rotationCylinder;
    tCylinder->AddChild(cylinder);

    double x = cylinder.GetBottomRadius();
    double y = cylinder.GetHeight();
    Transform* translationS1 = new Transform();
    translationS1->MakeTranslation(x, -y, 0);
    tS1 = translationS1;
    tS1->AddChild(s1);

    Transform* translationS2 = new Transform();
    translationS2->MakeTranslation(-x, -y, 0);
    tS2 = translationS2;
    tS2->AddChild(s2);

    Transform* translationS3 = new Transform();
    translationS3->MakeIdentity();
    tS3 = translationS3;
    tS3->AddChild(s3);

    // ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
    ComputeVertexNormals();
}


void Pau::MakeCylinder() {
    cylinder.SetHeight(4);
    cylinder.SetRadius(0.4);
    cylinder.SetMaterial(Material::PLASTIC_RED());
    cylinder.SetPartsVisibility(cylinder.ALL);
    cylinder.ComputeBoundingBox();
}


void Pau::MakeSpheres() {
    s1.SetRadius(0.6);
    s1.SetMaterial(Material::PLASTIC_RED());
    s1.ComputeBoundingBox();

    s2.SetRadius(0.6);
    s2.SetMaterial(Material::PLASTIC_RED());
    s2.ComputeBoundingBox();

    s3.SetRadius(0.45);
    s3.SetMaterial(Material::PLASTIC_RED());
    s3.ComputeBoundingBox();
}


int main(int argc, char** argv) {
    ViewerGlutOGL::Init(&argc, argv);
    static Scene scene;
    static ViewerGlutOGL viewer;

    Camera camera(Point4D(2, 6, 10), Point4D::ORIGIN(), Point4D::Y());

    Pau p;

    scene.AddObject(p.GetTCylinder());
    
    scene.AddObject(p.GetTS1());
    scene.AddObject(p.GetTS2());
    scene.AddObject(p.GetTS3());

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    viewer.SetTitle("PAUZAUM");
    viewer.SetScene(scene);

    scene.DrawLightsOGL();
    ViewerGlutOGL::MainLoop();

    return 0;
}
