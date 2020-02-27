#include "CSVAR.h"
#define EPSILON  0.01
void cimg_csvar3d:: get_firstOrderDerivatives(CImg<float> & I, CImg<float> & gx,
                               CImg<float> &gy, CImg<float> &gz){

//    if(scheme == 2){
//            const int h[3] = {1,2,1}; //smooth
//            const int hp[3] = {1, 0, -1}; //deriv

//            CImg<float> kx(3,3,3), ky(3,3,3), kz(3,3,3);
//            for(int i = 0; i < 3; ++i)
//                for(int j = 0; j < 3; ++j)
//                    for(int k = 0; k < 3; ++k){
//                        kx(i,j,k) = hp[i]*h[j]*h[k];
//                        ky(i,j,k) = h[i]*hp[j]*h[k];
//                        kz(i,j,k) = h[i]*h[j]*hp[k];
//                    }


//            gx = I.get_convolve(kx, true, false);
//            gy = I.get_convolve(ky, true, false);
//            gz = I.get_convolve(kz, true, false);
//            return;
//    }


   CImgList<float> l = I.get_gradient("xyz", 0);
    gx = l[0];
    gy = l[1];
    gz = l[2];


}

CImg<float> cimg_csvar3d::getHessSegmentation(CImg<float> &a)
{
    CImg<float> tensor = a.get_structure_tensors();
    tensor.blur(1);
    CImg<float> firstEigenvaluemap(a.width(), a.height(), a.depth());
    CImg<float> val, vec;
    cout<<"computing eigen:\n";
    cimg_forXYZ(tensor, x, y, z){
        tensor.get_tensor_at(x,y,z).symmetric_eigen(val, vec);
        firstEigenvaluemap(x,y,z) = val(0);

    }
    firstEigenvaluemap.normalize(0,1);
    return firstEigenvaluemap;
}

void cimg_csvar3d:: get_secondOrderDerivatives(CImg<float> & I, CImg<float> & gx,
                               CImg<float> &gy, CImg<float> &gz){
    get_firstOrderDerivatives(I,gx, gy, gz);

}

CImgList<float> cimg_csvar3d::get_directionalDerivatives(const CImg<float> &I, int n)
{


CImgList<float> delIm(n, I.width(), I.height(), I.depth(), I.spectrum());

    if(n == 26){

    delIm[0] = (-I + I.get_shift(-1, -1, -1,  0, 0));
    delIm[1] = (-I + I.get_shift(0,  -1, -1,  0, 0));
    delIm[2] = (-I + I.get_shift(1,  -1, -1,  0, 0));

    delIm[3] = (-I + I.get_shift(-1,  0, -1, 0,  0));
    delIm[4] = (-I + I.get_shift(0,   0, -1, 0,  0));
    delIm[5] = (-I + I.get_shift(1,   0, -1, 0,  0));

    delIm[6] = (-I + I.get_shift(-1, 1, -1, 0,  0));
    delIm[7] = (-I + I.get_shift(0,  1, -1, 0,  0));
    delIm[8] = (-I + I.get_shift(1,  1, -1, 0,  0));
    //============================================;
                                                  ;
    delIm[9] = (-I + I.get_shift(-1, -1, 0,  0, 0));
    delIm[10]= (-I + I.get_shift(0,  -1, 0,  0, 0));
    delIm[11]= (-I + I.get_shift(1,  -1, 0,  0, 0));
                                                  ;
    delIm[12]= (-I + I.get_shift(-1, 0, 0, 0,  0));
                                                 ;
    delIm[13]= (-I + I.get_shift(1,  0, 0, 0,  0));
                                                 ;
    delIm[14]= (-I + I.get_shift(-1, 1, 0, 0,  0));
    delIm[15]= (-I + I.get_shift(0,  1, 0, 0,  0));
    delIm[16]= (-I + I.get_shift(1, 1,  0, 0,  0));
                                                  ;
    //============================================;
                                                  ;
    delIm[17]= (-I + I.get_shift(-1, -1, 1,  0, 0));
    delIm[18]= (-I + I.get_shift(0,  -1, 1,  0, 0));
    delIm[19]= (-I + I.get_shift(1,  -1, 1,  0, 0));
                                                  ;
    delIm[20]= (-I + I.get_shift(-1, 0, 1, 0,  0));
    delIm[21]= (-I + I.get_shift(0,  0, 1, 0,  0));
    delIm[22]= (-I + I.get_shift(1,  0, 1, 0,  0));
                                                  ;
    delIm[23]= (-I + I.get_shift(-1, 1,  1, 0,  0));
    delIm[24]= (-I + I.get_shift(0,  1,  1, 0,  0));
    delIm[25]= (-I + I.get_shift(1,  1,  1, 0,  0));

    //===================================================

    }

    else if(n==6){

        delIm[0] = -I + I.get_shift(1, 0, 0, 0, 0);
        delIm[1] = -I + I.get_shift(-1, 0, 0, 0, 0);

        delIm[2] = -I + I.get_shift(0, -1, 0, 0, 0);
        delIm[3] = -I + I.get_shift(0, 1, 0, 0, 0);

        delIm[4] = -I + I.get_shift(0, 0, -1, 0, 0);
        delIm[5] = -I + I.get_shift(0, 0, 1, 0, 0);
    }

    return delIm;
}

