#ifndef Q1_H
#define Q1_H

#include <functional>

namespace q1 {


template<typename T, typename Func>
T gradient_descent(T init_value, T step, Func func = Func()) {
  double pos = init_value;
  while (true) {
    if (func(pos + step) < func(pos)) {
      pos = pos + step;
    } else if (func(pos - step) < func(pos)){
      pos = pos - step;
    } else {
      return pos;
    }
  }
}

}  // namespace q1

#endif //Q1_H