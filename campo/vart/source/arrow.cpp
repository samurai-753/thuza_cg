//FixMe: Translate to English, put headers.

#include <iostream>
#include "vart/arrow.h"
#include "vart/meshobject.h"
#include "vart/mesh.h"
#include "vart/transform.h"

using namespace std;
using namespace VART;

float VART::Arrow::relativeRadius = 0.02;
float VART::Arrow::relativeBaseLength = 0.90;
float VART::Arrow::relativeHeadRadius = 0.05;

// Construtor que cria uma seta no eixo X de tamanho LENGTH (seta padrao)
VART::Arrow::Arrow(double length){
	Inicializa(length);
}

// Construtor que cria uma seta a partir de um ponto e uma direcao (seta generica)
VART::Arrow::Arrow(Point4D point, Point4D direction)
{
	// calcula a norma do vetor
	Inicializa(direction.Length());
	
	// TRANSFORMACOES QUE LEVAM A SETA PADRAO ATE A SETA GENERICA
	// rotacao em Z
	double angulo;
	Point4D projecao_direction = direction;
	if ((direction.GetX() == 0) && (direction.GetZ() == 0))
		angulo = 1.570796327;
	else{
		projecao_direction.SetY(0);
		angulo = projecao_direction.GenericAngleTo(direction);
		if(direction.GetY() < 0)
			angulo = angulo * (-1);
	}
	Transform rotationZ;
	rotationZ.MakeZRotation(angulo);
	// rotacao em Y
	angulo = projecao_direction.GenericAngleTo(Point4D::X());
	if(projecao_direction.GetZ() > 0)
		angulo = angulo * (-1);
	Transform rotationY;
	rotationY.MakeYRotation(angulo);
	// translacao que leva a origem da seta padrao ate a origem da seta generica
	Transform translacao2;
	translacao2.MakeTranslation(point);
	
	//MATRIZ DE TRANSFORMACAO
	Transform transformacao;
	transformacao = translacao2 * rotationY * rotationZ;
	/* aplico essa transformacao a cada um dos pontos da seta padrao para que ela
	se transforme na seta generica */
	ApplyTransform(transformacao);
}

// metodo que inicializa uma seta. Este metodo eh usado nos construtores para
// gerar uma seta.
void VART::Arrow::Inicializa(double length)
{
	double radius, baseLength, headRadius;
	
	radius = length * relativeRadius;
	baseLength = length * relativeBaseLength;
	headRadius = length * relativeHeadRadius;
	
	// Array com as coordenadas dos vertices
    double coordinateArray[] = {0, -radius, -radius,    				//0
								0, -radius, radius,     				//1
								0, radius, radius,						//2
								0, radius, -radius,						//3
								baseLength, -radius, -radius,			//4
								baseLength, -radius, radius,			//5
								baseLength, radius, radius,				//6
								baseLength, radius, -radius,			//7
								baseLength, -headRadius, -headRadius,	//8 face 6
								baseLength, -headRadius, headRadius,	//9 face 6
								baseLength, headRadius, headRadius,		//10 face 6
								baseLength, headRadius, -headRadius,	//11 face 6
								length, 0, 0,							//12 face 7
								baseLength, -headRadius, -headRadius,	//8 face 9
								baseLength, -headRadius, -headRadius,	//8 face 10
								baseLength, -headRadius, headRadius,	//9 face 7
								baseLength, -headRadius, headRadius,	//9 face 10
								baseLength, headRadius, headRadius,		//10 face 7
								baseLength, headRadius, headRadius,		//10 face 8
								baseLength, headRadius, -headRadius,	//11 face 8
								baseLength, headRadius, -headRadius,	//11 face 9
								length, 0, 0,							//12 face 8
								length, 0, 0,							//12 face 9
								length, 0, 0};							//12 face 10
                           
    double* endOfCoordinateArray = coordinateArray + sizeof(coordinateArray)/sizeof(double);
   
	// Array que define as faces quadradas
	unsigned int indexArrayQuadraticFaces[] = { 0,1,2,3,		// face 1
                                  				1,5,6,2,		// face 2
                                  				2,6,7,3,		// face 3
								  				3,7,4,0,		// face 4
								  				4,5,1,0,		// face 5
								  				8,9,10,11};		// face 6
	unsigned int* endOfIndexArrayQuadraticFaces = indexArrayQuadraticFaces + sizeof(indexArrayQuadraticFaces)/sizeof(int);								  
	
	// Array que define as faces triangulares
	unsigned int indexArrayTriangularFaces[] = {12,10,9,	// face 7
									 			12,11,10,	// face 8
									 			11,12,8,	// face 9
									 			8,12,9};	// face 10		
                             
    unsigned int* endOfIndexArrayTriangularFaces = indexArrayTriangularFaces + sizeof(indexArrayTriangularFaces)/sizeof(int);

	// creates the base of the arrow
    VART::Mesh meshQuadratic;
    vertCoordVec.assign(coordinateArray,endOfCoordinateArray);
    meshQuadratic.type = VART::Mesh::QUADS;
    meshQuadratic.indexVec.assign(indexArrayQuadraticFaces,endOfIndexArrayQuadraticFaces);
    meshQuadratic.material = VART::Material::PLASTIC_GREEN(); // default material
    meshList.push_back(meshQuadratic);

	// creates the head of the arrow
	VART::Mesh meshTriangular;
	vertCoordVec.assign(coordinateArray,endOfCoordinateArray);
	meshTriangular.type = VART::Mesh::TRIANGLES;
	meshTriangular.indexVec.assign(indexArrayTriangularFaces,endOfIndexArrayTriangularFaces);
	meshTriangular.material = VART::Material::PLASTIC_GREEN(); // default material
	meshList.push_back(meshTriangular);

    ComputeVertexNormals();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}
