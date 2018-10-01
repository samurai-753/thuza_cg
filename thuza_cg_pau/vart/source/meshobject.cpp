/// \file meshobject.cpp
/// \brief Implementation file for V-ART class "MeshObject".
/// \version $Revision: 1.14 $

#include "vart/meshobject.h"
#include "vart/file.h"
#include <sstream>
#include <cassert>
#include <fstream>
#include <cstdlib>
#include <algorithm> // transform
#include <cctype> // tolower

using namespace std;

float VART::MeshObject::sizeOfNormals = 0.1f;

// === Auxiliary functions ===
unsigned int CountOccurrences(char c, string s)
{
    unsigned int occurrences = 0;
    string::size_type pos = s.find(c);
    while (pos != string::npos)
    {
        ++occurrences;
        pos = s.find(c, pos+1);
    }
    return occurrences;
}

// === Member funcitions ===
VART::MeshObject::MeshObject()
{
    howToShow = FILLED;
}

VART::MeshObject::MeshObject(const VART::MeshObject& obj)
{
    this->operator=(obj);
}

VART::MeshObject& VART::MeshObject::operator=(const VART::MeshObject& obj)
{
    this->GraphicObj::operator =(obj);
    vertVec = obj.vertVec;
    vertCoordVec = obj.vertCoordVec;
    normVec = obj.normVec;
    normCoordVec = obj.normCoordVec;
    textCoordVec = obj.textCoordVec;
    meshList = obj.meshList;
    return *this;
}

VART::SceneNode * VART::MeshObject::Copy()
{
    return new VART::MeshObject(*this);
}

void VART::MeshObject::Clear()
{
    vertVec.clear();
    vertCoordVec.clear();
    normVec.clear();
    normCoordVec.clear();
    textCoordVec.clear();
    meshList.clear();
}

void VART::MeshObject::SetMaterial(const VART::Material& mat)
{
    list<VART::Mesh>::iterator iter;
    for (iter = meshList.begin(); iter != meshList.end(); ++iter)
        iter->material = mat;
}

