#include "student_code.h"
#include "mutablePriorityQueue.h"
#include "halfEdgeMesh.h"

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
    if (!canFlip(e0)) {
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

  double HalfedgeMesh::getAngle(Vector3D a, Vector3D b, Vector3D c) {
    Vector3D ab = a - b;
    Vector3D cb = c - b;

    ab.normalize();
    cb.normalize();

    double d = dot(ab, cb);
    d = acos(d);
    d = d * 180 / M_PI;
    return d;
  }

  bool HalfedgeMesh::canFlip( EdgeIter e) {
    HalfedgeIter pq, qp;
    pq = e->halfedge();
    qp = pq->twin();

    Vector3D p, q, r, s;
    p = pq->vertex()->position;
    q = qp->vertex()->position;
    r = pq->next()->next()->vertex()->position;
    s = qp->next()->next()->vertex()->position;

    double prs, qrs, rps, prq, pqs,
      qps, spq, rpq, psr, qsr, rqs, psq, pqr, qpr, sqp, rqp;
    // |A−B||C−B|cos(ABC) for angle abc
    prs = getAngle(p, r, s);
    pqs = getAngle(p, q, s);
    psr = getAngle(p, s, r);
    pqr = getAngle(p, q, r);

    if(!(prs > fmin(M_PI_2, pqs) && psr > fmin(M_PI_2, pqr))) {
      return false;
    }

    qrs = getAngle(q, r, s);
    qps = getAngle(q, p, s);
    qsr = getAngle(q, s, r);
    qpr = getAngle(q, p, r);

    if(!(qrs > fmin(M_PI_2,  qps) && qsr > fmin(M_PI_2, qpr))) {
      return false;
    }

    rps = getAngle(r, p, s);
    rpq = getAngle(r, p, q);
    spq = getAngle(s, p, q);
    rqs = getAngle(r, q, s);
    rqp = getAngle(r, q, p);
    sqp = getAngle(s, q, p);

    if(!(rps > fmin(rpq, spq) && rqs > fmin(rqp, sqp))) {
      return false;
    }

    prq = getAngle(p, r, q);
    // prs
    // qrs
    psq = getAngle(p, s, q);
    // psr
    // qsr

    if(!(prq > fmin(prs,  qrs) && psq > fmin(psr, qsr))) {
      return false;
    }

    // if boundry, return false
    if (pq->face()->isBoundary() || qp->face()->isBoundary()) {
      return false;
    }
    // check if it overlaps with another mesh element


    return true;
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
  
  /*
  Old collapse edge
  VertexIter HalfedgeMesh::collapseEdge(EdgeIter e0)
  {
    // TODO Brian Ho.
    // TODO This method will delete the given edge and combine the endpoints.

    //if (e0->halfedge()->vertex()->isBoundary() || e0->halfedge()->twin()->vertex()->isBoundary()) {
	  //return e0->halfedge()->vertex();
    //}

    // Halfedges
    HalfedgeIter h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11;
    h0 = e0->halfedge();
    h1 = h0->twin();
    h2 = h0->next();
    h3 = h2->next();
    h4 = h1->next();
    h5 = h4->next();
    h6 = h5->twin();
    h7 = h2->twin();
    h8 = h3->twin();
    h9 = h4->twin();
    h10 = h9->next();
    h11 = h8->next();

    // Vertices
    VertexIter v0, v1, v2, v3;
    v0 = h0->vertex();
    v1 = h1->vertex();
    v2 = h5->vertex();
    v3 = h3->vertex();

    // Edges
    EdgeIter e1, e2, e3, e4;
    e1 = h4->edge();
    e2 = h5->edge();
    e3 = h3->edge();
    e4 = h2->edge();

    // Faces
    FaceIter f0, f1, f2, f3, f4, f5;
    f0 = h1->face();
    f1 = h0->face();
    f2 = h6->face();
    f3 = h7->face();
    f4 = h9->face();
    f5 = h8->face();


    // New coordinate of the singular vertex
    Vector3D newVertexLocation = (v0->position + v1->position) / 2.0;
	
	HalfedgeIter another = h1;
	do
	{
	  another->vertex() = v0;
	  another = another->twin()->next();
	} while (another != h1);

    // setNeighbors(next, twin, vertex, edge, face)
    h9->setNeighbors(h10, h6, v2, e1, f4);
    h8->setNeighbors(h11, h7, v0, e3, f5);
    h6->setNeighbors(h6->next(), h9, v0, e1, f2);
    h7->setNeighbors(h7->next(), h8, v3, e3, f3);
	
    // Vertices
    v0->halfedge() = h8;
    v2->halfedge() = h9;
    v3->halfedge() = h7;

    // Edges
    e1->halfedge() = h9;
    e3->halfedge() = h8;

    // Faces
    f2->halfedge() = h6;
    f3->halfedge() = h7;

    // Delete unneccessary stuff
    deleteHalfedge(h0);
    deleteHalfedge(h1);
    deleteHalfedge(h2);
    deleteHalfedge(h3);
    deleteHalfedge(h4);
    deleteHalfedge(h5);

    deleteVertex(v1);

	deleteEdge(e0);
	if (e2 != e4) {
	  deleteEdge(e4);
	}
	deleteEdge(e2);

    deleteFace(f0);
    deleteFace(f1);

    // Reposition vertex
    v0->position = newVertexLocation;
    return v0;

  }
   */
  
  VertexIter HalfedgeMesh::collapseEdge( EdgeIter e0 ) {
	
	HalfedgeIter h0, h1, h2, h3, h4, h5, h6, h7, h8, h9;
	h0 = e0->halfedge();
	h1 = h0->next();
	h2 = h1->next();
	h3 = h0->twin();
	h4 = h3->next();
	h5 = h4->next();
	h6 = h5->twin();
	h7 = h1->twin();
	h8 = h2->twin();
	h9 = h4->twin();
	
	VertexIter v0, v1, v2, v3;
	v0 = h0->vertex();
	v1 = h3->vertex();
	v2 = h5->vertex();
	v3 = h2->vertex();
	
	EdgeIter e1, e2, e3, e4;
	e1 = h5->edge();
	e2 = h1->edge();
	e3 = h2->edge();
	e4 = h4->edge();
	
	FaceIter f0, f1;
	f0 = h0->face();
	f1 = h3->face();
	
	
	/// Replacing the edge with a midpoint
	VertexIter midpoint = newVertex();
	
	Vector3D midpoint_position = (v0->position + v1->position) / 2.0;
	
	midpoint->position = midpoint_position;
	
	///Setting the right outside halfedges to have midpoint vertex
	HalfedgeIter right_outside_halfedge = h7->next();
	do
	{
	  right_outside_halfedge->setNeighbors(right_outside_halfedge->next(), right_outside_halfedge->twin(), midpoint, right_outside_halfedge->edge(), right_outside_halfedge->face());
	  right_outside_halfedge = right_outside_halfedge->twin()->next();
	  
	} while (right_outside_halfedge != h6);
	
	
	///Setting the left outside haldedges to have midpoint vertex
	HalfedgeIter left_outside_halfedge = h9->next();
	do
	{
	  left_outside_halfedge->setNeighbors(left_outside_halfedge->next(), left_outside_halfedge->twin(), midpoint, left_outside_halfedge->edge(), left_outside_halfedge->face());
	  left_outside_halfedge = left_outside_halfedge->twin()->next();
	  
	} while (left_outside_halfedge != h8);
	
	///Setting halfedges that will be modifed
	h6->setNeighbors(h6->next(), h9, midpoint, e4, h6->face());
	h7->setNeighbors(h7->next(), h8, v3, e3, h7->face());
	h8->setNeighbors(h8->next(), h7, midpoint, e3, h8->face());
	h9->setNeighbors(h9->next(), h6, v2, e4, h9->face());
	
	///Setting midpoint halfedge
	midpoint->halfedge() = h6;
	
	v2->halfedge() = h9;
	v3->halfedge() = h7;
	
	e4->halfedge() = h6;
	e3->halfedge() = h8;
	
	///Deleting halfedges since they point to everything
	deleteHalfedge(h0);
	deleteHalfedge(h1);
	deleteHalfedge(h2);
	deleteHalfedge(h3);
	deleteHalfedge(h4);
	deleteHalfedge(h5);
	
	deleteFace(f0);
	deleteFace(f1);
	
	deleteEdge(e0);
	deleteEdge(e1);
	deleteEdge(e2);
	
	deleteVertex(v0);
	deleteVertex(v1);
	
	return midpoint;
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

  std::set<HalfedgeIter> HalfedgeMesh::findIncidentEdges(VertexIter v, std::vector<HalfedgeIter>* outerHalfEdges) {
      HalfedgeIter h = v->halfedge();
      std::set<HalfedgeIter> incidentHalfEdges = std::set<HalfedgeIter>();
      do
      {
        incidentHalfEdges.insert(h);
        incidentHalfEdges.insert(h->twin());
        h->next()->vertex()->halfedge() = h->next();
        outerHalfEdges->push_back(h->next());
        h = h->twin()->next();
      }
      while(h != v->halfedge());
      return incidentHalfEdges;
  }

  void HalfedgeMesh::remeshEmptyPolygon(std::vector<HalfedgeIter> outerHalfEdges,
                                        std::vector<FaceIter> faces, int deg, VertexIter v,
                                        std::set<HalfedgeIter> incidentHalfEdges) {
    std::vector<HalfedgeIter> newEdges = std::vector<HalfedgeIter>();
    VertexIter start = outerHalfEdges[0]->vertex();
    VertexIter prev = start;
    int faceNum = 0;

    HalfedgeIter original = start->halfedge();

    HalfedgeIter nextHe = outerHalfEdges[0];
    for(int i = deg - 2; i > 1; i--) {
      VertexIter current = outerHalfEdges[i]->vertex();
      HalfedgeIter h1 = newHalfedge();
      HalfedgeIter h2 = newHalfedge();
      EdgeIter e = newEdge();
      e->halfedge() = h1;
      e->newPosition = current->position;


      // confirmed assigned in correct order
      h1->setNeighbors(nextHe, h2, current, e, faces[faceNum]);
      h2->setNeighbors(outerHalfEdges[i], h1, start, e, faces[faceNum + 1]);

      newEdges.push_back(h1);
      newEdges.push_back(h2);


      // if the current vertices he is one thats going to be deleted, set it to the new one.
      // still something wrong with the pointers... 
      if(current->halfedge()->next()->vertex() == v) {
        current->halfedge() = h1;
      }


      start->halfedge() = h2;
      faces[faceNum]->halfedge() = h1;
      outerHalfEdges[5]->next() = h2;
      nextHe = h2;
      faceNum += 1;
    }

    for(HalfedgeIter he : newEdges) {
      he->next()->next()->next() = he;
      FaceIter curr = he->face();
      curr->halfedge() = he;
    }



    //reset verts original he's
    start->halfedge() = original;

  }

  /** Deletes vertex v and its incident edges. Still needs to be fixed.**/
  void HalfedgeMesh::deleteMeshVertex(VertexIter v) {
    HalfedgeIter original = v->halfedge();
    HalfedgeIter curr = original->next();
    std::set<HalfedgeIter> incidentHalfEdges = std::set<HalfedgeIter>();
    std::set<EdgeIter> incidentEdges = std::set<EdgeIter>();
    std::vector<FaceIter> incidentFaces = std::vector<FaceIter>();
    int deg = v->degree();
    int count = 0;

    //collect incident vertices for later, and delete the inner faces
    while(count < deg) {
      if(incidentFaces.size() >= deg - 2) {
        deleteFace(curr->face());
      } else if (std::find(incidentFaces.begin(), incidentFaces.end(), curr->face()) == incidentFaces.end()){
        incidentFaces.push_back(curr->face());
      }
      curr = curr->next()->twin()->next();
      count++;
    }

    // find the incident half edges, as well as the corresponding edges, and delete the half edges, dont delete edges yet,
    // will cause seg fault.
    std::vector<HalfedgeIter> outerEdges = std::vector<HalfedgeIter>();
    incidentHalfEdges = findIncidentEdges(v, &outerEdges);

    remeshEmptyPolygon(outerEdges, incidentFaces, deg, v, incidentHalfEdges);
    for(HalfedgeIter h : incidentHalfEdges) {
      incidentEdges.insert(h->edge());
      deleteHalfedge(h);
    }

    // Delete the original edges.
    for (EdgeIter e: incidentEdges) {
      deleteEdge(e);
    }

    deleteVertex(v);
  }

  void MeshResampler::quadraticSimplify(HalfedgeMesh &mesh) {

    ///  The first part is to compute the quadrics for each face
    for (FaceIter face = mesh.facesBegin(); face != mesh.facesEnd(); face++) {
      face->calculate_quadric();
    }

    ///  Second part is to compute the quadrics for each vertex
    for (VertexIter vertex = mesh.verticesBegin(); vertex != mesh.verticesEnd(); vertex++) {
      vertex->calculate_quadratic();
    }

    ///  For each edge, create an edgeRecord and insert it into one global MutablePriorityQueue

      MutablePriorityQueue<EdgeRecord> m_queue;
    for (EdgeIter edge = mesh.edgesBegin(); edge != mesh.edgesEnd(); edge++) {
      EdgeRecord edge_record = EdgeRecord(edge);
      m_queue.insert(edge_record);
    }

    ///  Until a target number of triangles is reached, collapse the best/cheapest edge.
    ///  Set this number to 1/4th the number of triangles in the input (since subdivision will give you a factor of 4 in the opposite direction)
    Size target_triangles = mesh.nFaces() / 4;

    while (mesh.nFaces() > target_triangles && !m_queue.empty()) {
      EdgeRecord cheapest_record = m_queue.top();
      m_queue.pop();
	  
      EdgeIter cheap_edge = cheapest_record.edge;
      Matrix4x4 new_quadric = cheap_edge->halfedge()->vertex()->quadric + cheap_edge->halfedge()->twin()->vertex()->quadric;

      ///  Removing any edge touching either of its endpoints from the queue
      VertexIter edge_point1 = cheap_edge->halfedge()->vertex();
      VertexIter edge_point2 = cheap_edge->halfedge()->twin()->vertex();

      HalfedgeIter h1 = edge_point1->halfedge();

      do
      {
        if (h1->edge() != cheap_edge) {
          m_queue.remove(EdgeRecord(h1->edge()));
        }
        h1 = h1->twin()->next();
      } while (h1 != edge_point1->halfedge());


      HalfedgeIter h2 = edge_point2->halfedge();

      do
      {
        if (h2->edge() != cheap_edge) {
          m_queue.remove(EdgeRecord(h2->edge()));
        }
        h2 = h2->twin()->next();
      } while (h2 != edge_point2->halfedge());


      /// Collapse the edge
      VertexIter new_vertex = mesh.collapseEdge(cheap_edge);

      ///  Set the quadric of the new vertex to the quadric computed in Step 2.
      new_vertex->quadric = new_quadric;

      ///  Insert any edge touching the new vertex into the queue, creating new edge records for each of them.
      HalfedgeIter new_vertex_h = new_vertex->halfedge();

      do
      {

        m_queue.insert(EdgeRecord(new_vertex_h->edge()));
        new_vertex_h = new_vertex_h->twin()->next();

      } while (new_vertex_h != new_vertex->halfedge());

    }

  }

  void Vertex::computeCentroid() {
    HalfedgeIter h = this->halfedge();
    std::set<HalfedgeIter> incidentHalfEdges = std::set<HalfedgeIter>();
    do
    {
      incidentHalfEdges.insert(h);
      incidentHalfEdges.insert(h->twin());
      h->next()->vertex()->halfedge() = h->next();
      h = h->twin()->next();
    }
    while(h != this->halfedge());

    Vector3D pos(0.0, 0.0, 0.0);
    float count = 0.0;
    for(HalfedgeIter h: incidentHalfEdges) {
      pos += h->vertex()->position;
      count += 1.0;
    }
    pos /= count;
    this->centroid = pos;
  }

  VertexIter HalfedgeMesh::vertexShift(VertexIter v) {
    v->computeCentroid();
    v->position = v->centroid;
    return v;
  }

  void MeshResampler::centerVertices(HalfedgeMesh& mesh) {
    for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
      mesh.vertexShift(v);
    }
  }

  float MeshResampler::avgEdgeLength(HalfedgeMesh& mesh) {
    float totalLen = 0.0;
    float numEdges = 0.0;
    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      totalLen += e->length();
    }
    if (numEdges == 0.0) {
      return 0.0;
    }
    return totalLen / numEdges;

  }

  void MeshResampler::reduceValence(HalfedgeMesh& mesh) {

    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {

      // collect all verts around the edge we might flip
      VertexIter v = e->halfedge()->vertex();
      VertexIter v1 = e->halfedge()->twin()->vertex();
      VertexIter v2 = e->halfedge()->next()->next()->vertex();
      VertexIter v3 = e->halfedge()->twin()->next()->next()->vertex();

      // assure that absolute val of the deg of all 4 verts will decrease
      // if less than 6 is bad as well as being more than 6, but if edge, optimal valence is 4 so account for that
      int optimalValence = 6;
      if (e->isBoundary()) {
        optimalValence = 4;
      }

      int deg1 = v->degree();
      int deg2 = v1->degree();
      int deg3 = v2->degree();
      int deg4 = v3->degree();
	  
      // using formula from slide 25 of : http://www.hao-li.com/cs599-ss2015/slides/Lecture09.1.pdf
      int currTotalDeg = abs(deg1 - optimalValence) + abs(deg2 - optimalValence)
                         + abs(deg3 - optimalValence) + abs(deg4 - optimalValence);
      mesh.flipEdge(e);
      int newDeg1 = e->halfedge()->vertex()->degree();
      int newDeg2 = e->halfedge()->twin()->vertex()->degree();
      int newDeg3 = e->halfedge()->next()->next()->vertex()->degree();
      int newDeg4 = e->halfedge()->twin()->next()->next()->vertex()->degree();

      int newTotalDeg = abs(deg1 - optimalValence) + abs(deg2 - optimalValence)
                         + abs(deg3 - optimalValence) + abs(deg4 - optimalValence);

      // if flip doesn't improve valence, put it back
      if (currTotalDeg >= newTotalDeg) {
        mesh.flipEdge(e);
      }
    }
  }

  void MeshResampler::incrementalRemeshing(HalfedgeMesh& mesh) {
    float L = avgEdgeLength(mesh);
    float L_max = (4.0 / 3.0) * L;
    float L_min = (4.0 / 5.0) * L;

    std::vector<EdgeIter> toSplit = std::vector<EdgeIter>();
    std::vector<EdgeIter> toCollapse = std::vector<EdgeIter>();
    std::vector<EdgeIter> toFlip = std::vector<EdgeIter>();



    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      if (e->length() > L_max) {
        toSplit.push_back(e);
      } else if (e->length() < L_min) {
        toCollapse.push_back(e);
      }
    }

    for(EdgeIter e: toSplit)  {
      mesh.splitEdge(e);
    }

    for(EdgeIter e: toCollapse) {
      mesh.collapseEdge(e);
    }

    int total = 0;
    int g0 = 0;
    int l0 = 0;

    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
     if(6 > e->halfedge()->vertex()->degree()) {
       g0 += 1;
     } else if (6 < e->halfedge()->vertex()->degree()) {
       l0 += 1;
     }
      total += 1;
    }

    reduceValence(mesh);


    centerVertices(mesh);
  }



}
