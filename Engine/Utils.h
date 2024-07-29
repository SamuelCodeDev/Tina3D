#ifndef UTILS_H
#define UTILS_H

#ifndef _MSC_VER
#include <cstring>

template<typename T>
inline void* ZeroMemory(T* DesTination, const auto Length)
{ return memset(DesTination, 0, Length); }

template<typename T>
inline void* CopyMemory(T* DesTination, T* Source, const auto Length)
{ return memcpy(DesTination, Source, Length); }
#endif

template<typename T, auto N>
inline constexpr auto countof(T(&array)[N])
{ return N; }

template<typename T>
inline void SafeRelease(T*& pointer)
{ if (pointer) pointer->Release(); }

template<typename T>
inline void SafeDelete(T*& pointer)
{ if (pointer) delete pointer; pointer = nullptr; }

#endif