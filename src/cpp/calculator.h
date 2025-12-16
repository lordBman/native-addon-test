#pragma once

namespace calculator {
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    
    // Complex data structure
    struct ComplexNumber {
        double real;
        double imag;
    };
    
    ComplexNumber addComplex(const ComplexNumber& a, const ComplexNumber& b);
    
    // Async work class
    class HeavyCalculator {
    public:
        static double compute(int iterations);
    };
}