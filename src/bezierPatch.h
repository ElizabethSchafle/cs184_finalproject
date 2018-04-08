#ifndef BEZIERPATCH_H
#define	BEZIERPATCH_H

#include "CGL/CGL.h"
#include "mesh.h"

namespace CGL {

  class BezierPatch {
  public:
    BezierPatch();

    void add2mesh(Polymesh* mesh) const;
    void loadControlPoints(FILE* file);

    Vector3D lerp3D(Vector3D v0, Vector3D v1, double t) const {
      return (v0 - v0*t) + v1*t;
    }
  protected:
    std::vector< std::vector<Vector3D> > controlPoints;
    Vector3D evaluate(double u, double v) const;
    Vector3D evaluate1D(std::vector<Vector3D> points, double t) const;
    void addTriangle(Polymesh* mesh, const Vector3D& v0, const Vector3D& v1, const Vector3D& v2) const;
  };

}
#endif	/* BEZIERPATCH_H */
