# simd_demo

### test autovec for icc and gcc
* icpc -fPIC -g -O3 -march=native -funroll-loops -fopenmp -std=c++17 -DVCL -I./version1 main.cc; ./a.out
* g++ -fPIC -g -O3 -march=native -funroll-loops -fopenmp -std=c++17 -DVCL -I./version1 main.cc; ./a.out

### explicit vectorization(simd) libraries, there are more on github
* SCTL, https://github.com/dmalhotra/SCTL
* vectorclass, https://github.com/vectorclass
* highway, https://github.com/google/highway
* Enoki, https://github.com/mitsuba-renderer/enoki
* Vc, https://github.com/VcDevel/Vc
* xsimd, https://github.com/QuantStack/xsimd
* embree, https://github.com/embree/embree/tree/master/common/simd
* QuickVec, https://bitbucket.org/kellogg92/quickvec/src/master/
* c++ std::experimental::simd, https://github.com/VcDevel/std-simd
