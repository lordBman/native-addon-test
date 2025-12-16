import { join } from 'path';

// Load native addon
const nativeAddon: any = require(join(__dirname, '../../build/Release/mynative.node'));

// Re-export with proper typing
export const add = nativeAddon.add as (a: number, b: number) => number;
export const addComplex = nativeAddon.addComplex as (
    a: { real: number; imag: number },
    b: { real: number; imag: number }
) => { real: number; imag: number };

export const computeAsync = nativeAddon.computeAsync as (
    iterations: number,
    callback: (error: Error | null, result?: number) => void
) => void;

// Calculator class wrapper
export class Calculator {
    private instance: any;
    
    constructor(initialValue: number = 0) {
        this.instance = new nativeAddon.Calculator(initialValue);
    }
    
    add(num: number): number {
        return this.instance.add(num);
    }
    
    subtract(num: number): number {
        return this.instance.subtract(num);
    }
    
    get value(): number {
        return this.instance.value;
    }
    
    set value(val: number) {
        this.instance.value = val;
    }
    
    getValue(): number {
        return this.instance.getValue();
    }
}

// Utility functions
export function isLoaded(): boolean {
    return !!nativeAddon;
}

export default {
    add,
    addComplex,
    computeAsync,
    Calculator,
    isLoaded
};