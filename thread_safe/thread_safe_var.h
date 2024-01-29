#include <mutex>

template<typename T>
class ThreadSafeVar {
    public:

    ThreadSafeVar() { value_ = T(); }
    
    ThreadSafeVar(T initial_value) { value_ = initial_value; }

    void Add(T val) {
        std::lock_guard<std::mutex> lock(lock_);
        value_ += val;
    }

    void Sub(T val) {
        std::lock_guard<std::mutex> lock(lock_);
        value_ -= val;
    }

    T GetValue() { return value_; }

    private:
    
    std::mutex lock_;
    T value_;
};