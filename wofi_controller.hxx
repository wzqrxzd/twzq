#ifndef WOFI_CONTROLLER_HXX
#define WOFI_CONTROLLER_HXX

#include "controller.hxx"

class WofiController : public Controller {
  public:
    WofiController(const nlohmann::json* json);
    void apply() override;
};

#endif
