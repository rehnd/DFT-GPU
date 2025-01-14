#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <Eigen/Dense>

#include <cula.h>
#include <cula_lapack.h>

using namespace Eigen;
using std::string;
using std::vector;


class cell
{
private:
  // Parameters to be used:
  double _ecut;                // Energy cutoff
  double _latconst;            // Lattice constant
  double _a;                   // Possibly use this for latconst instead

  double _tau0, _tau1, _tau2;  // Two atoms are at +tau and -tau (units of a)
  Vector3d _a1, _a2, _a3;      // Direct lattice vectors
  Vector3d _b1, _b2, _b3;      // Reciprocal lattice vectors
  double _vol;                 // Volume (\Omega) of direct lattice

  MatrixXd _G;                 // G vectors (plane waves)
  std::vector<double> _G2;     // G^2 values
  MatrixXd _mill;              // Miller Indices of G vectors
  std::vector<double> _SG;     // Geometrical structure factor S(G)

  MatrixXd _igk;               // Index of G vector in list of k+G such that (k+G)^2 < Ecut

  int _npw;                    // Total Number of plane waves
  std::vector<int> _npw_perk;  // Number of plane waves for each k point
  int _npw_max;                // Maximum number of k+G plane waves

  std::vector<double> _H;      // Hamiltonian

  vector<double> _eigvecs;     // Eigenvectors of H
  vector<double> _eigvals;           // Eigenvalues of H

  int _nm0, _nm1, _nm2;        // Maximum values of Miller Indices
  int _nr0, _nr1, _nr2;        // Real-space Grid Dimensions
  int _nk;                     // Number of k-points in Brillouin Zone
  vector<double> _wk;          // Weights of k points
  MatrixXd _k;                 // k-points in the Brillouin Zone
  std::map<string, int> _indg; // Gives the index of a G vector from its Miller indices

  double _eps;                 // Small quantity for calculation of vsg
  double _e2;                  // 
  double _alpha;               // Mixing parameter for SCF loop
  double _threshold;           // Threshold for charge comparison
  int _nelec;                  // Number of electrons per unit cell
  int _nbands;                 // Number of occupied bands
  int _max_iter;               // Max # of SCF iterations

  vector<double> _rhoin;          // Input charge density
  vector<double> _rhoout;         // Output charge density
  vector<double> _v;              // Reciprocal space potential
  std::map<string, double> _rhor; // Real-space charge density
  std::map<string, double> _vr;   // Real-space exchange and coulomb potetial

  // Methods to be used:
  void _get_plane_waves();
  void _set_H_kin();
  void _set_H_pot();
  void _set_H_hartree();
  double _compute_eigs();
  void _compute_eigs_cula();
  void _update_hamiltonian(Vector3d& delta);
  // void _convert_H_to_culaDouble(const std::vector<double> &shiftedH, culaDouble* shiftedH_cula);
  void _get_SG();
  void _count_nk();
  void _fillH(int k);
  double _form_factor(double G2);
  double _diagH(int k);
  void _calcRho(int k);

public:
  cell(double ecut, double latconst, int nk);
  void _scf();
};
