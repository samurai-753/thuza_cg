/// \file meshobject.h
/// \brief Header file for V-ART class "MeshObject".
/// \version $Revision: 1.11 $

#ifndef VART_MESHOBJECT_H
#define VART_MESHOBJECT_H

#include "vart/mesh.h"
#include "vart/point4d.h"
#include "vart/graphicobj.h"
#include "vart/transform.h"
#include <vector>
#include <list>
#include <map>

namespace VART {
/// \class MeshObject meshobject.h
/// \brief Graphical object made of polygon meshes.
///
/// A mesh object, unlike other graphical objects, maintains information about
/// every vertex, normals, texture coordinates and edges. This is the class of
/// choice for creating objects from files or objects whose vertex are to be edited.
/// A mesh object may be optimized for display, and in that case vertex editing will
/// be no longer possible.
    class MeshObject : public GraphicObj {
        /// Output operator
        friend std::ostream& operator<<(std::ostream& output, const MeshObject& m);

        public:
        // PUBLIC METHODS
            MeshObject();
            MeshObject(const MeshObject& obj);

            /// \brief Makes a copy of the object, including the children list, but not the
            /// children themselves.
            MeshObject& operator=(const MeshObject& obj);

            /// \brief Returns a copy of an MeshObject. Every derived class must reimplements
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            /// \brief Assigns a material to all meshes of the mesh object.
            void SetMaterial(const Material& mat);

            /// \brief Creates a box aligned with the 3 reference planes (XY, XZ and YZ).
            /// \deprecated Use VART::Box.
            void MakeBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);

            /// \brief Computes the projection of all vertices along the Y axis.
            /// \return a list of projected points.
            /// \param resultPtr [in,out] A pointer to a list that will be filled with
            ///        resulting vertices.
            /// \param height [in] Y coordinate to use in the projection (defaults to 0).
            void GetYProjection(std::list<Point4D>* resultPtr, double height=0) const;

            /// \brief Optimize object for display.
            /// \bug Not implemented yet.
            ///
            /// This method creates an internal representation that is optimized for
            /// display (currently aimed at OpenGL - optimized representation may not
            /// be usefull for other renderers such as Direct3D). After being optimized,
            /// old data is discarded and the object can no longer be edited.
            void Optimize();

            /// \brief Erases internal structures.
            ///
            /// Erases previous vertices, indices, normals, texture coordinates, etc.
            /// Use before a complete change in the object.
            void Clear();

            /// \brief Sets the vector of vertices.
            /// \param vertexVec [in] The new vector of vertices.
            ///
            /// All vertices in vertexVec are copied into the MeshObject's internal
            /// representation. The meshList is cleared.
            void SetVertices(const std::vector<Point4D>& vertexVec);

            /// \brief Sets the vector of normals.
            ///
            /// Use this method to set normals for unoptimized objects. Note that normals are
            /// atomatically computed if AddFace is used to set the faces. This method clears
            /// the mesh list, so it should be called before AddFace.
            void SetNormals(const std::vector<Point4D>& normalVec);

            /// \brief Sets the vector of vertices.
            /// \param vertexStr [in] The set of vertices (a C-string of point
            /// coordinates).
            ///
            /// Parses all values in vertexStr and stores them into the MeshObject's
            /// internal representation. Parameter vertexStr should be set of points
            /// separated by commas, point coordinates should be separated by spaces,
            /// point may have 3 or 4 coordinates - i.e.: "1.2 0.2 3.1, 2 3 4 1, 1 2 3"
            /// The MeshObject becomes unoptimized. The meshList is cleared.
            void SetVertices(const char* vertexStr);

            /// \brief Changes one vertex.
            /// \param index [in] Index of the vertex to be changed.
            /// \param newValue [in] New value to be copied into the vertex.
            void SetVertex(unsigned int index, const Point4D& newValue);

            /// \brief Returns the coordinates of the vertices in the object.
            const std::vector<double>& GetVerticesCoordinates() { return vertCoordVec; }

            /// \brief Adds a face (a mesh of a single polygon) based on previously
            /// set vertices.
            /// \param indexStr [in] The set of indices of the face (a C-string of
            /// numbers separated by spaces) - i.e.: "0 1 2 3".
            /// \sa SetVertices(const char*), SetVertices(const std::vector<Point4D>&).
            ///
            /// Creates a new Mesh of a single polygon, based on the indices found
            /// in indexStr. Indices start at zero. The vertices referenced should be
            /// in counter-clockwise order. Vertices should be referenced in
            /// counter-clockwise order. No checking is done.
            /// Computes the face normal and uses it for every face vertex.
            void AddFace(const char* indexStr);

