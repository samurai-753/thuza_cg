#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/scene.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/transform.h>


using namespace std;
using namespace VART;

class MyIH : public ViewerGlutOGL::IdleHandler {
    private:
        // angulo de rotacao da bolas
        double rotationBolaParameter = 0.1;
        
    public:
        // transformacoes que ocorrerao durante a execucao do programa
        Transform* rotationXBola;
        Transform* rotationYBola;
        MyIH() {
        }
        virtual ~MyIH(){
        }

        virtual void OnIdle() {
            // faz a rotacao em X da bola
            rotationXBola->MakeRotation(Point4D::X(), rotationBolaParameter);
            // faz a rotacao em Y da bola
            rotationYBola->MakeRotation(Point4D::Y(), rotationBolaParameter);
            // varia o angulo de rotacao
            rotationBolaParameter += 0.1;

            viewerPtr->PostRedisplay();
        }
};



int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "AMANDA FOTO" << endl;
        return 0;
    }

    ViewerGlutOGL::Init(&argc, argv);
    static ViewerGlutOGL viewer;
    static Scene scene;

    Camera camera(Point4D(0, 0, 20), Point4D::ORIGIN(), Point4D::Y());

    Texture text;
    Material mat;
    Sphere bola;
    Transform tBola;
    Transform rotationXBola;
    Transform rotationYBola;

    // se foi possivel ler a textura
    if(text.LoadFromFile(argv[1])) {
        // adiciona a textura à bola
        mat.SetTexture(text);
        bola.SetMaterial(mat);
        bola.SetDescription("BOLA");

        // faz transformacao de escala
        tBola.MakeScale(6, 6, 6);
        // adiciona a bola como filha da escala
        tBola.AddChild(bola);

        // cria uma identidade para as rotacoes
        rotationXBola.MakeIdentity();
        rotationYBola.MakeIdentity();

        // adiciona a escala como filha da rotacao em X
        rotationXBola.AddChild(tBola);
        // adiciona a rotacao em X como filha da rotacao em Y
        rotationYBola.AddChild(rotationXBola);

    } else {
        cout << "FOTO AMANDADA NAO ACHADA TEU" << endl;
        return 0;
    }


    // setta os valores do idle
    MyIH idle;
    idle.rotationXBola = &rotationXBola;
    idle.rotationYBola = &rotationYBola;

    // adiciona a rotacao em Y na cena
    scene.AddObject(&rotationYBola);
    camera.SetDescription( "camera" );
    scene.AddCamera(&camera);

    //Add a light to the scene.
    scene.AddLight( Light::BRIGHT_AMBIENT() );
    scene.DrawLightsOGL();

    //Setup the viewer.
    viewer.SetTitle("V-ART BOLA");
    viewer.SetScene(scene);
    viewer.SetIdleHandler(&idle);
    //Enable texture in OpenGL
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Enter the main loop
    VART::ViewerGlutOGL::MainLoop();
}
