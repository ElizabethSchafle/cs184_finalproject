#include "halfEdgeMesh.h"

namespace CGL {



  bool Halfedge::isBoundary( void ) const
  // returns true if and only if this halfedge is on the boundary
  {
    // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
  }

  bool Edge::isBoundary( void ) const
  {
        // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
  }

  Vector3D Face::normal( void ) const
  {
       // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
  }

  void Face::calculate_quadric() {
      Vector3D face_normal = normal();
      double d = dot(face_normal, halfedge()->vertex()->position);
      Vector4D plane_p = Vector4D(face_normal.x, face_normal.y, face_normal.z, -d);
      Matrix4x4 face_quadric = outer(plane_p, plane_p);
      this->quadric = face_quadric;
  }
  
  void Vertex::calculate_quadratic() {
	Matrix4x4 quadratic_sum = Matrix4x4();
	quadratic_sum.zero(0.0);
	
	
	HalfedgeIter h = halfedge();
	
	do {
	  
	  Matrix4x4 face_quadric = h->face()->quadric;
	  quadratic_sum += face_quadric;
	  
	  h = h->twin()->next();
	  
	} while( h != halfedge() );
	
	this->quadric = quadratic_sum;
  }

    EdgeRecord::EdgeRecord(EdgeIter &_edge) {
	  
        HalfedgeIter h = _edge->halfedge();
        Matrix4x4 edge_quadric = h->vertex()->quadric + h->twin()->vertex()->quadric;
        Matrix3x3 A = Matrix3x3();

        A.column(0) = Vector3D(edge_quadric.column(0).x, edge_quadric.column(0).y, edge_quadric.column(0).z);
        A.column(1) = Vector3D(edge_quadric.column(1).x, edge_quadric.column(1).y, edge_quadric.column(1).z);
        A.column(2) = Vector3D(edge_quadric.column(2).x, edge_quadric.column(2).y, edge_quadric.column(2).z);

        Vector3D b = Vector3D(edge_quadric.column(3).x, edge_quadric.column(3).y, edge_quadric.column(3).z);
        b = -b;

        Vector3D x = A.inv() * b;

        optimalPoint = x;
	  	Vector4D x_homogeneous = Vector4D(x.x, x.y, x.z, 1.0);
        score = dot(x_homogeneous, edge_quadric * x_homogeneous);
	  
	  	edge = _edge;
    }

    void HalfedgeMesh :: build( const vector< vector<Index> >& polygons,
    const vector<Vector3D>& vertexPositions )
    // This method initializes the halfedge data structure from a raw list of polygons,
    // where each input polygon is specified as a list of vertex indices.  The input
    // must describe a manifold, oriented surface, where the orientation of a polygon
    // is determined by the order of vertices in the list.  Polygons must have at least
    // three vertices.  Note that there are no special conditions on the vertex indices,
    // i.e., they do not have to start at 0 or 1, nor does the collection of indices have
    // to be contiguous.  Overall, this initializer is designed to be robust but perhaps
    // not incredibly fast (though of course this does not affect the performance of the
    // resulting data structure).  One could also implement faster initializers that
    // handle important special cases (e.g., all triangles, or data that is known to be
    // manifold).
    //
    // Since there are no strong conditions on the indices of polygons, we assume that
    // the list of vertex positions is given in lexicographic order (i.e., that the
    // lowest index appearing in any polygon corresponds to the first entry of the list
    // of positions and so on).
    {
    // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 

    } // end HalfedgeMesh::build()

    const HalfedgeMesh& HalfedgeMesh :: operator=( const HalfedgeMesh& mesh )
    // The assignment operator does a "deep" copy of the halfedge mesh data structure; in
    // other words, it makes new instances of each mesh element, and ensures that pointers
    // in the copy point to the newly allocated elements rather than elements in the original
    // mesh.  This behavior is especially important for making assignments, since the mesh
    // on the right-hand side of an assignment may be temporary (hence any pointers to elements
    // in this mesh will become invalid as soon as it is released.)
    {
         // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
    }

    HalfedgeMesh :: HalfedgeMesh( const HalfedgeMesh& mesh )
    {
          // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
    }

  } // End of CMU 462 namespace.