CImgList<float> cimg_csvar3d::get_Hp(const CImgList<float> &f, float eps, int n)
{
    CImgList<float> fp(f);
#pragma omp parallel for
    for(int i = 0; i < n; ++i)
    cimg_forXYZ(f[i], x, y, z){
        fp[i](x,y,z) = -eps*eps/(2.*pow(eps*eps + pow(f[i](x,y,z), 2), 3./2.));
    }
    return fp;
}


CImgList<float> cimg_csvar3d:: get_Hpg(const CImgList<float> &f, const CImg<float> &geomap, int n)
{
    CImgList<float> fp(f);
#pragma omp parallel for
    for(int i = 0; i < n; ++i)
    cimg_forXYZ(f[i], x, y, z){
        float eps = 10;
        if(geomap(x,y,z) >15)
             eps = 80;
        fp[i](x,y,z) = -eps*eps/(2.*pow(eps*eps + pow(f[i](x,y,z), 2), 3./2.));
    }

    return fp;
}

CImgList<float> cimg_csvar3d::get_H(const CImgList<float> &f, float eps, int n)
{
    CImgList<float> fp(f);
#pragma omp parallel for
    for(int i = 0; i < n; ++i)
    cimg_forXYZ(f[i], x, y, z){
        fp[i](x,y,z) = 0.5*(1 - f[i](x,y,z)/sqrt(f[i](x,y,z)*f[i](x,y,z) + eps*eps));
    }
        return fp;
}

