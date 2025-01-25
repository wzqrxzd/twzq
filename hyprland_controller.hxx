#ifndef HYPRLAND_CONTROLLER_HXX
#define HYPRLAND_CONTROLLER_HXX

#include "controller.hxx"

class HyprlandController : public Controller {
  public:
    HyprlandController(nlohmann::json* json);
    void apply() override;
};

#endif
