#ifndef STUDENT_CODE_H
#define STUDENT_CODE_H

#include "halfEdgeMesh.h"
#include "bezierPatch.h"
#include "bezierCurve.h"

using namespace std;

namespace CGL {

  class MeshResampler{

  public:

    MeshResampler(){};
    ~MeshResampler(){}

    void upsample(HalfedgeMesh& mesh);

    void quadraticSimplify(HalfedgeMesh& mesh);
    void centerVertices(HalfedgeMesh& mesh);
    float avgEdgeLength(HalfedgeMesh& mesh);
    void incrementalRemeshing(HalfedgeMesh& mesh);
    void reduceValence(HalfedgeMesh& mesh);
  };
}

#endif // STUDENT_CODE_H
