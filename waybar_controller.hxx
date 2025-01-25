#ifndef WAYBAR_CONTROLLER_HXX
#define WAYBAR_CONTROLLER_HXX

#include "controller.hxx"

class WaybarController : public Controller {
  public:
    WaybarController(nlohmann::json* json);
    void apply() override;
};

#endif
