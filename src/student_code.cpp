#include "student_code.h"
#include "mutablePriorityQueue.h"

using namespace std;

namespace CGL
{
  void BezierCurve::evaluateStep()
  {
    // TODO Part 1.
    // Perform one step of the Bezier curve's evaluation at t using de Casteljau's algorithm for subdivision.
    // Store all of the intermediate control points into the 2D vector evaluatedLevels.
      const std::vector<Vector2D> &points = evaluatedLevels.back();
      if (points.size() == 1) {
          return;
      }
      std::vector<Vector2D> new_points;
      Vector2D toAdd;
      for (int i = 0; i < points.size() - 1; i++) {
          toAdd = lerp(points[i], points[i+1], t);
          new_points.push_back(toAdd);
      }
      evaluatedLevels.push_back(new_points);
  }


  Vector3D BezierPatch::evaluate(double u, double v) const
  {
    // TODO Part 2.
    // Evaluate the Bezier surface at parameters (u, v) through 2D de Casteljau subdivision.
    // (i.e. Unlike Part 1 where we performed one subdivision level per call to evaluateStep, this function
    // should apply de Casteljau's algorithm until it computes the final, evaluated point on the surface)
    Vector3D row_evaluated;
    std::vector<Vector3D> points_u_evaluated;
    for (int i = 0; i < controlPoints.size(); i++) {
        row_evaluated = evaluate1D(controlPoints[i], u);
        points_u_evaluated.push_back(row_evaluated);
    }
    return evaluate1D(points_u_evaluated, v);
  }

  Vector3D BezierPatch::evaluate1D(std::vector<Vector3D> points, double t) const
  {
    // TODO Part 2.
    // Optional helper function that you might find useful to implement as an abstraction when implementing BezierPatch::evaluate.
    // Given an array of 4 points that lie on a single curve, evaluates the Bezier curve at parameter t using 1D de Casteljau subdivision.

    //The proper value is returned using tail recursion. Even though tail call optimization is not standard in C++, this saves code
    // and hopefully stack space.
    /*
    if (points.size() == 1) {
        return points[0];
    }
    std::vector<Vector3D> new_points;
    Vector3D toAdd;
    for (int i = 0; i < points.size() - 1; i++) {
        toAdd = lerp3D(points[i], points[i+1], t);
        new_points.push_back(toAdd);
    }
    return evaluate1D(new_points, t); //Tail call to save stack space
    */

     //iterative version perscribed by instructors
     std::vector<Vector3D> current_points = points;
     std::vector<Vector3D> resulting_points;
     Vector3D toAdd;
     while (current_points.size() != 1) {
         for (int i = 0; i < current_points.size() - 1; i ++) {
             toAdd = lerp3D(current_points[i], current_points[i+ 1], t);
             resulting_points.push_back(toAdd);
         }
         current_points = resulting_points;
         resulting_points.clear();
     }

     return current_points[0];
  }



  Vector3D Vertex::normal( void ) const
  {
    // TODO Part 3.
    // TODO Returns an approximate unit normal at this vertex, computed by
    // TODO taking the area-weighted average of the normals of neighboring
    // TODO triangles, then normalizing.
    Vector3D n( 0., 0., 0. );
    HalfedgeCIter h = halfedge();
    HalfedgeCIter h_orig = h;

    Vector3D u, v;
    do
    {
      h = h->twin();
      u = h->vertex()->position - h_orig->vertex()->position;
      v = h->next()->twin()->vertex()->position - h_orig->vertex()->position;

      n += cross(v, u);

      h = h->next();
    }
    while( h != h_orig );

    return n.unit();
  }

  EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
  {
    // TODO Part 4.
    // TODO This method should flip the given edge and return an iterator to the flipped edge.

    //Halfedges
    HalfedgeIter h0, h1, h2, h3, h4, h5, h6, h7, h8, h9;
    h0 = e0->halfedge();
    h3 = h0->twin();
    if (h0->face()->isBoundary() || h3->face()->isBoundary()) {
        return e0;
    }
    h1 = h0->next();
    h2 = h1->next();
    h4 = h3->next();
    h5 = h4->next();
    h6 = h1->twin();
    h7 = h2->twin();
    h8 = h4->twin();
    h9 = h5->twin();

    //Vertices
    VertexIter v0, v1, v2, v3;
    v0 = h0->vertex();
    v1 = h3->vertex();
    v2 = h2->vertex();
    v3 = h5->vertex();

    //Edges
    EdgeIter e1, e2, e3, e4;
    e1 = h1->edge();
    e2 = h2->edge();
    e3 = h4->edge();
    e4 = h5->edge();

    //Faces
    FaceIter f0, f1;
    f0 = h0->face();
    f1 = h3->face();

    //Halfedges
    //setNeighbors(next, twin, vertex, edge, face)
    h0->setNeighbors(h1, h3, v3, e0, f0);
    h1->setNeighbors(h2, h7, v2, e2, f0);
    h2->setNeighbors(h0, h8, v0, e3, f0);
    h3->setNeighbors(h4, h0, v2, e0, f1);
    h4->setNeighbors(h5, h9, v3, e4, f1);
    h5->setNeighbors(h3, h6, v1, e1, f1);
    h6->setNeighbors(h6->next(), h5, v2, e1, h6->face());
    h7->setNeighbors(h7->next(), h1, v0, e2, h7->face());
    h8->setNeighbors(h8->next(), h2, v3, e3, h8->face());
    h9->setNeighbors(h9->next(), h4, v1, e4, h9->face());

    //Vertices
    v0->halfedge() = h2;
    v1->halfedge() = h5;
    v2->halfedge() = h3;
    v3->halfedge() = h0;

    //Edges
    e0->halfedge() = h0;
    e1->halfedge() = h5;
    e2->halfedge() = h1;
    e3->halfedge() = h2;
    e4->halfedge() = h4;

    //Faces
    f0->halfedge() = h0;
    f1->halfedge() = h3;
    return e0;
  }

  VertexIter HalfedgeMesh::splitEdge( EdgeIter e0 )
  {
    // TODO Part 5.
    // TODO This method should split the given edge and return an iterator to the newly inserted vertex.
    // TODO The halfedge of this vertex should point along the edge that was split, rather than the new edges.
    //Halfedges
    HalfedgeIter h0, h1, h2, h3, h4, h5, h6, h7, h8, h9;
    h0 = e0->halfedge();
    h3 = h0->twin();
    if (h0->face()->isBoundary() || h3->face()->isBoundary()) {
        return e0->halfedge()->vertex();
    }
    h1 = h0->next();
    h2 = h1->next();
    h4 = h3->next();
    h5 = h4->next();
    h6 = h1->twin();
    h7 = h2->twin();
    h8 = h4->twin();
    h9 = h5->twin();

    //Vertices
    VertexIter v0, v1, v2, v3;
    v0 = h0->vertex();
    v1 = h3->vertex();
    v2 = h2->vertex();
    v3 = h5->vertex();

    //Edges
    EdgeIter e1, e2, e3, e4;
    e1 = h1->edge();
    e2 = h2->edge();
    e3 = h4->edge();
    e4 = h5->edge();

    //Faces
    FaceIter f0, f1;
    f0 = h0->face();
    f1 = h5->face();

    //New Halfedges
    HalfedgeIter h10, h11, h12, h13, h14, h15;
    h10 = newHalfedge();
    h11 = newHalfedge();
    h12 = newHalfedge();
    h13 = newHalfedge();
    h14 = newHalfedge();
    h15 = newHalfedge();

    //New Vertices
    VertexIter m;
    m = newVertex();
    m->isNew = true;

    //New Edges
    EdgeIter e5, e6, e7;
    e5 = newEdge();
    e6 = newEdge();
    e7 = newEdge();

    //New Faces
    FaceIter f2, f3;
    f2 = newFace();
    f3 = newFace();

    m->position = (v0->position + v1->position) / 2.;

    //setNeighbors(next, twin, vertex, edge, face)
    h0->setNeighbors(h1, h3, m, e0, f0);
    h1->setNeighbors(h10, h6, v1, e1, f0);
    h2->setNeighbors(h12, h7, v2, e2, f2);
    h3->setNeighbors(h11, h0, v1, e0, f1);
    h4->setNeighbors(h14, h8, v0, e3, f3);
    h5->setNeighbors(h3, h9, v3, e4, f1);
    h6->setNeighbors(h6->next(), h1, v2, e1, h6->face());
    h7->setNeighbors(h7->next(), h2, v0, e2, h7->face());
    h8->setNeighbors(h8->next(), h4, v3, e3, h8->face());
    h9->setNeighbors(h9->next(), h5, v1, e4, h9->face());
    h10->setNeighbors(h0, h13, v2, e6, f0);
    h11->setNeighbors(h5, h14, m, e7, f1);
    h12->setNeighbors(h13, h15, v0, e5, f2);
    h13->setNeighbors(h2, h10, m, e6, f2);
    h14->setNeighbors(h15, h11, v3, e7, f3);
    h15->setNeighbors(h4, h12, m, e5, f3);

    //Vertices
    v0->halfedge() = h12;
    v1->halfedge() = h3;
    v2->halfedge() = h10;
    v3->halfedge() = h14;
    m->halfedge() = h0;

    //Edges
    e0->halfedge() = h0;
    e1->halfedge() = h1;
    e2->halfedge() = h2;
    e3->halfedge() = h4;
    e4->halfedge() = h5;
    e5->halfedge() = h15;
    e6->halfedge() = h13;
    e7->halfedge() = h14;

    //Faces
    f0->halfedge() = h0;
    f1->halfedge() = h3;
    f2->halfedge() = h13;
    f3->halfedge() = h14;

    e0->isNew = false;
    e5->isNew = false;
    e6->isNew = true;
    e7->isNew = true;
    return e0->halfedge()->vertex();
  }



