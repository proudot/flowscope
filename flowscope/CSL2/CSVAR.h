#ifndef CIMG_BROX_H
#define CIMG_BROX_H
#include "CImg.h"
#include <iostream>
#include "splinterp.h"
using namespace std;

#define SOR_PARAM 1.2

using namespace cimg_library;

class cimg_csvar3d
{
public:
    cimg_csvar3d();

    CImg<float> I1;
    CImg<float> I2;
    CImg<float> maskI1;

    CImg<float> I1x;
    CImg<float> I2x;
    CImg<float> I1y;
    CImg<float> I2y;
    CImg<float> I1z;
    CImg<float> I2z;


    CImg<float> I2w;


    CImg<float> I2wx;

    CImg<float> I2wy;

    CImg<float> I2wz;


    CImg<float>du, dv, dw;
    CImg<float>u, v, w;
    CImg<float>ux, uy, uz, vx, vy, vz, wx,wy, wz;
    CImg<float> psi1, psi2, psi3, psi4, psi5, psi6;
    CImg<float> warpField;

    CImgList<float> Hp,H1, H2, delIx, delIy, delIz, delIt, delIm1, delIm2;
    CImgList<float> Dx, Dy, Dz;

    //TVL1
    CImgList<float> p1, p2, p3;
    CImg<float> gradient, rho_const, uu, vv, ww, div_p1, div_p2, div_p3;
    //


    int width, height, depth;
    float alpha; //smoothness
    float gamma; //gradient weight
    float TOL;
    int inner_iter, outer_iter;
    int size;




    void get_firstOrderDerivatives(CImg<float> & I, CImg<float> & gx,
                                   CImg<float> &gy, CImg<float> &gz);

    void get_secondOrderDerivatives(CImg<float> & I, CImg<float> & gx,
                                    CImg<float> &gy, CImg<float> &gz);

    CImg<float> getHessSegmentation(CImg<float> &a);

    void estimate_singlescale(CImg<float> &source, CImg<float> &target,
                              CImg<float> &u, CImg<float> &v, CImg<float> &w,
                              double N, float ep,  //neightbourhood
                              float gamma, float omega,
                              float TOL, int max_iter);

    void estimate_singlescale_withDual(CImg<float> &source, CImg<float> &target,
                                       CImg<float> &u, CImg<float> &v, CImg<float> &w, float lambda, float theta,
                                       int N, float ep,  //neightbourhood
                                       float gamma, float omega,
                                       float TOL, int max_iter);

    CImgList<float> get_directionalDerivatives(const CImg<float> &I, int n);

    CImgList<float> get_Hp(const CImgList<float> &, float eps, int n);
    CImgList<float> get_H(const CImgList<float> &, float eps, int n);
    CImgList<float> get_Hpg(const CImgList<float> &f, const CImg<float> &geomap, int n);
    void divergence(const CImgList<float> &p, CImg<float> &div);
    void psi_smooth(CImg<float> &ux, CImg<float> &uy, CImg<float> &uz,
                                  CImg<float> &vx, CImg<float> &vy, CImg<float> &vz,
                                  CImg<float> &wx, CImg<float> &wy, CImg<float> &wz,
                                  CImg<float> &psiF);


    void psi_divergence(CImg<float> &psi,
                        CImg<float> &psi1, CImg<float> &psi2, CImg<float> &psi3,
                        CImg<float> &psi4, CImg<float> &psi5, CImg<float> &psi6);

CImg<float> splinterp_warper3(CImg<float> &source, CImg<float> &u, CImg<float> &v, CImg<float> &w);

};

#endif // CIMG_BROX_H
