#include <vector>
#include <iostream>

std::vector<std::string> split(std::string s, std::string delimiter) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;

  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s = s.substr(pos + delimiter.length());
  }

  return tokens;
}
