#include "wofi_controller.hxx"
#include <fstream>
#include <regex>

WofiController::WofiController(nlohmann::json* json)
{
  if (json == nullptr)
    throw std::invalid_argument("json: pointer == nullptr");

  if (!json->contains("wofi"))
    throw std::invalid_argument("json: wofi cfg not exists");

  if (!(*json)["wofi"].contains("path"))
    throw std::invalid_argument("json wofi_path not exists");

  config = json;
}

void WofiController::apply()
{
  std::ifstream ifile((*config)["wofi"]["path"].get<std::string>());
  if (!ifile.is_open())
    throw std::invalid_argument("wofi: path not valid");

  std::string configData((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
  ifile.close();

  std::regex pattern_selected(R"((#entry:selected\s*\{\s*background-color:\s*)#[0-9A-Fa-f]{6})");
  configData = std::regex_replace(configData, pattern_selected, "$1" + (*config)["wofi"]["color"].get<std::string>());

  std::ofstream ofile((*config)["wofi"]["path"].get<std::string>());
  if (!ofile.is_open())
    throw std::invalid_argument("wofi: path not valid");

  ofile << configData;
  ofile.close();


}
