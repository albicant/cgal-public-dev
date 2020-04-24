// Copyright (c) 2020 GeometryFactory SARL (France).
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

#ifndef CGAL_SHAPE_REGULARIZATION_ANGLE_REGULARIZATION_2_H
#define CGAL_SHAPE_REGULARIZATION_ANGLE_REGULARIZATION_2_H

// #include <CGAL/license/Shape_regularization.h>

// Boost includes.
#include <CGAL/boost/graph/named_params_helper.h>
#include <CGAL/boost/graph/Named_function_parameters.h>

// Internal includes.
#include <CGAL/Shape_regularization/internal/Segment_wrapper_2.h>
#include <CGAL/Shape_regularization/internal/Grouping_segments_2.h>
#include <CGAL/Shape_regularization/internal/Angle_conditions_2.h>

namespace CGAL {
namespace Shape_regularization {
namespace Segments {

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief An angle-based regularization type for 2D segments that preserves 
    parallelism and orthogonality relationships.

    \tparam GeomTraits 
    must be a model of `Kernel`.

    \tparam InputRange 
    must be a model of `ConstRange` whose iterator type is `RandomAccessIterator`.

    \tparam SegmentMap 
    must be a `ReadablePropertyMap` whose key type is the value type of the input 
    range and value type is `GeomTraits::Segment_2`. %Default is the 
    `CGAL::Identity_property_map<typename GeomTraits::Segment_2>`.

    \cgalModels `RegularizationType`
  */
  template<
  typename GeomTraits, 
  typename InputRange,
  typename SegmentMap = CGAL::Identity_property_map<typename GeomTraits::Segment_2> >
  class Angle_regularization_2 {
  public:

    /// \name Types
    /// @{
  
    /// \cond SKIP_IN_MANUAL
    using Traits = GeomTraits;
    using Input_range = InputRange;
    using Segment_map = SegmentMap;
    /// \endcond

    /// Number type.
    typedef typename GeomTraits::FT FT;

    /// \cond SKIP_IN_MANUAL
    using Point_2 = typename Traits::Point_2;
    using Vector_2 = typename Traits::Vector_2;
    using Segment_2 = typename Traits::Segment_2;

    using Segment_wrapper_2 = typename internal::Segment_wrapper_2<Traits>;
    using Conditions = typename internal::Angle_conditions_2<Traits>;
    using Grouping = internal::Grouping_segments_2<Traits, Conditions>;
    
    using Indices = std::vector<std::size_t>;
    using Size_pair = std::pair<std::size_t, std::size_t>;
    
    using Targets_map = 
      std::map<Size_pair, std::pair< FT, std::size_t> >;
    using Relations_map = 
      std::map<Size_pair, std::pair<int, std::size_t> >;
    /// \endcond

    /// @}

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      \tparam NamedParameters
      a sequence of \ref pmp_namedparameters "Named Parameters".

      \param input_range 
      an instance of `InputRange` with 2D segments

      \param np
      optional sequence of \ref pmp_namedparameters "Named Parameters" 
      among the ones listed below

      \param max_angle
      max angle bound in degrees, the default is 25 degrees 

      \param segment_map
      an instance of `SegmentMap` that maps an item from input range to `GeomTraits::Segment_2`, 
      if not provided, the default is used

      \pre `input_range.size() > 1`
      \pre `max_angle >= 0 && max_angle <= 90`
    */
    template<typename NamedParameters>
    Angle_regularization_2(
      InputRange& input_range, 
      const NamedParameters np,
      const SegmentMap segment_map = SegmentMap()) :
    m_input_range(input_range),
    m_segment_map(segment_map) { 
      
      CGAL_precondition(input_range.size() > 1);
      const FT max_angle = parameters::choose_parameter(
        parameters::get_parameter(np, internal_np::max_angle), FT(25));
      CGAL_precondition(max_angle >= FT(0) && max_angle <= FT(90));

      m_max_angle = max_angle;
      if (m_max_angle < FT(0) || m_max_angle > FT(90)) {
        std::cout << "WARNING: The max angle bound has to be within [0, 90]! ";
        std::cout << " Setting to the default value: 25 degrees." << std::endl;
        m_max_angle = FT(25);
      }
      clear();
    }

    /// @}

    /// \name Access
    /// @{ 

