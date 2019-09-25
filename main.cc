// icpc -fPIC -g -O3 -march=native -funroll-loops -fopenmp -std=c++17 -DVCL -I./version1 main.cc; ./a.out
// g++ -fPIC -g -O3 -march=native -funroll-loops -fopenmp -std=c++17 -DVCL -I./version1 main.cc; ./a.out
// /cm/shared/sw/pkg/vendor/intel-pstudio/2019-3/vtune_amplifier/bin64/amplxe-cl -collect hotspots ./a.out
#include <math.h> 
#include <vector>
#include <iostream>
#include <omp.h>

typedef double Real;

void test_kernel_ts(const Real* sources, int ns, const Real* charge,  
                 const Real* targets, int nt, Real* pot){

    for(int t = 0; t < nt; t++){
        for(int s = 0; s < ns; s++){
            
            Real dx = sources[s] - targets[t];
            Real r2 = dx*dx;
            pot[t] += exp(sqrt(r2)) * charge[s];

        }
    }

}

void test_kernel_st(const Real* sources, int ns, const Real* charge,  
                 const Real* targets, int nt, Real* pot){

    for(int s = 0; s < ns; s++){
        for(int t = 0; t < nt; t++){
            
            Real dx = sources[s] - targets[t];
            Real r2 = dx*dx;
            pot[t] += exp(sqrt(r2)) * charge[s];

        }
    }

}

#ifdef VCL
// Use Agner Fog's vector class library
#include "vectorclass.h"
#ifdef __INTEL_COMPILER
#include "vectormath_lib.h"
#else
#include "vectormath_exp.h"
#endif
void test_kernel_vec(const Real* sources, int ns, const Real* charge,  
                 const Real* targets, int nt, Real* pot){

    for(int t = 0; t < nt; t += 8){

        Vec8d trg_vec, pot_vec;
        // load
        trg_vec.load(targets + t);
        pot_vec.load(pot + t);

        for(int s = 0; s < ns; s++){
            Vec8d dx = Vec8d(sources[s]) - trg_vec;
            Vec8d r2 = dx*dx;
            pot_vec += exp(sqrt(r2)) * Vec8d(charge[s]);
        }

        // store
        pot_vec.store(pot + t);
    }

}
#endif

int main (void)
{
    int ns = 997, nt = 999, ntest = 100;
    std::vector<Real> Xs(ns), Xt(nt), charge(ns), pot(nt);

    // init
    for (auto& x : Xs) x = 10*M_PI*drand48();
    for (auto& x : Xt) x = 10*M_PI*drand48();
    for (auto& x : charge) x = drand48();
    for (auto& x : pot) x = 0.0;


    // kernel run vec
    Real ts = omp_get_wtime();
    for(int i=0; i<ntest; i++) {
        test_kernel_vec(&Xs[0], ns, &charge[0], 
                    &Xt[0], nt, &pot[0]);
    }
    Real te = omp_get_wtime();
    Real ans = 0;
    for(auto& x : pot) ans+=x;
    std::cout<<"manually simd ans: "<<ans<<"\n";
    std::cout<<"time elapsed: "<<te-ts<<"\n\n";
    
    // kernel run ts
    for (auto& x : pot) x = 0.0;
    ts = omp_get_wtime();
    for(int i=0; i<ntest; i++) {
        test_kernel_ts(&Xs[0], ns, &charge[0], 
                    &Xt[0], nt, &pot[0]);
    }
    te = omp_get_wtime();
    ans = 0;
    for(auto& x : pot) ans+=x;
    std::cout<<"target-source loop auto simd ans: "<<ans<<"\n";
    std::cout<<"time elapsed: "<<te-ts<<"\n\n";

    // kernel run st
    for (auto& x : pot) x = 0.0;
    ts = omp_get_wtime();
    for(int i=0; i<ntest; i++) {
        test_kernel_st(&Xs[0], ns, &charge[0], 
                    &Xt[0], nt, &pot[0]);
    }
    te = omp_get_wtime();
    ans = 0;
    for(auto& x : pot) ans+=x;
    std::cout<<"source-target loop auto simd ans: "<<ans<<"\n";
    std::cout<<"time elapsed: "<<te-ts<<"\n\n";

    return 0;
}
