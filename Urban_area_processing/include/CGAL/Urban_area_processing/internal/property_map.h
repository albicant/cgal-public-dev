// Copyright (c) 2020 SARL GeometryFactory (France).
// All rights reserved.
//
// This file is a part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s)     : Dmitry Anisimov, Simon Giraudot, Pierre Alliez, Florent Lafarge, and Andreas Fabri

#ifndef CGAL_URBAN_AREA_PROCESSING_INTERNAL_PROPERTY_MAP_H
#define CGAL_URBAN_AREA_PROCESSING_INTERNAL_PROPERTY_MAP_H

// #include <CGAL/license/Urban_area_processing.h>

namespace CGAL {
namespace Urban_area_processing {
namespace internal {

  struct Seed_property_map {                        
    using key_type = std::size_t;
    using value_type = std::size_t;
    using category = boost::lvalue_property_map_tag;

    const std::vector<std::size_t>& m_seeds;
    Seed_property_map(
      const std::vector<std::size_t>& seeds) : 
    m_seeds(seeds) 
    { }

    value_type operator[](const key_type key) const { 
      return m_seeds[key];
    }

    friend value_type get(
      const Seed_property_map& seed_map, 
      const key_type key) { 
      return seed_map[key];
    }
  };

} // internal
} // Urban_area_processing
} // CGAL

#endif // CGAL_URBAN_AREA_PROCESSING_INTERNAL_PROPERTY_MAP_H
