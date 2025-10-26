#pragma once
#include <cstddef>

namespace my {

template <typename T>
class unique_ptr {
public:
    // Constructors
    constexpr unique_ptr() noexcept {
        ptr_ = nullptr;
    }
    constexpr explicit unique_ptr(std::nullptr_t) noexcept {
        ptr_ = nullptr;
    }
    explicit unique_ptr(T* ptr) noexcept
        : ptr_(ptr) {
    }

    // Copy constructor
    unique_ptr(const unique_ptr&) = delete;

    // Copy assignment
    unique_ptr& operator=(const unique_ptr&) = delete;

    // Move constructor
    unique_ptr(unique_ptr&& new_ptr) noexcept {
        ptr_ = new_ptr.ptr_;
        new_ptr.ptr_ = nullptr;
    }

    // Move assignment
    unique_ptr& operator=(unique_ptr&& new_ptr) noexcept {
        if (this != &new_ptr) {
            delete ptr_;
            ptr_ = new_ptr.ptr_;
            new_ptr.ptr_ = nullptr;
        }
        return *this;
    }

    // Destructor
    ~unique_ptr() {
        delete ptr_;
    }

    // Methods
    T& operator*() const noexcept {
        return *ptr_;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void reset(T* ptr = nullptr) noexcept {
        if (ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }

private:
    T* ptr_;
};

}  // namespace my