void VART::MeshObject::SetVertices(const std::vector<VART::Point4D>& vertexVec)
{
    vertCoordVec.clear();
    unsigned int numberOfVertex = vertexVec.size();
    vertCoordVec.reserve(numberOfVertex * 3);
    // Fill vertCoordVec (optimized vertices)
    for (unsigned int i = 0; i < numberOfVertex; ++i) {
        vertCoordVec.push_back(vertexVec[i].GetX());
        vertCoordVec.push_back(vertexVec[i].GetY());
        vertCoordVec.push_back(vertexVec[i].GetZ());
    }
    // Copy the vertVec (unoptimized vertices) as well
    vertVec = vertexVec;
    meshList.clear();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VART::MeshObject::SetNormals(const vector<Point4D>& normalVec)
{
    normVec = normalVec;
    meshList.clear(); // FixMe: Why clear the meshlist?
    ComputeBoundingBox(); // FixMe: Why recompute the bounding box?
    ComputeRecursiveBoundingBox();
}

void VART::MeshObject::SetVertices(const char* vertexStr)
{
    string valStr = vertexStr;
    istringstream iss(valStr);
    double x,y,z,w;
    bool notFinished = true;
    VART::Point4D point;

    vertVec.clear();
    do {
        if (!(iss >> x >> y >> z)) // Try to read 3 values
            notFinished = false; // signal end of parsing
        else
        {
            if (!(iss >> w)) // try to read 4th value
            {
                w = 1.0; // use default value
                iss.clear(); // erase error flags
            }
            point.SetXYZW(x,y,z,w);
            vertVec.push_back(point);
            iss >> ws; // skip possible white space before comma
            iss.get(); // skip the comma
        }
    } while (notFinished);
    meshList.clear();
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VART::MeshObject::SetVertex(unsigned int index, const VART::Point4D& newValue)
{
    if (vertVec.empty())
    {
        unsigned int newIndex = index*3;
        vertCoordVec[newIndex] = newValue.GetX();
        vertCoordVec[newIndex+1] = newValue.GetY();
        vertCoordVec[newIndex+2] = newValue.GetZ();
    }
    else
    { // vertVec is not empty
        vertVec[index] = newValue;
    }
}

VART::Point4D VART::MeshObject::GetVertex(unsigned int pos)
{
    if (vertVec.empty())
    {
        unsigned int newPos = pos*3;
        return VART::Point4D(vertCoordVec[newPos],vertCoordVec[newPos+1],vertCoordVec[newPos+2]);
    }
    else
    { // vertVec is not empty
        return vertVec[pos];

    }
}

void VART::MeshObject::AddNormal(unsigned int idx, const Point4D& vec)
{
    unsigned int coordIdx = idx*3;
    normCoordVec[coordIdx] += vec.GetX();
    ++coordIdx;
    normCoordVec[coordIdx] += vec.GetY();
    ++coordIdx;
    normCoordVec[coordIdx] += vec.GetZ();
}

unsigned int VART::MeshObject::NumFaces()
{
    unsigned int result = 0;
    list<Mesh>::iterator iter = meshList.begin();
    // for each mesh
    for (; iter != meshList.end(); ++iter)
    {
        switch (iter->type)
        {
            case Mesh::QUADS:
                result += iter->indexVec.size() / 4;
            break;
            case Mesh::TRIANGLES:
                result += iter->indexVec.size() / 3;
            break;
            default:
                cerr << "Error: MeshObject::NumFaces not implementes for meshes of type "
                     << static_cast<int>(iter->type) << "\n";
                exit(1);
        }
    }
    return result;
}

void VART::MeshObject::SmallerVertex(Point4D* resultPtr)
{
    if (vertVec.empty())
    { // optimized representation
        Point4D smaller(vertCoordVec[0],vertCoordVec[1],vertCoordVec[2]);
        Point4D temp;
        unsigned int size = vertCoordVec.size();
        for (unsigned int i=3; i < size; i += 3)
        {
            // We want to use Point4D's flexible comparison, so we put coordinates inside a Point4D.
            temp.SetXYZ(vertCoordVec[i],vertCoordVec[i+1],vertCoordVec[i+2]);
            if (temp < smaller)
                smaller = temp;
        }
        *resultPtr = smaller;
    }
    else
    { // unoptimized representation
        Point4D smaller = vertVec[0];
        unsigned int size = vertVec.size();
        for (unsigned int i=1; i < size; ++i)
        {
            if (vertVec[i] < smaller)
                smaller = vertVec[i];
        }
        *resultPtr = smaller;
    }
}

VART::Point4D VART::MeshObject::GetVertexMedia()
// deprecated
// FixMe: Should be useful, probably as void ComputeCentroid(Point4D* resultPtr).
{
    cerr << "\aWarning: MeshObject::GetVertexMedia() is deprecated.\n";
    VART::Point4D mean = VART::Point4D(0,0,0,1);
    double size = 0;

    size = vertCoordVec.size();

    for( int i=0; i < size ; i+=3)
    {   // FixMe: inefficient, use separate sums
        mean += VART::Point4D( vertCoordVec[i], vertCoordVec[i+1], vertCoordVec[i+2]);
    }

    if( size == 0 )
        return VART::Point4D(0,0,0,1);
    else
        mean = mean / (size/3);

    return mean;
}

void VART::MeshObject::AddFace(const char* indexStr)
{
    string valStr = indexStr;
    istringstream iss(valStr);
    unsigned int value;
    unsigned int thisFacesNormalIndex = normVec.size();
    VART::Mesh mesh;

    mesh.type = VART::Mesh::POLYGON;
    while (iss >> value)
    {
        mesh.indexVec.push_back(value);
        mesh.normIndVec.push_back(thisFacesNormalIndex);
    }
    meshList.push_back(mesh);

    // Auto computation of face normal
    // FixMe: It should be possible to disable auto computation
    VART::Point4D v1 = vertVec[mesh.indexVec[1]] - vertVec[mesh.indexVec[0]];
    VART::Point4D v2 = vertVec[mesh.indexVec[2]] - vertVec[mesh.indexVec[1]];
    v1.Normalize();
    v2.Normalize();
    VART::Point4D normal = v1.CrossProduct(v2);
    normVec.push_back(normal);
}

void VART::MeshObject::AddMesh(const Mesh& m)
{
    meshList.push_back(m);
}

void VART::MeshObject::MakeBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
    cerr << "MeshObject::MakeBox is deprecated. Use VART::Box.\n";
    assert((minX <= maxX) && (minY <= maxY) && (minZ <= maxZ));
    // each vertex must repeat 3 times because there must be a vertex/normal correspondence
    // of 1:1
    double coordinateArray[] = { minX,minY,minZ, //0
                                 minX,maxY,minZ, //1
                                 maxX,maxY,minZ, //2
                                 maxX,minY,minZ, //3
                                 minX,minY,maxZ, //4
                                 minX,maxY,maxZ, //5
                                 maxX,maxY,maxZ, //6
                                 maxX,minY,maxZ, //7
                                 minX,minY,minZ,
                                 minX,maxY,minZ,
                                 maxX,maxY,minZ,
                                 maxX,minY,minZ,
                                 minX,minY,maxZ,
                                 minX,maxY,maxZ,
                                 maxX,maxY,maxZ,
                                 maxX,minY,maxZ,
                                 minX,minY,minZ,
                                 minX,maxY,minZ,
                                 maxX,maxY,minZ,
                                 maxX,minY,minZ,
                                 minX,minY,maxZ,
                                 minX,maxY,maxZ,
                                 maxX,maxY,maxZ,
                                 maxX,minY,maxZ };
    double* endOfCoordinateArray = coordinateArray + sizeof(coordinateArray)/sizeof(double);
    unsigned int indexArray[] = { 0,1,2,3, // back face
                                  4,7,6,5, // front face
                                  11,10,14,15, // right face
                                  13,9,8,12, // left face
                                  22,18,17,21, // top face
                                  23,20,16,19 }; // bottom face
    unsigned int* endOfIndexArray = indexArray + sizeof(indexArray)/sizeof(int);
    double normalArray[] = { 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
                             0,0,1, 0,0,1, 0,0,1, 0,0,1,
                             -1,0,0, -1,0,0, 1,0,0, 1,0,0,
                             -1,0,0, -1,0,0, 1,0,0, 1,0,0,
                             0,-1,0, 0,1,0, 0,1,0, 0,-1,0,
                             0,-1,0, 0,1,0, 0,1,0, 0,-1,0 };
    double* endOfNormalArray = normalArray + sizeof(normalArray)/sizeof(double);
    float textArray[] = { 0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0 };
    float* endOfTextArray = textArray + sizeof(textArray)/sizeof(float);

    VART::Mesh mesh;
    vertCoordVec.clear();
    normCoordVec.clear();
    textCoordVec.clear();
    meshList.clear();
    vertCoordVec.assign(coordinateArray,endOfCoordinateArray);
    normCoordVec.assign(normalArray,endOfNormalArray);
    textCoordVec.assign(textArray,endOfTextArray);
    mesh.type = VART::Mesh::QUADS;
    mesh.indexVec.assign(indexArray,endOfIndexArray);
    mesh.material = VART::Material::DARK_PLASTIC_GRAY(); // default material
    meshList.push_back(mesh);
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VART::MeshObject::GetYProjection(std::list<VART::Point4D>* resultPtr, double height) const
// height defaults to 0 (see headers).
{
    VART::Point4D point;
    resultPtr->clear();
    if (vertCoordVec.size() > 6)
    { // Found optimized data - use it
        unsigned int vertexVetEnd = vertCoordVec.size();
        for (unsigned int i=0; i < vertexVetEnd; i+=3)
        {
            point.SetXYZW(vertCoordVec[i],height,vertCoordVec[i+2],1);
            resultPtr->push_back(point);
        }
    }
    else
    { // Use unoptimized data
        unsigned int vertexVetEnd = vertVec.size();
        for (unsigned int i=0; i < vertexVetEnd; ++i)
        {
            point.SetXYZW(vertVec[i].GetX(), height, vertVec[i].GetZ(), 1);
            resultPtr->push_back(point);
        }
    }
}

void VART::MeshObject::Optimize()
{
    // Create optmized structures from unoptimized ones
    // Erase unoptimized data
}

void VART::MeshObject::ComputeBoundingBox() {
    if (vertCoordVec.size() > 0)
    { // Optimized structure found - use it!
        // Initialize
        bBox.SetBoundingBox(vertCoordVec[0], vertCoordVec[1], vertCoordVec[2],
                            vertCoordVec[0], vertCoordVec[1], vertCoordVec[2]);
        // Check against the others
        for (unsigned int i=3; i < vertCoordVec.size(); i+=3)
            bBox.ConditionalUpdate(vertCoordVec[i], vertCoordVec[i+1], vertCoordVec[i+2]);
    }
    else
    { // No optmized structure found - use vertVec
        // Initialize
        bBox.SetBoundingBox(vertVec[0].GetX(), vertVec[0].GetY(), vertVec[0].GetZ(),
                            vertVec[0].GetX(), vertVec[0].GetY(), vertVec[0].GetZ());
        // Check against the others
        for (unsigned int i=1; i < vertVec.size(); ++i)
            bBox.ConditionalUpdate(vertVec[i]);
    }
    bBox.ProcessCenter();
}

void VART::MeshObject::ComputeBoundingBox(const VART::Transform& trans, VART::BoundingBox* bbPtr) {

    VART::Point4D p;

    if (vertCoordVec.size() > 0)
    { // Optimized structure found - use it!
        // Initialize
        p.SetXYZW( vertCoordVec[0], vertCoordVec[1], vertCoordVec[2], 1 );
        p = trans * p;
        bbPtr->SetBoundingBox(p.GetX(), p.GetY(), p.GetZ() , p.GetX(), p.GetY(), p.GetZ());
        // Check against the others
        for (unsigned int i=3; i < vertCoordVec.size(); i+=3)
        {
            p.SetXYZW( vertCoordVec[i], vertCoordVec[i+1], vertCoordVec[i+2], 1 );
            p = trans * p;
            bbPtr->ConditionalUpdate( p );
        }
    }
    else
    { // No optmized structure found - use vertVec
        // Initialize
        p = vertVec[0];
        p = trans * p;
        bbPtr->SetBoundingBox(p.GetX(), p.GetY(), p.GetZ() , p.GetX(), p.GetY(), p.GetZ());
        // Check against the others
        for (unsigned int i=1; i < vertVec.size(); ++i)
        {
            p = vertVec[i];
            p = trans * p;
            bbPtr->ConditionalUpdate( p );
        }
    }
}

void VART::MeshObject::ComputeSubBBoxes( const Transform& trans, int subdivisions )
{
    VART::Point4D p;
    VART::BoundingBox globalBBox;
    std::vector<VART::Point4D> pointList;


    if (vertCoordVec.size() <= 0)
        return;

    pointList.reserve( (int) (vertCoordVec.size() / 3) );

    // Initialize
    p.SetXYZW( vertCoordVec[0], vertCoordVec[1], vertCoordVec[2], 1 );
    p = trans * p;
    pointList.push_back( p );
    globalBBox.SetBoundingBox(p.GetX(), p.GetY(), p.GetZ() , p.GetX(), p.GetY(), p.GetZ());

    // Check against the others
    for (unsigned int i=3; i < vertCoordVec.size(); i+=3)
    {
        p.SetXYZW( vertCoordVec[i], vertCoordVec[i+1], vertCoordVec[i+2], 1 );
        p = trans * p;
        pointList.push_back( p );
        globalBBox.ConditionalUpdate( p );
    }

    if( subdivisions <= 0)
    {
        subBBoxes.push_back( globalBBox );
        return;
    }
    subBBoxes.clear();
    subBBoxes.reserve( (int)pow(8.0f, subdivisions) );

    subDivideBBox( globalBBox, subdivisions-1, pointList);
}

//~ void VART::MeshObject::ComputeFaceNormal(unsigned int faceIdx)
//~ {
    //~ unsigned int vertexIdx;
    //~ Point4D p1(vertCoordVec
//~ }

void VART::MeshObject::ComputeVertexNormals()
{
    // The normal for each vertex will be the average for each face
    // initialize every normal to (0,0,0), to acumulate a vector sum at each normal
    normCoordVec.assign(vertCoordVec.size(), 0);

    // for each mesh
    list<Mesh>::iterator iter = meshList.begin();
    for (; iter != meshList.end(); ++iter)
    {
        unsigned int p1Idx = 0;
        unsigned int p2Idx = 1;
        unsigned int p3Idx = 2;
        unsigned int end = iter->indexVec.size();
        // for each face
        while (p3Idx < end)
        {
            // compute face normal
            Point4D normal;
            ComputeTriangleNormal(Vertex(iter->indexVec[p1Idx]),
                                  Vertex(iter->indexVec[p2Idx]),
                                  Vertex(iter->indexVec[p3Idx]), &normal);
            // Add face normal to vertex normal (3 vertices)
            AddNormal(iter->indexVec[p1Idx], normal); // Add face normal to vertex normal
            AddNormal(iter->indexVec[p2Idx], normal); // Add face normal to vertex normal
            AddNormal(iter->indexVec[p3Idx], normal); // Add face normal to vertex normal
            // Add face normal to other vertices
            switch (iter->type)
            {
                case Mesh::TRIANGLES:
                    p1Idx += 3;
                    p2Idx += 3;
                    p3Idx += 3;
                break;
                case Mesh::TRIANGLE_STRIP:
                    p1Idx += 1;
                    p2Idx += 1;
                    p3Idx += 1;
                break;
                case Mesh::TRIANGLE_FAN:
                    p3Idx += 1;
                break;
                case Mesh::QUADS:
                    AddNormal(iter->indexVec[p3Idx+1], normal);
                    p1Idx += 4;
                    p2Idx += 4;
                    p3Idx += 4;
                break;
                case Mesh::QUAD_STRIP:
                    AddNormal(iter->indexVec[p3Idx+1], normal);
                    p1Idx += 2;
                    p2Idx += 2;
                    p3Idx += 2;
                break;
                case Mesh::POLYGON:
                    ++p3Idx;
                    for (; p3Idx < end; ++p3Idx)
                        AddNormal(iter->indexVec[p3Idx], normal);
                break;
                default:
                    cerr << "Error: MeshObject::ComputeVertexNormals not implemented for mesh type "
                         << static_cast<int>(iter->type) << endl;
                    exit (1);
            }
        }
        // now, each normal holds the sum of face normals that share it
    }
    NormalizeAllNormals();
}

void VART::MeshObject::subDivideBBox( VART::BoundingBox motherBox, int subdivisions, std::vector<VART::Point4D> pointList )
{
    VART::Point4D center;
    VART::BoundingBox   newBBox;

    motherBox.ProcessCenter();
    center = motherBox.GetCenter();

    // Here we test the points for each original bbox octet;

    // octet 1 (max, max, max)
    newBBox.SetBoundingBox(center.GetX(),           center.GetY(),           center.GetZ(),
                           motherBox.GetGreaterX(), motherBox.GetGreaterY(), motherBox.GetGreaterZ());
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 2 (min, max, max)
    newBBox.SetBoundingBox(motherBox.GetSmallerX(),           center.GetY(),           center.GetZ(),
                           center.GetX(),           motherBox.GetGreaterY(), motherBox.GetGreaterZ());
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 3 (max, min, max)
    newBBox.SetBoundingBox(center.GetX(),           motherBox.GetSmallerY(),           center.GetZ(),
                           motherBox.GetGreaterX(), center.GetY(),           motherBox.GetGreaterZ());
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 4 (min, min, max)
    newBBox.SetBoundingBox(motherBox.GetSmallerX(), motherBox.GetSmallerY(), center.GetZ(),
                           center.GetX(),           center.GetY()          , motherBox.GetGreaterZ());
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 5 (max, max, min)
    newBBox.SetBoundingBox(center.GetX(),           center.GetY(),          motherBox.GetSmallerZ(),
                           motherBox.GetGreaterX(), motherBox.GetGreaterY(),center.GetZ()          );
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 6 (min, max, min)
    newBBox.SetBoundingBox(motherBox.GetSmallerX(), center.GetY(),           motherBox.GetSmallerZ(),
                           center.GetX(),           motherBox.GetGreaterY(), center.GetZ()          );
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 7 (max, min, min)
    newBBox.SetBoundingBox(center.GetX(),           motherBox.GetSmallerY(), motherBox.GetSmallerZ(),
                           motherBox.GetGreaterX(), center.GetY(),           center.GetZ()          );
    computeNewSubBBox( newBBox, subdivisions, pointList );

    // octet 8 (min, min, min)
    newBBox.SetBoundingBox(motherBox.GetSmallerX(), motherBox.GetSmallerY(), motherBox.GetSmallerZ(),
                           center.GetX(),           center.GetY(),           center.GetZ()          );
    computeNewSubBBox( newBBox, subdivisions, pointList );
}

void VART::MeshObject::computeNewSubBBox( VART::BoundingBox oldBox, int subdivisions, std::vector<VART::Point4D> pointList)
{
    std::vector<Point4D>  newPointList;
    VART::BoundingBox   newBBox;
    bool initialized = false;

    for( unsigned int i=0; (i < pointList.size()) && (!initialized); i++ )
        if( oldBox.testPoint( pointList[i] ))
        {
            newBBox.SetBoundingBox( pointList[i].GetX(), pointList[i].GetY(), pointList[i].GetZ(),
                                        pointList[i].GetX(), pointList[i].GetY(), pointList[i].GetZ());
            initialized = true;
        }

    if( !initialized )
        return;

    newBBox.SetColor( VART::Color::GREEN() );

    for( unsigned int i=0; i < pointList.size(); i+=3 )
        if( oldBox.testPoint( pointList[i] )   ||
            oldBox.testPoint( pointList[i+1] ) ||
            oldBox.testPoint( pointList[i+2] ) )
        {
            newBBox.ConditionalUpdate( pointList[i] );
            newBBox.ConditionalUpdate( pointList[i+1] );
            newBBox.ConditionalUpdate( pointList[i+2] );
        }

    newBBox.CutBBox( oldBox );

    if(subdivisions == 0)
        subBBoxes.push_back( newBBox );
    else
        subDivideBBox( newBBox, subdivisions-1, pointList);
}

void VART::MeshObject::MergeWith(const VART::MeshObject& obj) {
// both meshObjects must be optimized or the both must be unoptimized
    bool bothOptimized = vertVec.empty() && obj.vertVec.empty();
    list<VART::Mesh>::const_iterator iter = obj.meshList.begin();
    VART::Mesh mesh;
    unsigned int prevNumVertices;
    assert (bothOptimized || (vertCoordVec.empty() && obj.vertCoordVec.empty()));

    prevNumVertices = (bothOptimized? (vertCoordVec.size()/3) : vertVec.size());
    for (; iter != obj.meshList.end(); ++iter)
    {
        mesh = *iter;
        mesh.IncrementIndices(prevNumVertices);
        meshList.push_back(mesh);
    }

    vertVec.insert(vertVec.end(), obj.vertVec.begin(), obj.vertVec.end());
    vertCoordVec.insert(vertCoordVec.end(), obj.vertCoordVec.begin(), obj.vertCoordVec.end());
    normVec.insert(normVec.end(), obj.normVec.begin(), obj.normVec.end());
    normCoordVec.insert(normCoordVec.end(), obj.normCoordVec.begin(), obj.normCoordVec.end());
    textCoordVec.insert(textCoordVec.end(), obj.textCoordVec.begin(), obj.textCoordVec.end());
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VART::MeshObject::ApplyTransform(const VART::Transform& trans) {
    unsigned int i = 0;
    unsigned int size;

    if (vertCoordVec.empty())
    {
        for (size = vertVec.size(); i < size; ++i)
        {
            trans.ApplyTo(&(vertVec[i]));
        }
    }
    else
    {
        VART::Point4D vertex;
        for (size = vertCoordVec.size(); i < size; i+=3)
        {
            vertex.SetXYZW(vertCoordVec[i],vertCoordVec[i+1],vertCoordVec[i+2],1);
            trans.ApplyTo(&vertex);
            vertCoordVec[i] = vertex.GetX();
            vertCoordVec[i+1] = vertex.GetY();
            vertCoordVec[i+2] = vertex.GetZ();
        }
    }
    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VART::MeshObject::ComputeTriangleNormal(const VART::Point4D& v1, const VART::Point4D& v2,
                                         const VART::Point4D& v3, VART::Point4D* resultPtr) {
    VART::Point4D edge1 = v2 - v1;
    VART::Point4D edge2 = v3 - v2;
    *resultPtr = edge1.CrossProduct(edge2);
    resultPtr->Normalize();
}

bool VART::MeshObject::DrawInstanceOGL() const {
#ifdef VART_OGL
    bool result = true;
    list<VART::Mesh>::const_iterator iter;
    if (show) // if visible...
    {         // FixMe: no need to keep this old name; rename "show" to "visible".
        switch (howToShow)
        {
            case LINES:
            case LINES_AND_NORMALS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case POINTS:
            case POINTS_AND_NORMALS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
            default:
                glPolygonMode(GL_FRONT, GL_FILL);
                break;
        }
        if (vertCoordVec.size() > 0)
        { // Optimized structure found - draw it!
          // Note that vertex arrays must be enabled to allow drawing of optimized meshes. See
          // VART::ViewerGlutOGL.
            glVertexPointer(3, GL_DOUBLE, 0, &vertCoordVec[0]);
            glNormalPointer(GL_DOUBLE, 0, &normCoordVec[0]);
            if (!textCoordVec.empty())
                glTexCoordPointer(3, GL_FLOAT, 0, &textCoordVec[0]);
            if ((howToShow == LINES_AND_NORMALS) || (howToShow == POINTS_AND_NORMALS))
            { // Draw normals
                unsigned int numVertices = vertCoordVec.size() / 3;
                unsigned int index = 0;
                Material::PLASTIC_WHITE().DrawOGL(); //FixMe: make more generic.
                glBegin(GL_LINES);
                for (unsigned int i=0; i < numVertices; ++i)
                {
                    glVertex3dv(&vertCoordVec[index]);
                    glVertex3d(vertCoordVec[index] + sizeOfNormals*normCoordVec[index],
                               vertCoordVec[index+1]+sizeOfNormals*normCoordVec[index+1],
                               vertCoordVec[index+2]+sizeOfNormals*normCoordVec[index+2]);
                    index += 3;
                }
                glEnd();
            }
            for (iter = meshList.begin(); iter != meshList.end(); ++iter)
            { // for each mesh:
                //if (iter->material.GetTexture().HasTextureLoad() ) {
                    //glTexCoordPointer(3,GL_FLOAT,0,&textCoordVec[0]);
                //}
                result &= iter->DrawInstanceOGL();
            }
        }
        else
        { // No optmized structure found - draw vertices from vertVec
            unsigned int meshSize;
            unsigned int i;
            for (iter = meshList.begin(); iter != meshList.end(); ++iter)
            { // for each mesh:
                iter->material.DrawOGL();
                glBegin(iter->GetOglType());
                meshSize = iter->indexVec.size();
                assert(meshSize == iter->normIndVec.size());
                for (i = 0; i < meshSize; ++i)
                {
                    // FixMe: need to draw texture vertices for unoptimized mesh
                    glNormal3dv(normVec[iter->normIndVec[i]].VetXYZW());
                    glVertex4dv(vertVec[iter->indexVec[i]].VetXYZW());
                }
                glEnd();
            }
        }
    }
    if(bBox.visible)
        bBox.DrawInstanceOGL();
    if(recBBox.visible)
        recBBox.DrawInstanceOGL();
    return result;
#else
    return false;
#endif
}

bool VART::MeshObject::ReadFromOBJ(const string& filename, list<VART::MeshObject*>* resultPtr)
// passing garbage on *resultPtr makes the method crash. Remember to clean it before calling.

// Note: Blender saves obj files with multiple objects, reusing normal coordinates (and
// probably other stuff) between objects. V-ART has to repeat those coordinates because
// each object in the file turns into a mesh object with its own coordinates vector.
{
    ifstream file(filename.c_str());
    if(file.is_open())
        cout << "Loading " << filename << "...\n" << flush;
    else {
        ostringstream error;
        error << "MeshObject::ReadFromOBJ failed to open '" << filename << "'.\n";
        throw runtime_error(error.str());
    }

    VART::MeshObject* meshObjectPtr = NULL;
    istringstream iss;
    string line;
    string lineID;
    string name;
    //char trash;
    double x,y,z;
    map<string,VART::Material> materialMap;
    map<string,VART::Texture> textureMap;
    // vertIndexesMap maps vi/ni/ti triplets to unique array indices
    map<VertexTriplet, unsigned int> vertIndexesMap;
    vector<float> vertCoordTempVec; // cache of previous vertices described in a file
    vector<float> vertNormTempVec; // cache of previous normals described in a file
    vector<float> textCoordTempVec; // cache of previous textures described in a file
    VART::Mesh mesh;
//    unsigned int vi, ti, ni; // vertex, texture and normal indices
    unsigned int faceCounter = 0; // counts the number of faces in the file
    unsigned int objCounter = 0; // counts the number of objects in the file
    unsigned int lineNumber = 0;

    while (getline(file, line)) {
        iss.clear(); // reset error status
        iss.str(line); // iss <- line
        if(iss.peek() != -1) {
            iss >> lineID;
            ++lineNumber;
            static unsigned int index = 0;
            if (lineID == "v") { // vertex
                if (meshObjectPtr == NULL) {
                    ostringstream error;
                    error << "Error at line " << lineNumber << " of " << filename << ": No object defined. NULL object is not allowed.";
                    throw runtime_error(error.str());
                }
                iss >> x >> y >> z;
                // store coordinates in the mesh object
                // meshObjectPtr->vertCoordVec.push_back(x);
                // meshObjectPtr->vertCoordVec.push_back(y);
                // meshObjectPtr->vertCoordVec.push_back(z);
                // keep a local copy to deal with index beetween objects in the file
                vertCoordTempVec.push_back(x);
                vertCoordTempVec.push_back(y);
                vertCoordTempVec.push_back(z);
            }
            else if (lineID == "vn") { // vertex normal
                iss >> x >> y >> z;
                // store coordinates in the mesh object
                // meshObjectPtr->normCoordVec.push_back(x);
                // meshObjectPtr->normCoordVec.push_back(y);
                // meshObjectPtr->normCoordVec.push_back(z);
                // keep a local copy to deal with index beetween objects in the file
                vertNormTempVec.push_back(x);
                vertNormTempVec.push_back(y);
                vertNormTempVec.push_back(z);
            }
            else if (lineID == "vt") { // texture coordinates
                // Texture coordinates could be 2 or 3 values,
                // only the first 2 are read.
                iss >> x >> y;
                textCoordTempVec.push_back(x);
                textCoordTempVec.push_back(y);
                textCoordTempVec.push_back(0.0f);
            }
            else if (lineID == "f") { // face
                list<VertexTriplet> vertices;
                ReadVerticesLine(iss, &vertices);
                VART::Mesh::MeshType tempType;
                switch(vertices.size()) {
                    case 1: throw runtime_error("ReadFromObj found a face with 1 vertex!\n");
                    case 2: throw runtime_error("ReadFromObj found a face with 2 vertices!\n");
                    case 3: tempType = VART::Mesh::TRIANGLES;
                            break;
                    case 4: tempType = VART::Mesh::QUADS;
                            break;
                    default:
                            tempType = VART::Mesh::POLYGON;
                }
                // V-Art will create a new mesh every time the number
                // of vertices in a face is different than in the previous ones.
                if (mesh.type != Mesh::NONE) {
                    if (tempType != mesh.type) {
                        // start new mesh
                        // add old mesh to meshObject
                        meshObjectPtr->meshList.push_back(mesh);
                        mesh.indexVec.clear();
                        mesh.normIndVec.clear();
                        mesh.type = tempType;
                    }
                }
                else { // this is the first face in the mesh
                    mesh.type = tempType;
                }

                // A file has many objects, but each object gets separated on a MeshObject
                // with its on vertex coordinates vector. Need to subtract from last.
                // However, the file may refer to vertices or normals that were put with
                // previous mesh objects.
                for (auto& vTriple: vertices) {
                    map<VertexTriplet, unsigned int>::iterator pos =
                                            vertIndexesMap.find(vTriple);
                    if (pos != vertIndexesMap.end()) {
                        // index has been used before
                        mesh.indexVec.push_back(pos->second);
                    }
                    else { // index triple hasn't been used before
                        vertIndexesMap.insert(make_pair(vTriple, index));
                        mesh.indexVec.push_back(index);
                        ++index;

                        // copy vertex, texture and normal coordinates to this mesh object
                        unsigned int i = (vTriple.vertexIndex-1)*3; // x coordinate in vertCoordTempVec
                        meshObjectPtr->vertCoordVec.push_back(vertCoordTempVec[i]);
                        meshObjectPtr->vertCoordVec.push_back(vertCoordTempVec[++i]);
                        meshObjectPtr->vertCoordVec.push_back(vertCoordTempVec[++i]);

                        if (vTriple.textIndex != 0) {
                            i = (vTriple.textIndex-1)*3; // x coordinate in textCoordTempVec
                            meshObjectPtr->textCoordVec.push_back(textCoordTempVec[i]);
                            meshObjectPtr->textCoordVec.push_back(textCoordTempVec[++i]);
                            meshObjectPtr->textCoordVec.push_back(textCoordTempVec[++i]);
                        }

                        i = (vTriple.normIndex-1)*3; // x coordinate in normCoordTempVec
                        meshObjectPtr->normCoordVec.push_back(vertNormTempVec[i]);
                        meshObjectPtr->normCoordVec.push_back(vertNormTempVec[++i]);
                        meshObjectPtr->normCoordVec.push_back(vertNormTempVec[++i]);
                    }
                }
                ++faceCounter;
        } // end of face line
        else if (lineID == "usemtl") // material assignment
        { // start new mesh
            // add old mesh to meshObject
            if (mesh.indexVec.size() > 0)
            {
                meshObjectPtr->meshList.push_back(mesh);
                mesh.indexVec.clear();
                mesh.normIndVec.clear();
            }

            iss >> ws >> name;
            mesh.material = materialMap[name];
        }
        else if (lineID == "usemap") // texture of current mesh
        {
            iss >> name;
            // make sure the name is in lower case
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            VART::Texture texture = textureMap[name];
            if(!texture.HasData()) //Read a texture file not read yet
            {
                name = VART::File::GetPathFromString(filename)+name;
                if(! texture.LoadFromFile(name) )
                    cerr << "Error reading usemap in '" << VART::File::GetPathFromString(filename) << filename << "', line " << lineNumber <<
                    ": could not read texture file '" << name << "'" << endl;
                textureMap[name] = texture;
            }
            mesh.material.SetTexture( texture );
        }
        else if (lineID == "mtllib") // material library
        {
            iss >> ws >> name;
            ReadMaterialTable(VART::File::GetPathFromString(filename)+name, &materialMap);
        }
        else if (lineID == "maplib") //texture mapping library
        {//ignore this line, no maplib implemented yet in V-Art
        }
        else if (lineID == "o") // object delimiter
        {
            ++objCounter;
            iss >> ws >> name;
            // Add last mesh to last meshObject
            if (mesh.indexVec.size() > 0)
            {
                meshObjectPtr->meshList.push_back(mesh);
                mesh.indexVec.clear();
                mesh.type = VART::Mesh::NONE;
            }

//            if (meshObjectPtr->IsEmpty())
//            {
//                resultPtr->remove(meshObjectPtr);
//                delete meshObjectPtr;
//            }

            meshObjectPtr = new VART::MeshObject;
            meshObjectPtr->autoDelete = true;
            meshObjectPtr->SetDescription(name);
            resultPtr->push_back(meshObjectPtr);
            index = 0;
        }
        else if (lineID[0] == '#')
        { // ignore this line (comment)
        }
            else if (lineID == "g") // group names
            { // ignore this line (no groups in V-ART yet!)
            }
            else if (lineID == "s") // smoothing group
            { // ignore this line (no smoothing groups in V-ART yet!)
            }
            else
                cerr << "Error in '" << filename << "', line " << lineNumber << ": unknown ID '"
                     << lineID << "'" << endl;
        }
    }
    // Finished. Add last mesh to last meshObject
    if (mesh.indexVec.size() > 0)
    {
        meshObjectPtr->meshList.push_back(mesh);
    }
    // Compute bounding boxes
    list<VART::MeshObject*>::iterator iter;
    for (iter = resultPtr->begin(); iter != resultPtr->end(); ++iter)
    {
        (*iter)->ComputeBoundingBox();
        (*iter)->ComputeRecursiveBoundingBox();
    }
    clog << "File " << filename << " finished loading ("
         << objCounter << " objects, "
         << faceCounter << " polygons).\n";

    return true;
}

void VART::MeshObject::NormalizeAllNormals()
{
    unsigned int i0 = 0;
    unsigned int i1 = 1;
    unsigned int i2 = 2;
    double size;
    while (i2 < normCoordVec.size())
    {
        size = sqrt(normCoordVec[i0]*normCoordVec[i0] +
                    normCoordVec[i1]*normCoordVec[i1] +
                    normCoordVec[i2]*normCoordVec[i2]);
        normCoordVec[i0] /= size;
        normCoordVec[i1] /= size;
        normCoordVec[i2] /= size;
        i0 += 3;
        i1 += 3;
        i2 += 3;
    }
}

void VART::MeshObject::ReadMaterialTable(const string& filename, map<string,VART::Material>* matMapPtr)
// Reads a Wavefront material table (.mtl file)
{
    ifstream file(filename.c_str());
    istringstream iss;
    string line;
    string lineID;
    string materialName;
    string textureName;
    VART::Material material;
    VART::Texture texture;
    std::map<std::string,VART::Texture> textureMap;
    float r,g,b;
    float value;
    unsigned int type;
    unsigned int lineNumber = 0;

    material.SetEmissiveColor(VART::Color::BLACK());
    if( !file.is_open() )
    {
        cerr << "Error in '" << filename << "': could not read material table file." << endl;
        return;
    }
    while (getline(file,line))
    {
        iss.str(line); // iss <- line
        iss >> lineID;
        ++lineNumber;
        if (lineID == "newmtl")
        {
            // if not the first material, add the last material to the material table
            if (materialName.size() > 0)
                matMapPtr->insert(make_pair(materialName,material));
            // read name for current material
            iss >> ws >> materialName;
        }
        else if (lineID == "Ns")
        {
            iss >> value;
            // shininess values appear to be in the range 0..1000, so they should be
            // multiplied by 0.128
            material.SetShininess(value*0.128);
        }
        else if (lineID == "Kd")
        {
            iss >> r >> g >> b;
            material.SetDiffuseColor(VART::Color(static_cast<unsigned char>(r*255),
                                             static_cast<unsigned char>(g*255),
                                             static_cast<unsigned char>(b*255)));
        }
        else if (lineID == "Ka")
        {
            iss >> r >> g >> b;
            material.SetAmbientColor(VART::Color(static_cast<unsigned char>(r*255),
                                             static_cast<unsigned char>(g*255),
                                             static_cast<unsigned char>(b*255)));
        }
        else if (lineID == "Ks")
        {
            iss >> r >> g >> b;
            material.SetSpecularColor(VART::Color(static_cast<unsigned char>(r*255),
                                              static_cast<unsigned char>(g*255),
                                              static_cast<unsigned char>(b*255)));
        }
        else if (lineID == "Ke")
        {
            iss >> r >> g >> b;
            material.SetEmissiveColor(VART::Color(static_cast<unsigned char>(r*255),
                                              static_cast<unsigned char>(g*255),
                                              static_cast<unsigned char>(b*255)));
        }
        else if ((lineID == "d") || (lineID == "Tr"))
        {
            iss >> value; // transparency value
            // FixMe: use the value
        }
        else if (lineID == "illum")
        {
            iss >> type;
            if (type == 1) // no specular color for this material
                material.SetSpecularColor(VART::Color::BLACK());
                //~ material.SetSpecularColor(material.GetDiffuseColor());
        }
        else if (lineID == "map_Kd")
        {
            iss >> textureName;
            texture = textureMap[textureName];
            if(!texture.HasData())
            {
                string path = VART::File::GetPathFromString(filename);
                if( !texture.LoadFromFile( path+textureName ) )
                {
                    cerr << "Error reading map_Kd in '" << filename << "', line "
                         << lineNumber << ": could not read texture file '"
                         << path << textureName << "'" << endl;
                }
                textureMap[textureName] = texture;
            }
            material.SetTexture( texture );
        }
        else if (lineID == "Ni")
        { // "Ni" Specifies the optical density for the surface. This is also
          // known as index of refraction. The values can range from 0.001 to
          // 10. A value of 1.0 means that light does not bend as it passes
          // through an object. Increasing the optical_density increases the
          // amount of bending. Glass has an index of refraction of about 1.5.
          // Values of less than 1.0 produce bizarre results and are not
          // recommended.
            float opticalDensity;
            iss >> opticalDensity; // Currently there is no use for it.
        }
        else if (lineID == "#")
        { // comment, ignore this line
        }
        else
        {
            cerr << "Error in '" << filename << "', line " << lineNumber << ": unknown ID '"
                 << lineID << "'" << endl;
        }
        iss.clear();
    }
    // Add last material to material table
    if (materialName.size() > 0)
        matMapPtr->insert(make_pair(materialName,material));
}

// static protected
bool VART::MeshObject::ReadVertex(std::istringstream& iss,
                                  unsigned int* viPtr, unsigned int* tiPtr, unsigned int* niPtr)
// Reads a triple vi/ti/ni (vertex index, texture index, normal index).
// An OBJ file could also have the formats: vi/ti or vi//ni
// Indexes in the file should start on "1", this methods uses the value 0 to indicate that
// there was no value in the file.
{
    unsigned int& vi = *viPtr;
    unsigned int& ti = *tiPtr;
    unsigned int& ni = *niPtr;
    ti = ni = 0;
    string vertexString;
    iss >> vertexString; // iss is the whole line, vertexString is the triplet part
    if (vertexString.empty())
       return false;
    unsigned int slashOccurrences = CountOccurrences('/', vertexString);
    istringstream vStream(vertexString);
    try {
        char slash;
        vStream >> vi >> slash;
        if (slashOccurrences == 2) {
            if(vStream.peek() != '/')
                vStream >> ti;
            else {
                static bool notWarned = true;
                if (notWarned) {
                    clog << "Warning: OBJ file is missing texture indices.\n";
                    notWarned = false;
                }
            }
            vStream >> slash;
        }
        else {
            static bool notWarned = true;
            if (notWarned) {
                cerr << "Error: OBJ file is missing normal indices.\n";
                notWarned = false;
            }
        }
        vStream >> ni;
        return true;
    }
    catch (...) {
        throw runtime_error("Error while reading. See MeshObject::ReadVertex.\n");
    }
}
// static, protected
void VART::MeshObject::ReadVerticesLine(std::istringstream& input,
                                        std::list<VertexTriplet>* resultPtr)
// Reads all vertex triplets from a "face" line in a LightWaveOBJ file.
{
    resultPtr->clear();
    unsigned int vi, ni, ti;
    while(ReadVertex(input, &vi, &ni, &ti)) { // Read every vertex triplet
        resultPtr->push_back(VertexTriplet(vi, ni, ti));
    }
}

namespace VART
{
    ostream& operator<<(ostream& output, const MeshObject& m)
    {
        unsigned int size = m.vertCoordVec.size();
        output << "[ MeshObject '" << m.description << "'\n" << flush
               << "  " << size/3 << " vertices: (";
        for (unsigned int i = 0; i < size; ++i) {
            output << m.vertCoordVec[i] << ",";
            output << m.vertCoordVec[++i] << ",";
            output << m.vertCoordVec[++i] << "; ";
        }
        if (not m.textCoordVec.empty()) {
            output << ")\n  " << size/3 << " texture coordinates: ( ";
            for (unsigned int i = 0; i < size; ++i) {
                output << m.textCoordVec[i] << ",";
                output << m.textCoordVec[++i] << ",";
                output << m.textCoordVec[++i] << "; ";
            }
        }
        output << ")\n " << m.meshList.size() << " meshes: ";
        list<Mesh>::const_iterator iter = m.meshList.begin();
        for (; iter != m.meshList.end(); ++iter)
            output << "(" << *iter << ")\n";
        output << "]";
        return output;
    }
}
