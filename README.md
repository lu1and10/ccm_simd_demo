## simd demo
This C++ code shows how to accelerate a simple O(N^2) direct kernel summation of the type common in integral equation solvers, FMMs, etc.
It compares 3 methods: explicit SIMD vectorization via the VCL library (which for convenience is included in this repo), and letting the compiler vectorize for different inner-outer loop orders.

For more on what "SIMD vectorization" means, see here:   
https://en.wikipedia.org/wiki/SIMD  
https://software.intel.com/en-us/articles/improve-performance-using-vectorization-and-intel-xeon-scalable-processors  
https://software.intel.com/en-us/blogs/2012/01/31/vectorization-find-out-what-it-is-find-out-more  
https://www.codingame.com/playgrounds/283/sse-avx-vectorization/what-is-sse-and-avx

For some speedups in the Helmholtz kernel context, see demo_slides.pdf.  
Wen Yan's simd demo https://github.com/wenyan4work/DemoSIMD, which also has memory cache tutorials. 

## test autovec for icc and gcc
* icpc -fPIC -g -O3 -march=native -funroll-loops -fopenmp -std=c++17 -DVCL -I./version1 main.cc; ./a.out
* g++ -fPIC -g -O3 -march=native -funroll-loops -fopenmp -std=c++17 -DVCL -I./version1 main.cc; ./a.out

## explicit vectorization(simd) libraries, there are more on github
* SCTL, https://github.com/dmalhotra/SCTL
* vectorclass, https://github.com/vectorclass
* highway, https://github.com/google/highway
* Enoki, https://github.com/mitsuba-renderer/enoki
* Vc, https://github.com/VcDevel/Vc
* xsimd, https://github.com/QuantStack/xsimd
* embree, https://github.com/embree/embree/tree/master/common/simd
* QuickVec, https://bitbucket.org/kellogg92/quickvec/src/master/
* c++ std::experimental::simd, https://github.com/VcDevel/std-simd
