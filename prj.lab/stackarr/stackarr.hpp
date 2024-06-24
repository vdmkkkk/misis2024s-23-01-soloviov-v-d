#ifndef STACKARR
#define STACKARR

#include "complex/complex.hpp"
#include <cstddef>

class StackArr {
public:
    StackArr() = default;
    StackArr(StackArr&& rhs) noexcept;
    StackArr(const StackArr& rhs);

    StackArr& operator=(StackArr&& rhs) noexcept;
    StackArr& operator=(const StackArr& rhs);

    void Push(const Complex& rhs);
    void Pop() noexcept;
    Complex& Top();

    const Complex& Top() const;

    void Clear() noexcept;

    bool IsEmpty() const noexcept;

    ~StackArr();
private:
    int current_index = -1;
    std::ptrdiff_t capacity = 0;
    Complex* data_ = nullptr;
};

#endif
