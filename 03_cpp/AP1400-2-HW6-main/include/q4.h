#ifndef Q4_H
#define Q4_H

#include <algorithm>
#include <vector>

namespace q4 {

struct Vector2D {
  double x{};
  double y{};
};

struct Sensor {
  Vector2D pos;
  double accuracy;
};

Vector2D kalman_filter(std::vector<Sensor> sensors) {
  double size = 0;
  std::for_each(sensors.begin(), sensors.end(),
      [&size](const auto &sensor){
        size += sensor.accuracy;
      });

  double x = 0;
  std::for_each(sensors.begin(), sensors.end(),
      [&x](const auto &sensor){
        x += sensor.pos.x * sensor.accuracy;
      });

  double y = 0;
  std::for_each(sensors.begin(), sensors.end(),
      [&y](const auto &sensor){
        y += sensor.pos.y * sensor.accuracy;
      });
  return {x / size, y / size};
}

} // namespace q4

#endif //Q4_H