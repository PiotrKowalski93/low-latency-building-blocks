## Branch Prediction Hints: `[[likely]]` vs `__builtin_expect`

Both mechanisms provide branch prediction hints to the compiler, but they are not identical.

### `[[likely]]` / `[[unlikely]]` (C++20)

Standard C++ attributes that mark a branch as likely or unlikely to be taken.

```cpp
if (error) [[unlikely]]
{
    handle_error();
}
```

### `__builtin_expect()` (GCC/Clang)

Compiler-specific extension that expresses the expected value of an expression.

```cpp
if (__builtin_expect(error, 0))
{
    handle_error();
}
```

or

```cpp
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

if (UNLIKELY(error))
{
    handle_error();
}
```

### Key Differences

* `[[likely]]` / `[[unlikely]]` are **standard C++** and portable across modern compilers.
* `__builtin_expect()` is a **GCC/Clang extension** (not supported by MSVC).
* `[[unlikely]]` describes the **branch**.
* `__builtin_expect()` describes the **expected result of an expression**.
* The generated machine code is **compiler- and version-dependent**; neither construct guarantees a specific optimization.

### Practical Advice

Use `[[likely]]` / `[[unlikely]]` in modern C++ code for readability and portability:

```cpp
if (error) [[unlikely]]
{
    return;
}
```

Use `__builtin_expect()` only when targeting GCC/Clang and after profiling shows a measurable benefit.

### Important

Always verify with profiling and assembly inspection (`Compiler Explorer`, `objdump`, `perf`). Branch prediction hints are only suggestions to the compiler and may have no effect in a particular case.
