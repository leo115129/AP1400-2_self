#ifndef SUB_INGREDIENTS_H
#define SUB_INGREDIENTS_H
#include "ingredient.h"
/*#表示字符串 \表示将代码连在一起*/
#define DEFCLASS(class_name, price_units)                                  \
  class class_name : public Ingredient {                                   \
   public:                                                                 \
    class_name(size_t units) : Ingredient{price_units, units} {            \
      this->name = #class_name;                                            \
    }                                                                      \
    virtual std::string get_name() override { return this->name; }         \
    virtual Ingredient* clone() override { return new class_name(*this); } \
  }

DEFCLASS(Cinnamon, 5);
DEFCLASS(Chocolate, 5);
DEFCLASS(Sugar, 1);
DEFCLASS(Cookie, 10);
DEFCLASS(Espresso, 15);
DEFCLASS(Milk, 10);
DEFCLASS(MilkFoam, 5);
DEFCLASS(Water, 1);
#endif  // SUB_INGREDIENTS_H