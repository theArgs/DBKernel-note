#include "shared_ptr.h"

template<typename U>
SharedPtr<U> make_shared(U value) {
  return SharedPtr<U>(new U{value});
}

template<typename T>
SharedPtr<T>::SharedPtr() : _p(nullptr) {
  count = new size_t{0};
}

template<typename T>
SharedPtr<T>::SharedPtr(T *p) : _p(p) {
  count = new size_t{1};
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr &other) {
  if (_p && _p == other._p) {
    return;
  }
  _p = other._p;
  count = other.count;
  ++(*count);
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr &&other) {
  if (_p && _p == other._p) {
    return;
  }
  _p = other._p;
  count = other.count;
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
  if (count) --(*count);
  if (count && (*count) <= 0 && _p) {
    delete _p;
    delete count;
    _p = nullptr;
    count = nullptr;
  }
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &other) {
  if (_p && _p == other._p) {
    return *this;
  }
  if (_p) {
    --(*count);
    if (*count == 0) {
      delete _p;
    } 
  }
  _p = other._p;
  count = other.count;
  if (_p) {
    ++(*count);
  }
  return *this;
}

template<typename T>
T* SharedPtr<T>::get() {
  return _p;
}

template<typename T>
size_t SharedPtr<T>::use_count() {
  if (!count) return 0;
  return *count;
}

template<typename T>
T &SharedPtr<T>::operator*() {
  return *_p;
}

template<typename T>
T *SharedPtr<T>::operator->() {
  return _p;
}

template<typename T>
void SharedPtr<T>::reset() {
  if (_p && count) {
    --(*count);
    _p = nullptr;
  }
}

template<typename T>
void SharedPtr<T>::reset(T *ptr) {
  if (_p && count) {
    --(*count);
  }
  _p = ptr;
  count = new size_t{1};
}

template<typename T>
SharedPtr<T>::operator bool() {
  return _p != nullptr && count != nullptr && *count != 0;
}