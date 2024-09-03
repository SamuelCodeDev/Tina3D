#ifndef UTILS_H
#define UTILS_H

template<typename T, const auto N>
inline consteval auto Countof(T(&array)[N]) -> decltype(N)
{ return N; }

template<typename T>
inline void SafeRelease(T * pointer)
{ if (pointer) pointer->Release(); }

template<typename T>
inline void SafeDelete(T * pointer)
{ if(pointer) delete pointer; pointer = nullptr; }

template<typename T>
inline void SafeDeleteArray(T * pointer)
{ if(pointer) delete[] pointer; pointer = nullptr; }

#endif