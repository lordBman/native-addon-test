export interface ComplexNumber {
    real: number;
    imag: number;
}

export type ComputeCallback = (error: Error | null, result?: number) => void;

export interface NativeAddon {
    // Standalone functions
    add(a: number, b: number): number;
    addComplex(a: ComplexNumber, b: ComplexNumber): ComplexNumber;
    computeAsync(iterations: number, callback: ComputeCallback): void;
    
    // Calculator class
    Calculator: {
        new (initialValue?: number): CalculatorInstance;
    };
}

export interface CalculatorInstance {
    add(num: number): number;
    subtract(num: number): number;
    getValue(): number;
    value: number;
}