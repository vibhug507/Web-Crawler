#include <map>
#include <mutex>

template<typename Key, typename Value>
class ThreadSafeMap {
    public:
    std::mutex lock_;

    Value Get(Key K) {
        std::lock_guard<std::mutex> lock(lock_);
        return mp_[K];
    }

    void Set(Key K, Value V) {
        std::lock_guard<std::mutex> lock(lock_);
        mp_[K] = V;
    }

    void Add(Key K, Value V) {
        std::lock_guard<std::mutex> lock(lock_);
        mp_[K] += V;
    }

    std::map<std::string, int> GetMap() const { return mp_; }

    private:
    std::map<Key, Value> mp_;
};