#include <iostream>
#include <vector>
#include <cmath>
#include "gsl/gsl_fft_complex.h"
#include <gnuplot-iostream.h>
#include "gsl/gsl_c.h"

// Function to compute discrete Fourier transform coefficients
void fourierTransform(const std::vector<double>& signal, std::vector<double>& coefficients) {
    size_t N = signal.size();
    gsl_fft_complex_wavetable* wavetable = gsl_fft_complex_wavetable_alloc(N);
    gsl_fft_complex_workspace* workspace = gsl_fft_complex_workspace_alloc(N);
    
    std::vector<double> complexSignal(2 * N);
    std::copy(signal.begin(), signal.end(), complexSignal.begin());

    gsl_fft_complex_forward(complexSignal.data(), 1, N, wavetable, workspace);

    coefficients.resize(N / 2 + 1);
    std::copy(complexSignal.begin(), complexSignal.begin() + N / 2 + 1, coefficients.begin());

    gsl_fft_complex_wavetable_free(wavetable);
    gsl_fft_complex_workspace_free(workspace);
}

int main() {
    // Sample signal (replace this with your drawn line)
    // For simplicity, let's assume a sine wave
    const size_t N = 1000;
    std::vector<double> signal(N);
    for (size_t i = 0; i < N; ++i) {
        double t = static_cast<double>(i) / N;
        signal[i] = std::sin(2 * M_PI * 5 * t);  // 5 Hz sine wave
    }

    // Compute Fourier coefficients
    std::vector<double> coefficients;
    fourierTransform(signal, coefficients);

    // Plot Fourier coefficients
    Gnuplot gp;
    gp << "set terminal wxt size 800,400\n";
    gp << "set xlabel 'Frequency'\n";
    gp << "set ylabel 'Magnitude'\n";
    gp << "set title 'Fourier Transform'\n";
    gp << "plot '-' with impulses title 'Magnitude Spectrum'\n";
    for (size_t i = 0; i < coefficients.size(); ++i) {
        gp << i << " " << std::abs(coefficients[i]) << "\n";
    }
    gp << "e\n";
    gp.flush();

    std::cout << "Press Enter to exit." << std::endl;
    std::cin.get();

    return 0;
}
