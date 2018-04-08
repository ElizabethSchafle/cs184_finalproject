#ifndef CGL_COLLADA_H
#define CGL_COLLADA_H

#include <string>
#include <vector>

#include "CGL/CGL.h"
#include "CGL/tinyxml2.h"

#include "scene.h"
#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "material.h"


using namespace tinyxml2;

namespace CGL {

  class ColladaParser {
  public:

    static int load( const char* filename, Scene* scene );
    static int save( const char* filename, const Scene* scene );

  private:

    // XML entry points for COLLADA libraries (set on load) //
    static XMLElement* e_scenes;       // COLLADA library: scenes
    static XMLElement* e_cameras;      // COLLADA library: cameras
    static XMLElement* e_lights;       // COLLADA library: lights
    static XMLElement* e_geometries;   // COLLADA library: geometries
    static XMLElement* e_materials;    // COLLADA library: materials
    static XMLElement* e_effects;      // COLLADA library: effects

    static void parseScene    ( XMLElement* xml, Scene& scene       );
    static void parseNode     ( XMLElement* xml, Node& node         );
    static void parseCamera   ( XMLElement* xml, Camera& camera     );
    static void parseLight    ( XMLElement* xml, Light& light       );
    static void parsePolymesh ( XMLElement* xml, Polymesh& polymesh );
    static void parseMaterial ( XMLElement* xml, Material& material );

  }; // class ColladaParser

} // namespace CGL

#endif // CGL_COLLADA_H
