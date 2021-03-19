#include <iostream>
#include <vector>

using namespace std;
typedef float float8_t __attribute__((ext_vector_type(8)));

void step(float *r, const float *d, int n);
constexpr float infty = numeric_limits<float>::infinity();

constexpr float8_t f8infty{infty, infty, infty, infty,
                           infty, infty, infty, infty};

static float8_t *float8_alloc(size_t n) {
  void *tmp = 0;
  if (posix_memalign(&tmp, sizeof(float8_t), sizeof(float8_t) * n))
    throw bad_alloc();
  return (float8_t *)tmp;
}

static inline float hmin8(float8_t vv) {
  float v = infty;
  for (int i = 0; i < 8; ++i)
    v = min(vv[i], v);
  return v;
}

static inline float8_t min8(float8_t x, float8_t y) {
  float8_t z;
  for (int i = 0; i < 8; i++)
    z[i] = x[i] < y[i] ? x[i] : y[i];
  return z;
}

float *generate_dists(int n) {
  float *d = (float *)new float[n * n];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      *(d + i * n + j) = i == j ? 0.0f : (float)(rand() % 100 + 1);
  return d;
}

int main(int argc, char **argv) {
  constexpr int n = 1000;
  srand(time(NULL));
  const float *d = generate_dists(n);
  cout << "Generated" << endl;
  float r[n * n];
  clock_t start;
  double duration;
  start = clock();
  step(r, d, n);
  duration = (clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Stepped" << endl;
  /*for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cout << r[i * n + j] << " ";
      if (j % 10 == 9)
        cout << endl;
    }
    cout << endl;
    cout << endl;
  }*/
  cout << "step: " << duration << " seconds" << endl;
  delete[] d;
}

void step(float *r, const float *d_, int n) {
  // elements per vector
  constexpr int nb = 8;
  // vectors per input row
  int na = (n + nb - 1) / nb;

  // block size
  constexpr int nd = 3;
  // how many blocks of rows
  int nc = (n + nd - 1) / nd;
  // number of rows after padding
  int ncd = nc * nd;

  // input data, padded, converted to vectors
  float8_t *vd = float8_alloc(ncd * na);
  // input data, transposed, padded, converted to vectors
  float8_t *vt = float8_alloc(ncd * na);

#pragma omp parallel for
  for (int j = 0; j < n; ++j) {
    for (int ka = 0; ka < na; ++ka) {
      for (int kb = 0; kb < nb; ++kb) {
        int i = ka * nb + kb;
        vd[na * j + ka][kb] = i < n ? d_[n * j + i] : infty;
        vt[na * j + ka][kb] = i < n ? d_[n * i + j] : infty;
      }
    }
  }
  for (int j = n; j < ncd; ++j) {
    for (int ka = 0; ka < na; ++ka) {
      for (int kb = 0; kb < nb; ++kb) {
        vd[na * j + ka][kb] = infty;
        vt[na * j + ka][kb] = infty;
      }
    }
  }

#pragma omp parallel for
  for (int ic = 0; ic < nc; ++ic) {
    for (int jc = 0; jc < nc; ++jc) {
      float8_t vv[nd][nd];
      for (int id = 0; id < nd; ++id) {
        for (int jd = 0; jd < nd; ++jd) {
          vv[id][jd] = f8infty;
        }
      }
      for (int ka = 0; ka < na; ++ka) {
        float8_t y0 = vt[na * (jc * nd + 0) + ka];
        float8_t y1 = vt[na * (jc * nd + 1) + ka];
        float8_t y2 = vt[na * (jc * nd + 2) + ka];
        float8_t x0 = vd[na * (ic * nd + 0) + ka];
        float8_t x1 = vd[na * (ic * nd + 1) + ka];
        float8_t x2 = vd[na * (ic * nd + 2) + ka];
        vv[0][0] = min8(vv[0][0], x0 + y0);
        vv[0][1] = min8(vv[0][1], x0 + y1);
        vv[0][2] = min8(vv[0][2], x0 + y2);
        vv[1][0] = min8(vv[1][0], x1 + y0);
        vv[1][1] = min8(vv[1][1], x1 + y1);
        vv[1][2] = min8(vv[1][2], x1 + y2);
        vv[2][0] = min8(vv[2][0], x2 + y0);
        vv[2][1] = min8(vv[2][1], x2 + y1);
        vv[2][2] = min8(vv[2][2], x2 + y2);
      }
      for (int id = 0; id < nd; ++id) {
        for (int jd = 0; jd < nd; ++jd) {
          int i = ic * nd + id;
          int j = jc * nd + jd;
          if (i < n && j < n) {
            r[n * i + j] = hmin8(vv[id][jd]);
          }
        }
      }
    }
  }

  free(vt);
  free(vd);
}
