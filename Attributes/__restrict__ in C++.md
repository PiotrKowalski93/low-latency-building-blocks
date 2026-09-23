# `__restrict__` in C++

`__restrict__` (or `__restrict`) is a compiler extension that tells the compiler a pointer is the **only way to access the referenced memory** within the current scope.
This removes potential **aliasing** concerns and enables more aggressive optimizations.

## Example

```cpp
void add(
    float* __restrict__ dst,
    const float* __restrict__ lhs,
    const float* __restrict__ rhs,
    std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        dst[i] = lhs[i] + rhs[i];
    }
}
```

The compiler can assume below and optimize accordingly.

```cpp
dst != lhs
dst != rhs
lhs != rhs
```
## Why it matters

Without `__restrict__`, the compiler must conservatively assume pointers may overlap and could potentially modify memory later read through another pointer.

```cpp
dst[i] = lhs[i] + rhs[i];
```

With `__restrict__`, the compiler gains freedom to:

* Keep values in registers
* Eliminate redundant loads/stores
* Improve instruction scheduling
* Generate wider SIMD code (SSE/AVX/AVX-512)
* Vectorize loops more aggressively

## Important

`__restrict__` is a contract. Violating it results in **undefined behavior**:

```cpp
float x = 0.0f;

add(&x, &x, &x, 1); // UB
```

## HPC / HFT Takeaway

`__restrict__` is commonly used in performance-critical code where memory access patterns are well understood:

* Numerical kernels
* SIMD-heavy code
* Market data processing
* Order book updates
* Matching engines
* Low-latency messaging pipelines

It does **not** guarantee a speedup, but it removes one of the major barriers to compiler optimizations and automatic vectorization.