    /*!
      \brief calculates the target value between 2 segments, which are
      direct neighbors to each other. The traget value is the angle.

      \param i
      index of the first segment

      \param j
      index of the second segment

      \pre `i >= 0 && i < input_range.size()`
      \pre `j >= 0 && j < input_range.size()`
    */
    FT target(
      const std::size_t i, 
      const std::size_t j) {

      CGAL_assertion(m_wraps.size() == m_input_range.size());
      CGAL_assertion(i >= 0 && i < m_wraps.size());
      CGAL_assertion(j >= 0 && j < m_wraps.size());
      
      const auto& wrapi = m_wraps[i];
      CGAL_assertion(wrapi.is_used);
      const auto& wrapj = m_wraps[j];
      CGAL_assertion(wrapj.is_used);

      const FT diff_ij = wrapi.orientation - wrapj.orientation;
      const int diff_90 = static_cast<int>(
        std::floor(CGAL::to_double(diff_ij / FT(90))));

      const FT to_lower = FT(90) * (static_cast<FT>(diff_90) + FT(0)) - diff_ij;
      const FT to_upper = FT(90) * (static_cast<FT>(diff_90) + FT(1)) - diff_ij;
      
      const FT abs_lower = CGAL::abs(to_lower);
      const FT abs_upper = CGAL::abs(to_upper);
      const FT angle = abs_lower < abs_upper ? to_lower : to_upper;
      const FT target_value = angle;
      
      const FT abs_angle = CGAL::abs(angle);
      if (abs_angle < bound(i) + bound(j)) {

        // A target value for the given graph edge i <-> j.
        m_targets[std::make_pair(i, j)] = target_value;
 
        std::size_t regularity = std::size_t(-1);
        if (abs_lower < abs_upper)
          regularity = ((90 * (diff_90 + 0)) % 180 == 0 ? 0 : 1);
        else
          regularity = ((90 * (diff_90 + 1)) % 180 == 0 ? 0 : 1);
        
        // A type of regularity: 0 -> parallel; 1 -> orthogonal.
        m_regularities[std::make_pair(i, j)] = regularity;
      } 
      return target_value;
    }

    /*!
      \brief returns `max_angle`.
    */
    const FT bound(const std::size_t) const {
      return m_max_angle;
    }

    /*!
      \brief applies new orientations computed by the QP solver 
      to the initial segments.

      \param solution
      a vector with updated segment orientations.

      \pre `solution.size() > 0`
    */
    void update(const std::vector<FT>& solution) {
      CGAL_assertion(solution.size() > 0);

      Targets_map targets;
      Relations_map relations;
      std::map<std::size_t, Segment_wrapper_2> segments;
      std::map<FT, Indices> parallel_groups;

      CGAL_assertion(m_targets.size() > 0);
      CGAL_assertion(m_targets.size() == m_regularities.size());

      for (const auto& group : m_groups) {
        if (group.size() < 2) continue;

        segments.clear(); targets.clear(); relations.clear();
        build_grouping_data(group, segments, targets, relations);

        parallel_groups.clear();
        if (segments.size() > 0) {
          const std::size_t n = m_input_range.size();

          m_grouping.make_groups(
            m_max_angle, n, segments, solution, 
            parallel_groups, targets, relations);
          rotate_parallel_segments(parallel_groups);
        }
      }
    }

    /// @}

    /// \name Miscellaneous
    /// @{ 

    /*!
      \brief returns indices of parallel segments organized into groups.

      \param groups
      an instance of OutputIterator
    */
    template<typename OutputIterator>
    OutputIterator parallel_groups(OutputIterator groups) {
      for (const auto& parallel_group : m_parallel_groups) {
        const auto& group = parallel_group.second;
        *(groups++) = group;
      }
      return groups;
    }

    /*!
      \brief inserts a group of segments from `input_range`.

      \tparam IndexRange 
      must be a model of `ConstRange` whose iterator type is `RandomAccessIterator`.
      The value type is `std::size_t`.

      \param index_range
      a const range of segment indices

      \pre `index_range.size() > 1`
    */
    template<typename IndexRange>
  	void add_group(
      const IndexRange& index_range) { 
      
      CGAL_precondition(index_range.size() > 1);
      if (index_range.size() < 2) return;
      
      Indices group;
      group.reserve(index_range.size());
      for (const auto seg_index : index_range)
        group.push_back(seg_index);
      
      m_groups.push_back(group);
      update_segment_data(group);
    }

    /*!
      \brief inserts all input segments from `input_range` as one unique group.

      For more details, 
      see `CGAL::Shape_regularization::Angle_regularization_2::add_group()`.
    */
    void create_unique_group() {
      
      CGAL_precondition(m_input_range.size() > 1);
      if (m_input_range.size() < 2) return;

      Indices group(m_input_range.size());
      std::iota(group.begin(), group.end(), 0);
      add_group(group);
    }

    /*!
      \brief returns the number of modifed segments.
    */
    std::size_t number_of_modified_segments() const {
      return m_num_modified_segments;
    }

    /// @}

    /// \name Internal data management
    /// @{ 

    /*!
      \brief clears all internal data structures.
    */
    void clear() {
      m_wraps.clear();
      m_wraps.resize(m_input_range.size());
      m_groups.clear();
      m_targets.clear();
      m_regularities.clear();
      m_num_modified_segments = 0;
    }

    /// @}

    // EXTRA METHODS TO TEST THE CLASS!
    /// \cond SKIP_IN_MANUAL
    const std::size_t number_of_groups() const { 
      return m_groups.size();
    }
    /// \endcond

  private:
    Input_range& m_input_range;
    const Segment_map m_segment_map;
    FT m_max_angle;

