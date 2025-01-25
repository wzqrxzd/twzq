#ifndef CONTROLLER_HXX
#define CONTROLLER_HXX

#include "json.hpp"

class Controller {
  public:
    virtual ~Controller() = default;
    virtual void apply() = 0;

  protected:
    nlohmann::json* config;
};

#endif