void cimg_csvar3d::divergence(const CImgList<float> &p, CImg<float> &div)
{
#pragma omp parallel for schedule(dynamic)
    for(int z = 1; z < p[0].depth()-1; ++z)
        for(int y = 1; y < p[0].height()-1; ++y)
            for(int x = 1; x < p[0].width()-1; ++x){
                float ux = p[0](x,y,z) - p[0](x-1, y,z);
                float uy = p[1](x,y,z) - p[1](x, y-1,z);
                float uz = p[2](x,y,z) - p[2](x, y,z-1);
                div(x,y,z) = ux+uy+uz;
            }

    for(int z = 1; z < p[0].depth()-1; ++z)
        for(int x = 1; x < p[0].width()-1; ++x){
            //first row
            int y = 0;
            float ux = p[0](x,y,z) - p[0](x-1, y,z);
            float uy = p[1](x,y,z) - 0 ;
            float uz = p[2](x,y,z) - p[2](x, y,z-1);
            div(x,y,z) = ux+uy+uz;

            y = p[0].height()-1;
            ux = p[0](x,y,z) - p[0](x-1, y,z);
            uy = /*p[1](x,y,z)*/ - p[1](x, y-1,z);
            uz = p[2](x,y,z) - p[2](x, y,z-1);
            div(x,y,z) = ux+uy+uz;
        }


    //for the first and the last columns of each slice
    for(int z = 1; z < p[0].depth()-1; ++z)
        for(int y = 1; y < p[0].height()-1; ++y){
            //first row
            int x = 0;
            float ux = p[0](x,y,z) - 0;
            float uy = p[1](x,y,z) - p[1](x, y-1,z);
            float uz = p[2](x,y,z) - p[2](x, y,z-1);
            div(x,y,z) = ux+uy+uz;

            x = p[0].width()-1;
            ux = /*p[0](x,y,z)*/ - p[0](x-1, y,z)- 0;
            uy = p[1](x,y,z) - p[1](x, y-1,z);
            uz = p[2](x,y,z) - p[2](x, y,z-1);
            div(x,y,z) = ux+uy+uz;
        }

    //for the first and the last slice
    for(int x = 1; x < p[0].width()-1; ++x)
        for(int y = 1; y < p[0].height()-1; ++y){
            //first row
            int z = 0;
            float ux = p[0](x,y,z) - p[0](x-1, y,z);
            float uy = p[1](x,y,z) - p[1](x, y-1,z);
            float uz = p[2](x,y,z) - 0 ;
            div(x,y,z) = ux+uy+uz;

            z = p[0].depth()-1;
            ux = p[0](x,y,z) - p[0](x-1, y,z);
            uy = p[1](x,y,z) - p[1](x, y-1,z);
            uz = /*p[2](x,y,z)*/ - p[2](x, y,z-1);
            div(x,y,z) = ux+uy+uz;
        }

    //for the corners
    int x = 0, y = 0, z = 0;
    float ux = p[0](x,y,z) - 0;
    float uy = p[1](x,y,z) - 0;
    float uz = p[2](x,y,z) - 0;
    div(x,y,z) = ux+uy+uz;

    x = p[0].width()-1; y = 0; z = 0;
    ux = /*p[0](x,y,z)*/ - p[0](x-1, y,z)- 0;
    uy = p[1](x,y,z) - 0;
    uz = p[2](x,y,z) - 0;
    div(x,y,z) = ux+uy+uz;

    x = p[0].width()-1; y = p[0].height()-1; z = 0;
    ux = /*p[0](x,y,z)*/ - p[0](x-1, y,z)- 0;
    uy = /*p[1](x,y,z)*/ - p[1](x, y-1,z)- 0;
    uz = p[2](x,y,z) - 0;
    div(x,y,z) = ux+uy+uz;

    x = 0; y = p[0].height()-1; z = 0;
    ux = p[0](x,y,z) - 0;
    uy = /*p[1](x,y,z)*/ - p[1](x, y-1,z)- 0;
    uz = p[2](x,y,z) - 0;
    div(x,y,z) = ux+uy+uz;

    //bottom slice corners
    x = 0; y = 0; z = p[0].depth()-1;
    ux = p[0](x,y,z) - 0;
    uy = p[1](x,y,z) - 0;
    uz = /*p[2](x,y,z)*/ -  p[2](x, y,z-1);
    div(x,y,z) = ux+uy+uz;

    x = p[0].width()-1; y = 0; z = p[0].depth()-1;;
    ux = /*p[0](x,y,z)*/ - p[0](x-1, y,z)- 0;
    uy = p[1](x,y,z) - 0;
    uz = /*p[2](x,y,z)*/ - p[2](x, y,z-1);
    div(x,y,z) = ux+uy+uz;

    x = p[0].width()-1; y = p[0].height()-1; z = p[0].depth()-1;;
    ux = /*p[0](x,y,z)*/ - p[0](x-1, y,z)- 0;
    uy = /*p[1](x,y,z) */- p[1](x, y-1,z)- 0;
    uz = /*p[2](x,y,z) */- p[2](x, y,z-1);
    div(x,y,z) = ux+uy+uz;

    x = 0; y = p[0].height()-1; z = p[0].depth()-1;;
    ux = p[0](x,y,z) - 0;
    uy = /*p[1](x,y,z)*/ - p[1](x, y-1,z)- 0;
    uz = /*p[2](x,y,z) */- p[2](x, y,z-1);
    div(x,y,z) = ux+uy+uz;
}

//Robust terms

