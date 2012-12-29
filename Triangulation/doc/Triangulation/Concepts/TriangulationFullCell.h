
/*!
\ingroup PkgTriangulationsConcepts
\cgalConcept

The concept `TriangulationFullCell` describes the requirements on the type used by the
class `Triangulation<TriangulationTraits, TriangulationDataStructure>`, and its derived classes, to
represent a full cell.

\refines ::TriangulationDSFullCell
We only list below the additional specific requirements of \refines ::TriangulationFullCell.

\cgalHasModel CGAL::Triangulation_full_cell<TriangulationTraits, TriangulationDSFullCell>

Input/Output
--------------

These operators can be used directly and are called by the I/O
operator of class `Triangulation`.

\sa `Triangulation_full_cell<TriangulationTraits, TriangulationDSFullCell>`
\sa `TriangulationVertex`
\sa `Triangulation<TriangulationTraits, TriangulationDataStructure>`

*/

class TriangulationFullCell {
/*!
Inputs additional information stored in the full cell.
*/
std::istream & operator>>(std::istream & is, TriangulationFullCell & c);

/*!
Outputs additional information stored in the full cell.
*/
std::ostream & operator<<(std::ostream & os, const TriangulationFullCell & c);

/// @}

}; /* end TriangulationFullCell */
