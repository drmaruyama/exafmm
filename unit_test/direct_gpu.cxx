#include "dataset.h"
#include "evaluator.h"

int main() {
  double tic,toc;
  const int numBodies(10000);
  tic = get_time();
  Bodies bodies(numBodies);
  Dataset D;
  Evaluator E;
  E.initialize();
  toc = get_time();
  std::cout << "Allocate      : " << toc-tic << std::endl;

  tic = get_time();
  D.sphere(bodies);
  toc = get_time();
  std::cout << "Set bodies    : " << toc-tic << std::endl;

  tic = get_time();
  E.evalP2P(bodies,bodies);
  toc = get_time();
  std::cout << "Direct GPU    : " << toc-tic << std::endl;

  tic = get_time();
  real err(0),rel(0);
  for( B_iter BI=bodies.begin(); BI!=bodies.end(); ++BI ) {
    real pot = -BI->scal / std::sqrt(EPS2);
    BI->pot -= BI->scal / std::sqrt(EPS2);
    for( B_iter BJ=bodies.begin(); BJ!=bodies.end(); ++BJ ) {
      vect dist = BI->pos - BJ->pos;
      real r = std::sqrt(norm(dist) + EPS2);
      pot += BJ->scal / r;
    }
    err += (BI->pot - pot) * (BI->pot - pot);
    rel += pot * pot;
  }
  toc = get_time();
  std::cout << "Direct CPU    : " << toc-tic << std::endl;
  std::cout << "Error         : " << std::sqrt(err/rel) << std::endl;
  E.finalize();
}