  void MeshResampler::upsample( HalfedgeMesh& mesh )
  {
    // TODO Part 6.
    // This routine should increase the number of triangles in the mesh using Loop subdivision.
    // Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
    // Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
    // using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
    // the new subdivided (fine) mesh, which has more elements to traverse. We will then assign vertex positions in
    // the new mesh based on the values we computed for the original mesh.


    // TODO Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
    // TODO and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
    // TODO a vertex of the original mesh.

    for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
      v->isNew = false;
      HalfedgeIter h = v->halfedge();
      HalfedgeIter h_orig = h;
      Vector3D neighbor_position_sum( 0., 0., 0. );
      int n = 0;
      do {
        n++;
        neighbor_position_sum += h->twin()->vertex()->position;
        h = h->twin()->next();

      }
      while( h != h_orig);
      double u;
      if (n==3) {
          u = (3.0/16.0);
      } else {
          u = (3.0/(8.0*n));
      }
      v->newPosition = (1.0-n*u) * v->position + u*neighbor_position_sum;
    }

    // TODO Next, compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
        e->isNew = false;
        HalfedgeIter h = e->halfedge();
        Vector3D A = h->vertex()->position;
        Vector3D B = h->twin()->vertex()->position;
        Vector3D C = h->next()->next()->vertex()->position;
        Vector3D D = h->twin()->next()->next()->vertex()->position;
        e->newPosition = (3.0/8.0) * (A + B) + (1.0/8.0) * (C + D);
    }

    // TODO Next, we're going to split every edge in the mesh, in any order.  For future
    // TODO reference, we're also going to store some information about which subdivided
    // TODO edges come from splitting an edge in the original mesh, and which edges are new,
    // TODO by setting the flat Edge::isNew.  Note that in this loop, we only want to iterate
    // TODO over edges of the original mesh---otherwise, we'll end up splitting edges that we
    // TODO just split (and the loop will never end!)
    Size n = mesh.nEdges();
    EdgeIter e = mesh.edgesBegin();
    for (int i = 0; i < n; i++) {
        EdgeIter nextEdge = e;
        nextEdge++;
        VertexIter m = mesh.splitEdge(e);
        m->newPosition = e->newPosition;
        e = nextEdge;
    }

    // TODO Now flip any new edge that connects an old and new vertex.
    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
        if (e->isNew) {
            bool v1 = e->halfedge()->vertex()->isNew;
            bool v2 = e->halfedge()->twin()->vertex()->isNew;
            if (v1 ^ v2) {
                mesh.flipEdge(e);
            }
        } else {
            continue;
        }
    }

    // TODO Finally, copy the new vertex positions into final Vertex::position.

    for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
        v->position = v->newPosition;
    }

  }

}
