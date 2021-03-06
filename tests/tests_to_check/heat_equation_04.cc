#include "pidomus.h"
#include "interfaces/heat_equation.h"
#include "tests.h"

typedef TrilinosWrappers::MPI::BlockVector VEC;


template<int fdim, int fspacedim, int fn_components>
void test(piDoMUS<fdim,fspacedim,fn_components> &pb)
{
  pb.make_grid_fe();
  pb.setup_dofs();


  VEC &d = pb.differential_components();
  VEC &sol = pb.solution;
  VEC &sol_dot = pb.solution_dot;
  VEC residual(sol);

  deallog << "differential components" <<std::endl;
  d.print(deallog.get_file_stream());

  deallog << "solution" <<std::endl;
  sol.print(deallog.get_file_stream());

  deallog << "solution_dot" <<std::endl;
  sol_dot.print(deallog.get_file_stream());

  pb.residual(0.0,sol,sol_dot,residual);
  deallog << "residual  " <<std::endl;
  residual.print(deallog.get_file_stream());
}


using namespace dealii;
int main (int argc, char *argv[])
{

  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv,
                                                      numbers::invalid_unsigned_int);

  initlog();

  const int dim = 2;
  HeatEquation<dim> energy;
  piDoMUS<dim,dim,1> n_problem (energy);
  ParameterAcceptor::initialize(SOURCE_DIR "/parameters/heat_equation_04.prm", "used_parameters.prm");

  deallog << "##########################################"<<std::endl;
  deallog << " BC = 0, sol = x^2, sol_dot = 0, F = -2" <<std::endl;
  deallog << "##########################################"<<std::endl;

  test(n_problem);

  return 0;
}
