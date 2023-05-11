#ifndef ESPRESSO_BASED_H
#define ESPRESSO_BASED_H
#include <iostream>
#include <string>
#include <vector>

#include "ingredient.h"
class EspressoBased {
 public:
  virtual std::string get_name() = 0;
  virtual double price() = 0;

  virtual void brew() = 0;
  std::vector<Ingredient*>& get_ingredients() { return ingredients; }

  virtual ~EspressoBased() {
    for (const auto& i : ingredients) delete i;
    ingredients.clear();
  }

 protected:
  EspressoBased();
  EspressoBased(std::string str) : name(str) {}
  EspressoBased(const EspressoBased& esp) {
    for (const auto& i : esp.ingredients) {
      ingredients.push_back(i->clone());
    }
  }
  void operator=(const EspressoBased& esp);

  std::vector<Ingredient*> ingredients;
  std::string name;
};
#endif  // ESPRESSO_BASED_H