    std::vector<Indices> m_groups;
    std::vector<Segment_wrapper_2> m_wraps;
    std::size_t m_num_modified_segments;

    std::map<Size_pair, FT> m_targets;
    std::map<Size_pair, std::size_t> m_regularities;

    Grouping m_grouping;
    std::map<FT, Indices> m_parallel_groups;

    void update_segment_data(
      const Indices& group) {
      if (group.size() < 2) return;

      Segment_wrapper_2 wrap;
      for (const std::size_t seg_index : group) {
        CGAL_assertion(
          seg_index >= 0 && seg_index < m_wraps.size());
        auto& wrap = m_wraps[seg_index];
        const auto& segment = get(m_segment_map, 
          *(m_input_range.begin() + seg_index));
        wrap.set_qp(seg_index, segment);
      }
    }

    void build_grouping_data(
      const Indices& group,
      std::map<std::size_t, Segment_wrapper_2>& segments,
      Targets_map& targets,
      Relations_map& relations) {
      
      for (const std::size_t seg_index : group) {
        CGAL_assertion(
          seg_index >= 0 && seg_index < m_wraps.size());
        const auto& wrap = m_wraps[seg_index];
        CGAL_assertion(wrap.is_used);

        segments.emplace(seg_index, wrap);
        std::size_t index = 0;

        auto rit = m_regularities.begin();
        for (const auto& target : m_targets) {
          
          const std::size_t ti = target.first.first;
          const std::size_t tj = target.first.second;
          const FT target_value = target.second;

          auto& regularity = *rit;
          const std::size_t ri = regularity.first.first;
          const std::size_t rj = regularity.first.second;
          const int regularity_type = regularity.second;

          CGAL_assertion(ti == ri);
          CGAL_assertion(tj == rj);

          if (ti == seg_index && ri == seg_index) {
            targets[std::make_pair(ti, tj)] = 
              std::make_pair(target_value, index);
            relations[std::make_pair(ri, rj)] = 
              std::make_pair(regularity_type, index);
          }
          ++rit; ++index;
        }
      }
      CGAL_assertion(
        targets.size() == relations.size());
    }

    void rotate_parallel_segments(
      const std::map<FT, Indices>& parallel_groups) {
      
      for (const auto& parallel_group : parallel_groups) {
        const FT angle = parallel_group.first;
        const auto& group = parallel_group.second;

        if (m_parallel_groups.find(angle) == m_parallel_groups.end())
          m_parallel_groups[angle] = group;

        // Each group of parallel segments has a normal vector 
        // that we compute with alpha.
        const double angle_rad = 
          CGAL::to_double(angle * static_cast<FT>(CGAL_PI) / FT(180));
        const FT x = static_cast<FT>(std::cos(angle_rad));
        const FT y = static_cast<FT>(std::sin(angle_rad));

        Vector_2 direction = Vector_2(x, y);
        const Vector_2 orth = Vector_2(-direction.y(), direction.x());
        const FT a = orth.x();
        const FT b = orth.y();

        // Rotate segments with precision.
        // Compute equation of the supporting line of the rotated segment.
        for (const std::size_t seg_index : group) {
          CGAL_assertion(
            seg_index >= 0 && seg_index < m_wraps.size());
          const auto& wrap = m_wraps[seg_index];
          CGAL_assertion(wrap.is_used);

          const auto& barycenter = wrap.barycenter;
          const FT c = -a * barycenter.x() - b * barycenter.y();
          set_orientation(seg_index, a, b, c, direction);
        }
      }
    }

    void set_orientation(
      const std::size_t seg_index, 
      const FT a, const FT b, const FT c, 
      Vector_2 direction) {
      
      if (
        direction.y() < FT(0) || (
        direction.y() == FT(0) && direction.x() < FT(0))) 
        direction = -direction;

      FT x1, y1, x2, y2;
      CGAL_assertion(
        seg_index >= 0 && seg_index < m_wraps.size());
      const auto& wrap = m_wraps[seg_index];
      CGAL_assertion(wrap.is_used);
      
      const auto& barycenter = wrap.barycenter;
      const FT length = wrap.length;

      if (CGAL::abs(direction.x()) > CGAL::abs(direction.y())) { 
        x1 = barycenter.x() - length * direction.x() / FT(2);
        x2 = barycenter.x() + length * direction.x() / FT(2);
        y1 = (-c - a * x1) / b;
        y2 = (-c - a * x2) / b;
      }  else {
        y1 = barycenter.y() - length * direction.y() / FT(2);
        y2 = barycenter.y() + length * direction.y() / FT(2);
        x1 = (-c - b * y1) / a;
        x2 = (-c - b * y2) / a;
      }
      const Point_2 source = Point_2(x1, y1);
      const Point_2 target = Point_2(x2, y2);

      m_input_range[seg_index] = Segment_2(source, target);
      ++m_num_modified_segments;
    } 
  };

} // namespace Segments
} // namespace Shape_regularization
} // namespace CGAL

#endif // CGAL_SHAPE_REGULARIZATION_ANGLE_REGULARIZATION_2_H
