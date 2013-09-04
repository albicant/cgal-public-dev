namespace CGAL {

/*!
\ingroup PkgConvexHull2Traits

The class `Convex_hull_constructive_traits_2` serves as a traits class for all the two-dimensional 
convex hull and extreme point calculation function. Unlike the class 
`CGAL::Convex_hull_traits_2<R>`, this class makes use of previously 
computed results to avoid redundancy. For example, 
in the sidedness tests, lines (of type `R::Line_2`) are constructed, 
which is equivalent to the precomputation of subdeterminants of 
the orientation-determinant for three points. 

\cgalModels `ConvexHullTraits_2`

\sa `CGAL::Convex_hull_projective_xy_traits_2<Point_3>` 
\sa `CGAL::Convex_hull_projective_xz_traits_2<Point_3>` 
\sa `CGAL::Convex_hull_projective_yz_traits_2<Point_3>` 
\sa `CGAL::Convex_hull_traits_2<R>` 

*/
template< typename R >
class Convex_hull_constructive_traits_2 {
public:

/// \name Types 
/// @{

/*!

*/ 
typedef R::Point_2 Point_2; 

/*!

*/ 
typedef R::Less_xy_2 Less_xy_2; 

/*!

*/ 
typedef R::Less_yx_2 Less_yx_2; 

/*!

*/ 
typedef CGAL::r_Less_dist_to_line<R> 
Less_signed_distance_to_line_2; 

/*!

*/ 
typedef R::Less_rotate_ccw Less_rotate_ccw_2; 

/*!

*/ 
typedef R::Left_turn_2 Left_turn_2; 

/*!

*/ 
typedef R::Equal_2 Equal_2; 

/// @} 

/// \name Creation 
/// @{

/*!
default constructor. 
*/ 
Convex_hull_constructive_traits_2(); 

/// @} 

/// \name Operations 
/// @{

/*!

*/ 
Less_xy_2 less_xy_2_object(); 

/*!

*/ 
Less_yx_2 less_yx_2_object(); 

/*!

*/ 
Less_signed_distance_to_line_2 
less_signed_distance_to_line_2_object(); 

/*!

*/ 
Less_rotate_ccw_2 
less_rotate_ccw_2_object(); 

/*!

*/ 
Left_turn_2 left_turn_2_object(); 

/*!

*/ 
Equal_2 equal_2_object(); 

/// @}

}; /* end Convex_hull_constructive_traits_2 */
} /* end namespace CGAL */