void cimg_csvar3d::psi_smooth(CImg<float> &ux, CImg<float> &uy, CImg<float> &uz,
                              CImg<float> &vx, CImg<float> &vy, CImg<float> &vz,
                              CImg<float> &wx, CImg<float> &wy, CImg<float> &wz,
                              CImg<float> &psiS)
{

    for(int z = 0; z<ux.depth(); ++z)
        for(int y= 0; y<ux.height(); ++y)
            for(int x = 0; x < ux.width(); ++x){
                float deltau = ux(x,y,z)*ux(x,y,z) + uy(x,y,z)*uy(x,y,z) + uz(x,y,z)*uz(x,y,z);
                float deltav = vx(x,y,z)*vx(x,y,z) + vy(x,y,z)*vy(x,y,z) + vz(x,y,z)*vz(x,y,z);
                float deltaw = wx(x,y,z)*wx(x,y,z) + wy(x,y,z)*wy(x,y,z) + wz(x,y,z)*wz(x,y,z);

                psiS(x,y,z) = 0.5/sqrt(deltau+deltav+deltaw + EPSILON*EPSILON);

            }
}


void cimg_csvar3d::psi_divergence(CImg<float> &psi,
                               CImg<float> &psi1, CImg<float> &psi2, CImg<float> &psi3,
                               CImg<float> &psi4, CImg<float> &psi5, CImg<float> &psi6)
{
    for(int z = 1; z<psi.depth()-1; ++z){
        for(int y= 1; y<psi.height()-1; ++y){
            for(int x = 1; x < psi.width()-1; ++x){
                psi1(x,y,z) = 0.5*(psi(x+1,y,z) + psi(x,y,z));
                psi2(x,y,z) = 0.5*(psi(x-1,y,z) + psi(x,y,z));


                psi3(x,y,z) = 0.5*(psi(x,y+1,z) + psi(x,y,z));
                psi4(x,y,z) = 0.5*(psi(x,y-1,z) + psi(x,y,z));
                psi5(x,y,z) = 0.5*(psi(x,y,z+1) + psi(x,y,z));
                psi6(x,y,z) = 0.5*(psi(x,y,z-1) + psi(x,y,z));
            }
        }
        //rows at extremes y= 0 and y = height-1
        for(int x = 1; x < psi.width()-1; ++x){
            psi1(x, 0, z) = 0.5*(psi(x+1, 0, z) + psi(x,0,z));
            psi2(x, 0, z) = 0.5*(psi(x-1, 0, z) + psi(x,0,z));
            psi3(x, 0, z) = 0.5*(psi(x, 1, z) + psi(x,0,z));
            psi4(x, 0, z) = 0;
            psi5(x,0,z) = 0.5*(psi(x,0,z+1) + psi(x,0,z));
            psi6(x,0,z) = 0.5*(psi(x,0,z-1) + psi(x,0,z));

            int yy = psi.height()-1;
            psi1(x, yy, z) = 0.5*(psi(x+1, yy, z) + psi(x,yy,z));
            psi2(x, yy, z) = 0.5*(psi(x-1, yy, z) + psi(x,yy,z));
            psi3(x, yy, z) = 0;
            psi4(x, yy, z) = 0.5*(psi(x, yy-1, z) + psi(x,yy,z));
            psi5(x,yy,z) = 0.5*(psi(x,yy,z+1) + psi(x,yy,z));
            psi6(x,yy,z) = 0.5*(psi(x,yy,z-1) + psi(x,yy,z));
        }

        //columns at extremes x = 0 and x = width-1
        for(int y = 1; y < psi.height()-1; ++y){
            psi1(0, y, z) = 0.5*(psi(1, y, z) + psi(0,y,z));
            psi2(0, y,z) =  0;
            psi3(0, y, z) = 0.5*(psi(0, y+1, z) + psi(0,y,z));
            psi4(0, y, z) = 0.5*(psi(0, y-1, z) + psi(0,y,z));
            psi5(0,y,z) = 0.5*(psi(0,y,z+1) + psi(0,y,z));
            psi6(0,y,z) = 0.5*(psi(0,y,z-1) + psi(0,y,z));

            int xx = psi.width()-1;
            psi1(xx, y, z) = 0;
            psi2(xx, y, z) = 0.5*(psi(xx-1, y, z) + psi(xx,y,z));
            psi3(xx, y, z) = 0.5*(psi(xx, y+1, z) + psi(xx,y,z));
            psi4(xx, y, z) = 0.5*(psi(xx, y-1, z) + psi(xx,y,z));
            psi5(xx, y, z) = 0.5*(psi(xx,y,z+1) + psi(xx, y, z));
            psi6(xx, y, z) = 0.5*(psi(xx,y,z-1) + psi(xx, y, z));
        }

        //corners
        //top left
        psi1(0,0,z) = 0.5*(psi(1,0,z) + psi(0,0,z));
        psi2(0,0,z) = 0;
        psi3(0,0,z) = 0.5*(psi(0,1,z) + psi(0,0,z));
        psi4(0,0,z) = 0;
        psi5(0, 0, z) = 0.5*(psi(0,0,z+1) + psi(0, 0, z));
        psi6(0, 0, z) = 0.5*(psi(0,0,z-1) + psi(0, 0, z));
        //bottom right
        int xx = psi.width()-1; int yy = psi.height()-1;
        psi1(xx,yy,z) = 0;
        psi2(xx,yy,z) = 0.5*(psi(xx-1,yy,z) + psi(xx,yy,z));
        psi3(xx,yy,z) = 0;
        psi4(xx,yy,z) = 0.5*(psi(xx,yy-1,z) + psi(xx,yy,z));
        psi5(xx,yy,z) = 0.5*(psi(xx,yy,z+1) + psi(xx,yy, z));
        psi6(xx,yy,z) = 0.5*(psi(xx,yy,z-1) + psi(xx,yy, z));
        //top right
        psi1(xx,0,z) = 0;
        psi2(xx,0,z) = 0.5*(psi(xx-1,0,z) + psi(xx,0,z));
        psi3(xx,0,z) = 0.5*(psi(xx,1,z)   + psi(xx,0,z));
        psi4(xx,0,z) = 0;
        psi5(xx,0,z) = 0.5*(psi(xx,0,z+1) + psi(xx,0, z));
        psi6(xx,0,z) = 0.5*(psi(xx,0,z-1) + psi(xx,0, z));
        //bottom left
        psi1(0,yy,z) = 0.5*(psi(1,yy,z) + psi(0,yy,z));
        psi2(0,yy,z) = 0;
        psi3(0,yy,z) = 0;
        psi4(0,yy,z) = 0.5*(psi(0,yy-1,z) + psi(0,yy,z));
        psi5(0,yy,z) = 0.5*(psi(0,yy,z+1) + psi(0,yy, z));
        psi6(0,yy,z) = 0.5*(psi(0,yy,z-1) + psi(0,yy, z));
    }

    //Now the extreme slices z = 0 -> z=depth
    int zz = 0;
    for(int y= 1; y<psi.height()-1; ++y){
        for(int x = 1; x < psi.width()-1; ++x){
            psi1(x,y,zz) = 0.5*(psi(x+1, y, zz)+ psi(x,y,zz));
            psi2(x,y,zz) = 0.5*(psi(x-1, y, zz)+ psi(x,y,zz));
            psi3(x,y,zz) = 0.5*(psi(x, y+1, zz)+ psi(x,y,zz));
            psi4(x,y,zz) = 0.5*(psi(x+1, y-1, zz)+ psi(x,y,zz));
            psi5(x,y,zz) = 0.5*(psi(x, y, zz+1)+ psi(x,y,zz));
            psi6(x,y,zz) = 0;
        }
    }
    zz = psi.depth()-1;
    for(int y= 1; y<psi.height()-1; ++y){
        for(int x = 1; x < psi.width()-1; ++x){
            psi1(x,y,zz) = 0.5*(psi(x+1, y, zz)+ psi(x,y,zz));
            psi2(x,y,zz) = 0.5*(psi(x-1, y, zz)+ psi(x,y,zz));
            psi3(x,y,zz) = 0.5*(psi(x, y+1, zz)+ psi(x,y,zz));
            psi4(x,y,zz) = 0.5*(psi(x+1, y-1, zz)+ psi(x,y,zz));
            psi5(x,y,zz) = 0;
            psi6(x,y,zz) = 0.5*(psi(x, y, zz-1)+ psi(x,y,zz));
        }
    }

    //corners
    //top top left

    psi1(0,0,0) = 0.5*(psi(1,0,0) + psi(0,0,0));
    psi2(0,0,0) = 0;
    psi3(0,0,0) = 0.5*(psi(0,1,0) + psi(0,0,0));
    psi4(0,0,0) = 0;
    psi5(0,0,0) = 0.5*(psi(0,0,1) + psi(0, 0,1));
    psi6(0,0,0) = 0;



    //top top right
    int xx = psi.width()-1;  int yy = 0; zz = 0;
    psi1(xx,yy,zz) = 0;
    psi2(xx,yy,zz) = 0.5*(psi(xx-1,yy,zz)+psi(xx,yy,zz));
    psi3(xx,yy,zz) = 0.5*(psi(xx,yy+1,zz)+psi(xx,yy,zz));
    psi4(xx,yy,zz) = 0;
    psi5(xx,yy,zz) = 0.5*(psi(xx,yy,zz+1)+psi(xx,yy,zz));
    psi6(xx,yy,zz) = 0;

    //top bottom left
    xx = 0;  yy = psi.height()-1; zz = 0;
    psi1(xx,yy,zz) = 0.5*(psi(xx+1,yy,zz)+psi(xx,yy,zz));
    psi2(xx,yy,zz) = 0;
    psi3(xx,yy,zz) = 0;
    psi4(xx,yy,zz) = 0.5*(psi(xx,yy-1,zz)+psi(xx,yy,zz));
    psi5(xx,yy,zz) = 0.5*(psi(xx,yy,zz+1)+psi(xx,yy,zz));
    psi6(xx,yy,zz) = 0;

    //top bottom right
    xx = psi.width()-1;   yy = psi.height()-1; zz = 0;
    psi1(xx,yy,zz) = 0;
    psi2(xx,yy,zz) = 0.5*(psi(xx-1,yy,zz)+psi(xx,yy,zz));
    psi3(xx,yy,zz) = 0;
    psi4(xx,yy,zz) = 0.5*(psi(xx,yy-1,zz)+psi(xx,yy,zz));
    psi5(xx,yy,zz) = 0.5*(psi(xx,yy,zz+1)+psi(xx,yy,zz));
    psi6(xx,yy,zz) = 0;


    //last slice here-->
    //bottom top left
    zz = psi.depth()-1;
    psi1(0,0,zz) = 0.5*(psi(1,0,zz) + psi(0,0,zz));
    psi2(0,0,zz) = 0;
    psi3(0,0,zz) = 0.5*(psi(0,1,zz) + psi(0,0,zz));
    psi4(0,0,zz) = 0;
    psi5(0,0,zz) = 0;
    psi6(0,0,zz) = 0.5*(psi(0,0,zz-1) + psi(0, 0,zz));

    //bottom top right
    xx = psi.width()-1;   yy = 0;
    psi1(xx,yy,zz) = 0;
    psi2(xx,yy,zz) = 0.5*(psi(xx-1,yy,zz)+psi(xx,yy,zz));
    psi3(xx,yy,zz) = 0.5*(psi(xx,yy+1,zz)+psi(xx,yy,zz));
    psi4(xx,yy,zz) = 0;
    psi5(xx,yy,zz) = 0;
    psi6(xx,yy,zz) = 0.5*(psi(xx,yy,zz-1) + psi(xx, yy,zz));


    //top bottom left
    xx = 0;  yy = psi.height()-1;
    psi1(xx,yy,zz) = 0.5*(psi(xx+1,yy,zz)+psi(xx,yy,zz));
    psi2(xx,yy,zz) = 0;
    psi3(xx,yy,zz) = 0;
    psi4(xx,yy,zz) = 0.5*(psi(xx,yy-1,zz)+psi(xx,yy,zz));
    psi5(xx,yy,zz) = 0;
    psi6(xx,yy,zz) = 0.5*(psi(xx,yy,zz-1) + psi(xx, yy,zz));

    //bottom bottom right
    xx = psi.width()-1; yy = psi.height()-1;
    psi1(xx,yy,zz) = 0;
    psi2(xx,yy,zz) = 0.5*(psi(xx-1,yy,zz)+psi(xx,yy,zz));
    psi3(xx,yy,zz) = 0;
    psi4(xx,yy,zz) = 0.5*(psi(xx,yy-1,zz)+psi(xx,yy,zz));
    psi5(xx,yy,zz) = 0;
    psi6(xx,yy,zz) = 0.5*(psi(xx,yy,zz-1)+psi(xx,yy,zz));





}
