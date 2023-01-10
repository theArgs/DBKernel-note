#include "unique_ptr.h"

template<typename U>
UniquePtr<U> make_unique(U value) {
  return UniquePtr<U>(new U{value});
}

template<typename T>
UniquePtr<T>::UniquePtr() : _p(nullptr) {}

template<typename T>
UniquePtr<T>::UniquePtr(T* p) : _p(p) {}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) {
  if (_p == other._p) {
    return;
  }
  _p = other._p;
  other._p = nullptr;
}

template<typename T>
UniquePtr<T>::~UniquePtr() {
  if (_p) {
    delete _p;
    _p = nullptr;
  }
}

template<typename T>
T* UniquePtr<T>::get() {
  return _p;
}

template<typename T>
T& UniquePtr<T>::operator*() {
  return *_p;
}

template<typename T>
T* UniquePtr<T>::operator->() {
  return _p;
}

template<typename T>
void UniquePtr<T>::reset() {
  if (_p) {
    delete _p;
    _p = nullptr;
  }
}

template<typename T>
void UniquePtr<T>::reset(T *ptr) {
  if (_p) {
    delete _p;
    _p = nullptr;
  }
  _p = ptr;
}

template<typename T>
T* UniquePtr<T>::release() {
  T *res = _p;
  _p = nullptr; 
  return res;
}

template<typename T>
UniquePtr<T>::operator bool() {
  return _p != nullptr;
}