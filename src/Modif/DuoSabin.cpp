
#ifdef USING_CGAL

#include "Modif.h"



#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>

// Define the kernel and triangulation types
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_3<K> Triangulation;



using namespace PP3d;

void CGAL_DDuoSabin() {
  // Create a vector of points to be triangulated
  std::vector<K::Point_3> points = {
    K::Point_3(0,0,0), K::Point_3(1,0,0), K::Point_3(0,1,0), K::Point_3(0,0,1)
  };

  // Create a triangulation object and insert the points
  Triangulation triangulation;
  triangulation.insert(points.begin(), points.end());
}

#endif