            /// \brief Adds a copy of the mesh to the object.
            ///
            /// Use this method to create a custom MeshObject by hand. Since a MeshObject
            /// is composed of meshes, this is equivalent of adding a new part of the
            /// object.
            void AddMesh(const Mesh& m);

            /// Computes the bounding box.
            virtual void ComputeBoundingBox();
            void ComputeBoundingBox(const Transform& trans, BoundingBox* bbPtr);

            /// \brief Computes de SubBBoxes and stores them.
            ///
            /// Computes 8^n, n=subdivisions,  bounding boxes storing them at
            /// subBBoxes list. Trans is the vertex transformations.
            void ComputeSubBBoxes( const Transform& trans, int subdivisions );

            /// returns the list of subdivided bounding boxes.
            std::vector<VART::BoundingBox> GetSubBBoxes() { return subBBoxes; };

            virtual TypeID GetID() const { return MESH_OBJECT; }

            /// \brief Merges one mesh object with another.
            ///
            /// All vertices, normals, meshes, etc. are copied inside the active object.
            void MergeWith(const MeshObject& obj);

            /// \brief Apply Transformation to all vertices.
            void ApplyTransform(const Transform& trans);

            //~ /// \brief Splits the object along some plane parallel to XZ.
            //~ /// \param y [in] Position of the cutting plane.
            //~ /// \param ptObjAbove [out] Parts that lie below the cutting plane.
            //~ /// \param ptObjBelow [out] Parts that lie above the cutting plane.
            //~ ///
            //~ /// Splits an object into two, along a cutting plane. The two resulting objects
            //~ /// are returned by filling the objects pointed by ptObjAbove and ptObjBelow, which
            //~ /// must not contain previous geometry (use Clear()). If the cutting plane does not
            //~ /// intersect the object, one of the resulting objects will be empty (check with
            //~ /// IsEmpty()).
            //~ void YSplit(double y, MeshObject* ptObjBelow, MeshObject* ptObjAbove) const;

            /// \brief Checks whether the object contains geometry data.
            ///
            /// Returns true if the object contains no geometry data. Actually it checks whether
            /// the mesh list is empty, which should be enough for consistent mesh objects.
            bool IsEmpty() { return meshList.empty(); }

            /// \brief Returns a copy of a vertex.
            Point4D GetVertex(unsigned int pos);

            /// \brief Computes and returns the smaller vertex.
            ///
            /// \sa Point4D::operatorLess
            void SmallerVertex(Point4D* resultPtr);

            /// \brief Return the Aritmethic mean of vertexes values
            /// \deprecated This method has a wrong name, wrong description, returns a point by copy
            ///             and is inefficient. The geometric centroid, also known as "center of mass"
            ///             is the average of all vertices.
            Point4D GetVertexMedia();

            /// \brief Computes the normal of every vertex
            ///
            /// Computes the normal of every vertex by computing face normals and then computing
            /// the average of all normals for faces that share a vertex.
            void ComputeVertexNormals();

        // STATIC PUBLIC METHODS
            /// \brief Computes the normal of a triangle.
            /// \param v1 [in] 1st triangle vertex
            /// \param v2 [in] 2nd triangle vertex
            /// \param v3 [in] 3rd triangle vertex
            /// \param resultPtr [out] Triangle normal (unit length).
            ///
            /// V1, V2 and V3 should be in counter clockwise order.
            static void ComputeTriangleNormal(const Point4D& v1, const Point4D& v2,
                                              const Point4D& v3, Point4D* resultPtr);

            /// \brief Read MeshObjects from a Wavefront OBJ file
            /// \bug Currently reads only triangle meshes.
            /// \param filename [in] The file (Wavefront OBJ file) name.
            /// \param resultPtr [out] The address of an empty list of pointers to mesh objects
            /// to be filled with the addresses of newly allocated mesh objects.
            /// \return True is returned if filename could be read. False otherwise. Note that
            /// if a empty file is read, true is returned, but the resultPtr list will be returned
            /// empty.
            ///
            /// This method creates mesh objects marked as auto-delete, ie, they will be
            /// automatically deleted if attached to scene. If not, the application programmer
            /// should delete them.
            static bool ReadFromOBJ(const std::string& filename, std::list<MeshObject*>* resultPtr);

