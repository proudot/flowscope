
#include "CSVAR.h"
#define EPSILON 0.001
#include <iostream>
#define T 1.f

cimg_csvar3d::cimg_csvar3d()
{

}



CImg<float> cimg_csvar3d::splinterp_warper3(CImg<float> &source, CImg<float> &u, CImg<float> &v, CImg<float> &w){
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


void cimg_csvar3d::estimate_singlescale(CImg<float> &source, CImg<float> &target, /*CImg<float> &geomap,*/
                                        CImg<float> &u, CImg<float> &v, CImg<float> &w,
                                        double N, float ep,
                                        float gamma, float omega,
                                        float TOL, int max_iter)
{

    I1 =source; I2 = target;
    width = I1.width();height = I1.height(); depth = I1.depth();
    CImg<float> uu(u), vv(v), ww(w);

    //saptial gradients of source
    I1x.assign(I1.width(), I1.height(), I1.depth(),1,0);
    I1y.assign(I1.width(), I1.height(), I1.depth(),1,0);
    I1z.assign(I1.width(), I1.height(), I1.depth(),1,0);


    cout<<"Initiated ..\n";

    cout<<"Directional derivatives of image gradients computed.\n";
    //Directional derivatives of source needs to be computed only once for this scale
    delIm1 = get_directionalDerivatives(source, N);


    //using 5 outer iterations to deal with non linearity (todo: make it a function parameter)
    int out_it = 5;
    //using 10 SOR iterations
    max_iter = 10;

    for(int i = 0; i < out_it; ++i){
        struct timeval sstart, send; //get current time
        gettimeofday(&sstart, NULL);

        //save scalar fields temporarily
        uu = u; vv = v; ww = w;

        //warp target with the 3D flow field. This bring target close to source
        I2w  = splinterp_warper3(I2,  u,v,w);

        delIm2 = get_directionalDerivatives(I2w, N);
        Hp = get_Hp(delIm2, ep, N);

        CImg<float> It = I2w- I1; //temporal image derivative

        get_firstOrderDerivatives(I2w, I2x, I2y, I2z);
        cout<<"Now computing Directional derivatives of image gradients...\n";
        delIx = get_directionalDerivatives(I2x,N);
        delIy = get_directionalDerivatives(I2y,N);
        delIz = get_directionalDerivatives(I2z,N);

        delIt = get_directionalDerivatives(It,N);
        float Dnx, Dny, Dnz, ulocal, vlocal, wlocal;
        int n = 0;
        float err = 1e9;
        u.assign(u.width(), u.height(), u.depth(), 1, 0.);
        v.assign(u.width(), u.height(), u.depth(), 1, 0.);
        w.assign(u.width(), u.height(), u.depth(), 1, 0.);



        cout<<"Starting SOR Iterations...\n";

        while(err > TOL && n < max_iter){
            n++;
            err = 0;

            cout<<n<<" iteration\n";
            for(int z = 1; z < I1.depth()-1; ++z)
                for(int y = 1; y < I1.height()-1; ++y)
                    for(int x = 1; x < I1.width()-1; ++x)  {



                        ulocal = u(x,y,z);
                        vlocal = v(x,y,z);
                        wlocal = w(x,y,z);
                        float intensityTerm = 0.0;
                        float denominator = 0.0;
                        for(int i = 0; i < N; ++i) //N is  the index of the direction
                        {
                            Dnx = pow(Hp[i](x,y,z),2)*delIx[i](x,y,z);

                            intensityTerm += Dnx*(vlocal*delIy[i](x,y,z) + wlocal*delIz[i](x,y,z) + T*delIt[i](x,y,z));

                            denominator += Dnx*delIx[i](x,y,z);
                        }

                        //for divergence term

                        float div_u, div_v, div_w;

                        // If low signal, suppress motion.
                        //This step is not necessary.
                        //Only for speed up purposes.
                        if(I1(x,y,z) < 1)
                        {
                            u(x,y,z) = 0;
                            v(x,y,z) = 0;
                            w(x,y,z) = 0;
                            continue;
                        }

                        if(N==26)
                            div_u = u(x-1, y, z) + u(x+1, y, z) + u(x-1, y-1, z) + u(x-1, y+1, z) + u(x+1, y-1, z) + u(x+1, y+1, z) +

                                    u(x-1, y, z-1) + u(x+1, y, z-1) + u(x-1, y-1, z-1) + u(x-1, y+1, z-1) + u(x+1, y-1, z-1) + u(x+1, y+1, z-1) +

                                    u(x-1, y, z+1) + u(x+1, y, z+1) + u(x-1, y-1, z+1) + u(x-1, y+1, z+1) + u(x+1, y-1, z+1) + u(x+1, y+1, z+1) +

                                    u(x, y-1, z) + u(x, y+1, z) + u(x, y-1, z-1) + u(x, y+1, z-1) + u(x, y-1, z+1) + u(x, y+1, z+1) +

                                    u(x, y, z-1) + u(x, y, z+1);

                        else if(N==6)
                            div_u = u(x-1, y, z) + u(x+1, y, z) +
                                    u(x, y-1, z) + u(x, y+1, z) +
                                    u(x, y, z-1) + u(x, y, z+1);


                        float numerator = -1./N*intensityTerm +
                                gamma*(div_u);



                        ulocal = (1.0- omega)*ulocal + omega*numerator/(1./N*denominator+gamma*N );


                        intensityTerm = 0.0;
                        denominator = 0.0;
                        for(int i = 0; i < N; ++i) //this the index of the direction
                        {
                            Dny = pow(Hp[i](x,y,z),2)*delIy[i](x,y,z);
                            intensityTerm += Dny*(ulocal*delIx[i](x,y,z) + wlocal*delIz[i](x,y,z) + T*delIt[i](x,y,z));
                            denominator += Dny*delIy[i](x,y,z);
                        }

                        if(N==26)
                            div_v = v(x-1, y, z) + v(x+1, y, z) + v(x-1, y-1, z) + v(x-1, y+1, z) + v(x+1, y-1, z) + v(x+1, y+1, z) +

                                    v(x-1, y, z-1) + v(x+1, y, z-1) + v(x-1, y-1, z-1) + v(x-1, y+1, z-1) + v(x+1, y-1, z-1) + v(x+1, y+1, z-1) +

                                    v(x-1, y, z+1) + v(x+1, y, z+1) + v(x-1, y-1, z+1) + v(x-1, y+1, z+1) + v(x+1, y-1, z+1) + v(x+1, y+1, z+1) +

                                    v(x, y-1, z) + v(x, y+1, z) + v(x, y-1, z-1) + v(x, y+1, z-1) + v(x, y-1, z+1) + v(x, y+1, z+1) +

                                    v(x, y, z-1) + v(x, y, z+1);

                        else if(N==6)
                            div_v = v(x-1, y, z) + v(x+1, y, z) +
                                    v(x, y-1, z) + v(x, y+1, z) +
                                    v(x, y, z-1) + v(x, y, z+1);


                        numerator = -1./N*intensityTerm +
                                gamma*(div_v);
                        vlocal = (1.0- omega)*vlocal + omega*numerator/(1./N*denominator+gamma*N);


                        intensityTerm = 0.0;
                        denominator = 0.0;
                        for(int i = 0; i < N; ++i) //this the index of the direction
                        {
                            Dnz = pow(Hp[i](x,y,z),2)*delIz[i](x,y,z);
                            intensityTerm += Dnz*(ulocal*delIx[i](x,y,z) + vlocal*delIy[i](x,y,z) + T*delIt[i](x,y,z));
                            denominator += Dnz*delIz[i](x,y,z);
                        }

                        if(N==26)
                            div_w = w(x-1, y, z) + w(x+1, y, z) + w(x-1, y-1, z) + w(x-1, y+1, z) + w(x+1, y-1, z) + w(x+1, y+1, z) +

                                    w(x-1, y, z-1) + w(x+1, y, z-1) + w(x-1, y-1, z-1) + w(x-1, y+1, z-1) + w(x+1, y-1, z-1) + w(x+1, y+1, z-1) +

                                    w(x-1, y, z+1) + w(x+1, y, z+1) + w(x-1, y-1, z+1) + w(x-1, y+1, z+1) + w(x+1, y-1, z+1) + w(x+1, y+1, z+1) +

                                    w(x, y-1, z) + w(x, y+1, z) + w(x, y-1, z-1) + w(x, y+1, z-1) + w(x, y-1, z+1) + w(x, y+1, z+1) +

                                    w(x, y, z-1) + w(x, y, z+1);

                        else if(N==6)
                            div_w = w(x-1, y, z) + w(x+1, y, z) +
                                    w(x, y-1, z) + w(x, y+1, z) +
                                    w(x, y, z-1) + w(x, y, z+1);


                        numerator = -1./N*intensityTerm +
                                gamma*(div_w);
                        wlocal = (1.0- omega)*wlocal + omega*numerator/(1./N*denominator+gamma*N );


                        err += pow(ulocal-u(x,y,z),2) + pow(vlocal - v(x,y,z),2) + pow(wlocal - w(x,y,z),2);
                        u(x,y,z) = ulocal;
                        v(x,y,z) = vlocal;
                        w(x,y,z) = wlocal;


                    }         float a;

            cout<<"Error: "<<err<<endl;

        }
        gettimeofday(&send, NULL);

        double delta = ((send.tv_sec  - sstart.tv_sec) * 1000000u +
                        send.tv_usec - sstart.tv_usec) / 1.e6;
        cout<<"time elapsed: "<<delta<<endl;

        //augment the flow field computed in this level
        //each finer level will have smaller motion magnitude,
        //which needs to be summed
        u = uu+u;
        v = vv+v;
        w = ww+w;
    }



}

// one outer loop:482.312 for 290x182x115








