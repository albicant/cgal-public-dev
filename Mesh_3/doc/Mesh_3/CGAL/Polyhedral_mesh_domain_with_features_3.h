namespace CGAL {

/*!
\ingroup PkgMesh_3Domains

The class `Polyhedral_mesh_domain_with_features_3` implements a domain whose 
boundary is a simplicial polyhedral surface. 
This surface must be free of intersection. 
It must also be either closed 
or included inside another polyhedral surface which is closed and free of intersection.	

It is a model of the concept `MeshDomainWithFeatures_3`. It also 
provides a member function to automatically detect sharp features from 
the input polyhedral surface(s). 

\tparam IGT stands for a geometric traits class providing the types 
and functors required to implement the intersection tests and intersection computations 
for polyhedral boundary surfaces. This parameter has to be 
instantiated with a model of the concept `IntersectionGeometricTraits_3`. 

\cgalModels `MeshDomainWithFeatures_3` 

\sa `CGAL::Mesh_domain_with_polyline_features_3<MeshDomain>` 
\sa `CGAL::Polyhedral_mesh_domain_3<Polyhedron,IGT,TriangleAccessor>` 
\sa `CGAL::Mesh_polyhedron_3<IGT>` 
*/
template< typename IGT >
class Polyhedral_mesh_domain_with_features_3
  : public CGAL::Mesh_domain_with_polyline_features_3<
  CGAL::Polyhedral_mesh_domain_3< CGAL::Mesh_polyhedron_3<IGT>::type, IGT> >
 {
public:

/// \name Types 
/// @{

/*!
Numerical type. 
*/ 
typedef unspecified_type FT; 

/// @} 

/// \name Creation 
/// @{

/*!
Constructs a `Polyhedral_mesh_domain_with_features_3` from a polyhedral surface of type `Polyhedron`. 
The only requirement on type `Polyhedron` is that `CGAL::Mesh_polyhedron_3<IGT>::%type` should 
be constructible from `Polyhedron`. 
No feature detection is done at this level. Note that a copy of `bounding_polyhedron` will be done. 
The interior of `bounding_polyhedron` will be meshed.
*/ 
template <typename Polyhedron> 
Polyhedral_mesh_domain_with_features_3(Polyhedron bounding_polyhedron); 

/*!
Constructs a `Polyhedral_mesh_domain_with_features_3` from an off file. No feature 
detection is done at this level. 
*/ 
Polyhedral_mesh_domain_with_features_3(const std::string& filename); 

/*!
Constructs  a `Polyhedral_mesh_domain_with_features_3` a polyhedral surface, and a bounding polyhedral surface,.
`CGAL::Mesh_polyhedron_3<IGT>::%type` should be constructible from `Polyhedron`. 
The first polyhedron should be entirely included inside `bounding_polyhedron`, which has to be closed 
and free of intersections. 
Using this constructor allows to mesh a polyhedral surface which is not closed, or has holes.
The inside of `bounding_polyhedron` will be meshed.
*/
template <typename Polyhedron> 
Polyhedral_mesh_domain_with_features_3(Polyhedron polyhedron,
							 Polyhedron bounding_polyhedron);

/// @} 

/// \name Operations 
/// @{

/*!
Detects sharp features of the internal bounding polyhedron (and the potential internal polyhedron) 
and inserts them in as features of the domain. `angle_bound` gives the maximum dihedral angle 
(in degrees) between two triangles of the input polyhedron/a which is used to consider that the edge between 
those triangles is a feature edge. 
*/ 
void detect_features(FT angle_bound=120); 

/// @}

}; /* end Polyhedral_mesh_domain_with_features_3 */
} /* end namespace CGAL */