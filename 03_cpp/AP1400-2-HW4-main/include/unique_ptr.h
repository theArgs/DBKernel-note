#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template<typename T>
class UniquePtr {
 public:
  template<typename U>
  friend UniquePtr<U> make_unique(U value);

  UniquePtr();
  UniquePtr(T* p);
  UniquePtr(const UniquePtr &other) = delete;
  UniquePtr(UniquePtr &&other);
  ~UniquePtr();

  UniquePtr &operator=(const UniquePtr &other) = delete;

  T *get();

  T &operator*();
  T *operator->();

  void reset();
  void reset(T *ptr);

  T *release();

  operator bool();
 private:
  T* _p;
};

template<typename U>
UniquePtr<U> make_unique(U value);

#include "unique_ptr.hpp"

#endif //UNIQUE_PTR