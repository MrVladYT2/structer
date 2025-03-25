#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
const int N = 4096;
const int TRIALS = 1;
const int BLOCK_SIZE = 256;
void initialize_matrix(double* matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for (int i = 0; i < N*N; ++i) {
        matrix[i] = dist(gen);
    }
}

void block_multiply(const double* A, const double* B, double* C) {
    for (int i_block = 0; i_block < N; i_block += BLOCK_SIZE) {
        for (int j_block = 0; j_block < N; j_block += BLOCK_SIZE) {
            for (int k_block = 0; k_block < N; k_block += BLOCK_SIZE) {
                
                int i_end = std::min(i_block + BLOCK_SIZE, N);
                int j_end = std::min(j_block + BLOCK_SIZE, N);
                int k_end = std::min(k_block + BLOCK_SIZE, N);
                
                for (int i = i_block; i < i_end; ++i) {
                    for (int k = k_block; k < k_end; ++k) {
                        double a = A[i*N + k];
                        for (int j = j_block; j < j_end; ++j) {
                            C[i*N + j] += a * B[k*N + j];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    double* A = static_cast<double*>(aligned_alloc(64, N*N*sizeof(double)));
    double* B = static_cast<double*>(aligned_alloc(64, N*N*sizeof(double)));
    double* C = static_cast<double*>(aligned_alloc(64, N*N*sizeof(double)));

    initialize_matrix(A);
    initialize_matrix(B);
    std::fill(C, C + N*N, 0.0);

    const double complexity = 2.0 * N * N * N;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int t = 0; t < TRIALS; ++t) {
        std::fill(C, C + N*N, 0.0);
        block_multiply(A, B, C);
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    double time_sec = duration / 1000.0 / TRIALS;
    double mflops = complexity / time_sec / 1e6;

    std::cout << "Оптимизированный алгоритм:\n";
    std::cout << "Время: " << time_sec << " сек\n";
    std::cout << "Производительность: " << mflops << " MFlops\n";

    free(A); free(B); free(C);
    return 0;
}
