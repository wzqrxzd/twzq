#include "hyprland_controller.hxx"
#include <fstream>
#include <regex>

HyprlandController::HyprlandController(nlohmann::json* json)
{
  if (json == nullptr)
    throw std::invalid_argument("json: pointer == nullptr");

  if (!json->contains("hyprland"))
    throw std::invalid_argument("json: hyprland cfg not exists");

  if (!(*json)["hyprland"].contains("path"))
    throw std::invalid_argument("json: hyprland_path not exists");

  if (!(*json)["hyprland"].contains("active_color"))
    throw std::invalid_argument("json: hyprland active color not exists");

  if (!(*json)["hyprland"].contains("inactive_color"))
    throw std::invalid_argument("json: hyprland inactive color not exists");

  config = json;
}

void HyprlandController::apply()
{
  std::ifstream ifile((*config)["hyprland"]["path"].get<std::string>());
  if (!ifile.is_open())
    throw std::invalid_argument("hyprland: path not valid");

  std::string configData((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());

  ifile.close();

  std::regex activeBorderRegex(R"(col\.active_border = .+)");
  std::regex inactiveBorderRegex(R"(col\.inactive_border = .+)");

  configData = std::regex_replace(configData, activeBorderRegex, "col.active_border = " + (*config)["hyprland"]["active_color"].get<std::string>());
  configData = std::regex_replace(configData, inactiveBorderRegex, "col.inactive_border = " + (*config)["hyprland"]["inactive_color"].get<std::string>());

  std::ofstream ofile((*config)["hyprland"]["path"].get<std::string>());
  if (!ofile.is_open())
    throw std::invalid_argument("hyprland: path not valid");

  ofile << configData;
  ofile.close();
}
