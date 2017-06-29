#include"typedefs.h"
#include"hexextr.h"
#include<boost/numeric/ublas/matrix.hpp> 
#include<boost/numeric/ublas/vector.hpp> //is this needed?
#ifdef CGAL_EIGEN3_ENABLED
#include <CGAL/Eigen_matrix.h>
#include <CGAL/Eigen_vector.h>
#include <CGAL/Eigen_svd.h>
#include<cmath>
typedef CGAL::Eigen_svd Svd;
#endif
typedef Svd::FT     FT;
typedef Svd::Vector Eigen_vector;
typedef Svd::Matrix Eigen_matrix;
using matrix = boost::numeric::ublas::matrix;
using vect = boost::numeric::ublas::vector;

find_rotation_matrix(Vector_3 n){
  double alpha = acos((-1)*n[1]/sqrt(a*a+ b*b)); 
  double beta = acos(c/sqrt(a*a +b*b+c*c));
  double gamma = 0;
}


void sort_vertices(vector<Vertex_handle>& vertices){
  std::sort(vertices.begin(), vertices.end(), comp);
}

int find_number_of_boundary_vertices(LCC_3& lcc){
  int count = 0;
  for(LCC_3::One_dart_per_cell_range<0>::iterator it = lcc.one_dart_per_cell<0>().begin(), itend = lcc.one_dart_per_cell<0>().end(); it!= itend; it++){
    if(lcc.is_free(it, 3)) count++;
  }
  return count;
}

void closest_frame(){

}

void add_smoothing_terms(HexExtr &h, vector<vector<double>>& A, vector<double>& b){
  //didn't understand the representation for edges
}

void add_local_optim_constraints(HexExtr& h, vector<vector<double>>& a, vector<vector<double>& A, vector<double>& b){
  matrix<double> Ex(9,9), Ey(9,9), Ez(9,9);
  
  for(int i = 0; i<9; i++){
    for(int j = 0; j<9; j++){
      Ex.set(i,j,0);  Ey.set(i,j,0); Ez.set(i,j,0);
    }  
  }
  Ex(0,7)= (-1)*sqrt(2)); Ex(1,6) = (-1)*sqrt(3.5); Ex(2,5) = (-1)*sqrt(4.5); Ex(3, 4) = (-1)*sqrt(10);
  Ex(1,8) = (-1)*sqrt(2); Ex(2,7) = (-1)*sqrt(3.5); Ex(3,6) = (-1)*sqrt(4.5); 
  Ex(7,0) = sqrt(2); Ex(6,1) = sqrt(3.5); Ex(5,2) = sqrt(4.5); Ex(4,3) = sqrt(10)); 
  Ex(8,1) = sqrt(2); Ex(7,2) = sqrt(3.5); Ex(6,3) = sqrt(4.5);
  Ey(0,1) = sqrt(2); Ey(1,2) = sqrt(3.5); Ey(2,3) = sqrt(4.5); Ey(4,5) = (-1)*sqrt(10)); 
  Ey(5,6) = (-1)*sqrt(4.5); Ey(6,7) = (-1)*sqrt(3.5); Ey(7,8) = (-1)*sqrt(2); 
  Ey(1,0) =(-1)*sqrt(2); Ey(2,1) = (-1)*sqrt(3.5); Ey(3,2) = (-1)*sqrt(4.5); 
  Ey(5,4) = sqrt(10); Ey(6,5) = sqrt(4.5); Ey(7,6) = sqrt(3.5); Ey(8,7) = sqrt(2);
  Ez(0,8) = 4; Ez(1,7) = 3; Ez(2,6) = 2; Ez(3,5) = 1; Ez(5,3) = -1; Ez(6,2) = -2; Ez(7,1) = -3; Ez(8,0) = -4;
 
  for(int i = 0; i<nv; i++){
    vect ai(a[i].size());
    for(int j = 0; j<a[i].size();j++) ai[j] = (a[i])[j]; 
    vect<double> cx = boost::numeric::ublas::prod(Ex, ai);
    vect<double> cy = boost::numeric::ublas::prod(Ey, ai);
    vect<double> cz = boost::numeric::ublas::prod(Ez, ai);
    int lambda = 100; //quadratic penalty multiplier
    for(int d = 0; d < 9; d++){
      vector<double> row(9*nv+2*nl+3*nv);
      row.fill(row.begin(), row.end(), 0);
      row[9*i + d] = lambda;
      row[9*nv + 2*nl + 3*i] = (-1)*lambda*cx(d);
      row[9*nv + 2*nl + 3*i + 1] = (-1)*lambda*cy(d);
      row[9*nv + 2*nl + 3*i + 2] = (-1)*lambda*cz(d);
      A.push_back(row);
      b.push_back(lambda*ai(d));      
    }
  }
}

void add_normal_constraints(HexExtr& h, vector<vector<double>>& A, vector<double>& b){
  for(i = 0; i < nl; i++){
    Vector_3 n = CGAL::compute_normal_of_cell_0(h.input_tet_mesh, (vertices[i].incident_dart));
   // find_euler_angles(n);//TODO
    matrix<double> R = find_rotation_matrix(n); //TODO
    vect<double> temp(9);
    temp(0) = 1; temp(1) = 0; temp(2) = 0; temp(3) = 0; temp(4) = 0; temp(5) = 0; temp(6) = 0; temp(7) = 0; temp(8) = 0;
    vect<double> h0 = boost::numeric::ublas::prod(R, temp);
    temp(0) = 0; temp(4) = 1;
    vect<double> h4 = boost::numeric::ublas::prod(R, temp);
    temp(4) = 0; temp(8) = 1;
    vect<double> h8 = boost::numeric::ublas::prod(R, temp);
    int lambda = 100; //quadratic penalty multiplier
    for(int d = 0; d < 9; d++){
      vector<double> row(9*nv+2*nl+3*nv);
      row.fill(row.begin(), row.end(), 0);
      row[9*i + d] = lambda;
      row[9*nv + 2*i] = lambda*h0(d);
      row[9*nv + 2*i + 1] = lambda*h8(d);
      A.push_back(row);
      b.push_back(lambda*sqrt((double)7/12)*h4(d));
    }
  }
}

void optimise_frame_field(HexExtr& h, int n){ // n is the number of smoothing iterations
  int nl = find_number_of_boundary_vertices(h.input_tet_mesh);
  int nv = (h.vertices).length(); 
  sort_vertices(); //TODO - DONE
  vector<vector<double>> a;
  for(int i = 0; i < n; i++){
    Eigen_matrix<double> A(0, (9*nv + 2*nl + 3*nv));
    vector<vector<double>> A_tobeconverted;
    Eigen_vector<double> b;
    vector<double> b_tobeconverted;
    add_smoothing_terms(h, A, b); //TODO
    add_normal_constraints(h, A_tobeconverted, b_tobeconverted);
    if(i>0){
      add_local_optim_constraints(h, a, A_tobeconverted, b_tobeconverted);
    }
    if(i == 0) vector<vector<double>> a;
    Eigen_vector X = b;
    Svd::solve(A, X); //solution gets stored in X
    a.clear();
    for(int j = 0;j<nv;j++){
      vector<double> temp;
      for(int k = 9*j; k<(9*j+9);k++){
          temp.push_back(X.at(k));
        }
      a.push_back(temp);
            
      //closest frame ? //TODO
    }
  }
}