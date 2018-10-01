#include <iostream>
#include "vart/arrow.h"
#include "vart/graphicobj.h"
#include "vart/point4d.h"
#include "vart/refsystem.h"
#include "vart/scenenode.h"
#include "vart/arrow.h"
#include "vart/transform.h"
#include <GL/glu.h>
#include <sstream>
#include <cassert>
#include <fstream>


using namespace std;
using namespace VART;

double VART::RefSystem::axisLength = 2.0;

// build a default reference system
VART::RefSystem::RefSystem() : axisX(axisLength), axisY(axisLength), axisZ(axisLength)
{
	axisX.SetMaterial(VART::Material::PLASTIC_RED ());
	
	// Transformacao que leva o eixo Y para a direcao (0, 1, 0)
	Transform rotationAxisY;
	rotationAxisY.MakeZRotation(1.57);
	axisY.ApplyTransform(rotationAxisY);
	axisY.SetMaterial(VART::Material::PLASTIC_GREEN ());
	
	// Transformacao que leva o eixo Z para a direcao (0, 0, 1)
	Transform rotationAxisZ;
	rotationAxisZ.MakeYRotation(-1.57);
	axisZ.ApplyTransform(rotationAxisZ);
	axisZ.SetMaterial(VART::Material::PLASTIC_BLUE ());
}

// hide the axis Z
void VART::RefSystem::HideZ()
{	
	axisZ.Hide();
}

// virtual
VART::SceneNode * VART::RefSystem::Copy()
{
	// FixMe
}

// virtual
void VART::RefSystem::ComputeBoundingBox()
{
	double headRadius;
	headRadius = VART::Arrow::relativeHeadRadius * axisLength; 
	bBox.SetBoundingBox(-headRadius, -headRadius, -headRadius, axisLength, axisLength, axisLength);
}

// virtual
bool VART::RefSystem::DrawInstanceOGL() const {
	axisX.DrawOGL();
	axisY.DrawOGL();
	axisZ.DrawOGL();
}
