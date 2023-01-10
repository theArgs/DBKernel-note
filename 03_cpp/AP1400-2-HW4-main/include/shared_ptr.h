#ifndef SHARED_PTR
#define SHARED_PTR

template<typename T>
class SharedPtr {
 public:
  template<typename U>
  friend SharedPtr<U> make_shared(U value);

  SharedPtr();
  SharedPtr(T *p);
  SharedPtr(const SharedPtr &other);
  SharedPtr(SharedPtr &&other);
  ~SharedPtr();

  SharedPtr &operator=(const SharedPtr &other);

  T *get();
  size_t use_count();

  T &operator*();
  T *operator->();

  void reset();
  void reset(T *ptr);

  operator bool();
 private:
  T *_p;
  size_t *count;
};

template<typename U>
SharedPtr<U> make_shared(U value);

#include "shared_ptr.hpp"

#endif //SHARED_PTR