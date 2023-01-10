#ifndef SUB_INGREDIENTS_H
#define SUB_INGREDIENTS_H

#define DEFCLASS(Name, price_unit)                    \
class Name : public Ingredient {                      \
 public:                                              \
  Name(size_t units) : Ingredient{price_unit, units} {\
    this->name = #Name;                               \
  }                                                   \
  virtual std::string get_name() override {           \
    return this->name;                                \
  }                                                   \
  virtual Ingredient *copy() override {               \
    return new Name(this->units);                    \
  }                                                   \
};

DEFCLASS(Cinnamon, 5);
DEFCLASS(Chocolate, 5);
DEFCLASS(Sugar, 1);
DEFCLASS(Cookie, 10);
DEFCLASS(Espresso, 15);
DEFCLASS(Milk, 10)
DEFCLASS(MilkFoam, 5);
DEFCLASS(Water, 1);

#endif // SUB_INGREDIENTS_H