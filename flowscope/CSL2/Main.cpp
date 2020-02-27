/***************************
 * 27th August, 2018
 * Author: Sandeep MANANDHAR
 * Team: Serpico
 * INRIA, Rennes
 * Census signature based 3D variational optical flow
 * for 3D fluorescence light microscopy
 *
 *
 * Smoothness term is inversly proportional to gradient of geomap
 * *************************/
#include<iostream>
#include <random>
#include <chrono>
#include <CSVAR.h>
#include <set>
#include <vector>
#include "CImg.h"
#include <string>
#include <splinterp.h>


#define DEF_ALPHA 2.5
#define DEF_GAMMA .9
#define DEF_NSCALES 7
#define DEF_SCALEFACTOR 0.95
#define DEF_TOL 0.0001
#define DEF_SOR_ITER 100

using namespace splinterp;


CImg<float> splinterp_warper(CImg<float> &source, CImg<float> &u,CImg<float> &v,CImg<float> &w){
    float *data = source.data();
    int width, height, depth;
    width  = source.width(), height = source.height(), depth = source.depth();

    CImg<float> X(width, height, depth);
    CImg<float> Y(width, height, depth);
    CImg<float> Z(width, height, depth);
    cimg_forXYZ(X, x, y, z){
        X(x,y,z)=x;
    }
    cimg_forXYZ(Y, x, y, z){
        Y(x,y,z)=y;
    }
    cimg_forXYZ(Z, x, y, z){
        Z(x,y,z)=z;
    }

    X = X+u;
    Y = Y+v;
    Z = Z+w;

    CImg<float> res(width, height, depth, 1, 0);

    splinterp::interp3_F<float>(data, width, height, depth, X.data(), Y.data(), Z.data(), width*height*depth,  res );
    return res;
}


using namespace std;
int dx27[27] = {0, -1, -1, -1, 0,  0, 1, 1,  1, 0, -1, -1, -1, 0,  0, 1, 1,  1, 0, -1, -1, -1, 0,  0, 1, 1,  1};
int dy27[27] = {0,  1,  0, -1, 1, -1, 1, 0, -1, 0,  1,  0, -1, 1, -1, 1, 0, -1, 0,  1,  0, -1, 1, -1, 1, 0, -1};
int dz27[27] = {0,  0,  0,  0, 0,  0, 0, 0,  0, 1,  1,  1,  1, 1,  1, 1, 1,  1,-1, -1, -1, -1,-1, -1,-1,-1, -1};


using namespace cimg_library;


CImg<int> flowconsistency(CImg<float> &fu, CImg<float> &fv, CImg<float> &fw,
                          CImg<float> &bu, CImg<float> &bv, CImg<float> &bw){
    CImg<float> usum = fu+bu;
    CImg<float> vsum = fv+bv;
    CImg<float> wsum = fw+bw;

    CImg<float> score = usum+vsum+wsum;
    CImg<int> score_copy(score);


    int i, j, k;
#pragma omp parallel shared(score) private(i,j,k)
    for(i = 0; i<score.width(); ++i){
        for(j=0; j < score.height(); ++j){
            for(k = 0; k < score.depth(); ++k){
                if(fabs(score(i,j,k))>0.5){
                    fu(i,j,k) = 0;
                    fv(i,j,k) = 0;
                    fw(i,j,k) = 0;
                    bu(i,j,k) = 0;
                    bv(i,j,k) = 0;
                    bw(i,j,k) = 0;
                    score_copy(i,j,k) = 0;

                }
                else{

                    score_copy(i,j,k) = 1;

                }
            }
        }
    }

    return score_copy;
}


std::vector<std::string> splitpath(
  const std::string& str
  , const std::set<char> delimiters)
{
  std::vector<std::string> result;

  char const* pch = str.c_str();
  char const* start = pch;
  for(; *pch; ++pch)
  {
    if (delimiters.find(*pch) != delimiters.end())
    {
      if (start != pch)
      {
        std::string str(start, pch);
        result.push_back(str);
      }
      else
      {
        result.push_back("");
      }
      start = pch + 1;
    }
  }
  result.push_back(start);

  return result;
}



