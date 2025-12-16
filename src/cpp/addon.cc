#include <napi.h>
#include "calculator.h"

// Helper function to convert Napi::Value to calculator::ComplexNumber
calculator::ComplexNumber ExtractComplexNumber(const Napi::CallbackInfo& info, int index) {
    Napi::Env env = info.Env();
    Napi::Object obj = info[index].As<Napi::Object>();
    return {
        obj.Get("real").As<Napi::Number>().DoubleValue(),
        obj.Get("imag").As<Napi::Number>().DoubleValue()
    };
}

// Helper function to convert calculator::ComplexNumber to Napi::Object
Napi::Object CreateComplexObject(Napi::Env env, const calculator::ComplexNumber& c) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("real", c.real);
    obj.Set("imag", c.imag);
    return obj;
}

// Synchronous functions
Napi::Number Add(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return Napi::Number::New(env, 0);
    }
    
    double a = info[0].As<Napi::Number>().DoubleValue();
    double b = info[1].As<Napi::Number>().DoubleValue();
    
    return Napi::Number::New(env, calculator::add(a, b));
}

Napi::Object AddComplex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    
    auto a = ExtractComplexNumber(info, 0);
    auto b = ExtractComplexNumber(info, 1);
    auto result = calculator::addComplex(a, b);
    
    return CreateComplexObject(env, result);
}

// Async Worker class
class ComputeAsyncWorker : public Napi::AsyncWorker {
public:
    ComputeAsyncWorker(Napi::Function& callback, int iterations)
        : Napi::AsyncWorker(callback), iterations_(iterations), result_(0.0) {}
    
    void Execute() override {
        // This runs in a separate thread
        result_ = calculator::HeavyCalculator::compute(iterations_);
    }
    
    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::Number::New(Env(), result_)});
    }
    
    void OnError(const Napi::Error& e) override {
        Napi::HandleScope scope(Env());
        Callback().Call({e.Value(), Env().Undefined()});
    }
    
private:
    int iterations_;
    double result_;
};

// Async function
Napi::Value ComputeAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int iterations = info[0].As<Napi::Number>().Int32Value();
    Napi::Function callback = info[1].As<Napi::Function>();
    
    ComputeAsyncWorker* worker = new ComputeAsyncWorker(callback, iterations);
    worker->Queue();
    
    return env.Undefined();
}

// Object-oriented approach
class CalculatorWrapper : public Napi::ObjectWrap<CalculatorWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "Calculator", {
            InstanceMethod("add", &CalculatorWrapper::Add),
            InstanceMethod("subtract", &CalculatorWrapper::Subtract),
            InstanceMethod("getValue", &CalculatorWrapper::GetValue),
            InstanceAccessor("value", &CalculatorWrapper::GetValue, &CalculatorWrapper::SetValue)
        });
        
        exports.Set("Calculator", func);
        return exports;
    }
    
    CalculatorWrapper(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<CalculatorWrapper>(info), value_(0.0) {
        if (info.Length() > 0) {
            value_ = info[0].As<Napi::Number>().DoubleValue();
        }
    }
    
private:
    double value_;
    
    Napi::Value Add(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        
        if (info.Length() < 1) {
            Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
            return env.Null();
        }
        
        double num = info[0].As<Napi::Number>().DoubleValue();
        value_ += num;
        
        return Napi::Number::New(env, value_);
    }
    
    Napi::Value Subtract(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        
        if (info.Length() < 1) {
            Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
            return env.Null();
        }
        
        double num = info[0].As<Napi::Number>().DoubleValue();
        value_ -= num;
        
        return Napi::Number::New(env, value_);
    }
    
    Napi::Value GetValue(const Napi::CallbackInfo& info) {
        return Napi::Number::New(info.Env(), value_);
    }
    
    void SetValue(const Napi::CallbackInfo& info, const Napi::Value& value) {
        value_ = value.As<Napi::Number>().DoubleValue();
    }
};

// Initialize the module
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Export standalone functions
    exports.Set("add", Napi::Function::New(env, Add));
    exports.Set("addComplex", Napi::Function::New(env, AddComplex));
    exports.Set("computeAsync", Napi::Function::New(env, ComputeAsync));
    
    // Export class
    CalculatorWrapper::Init(env, exports);
    
    return exports;
}

NODE_API_MODULE(mynative, Init)