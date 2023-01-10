#include "espresso_based.h"

#include <iostream>

EspressoBased::EspressoBased() {}

EspressoBased::EspressoBased(const EspressoBased& esp) : name(esp.name) {
  for (auto i : esp.ingredients) {
    ingredients.push_back(i->copy());
  }
}

void EspressoBased::operator=(const EspressoBased& esp) {
  if (this == &esp) {
    return;
  }
  for (const auto &i : ingredients) {
    delete i;
  }
  ingredients.clear();
  for (auto ingredient : esp.ingredients) {
    ingredients.push_back(ingredient->copy());
  }
  name = esp.name;
}

EspressoBased::~EspressoBased() {
  for (const auto &i : ingredients) {
    delete i;
  }
  ingredients.clear();
}

void EspressoBased::brew() {
  for (const auto &i : ingredients) {
    std::cout << i->out() << std::endl;
  }
}

std::vector<Ingredient*>& EspressoBased::get_ingredients() {
  return ingredients;
}