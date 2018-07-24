#include "student_code.h"
#include "mutablePriorityQueue.h"
#include "halfEdgeMesh.h"

using namespace std;

namespace CGL
{


 

  Vector3D Vertex::normal( void ) const
  {
    // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
  }

  EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
  {   
    // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
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
       // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
  }

  /** return num verts connected to e0 **/
  int HalfedgeMesh::numberOfNeighboringVerts( EdgeIter e0) {
    int numNeighbors = 0;
    HalfedgeIter h1 = e0->halfedge();
    HalfedgeIter h2 = h1->twin();
    HalfedgeIter curr = h1;
    VertexIter currVert;
    VertexIter checkerVert;
    HalfedgeIter checker = curr->twin();

    do {
      do {
        // check if verts are the same
        if (checker->twin()->vertex() == curr->twin()->vertex()) {
          numNeighbors += 1;
        }
        // go to the next edge w/ checker
        checker = checker->twin()->next();
      } while (checker != h2);
      curr = curr->twin()->next();
    } while (curr != h1);

    return numNeighbors;
  }

  VertexIter HalfedgeMesh::collapseEdge( EdgeIter e ) {
    // dont collapse boundry edges
    if (e->isBoundary()) {
      return e->halfedge()->vertex();
    }

    // check for the edge case of less than 1 or more than 2 incident verts to the edge
    // if so, dont collapse, return the edges vertex.
    if (numberOfNeighboringVerts(e) != 2) {
      return e->halfedge()->vertex();
    }

    HalfedgeIter h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10;
    h0 = e->halfedge();
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
	v2 = h7->vertex();
	v3 = h9->vertex();

    EdgeIter e1, e2, e3, e4;
	e1 = h5->edge();
	e2 = h1->edge();
	e3 = h2->edge();
    e4 = h4->edge();

   	FaceIter f0, f1;
   	f0 = h0->face();
   	f1 = h3->face();

    VertexIter midpoint = newVertex();

    Vector3D midpoint_position = (v0->position + v1->position) / 2.0;

    if( v0->isBoundary() ) {
      midpoint->position = v0->position;
    } else if( v1->isBoundary() ) {
      midpoint->position = v1->position;
    } else {
      midpoint->position = midpoint_position;
    }

    // create new mesh elements
    EdgeIter enew0 = newEdge();
    EdgeIter enew1 = newEdge();

    midpoint->halfedge() = h6;


    // Setting the right outside half-edges to have midpoint vertex
    HalfedgeIter right_outside_halfedge = v0->halfedge();
    do
    {
      right_outside_halfedge->setNeighbors(right_outside_halfedge->next(),
                                           right_outside_halfedge->twin(), midpoint,
                                           right_outside_halfedge->edge(),
                                           right_outside_halfedge->face());
      right_outside_halfedge = right_outside_halfedge->twin()->next();

    } while (right_outside_halfedge != v0->halfedge());


    // Setting the left outside half-edges to have midpoint vertex
    HalfedgeIter left_outside_halfedge = v1->halfedge();
    do
    {
      left_outside_halfedge->setNeighbors(left_outside_halfedge->next(),
                                          left_outside_halfedge->twin(), midpoint,
                                          left_outside_halfedge->edge(),
                                          left_outside_halfedge->face());
      left_outside_halfedge = left_outside_halfedge->twin()->next();

    } while (left_outside_halfedge != v1->halfedge());

    h6->setNeighbors(h6->next(), h9, h6->vertex(), enew1, h6->face());
    h7->setNeighbors(h7->next(), h8, h7->vertex(), enew0, h7->face());
    h8->setNeighbors(h8->next(), h7, h8->vertex(), enew0, h8->face());
    h9->setNeighbors(h9->next(), h6, h9->vertex(), enew1, h9->face());

    enew0->halfedge() = h8;
    enew1->halfedge() = h6;
    v2->halfedge() = h7;
    v3->halfedge() = h9;


    //Deleting halfedges since they point to everything
    deleteHalfedge(h0);
    deleteHalfedge(h1);
    deleteHalfedge(h2);
    deleteHalfedge(h3);
    deleteHalfedge(h4);
    deleteHalfedge(h5);

    deleteFace(f0);
    deleteFace(f1);

    //delete all of the original edges
    deleteEdge(e);
    deleteEdge(e1);
    deleteEdge(e2);
    deleteEdge(e3);
    deleteEdge(e4);

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
    


    // This part of the project is unpublishible due to the fact that it is a part of a project that is still 
    // being used at UC Berkeley CS184 
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


  /** DEPRECATED **/
  /** Deletes vertex v and its incident edges. **/
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

  VertexIter HalfedgeMesh::computeCentroids(VertexIter v){
    v->computeCentroid();
    return v;
  }

  void MeshResampler::centerVertices(HalfedgeMesh& mesh) {
    for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
      v->computeCentroid();
    }
    for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
      Vector3D diff_vec = v->centroid - v->position;
      Vector3D vertex_normal = v->normal();
      diff_vec = diff_vec - dot(vertex_normal, diff_vec) * vertex_normal;
      v->position = v->position + (4.0/5.0) * diff_vec;
    }
  }

  float MeshResampler::avgEdgeLength(HalfedgeMesh& mesh) {
    float totalLen = 0.0;
    float numEdges = 0.0;
    for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      totalLen += e->length();
      numEdges++;
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
      int currValenceError = abs(deg1 - optimalValence) + abs(deg2 - optimalValence)
                         + abs(deg3 - optimalValence) + abs(deg4 - optimalValence);

      int newValenceError = abs(deg1 - optimalValence - 1) + abs(deg2 - optimalValence - 1)
      + abs(deg3 - optimalValence + 1) + abs(deg4 - optimalValence + 1);

      if (currValenceError > newValenceError) {
        mesh.flipEdge(e);
      }
    }
  }

  void MeshResampler::incrementalRemeshing(HalfedgeMesh& mesh) {
    float L = avgEdgeLength(mesh);
    float L_max = (4.0 / 3.0) * L;
    float L_min = (4.0 / 5.0) * L;

    Size n = mesh.nEdges();
    EdgeIter e = mesh.edgesBegin();
    for (int i = 0; i < n; i++) {
      EdgeIter nextEdge = e;
      nextEdge++;
      if (e->length() > L_max) {
        mesh.splitEdge(e);
      }
      e = nextEdge;
    }

    for(EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      if(e->length() < L_min) {
        int numEdges = mesh.nEdges();
        mesh.collapseEdge(e);
        int currEdges = mesh.nEdges();
        int diff = abs(numEdges - currEdges);
        if(diff > 0) {
          e = mesh.edgesBegin();
        }
      }
    }

    reduceValence(mesh);
    centerVertices(mesh);
  }



}
