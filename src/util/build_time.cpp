#include "./build_time.hpp"

#include <string>

static std::string formatBuildTime() {
  constexpr std::string_view date = __DATE__; // "Apr 28 2025"
  constexpr std::string_view time = __TIME__; // "18:12:03"

  // Parse month
  int month = 0;
  if (date.substr(0, 3) == "Jan")
    month = 1;
  else if (date.substr(0, 3) == "Feb")
    month = 2;
  else if (date.substr(0, 3) == "Mar")
    month = 3;
  else if (date.substr(0, 3) == "Apr")
    month = 4;
  else if (date.substr(0, 3) == "May")
    month = 5;
  else if (date.substr(0, 3) == "Jun")
    month = 6;
  else if (date.substr(0, 3) == "Jul")
    month = 7;
  else if (date.substr(0, 3) == "Aug")
    month = 8;
  else if (date.substr(0, 3) == "Sep")
    month = 9;
  else if (date.substr(0, 3) == "Oct")
    month = 10;
  else if (date.substr(0, 3) == "Nov")
    month = 11;
  else if (date.substr(0, 3) == "Dec")
    month = 12;

  // Extract day
  int day = std::stoi(std::string(date.substr(4, 2)));

  // Extract year
  int year = std::stoi(std::string(date.substr(7, 4)));

  // Build final RFC 3339 string
  char buffer[32];
  std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%sZ", year, month, day,
                time.data());
  return std::string(buffer);
}



std::string_view buildTime() {
  static std::string str = formatBuildTime();
  return str;
}