#undef Bool
#define cimg_display 0
int main(int argc, char **argv){


    cimg_usage("Retrieve command line arguments");
    const char* sourcefilepath = cimg_option("-s"," ","Source volume file");
    const char* outputdir = cimg_option("-o"," ","Source volume file");
    const char* targetfilepath   = cimg_option("-t"," ","Target volume file");
    const char* ufilepath = cimg_option("-uu","","u scalar volume file");
    const char* vfilepath = cimg_option("-vv","","v scalar volume file");
    const char* wfilepath = cimg_option("-ww","","w scalar volume file");


    const double sx   = cimg_option("-sx",1.0,"Standard variation of the gaussian smoothing in x-axis");
    const double sy   = cimg_option("-sx",1.0,"Standard variation of the gaussian smoothing in y-axis");
    const double sz   = cimg_option("-sx",1.0,"Standard variation of the gaussian smoothing in z-axis");



    const  double N  = cimg_option("-n",6,"site cliques");
    const  float ep  = cimg_option("-p",80,"epsilon to penalize noise");
    const  float aniso_factor  = cimg_option("-a",4,"anisotropy factor");
    const  int f  = cimg_option("-f",2,"Scaling factor");
    const float gamma    = cimg_option("-g", 1.2, "Flow Smoothing factor");
    const float omega    = cimg_option("-w", 1.9, "Fixed point iteration weight");
    const float TOL    = cimg_option("-e", 0.01, "Error tolerance");
    const int max_iter    = cimg_option("-i", 1, "Max SOR iteration count");


    CImg<float> source(sourcefilepath);
    CImg<float> target(targetfilepath);

    CImg<float> u(source.width(), source.height(), source.depth(), 1, 0);
    CImg<float> v(source.width(), source.height(), source.depth(), 1, 0);
    CImg<float> w(source.width(), source.height(), source.depth(), 1, 0);

    if(!string(ufilepath).empty()){
        u.load(ufilepath);
        v.load(vfilepath);
        w.load(wfilepath);
    }

    std::set<char> delims{'/'};

    std::vector<std::string> sourcename = splitpath(sourcefilepath, delims);
    std::vector<std::string> targetname = splitpath(targetfilepath, delims);


    string saveu = string(outputdir)+"/"+sourcename.back()+"_"+targetname.back()+"soru.tif";
    string savev = string(outputdir)+"/"+sourcename.back()+"_"+targetname.back()+"sorv.tif";
    string savew = string(outputdir)+"/"+sourcename.back()+"_"+targetname.back()+"sorw.tif";

    string mag = string("./")+sourcename.back()+"_"+targetname.back()+"mag.tif";
    //==============================
    cout<<"Parameters:\n";
    cout<<"sigmas: "<<sx<<", "<<sy<<", "<<sz<<endl;
    cout<<"noise penalizer: "<<ep<<endl;
    cout<<"site cliques: "<<N<<endl;
    cout<<"anisotropic factor: "<<aniso_factor<<endl;
    cout<<"downscaling factor: "<<f<<endl;
    cout<<"gamma: "<<gamma<<", "<<"omega: "<<omega<<endl;
    cout<<"Tolerance: "<<TOL<<", "<<"Max iterations: "<<max_iter<<endl;
    cout<<"Inputfiles: \n";
    cout<<sourcefilepath<<endl;
    cout<<targetfilepath<<endl;



cout<<"=================================\n";
cout<<"Now Processing...\n";
cout<<"Anisotropy factor: "<<aniso_factor<<" was chosen. Now creating isotropic data...\n";
cout<<"Original Volume size: "<<source.width()<<"x"<<source.height()<<"x"<<source.depth()<<endl;
source.blur(sx, sy, sz, true);
target.blur(sx, sy, sz, true);
source.resize(source.width()/f, source.height()/f, (uint)source.depth()*aniso_factor/f);
target.resize(source.width(), source.height(), source.depth());

cout<<"New(isotropic) Volume size: "<<source.width()<<"x"<<source.height()<<"x"<<source.depth()<<endl;
cout<<"=================================\n";
bool _3D = TRUE;
if(_3D == TRUE){

    //////////////////////////3d///////////////////////
    source.normalize(0,255);
    target.normalize(0,255);
    CImg<float> uf(source.width(), source.height(), source.depth(), 1, 0),
            vf(source.width(), source.height(), source.depth(), 1,0),
            wf(source.width(), source.height(), source.depth(), 1,0);
    CImg<float>s , t;

    //using quadruple scales of resolution with downsampling factor of 2
    int fac[4] = {8,4,2,1};

    for(int i = 0; i < 4; ++i){

        //downsample source and target
        s = source.get_resize(source.width()/fac[i], source.height()/fac[i], source.depth()/fac[i], 1, 5 );
        t = target.get_resize(s.width(), s.height(), s.depth(),1 ,5);
        s.normalize(0,255);
        t.normalize(0,255);

        //upscale scalar fields of the flow field from previous coarse scale with factor of 2
        uf = 2*uf;
        vf = 2*vf;
        wf = 2*wf;
        //upsample scalar fields of the flow field from previous coarse scale with factor of 2
        uf.resize(s.width(), s.height(), s.depth(),1 ,5);
        vf.resize(s.width(), s.height(), s.depth(),1 ,5);
        wf.resize(s.width(), s.height(), s.depth(),1 ,5);

        //object of our method's class
        cimg_csvar3d of;

        //estimate 3D optical flow in a single scale
        of.estimate_singlescale(s, t, uf, vf, wf, N, ep, gamma, omega, TOL, max_iter);


        //save the scalar fields of the 3D flow field in three separate tif files.
        //a better alternative would be to save it in .mat file. See "MATIO" library for c++
        (uf).save_tiff(saveu.c_str());
        (vf).save_tiff(savev.c_str());
        (wf).save_tiff(savew.c_str());
    }
}
    return 1;
}