            /// \brief Computes the number of faces
            unsigned int NumFaces();

        // STATIC PUBLIC ATTRIBUTES
            /// Size of normals for rendering (in world coordinates).
            static float sizeOfNormals;

        protected:
        // PROTECTED NESTED CLASSES
            class VertexTriplet {
                public:
                    VertexTriplet(unsigned int vi, unsigned int ti, unsigned int ni)
                        :vertexIndex(vi), textIndex(ti), normIndex(ni) {}
                    bool operator < (VertexTriplet indexes) const {
                        if (vertexIndex != indexes.vertexIndex)
                            return vertexIndex < indexes.vertexIndex;
                        if (textIndex != indexes.textIndex)
                            return textIndex < indexes.textIndex;
                        else
                            return normIndex < indexes.normIndex;
                    }
                    unsigned int vertexIndex;
                    unsigned int textIndex;
                    unsigned int normIndex;
            };

        // PROTECTED METHODS
            virtual bool DrawInstanceOGL() const;

            /// \brief Adds a vector to a vertex normal
            /// \param idx [in] vertex normal index
            /// \param vec [in] vector to add
            void AddNormal(unsigned int idx, const Point4D& vec);

            /// \brief Returns a vertex as a Point4D
            ///
            /// Requires the the MeshObject is in optimized form.
            Point4D Vertex(unsigned int i) const {
                return Point4D(vertCoordVec[i*3], vertCoordVec[i*3+1], vertCoordVec[i*3+2]); }

            /// \brief Normalizes all vertex normals
            void NormalizeAllNormals();

        // PROTECTED ATTRIBUTES
            /// \brief Vector of all vertices.
            ///
            /// This is the unoptimized storage of vertices. It must be a vector (not
            /// a list) because the internal meshes identifies vertices by integers.
            std::vector<Point4D> vertVec;

            /// \brief Vector of all vertex coordinates (their coordinates in sequence).
            ///
            /// This is the optimized storage of vertices. It must be a vector (not
            /// a list) because OpenGL will see it as a C array. Every 3 sucessive
            /// coordinates in the array describe a vertex in the object.
            std::vector<double> vertCoordVec;

            /// \brief Vector of all normals.
            ///
            /// This is the unoptimized storage of normals. It must be a vector (not
            /// a list) because the internal meshes identifies vertices by integers.
            std::vector<Point4D> normVec;

            /// \brief Vector of all vertex normals (their coordinates in sequence).
            ///
            /// This is the optimized storage of normals. It must be a vector (not
            /// a list) because OpenGL will see it as a C array.
            std::vector<double> normCoordVec;

            /// \brief Vector of all texture coordinates.
            ///
            /// This is both the optimized and the unoptimized storage of texture coordinates.
            /// It must be a vector (not a list) because OpenGL will see it as a C array.
            std::vector<float> textCoordVec;

            /// \brief List of Meshes
            std::list<Mesh> meshList;

        // PROTECTED STATIC METHODS
            /// \brief Reads a vertex description from a face on a OBJ file.
            /// \return Returns true if a vertex triple has been read.
            static bool ReadVertex(std::istringstream& iss, unsigned int* vi, unsigned int* ti, unsigned int* ni);

            static void ReadMaterialTable(const std::string& filename,
                                          std::map<std::string,Material>* matMapPtr);

            static void ReadVerticesLine(std::istringstream& input,
                                         std::list<VertexTriplet>* resultPtr);


        private:
        // PRIVATE METHODS
            /// \brief Take a bbox and subdivide it in 8.
            ///
            /// When subdivisions is 1 the resulting bounding boxes are stored in subBBoxes vector,
            /// otherwise the function is called recursively for each resulting bbox.
            void subDivideBBox( VART::BoundingBox motherBox, int subdivisions, std::vector<VART::Point4D> pointList );

            /// Helps on subdividebbox
            void computeNewSubBBox( VART::BoundingBox oldBox, int subdivisions, std::vector<VART::Point4D> pointList);

        // PRIVATE ATRIBUTES
            /// \brief List of Boundingboxes.
            ///
            /// This will store a list of bboxes for refined colisions tests.
            std::vector<VART::BoundingBox> subBBoxes;

    }; // end class declaration
} // end namespace

#endif
