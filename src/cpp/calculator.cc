#include "calculator.h"
#include <cmath>
#include <thread>
#include <chrono>

namespace calculator {
    double add(double a, double b) {
        return a + b;
    }
    
    double subtract(double a, double b) {
        return a - b;
    }
    
    double multiply(double a, double b) {
        return a * b;
    }
    
    double divide(double a, double b) {
        if (b == 0.0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }
    
    ComplexNumber addComplex(const ComplexNumber& a, const ComplexNumber& b) {
        return {a.real + b.real, a.imag + b.imag};
    }
    
    double HeavyCalculator::compute(int iterations) {
        double result = 0.0;
        for (int i = 0; i < iterations; ++i) {
            result += std::sin(i * 0.01) * std::cos(i * 0.01);
            // Simulate heavy computation
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        return result;
    }
}