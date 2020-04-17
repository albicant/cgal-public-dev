#include <CGAL/property_map.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Shape_regularization.h>

using Kernel = CGAL::Simple_cartesian<double>;
using FT = typename Kernel::FT;
using Point_2 = typename Kernel::Point_2;
using Segment_2 = typename Kernel::Segment_2;
using Input_range = std::vector<Segment_2>;

struct User_neighbor_query_2 {
  void operator()(
    const std::size_t query_index,
    std::vector<std::size_t>& neighbors) {
    neighbors.clear();
    // Add your code here!
  }
};

struct User_regularization_2 {
  FT bound(
    const std::size_t query_index) const { 
    // Add your code here!
    return FT(0);
  }
  FT target(
    const std::size_t query_index_i, 
    const std::size_t query_index_j) {
    // Add your code here!
    return FT(0);
  }
  void update(
    const std::vector<FT>& solution) {
    // Add your code here!
  }
};

template<typename NT>
class User_quadratic_program : 
  public CGAL::Quadratic_program<int> {
  using Quadratic_program = CGAL::Quadratic_program<int>;
  
  public:
    User_quadratic_program() : 
      Quadratic_program(
        CGAL::SMALLER, true, -NT(100000), true, +NT(100000))
    { }
};

bool solve_quadratic_program(
  User_quadratic_program<FT>& qp,
  std::vector<FT>& solution) {
  // Add your code here!
  solution.clear();
  return false; // return status of the computation
}

// Choose a type of a solver.
#define OSQP_SOLVER
// #define CGAL_SOLVER
// #define USER_SOLVER

#if defined(OSQP_SOLVER)
using Quadratic_program = 
  CGAL::Shape_regularization::OSQP_quadratic_program<FT>; // OSQP sparse solver
#endif
#if defined(CGAL_SOLVER)
using Quadratic_program = 
  CGAL::Shape_regularization::CGAL_quadratic_program<FT>; // CGAL dense solver
#endif
#if defined(USER_SOLVER)
using Quadratic_program = 
  CGAL::Shape_regularization::USER_quadratic_program<FT>; // USER custom solver
#endif

using NQ = User_neighbor_query_2;
using RT = User_regularization_2;
using QP = Quadratic_program;
using Regularizer = 
  CGAL::Shape_regularization::QP_regularization<Kernel, Input_range, NQ, RT, QP>;

int main(int argc, char *argv[]) {
  
  NQ neighbor_query;
  RT angle_regularization;
  QP quadratic_program;

  Input_range input_range;
  Regularizer regularizer(
    input_range, neighbor_query, angle_regularization, quadratic_program);
  regularizer.regularize();
}
