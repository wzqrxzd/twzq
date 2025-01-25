#include "waybar_controller.hxx"
#include <fstream>
#include <regex>

WaybarController::WaybarController(nlohmann::json* json)
{
  if (json == nullptr)
    throw std::invalid_argument("json: pointer == nullptr");

  if (!json->contains("waybar"))
    throw std::invalid_argument("json: waybar cfg not exists");

  if (!(*json)["waybar"].contains("path"))
    throw std::invalid_argument("json: waybar_path not exists");

  if (!(*json)["waybar"].contains("color"))
    throw std::invalid_argument("json: waybar color not exists");

  config = json;
}

void WaybarController::apply()
{
  std::ifstream ifile((*config)["waybar"]["path"].get<std::string>());
  if (!ifile.is_open())
    throw std::invalid_argument("waybar: path not valid");

  std::string configData((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
  ifile.close();

  std::regex pattern_music(R"((#mpris\s*\{[^}]*color:\s*)#[0-9A-Fa-f]{6})");
  std::regex pattern_workspaces(R"((#workspaces\s*button\s*\{[^}]*color:\s*)#[0-9A-Fa-f]{6})");
  std::regex pattern_workspaces_active(R"((#workspaces\s*button\.active\s*\{[^}]*color:\s*)#[0-9A-Fa-f]{6})");
  std::regex pattern_clock(R"((#clock\s*\{[^}]*color:\s*)#[0-9A-Fa-f]{6})");
  std::regex pattern_current_window(R"((#window\s*\{[^}]*color:\s*)#[0-9A-Fa-f]{6})");
  std::regex pattern_network(R"((#network\s*\{[^}]*color:\s*)#[0-9A-Fa-f]{6})");

  configData = std::regex_replace(configData, pattern_music, "$1" + (*config)["waybar"]["color"].get<std::string>());
  configData = std::regex_replace(configData, pattern_workspaces, "$1" + (*config)["waybar"]["color"].get<std::string>());
  configData = std::regex_replace(configData, pattern_workspaces_active, "$1" + (*config)["waybar"]["color"].get<std::string>());
  configData = std::regex_replace(configData, pattern_clock, "$1" + (*config)["waybar"]["color"].get<std::string>());
  configData = std::regex_replace(configData, pattern_current_window, "$1" + (*config)["waybar"]["color"].get<std::string>());
  configData = std::regex_replace(configData, pattern_network, "$1" + (*config)["waybar"]["color"].get<std::string>());

  std::ofstream ofile((*config)["waybar"]["path"].get<std::string>());
  if (!ofile.is_open())
    throw std::invalid_argument("waybar: path not valid");

  ofile << configData;
  ofile.close();
}
