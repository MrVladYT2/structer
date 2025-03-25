
#include <iostream>
#include <random>
#include <chrono>
#include <cblas.h>

const int N = 4096;
const int TRIALS = 1;

void initialize_matrix(double* matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for (int i = 0; i < N*N; ++i) {
        matrix[i] = dist(gen);
    }
}

int main() {
    double* A = new double[N*N];
    double* B = new double[N*N];
    double* C = new double[N*N]();

    initialize_matrix(A);
    initialize_matrix(B);

    const double complexity = 2.0 * N * N * N;
    const double alpha = 1.0;
    const double beta = 0.0;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int t = 0; t < TRIALS; ++t) {
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    N, N, N, alpha, A, N, B, N, beta, C, N);
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    double time_sec = duration / 1000.0 / TRIALS;
    double mflops = complexity / time_sec / 1e6;

    std::cout << "BLAS реализация:\n";
    std::cout << "Время: " << time_sec << " сек\n";
    std::cout << "Производительность: " << mflops << " MFlops\n";

    delete[] A; delete[] B; delete[] C;
    return 0;
}
