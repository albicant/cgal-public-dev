// Copyright (c) 2019 GeometryFactory Sarl (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s)     : Jean-Philippe Bauchet, Florent Lafarge, Gennadii Sytov, Dmitry Anisimov
//

#ifndef CGAL_SHAPE_REGULARIZATION_CGAL_QUADRATIC_PROGRAM_H
#define CGAL_SHAPE_REGULARIZATION_CGAL_QUADRATIC_PROGRAM_H

// #include <CGAL/license/Shape_regularization.h>

// STL includes.
#include <vector>
#include <utility>

// CGAL includes.
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// Internal includes.
#include <CGAL/Shape_regularization/internal/utils.h>

namespace CGAL {
namespace Shape_regularization {  

  /*!
    \ingroup PkgShapeRegularizationRefSolvers
    
    \brief wraps the \cgal QP solver.

    This class wraps the \ref PkgQPSolver "CGAL solver"
    and sets all its parameters to default.

    \tparam FT
    number type.
  */
  template<typename FT>
  class CGAL_quadratic_program : 
    public CGAL::Quadratic_program<int> {

    using Quadratic_program = CGAL::Quadratic_program<int>;

  public:

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.
    */
    CGAL_quadratic_program() : 
      Quadratic_program(
        CGAL::SMALLER, 
        true, 
        -internal::max_value<FT>(),
        true,
        +internal::max_value<FT>())
    { }

    /// @}

    /// \cond SKIP_IN_MANUAL
    void set_a(int i, int j, const FT& val) {
      Quadratic_program::set_a(j, i, val);
    }
    /// \endcond
  };

  /*!
    \ingroup PkgShapeRegularizationRefSolvers
    
    \brief solves a \cgal quadratic program.

    \tparam FT
    number type.

    \param qp
    a quadratic program to be solved
  */
  template<typename FT>
  CGAL::Quadratic_program_solution<FT> solve_quadratic_program(
    const CGAL::Shape_regularization::CGAL_quadratic_program<FT>& qp) {
    return CGAL::solve_quadratic_program(qp, FT());
  }

} // namespace Shape_regularization
} // namespace CGAL

#endif // CGAL_SHAPE_REGULARIZATION_CGAL_QUADRATIC_PROGRAM_H
