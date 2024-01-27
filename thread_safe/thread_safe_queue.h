#include <queue>
#include <mutex>

template<typename T>
class ThreadSafeQueue {
    public:
    std::mutex lock_;

    void Push(T value) {
        std::lock_guard<std::mutex> lock(lock_);
        q_.push(value);
    }

    T Front() {
        std::lock_guard<std::mutex> lock(lock_);
        if (q_.empty()) return T();
        return q_.front();
    }

    int Pop() {
        std::lock_guard<std::mutex> lock(lock_);
        if (q_.empty()) return -1;
        q_.pop();
        return 0;
    }

    T GetAndPop() {
        std::cout << "called\n";
        std::lock_guard<std::mutex> lock(lock_);
        if (q_.empty()) return T();
        T value = q_.front();
        q_.pop();
        return value;
    }

    private:
    std::queue<T> q_;

};