#ifndef WALLPAPER_CONTROLLER_HXX
#define WALLPAPER_CONTROLLER_HXX

#include "controller.hxx"

class WallpaperController : public Controller
{
  public:
    WallpaperController(nlohmann::json* json);
    void apply() override;
};

#endif
