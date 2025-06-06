#include "behaviortree_cpp/basic_types.h"
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <charconv>

namespace BT
{
template <>
std::string toStr<NodeStatus>(NodeStatus status)
{
  switch (status)
  {
    case NodeStatus::SUCCESS:
      return "SUCCESS";
    case NodeStatus::FAILURE:
      return "FAILURE";
    case NodeStatus::RUNNING:
      return "RUNNING";
    case NodeStatus::IDLE:
      return "IDLE";
    case NodeStatus::SKIPPED:
      return "SKIPPED";
  }
  return "";
}

std::string toStr(std::string value)
{
  return value;
}

std::string toStr(NodeStatus status, bool colored)
{
  if (!colored)
  {
    return toStr(status);
  }
  else
  {
    switch (status)
    {
      case NodeStatus::SUCCESS:
        return "\x1b[32m"
               "SUCCESS"
               "\x1b[0m";   // RED
      case NodeStatus::FAILURE:
        return "\x1b[31m"
               "FAILURE"
               "\x1b[0m";   // GREEN
      case NodeStatus::RUNNING:
        return "\x1b[33m"
               "RUNNING"
               "\x1b[0m";   // YELLOW
      case NodeStatus::SKIPPED:
      case NodeStatus::IDLE:
        return "\x1b[36m"
               "IDLE"
               "\x1b[0m";   // CYAN
    }
  }
  return "Undefined";
}

template <>
std::string toStr<PortDirection>(PortDirection direction)
{
  switch (direction)
  {
    case PortDirection::INPUT:
      return "Input";
    case PortDirection::OUTPUT:
      return "Output";
    case PortDirection::INOUT:
      return "InOut";
  }
  return "InOut";
}

template <>
std::string toStr<NodeType>(NodeType type)
{
  switch (type)
  {
    case NodeType::ACTION:
      return "Action";
    case NodeType::CONDITION:
      return "Condition";
    case NodeType::DECORATOR:
      return "Decorator";
    case NodeType::CONTROL:
      return "Control";
    case NodeType::SUBTREE:
      return "SubTree";
    default:
      return "Undefined";
  }
}

template <>
std::string convertFromString<std::string>(StringView str)
{
  return std::string(str.data(), str.size());
}

template <>
int convertFromString<int>(StringView str)
{
  int result = 0;
  std::from_chars(str.data(), str.data() + str.size(), result);
  return result;
}

template <>
long convertFromString<long>(StringView str)
{
  long result = 0;
  std::from_chars(str.data(), str.data() + str.size(), result);
  return result;
}

template <>
unsigned convertFromString<unsigned>(StringView str)
{
  unsigned result = 0;
  std::from_chars(str.data(), str.data() + str.size(), result);
  return result;
}

template <>
unsigned long convertFromString<unsigned long>(StringView str)
{
  unsigned long result = 0;
  std::from_chars(str.data(), str.data() + str.size(), result);
  return result;
}

template <>
double convertFromString<double>(StringView str)
{
  // see issue #120
  // http://quick-bench.com/DWaXRWnxtxvwIMvZy2DxVPEKJnE

  std::string old_locale = setlocale(LC_NUMERIC, nullptr);
  setlocale(LC_NUMERIC, "C");
  double val = std::stod(str.data());
  setlocale(LC_NUMERIC, old_locale.c_str());
  return val;
}

template <>
float convertFromString<float>(StringView str)
{
  std::string old_locale = setlocale(LC_NUMERIC, nullptr);
  setlocale(LC_NUMERIC, "C");
  float val = std::stof(str.data());
  setlocale(LC_NUMERIC, old_locale.c_str());
  return val;
}

template <>
std::vector<int> convertFromString<std::vector<int>>(StringView str)
{
  auto parts = splitString(str, ';');
  std::vector<int> output;
  output.reserve(parts.size());
  for (const StringView& part : parts)
  {
    output.push_back(convertFromString<int>(part));
  }
  return output;
}

template <>
std::vector<double> convertFromString<std::vector<double>>(StringView str)
{
  auto parts = splitString(str, ';');
  std::vector<double> output;
  output.reserve(parts.size());
  for (const StringView& part : parts)
  {
    output.push_back(convertFromString<double>(part));
  }
  return output;
}

template <>
bool convertFromString<bool>(StringView str)
{
  if (str.size() == 1)
  {
    if (str[0] == '0')
    {
      return false;
    }
    if (str[0] == '1')
    {
      return true;
    }
  }
  else if (str.size() == 4)
  {
    if (str == "true" || str == "TRUE" || str == "True")
    {
      return true;
    }
  }
  else if (str.size() == 5)
  {
    if (str == "false" || str == "FALSE" || str == "False")
    {
      return false;
    }
  }
  throw RuntimeError("convertFromString(): invalid bool conversion");
}

template <>
NodeStatus convertFromString<NodeStatus>(StringView str)
{
  if (str == "IDLE")
    return NodeStatus::IDLE;
  if (str == "RUNNING")
    return NodeStatus::RUNNING;
  if (str == "SUCCESS")
    return NodeStatus::SUCCESS;
  if (str == "FAILURE")
    return NodeStatus::FAILURE;
  throw RuntimeError(std::string("Cannot convert this to NodeStatus: ") +
                     static_cast<std::string>(str));
}

template <>
NodeType convertFromString<NodeType>(StringView str)
{
  if (str == "Action")
    return NodeType::ACTION;
  if (str == "Condition")
    return NodeType::CONDITION;
  if (str == "Control")
    return NodeType::CONTROL;
  if (str == "Decorator")
    return NodeType::DECORATOR;
  if (str == "SubTree")
    return NodeType::SUBTREE;
  return NodeType::UNDEFINED;
}

template <>
PortDirection convertFromString<PortDirection>(StringView str)
{
  if (str == "Input" || str == "INPUT")
    return PortDirection::INPUT;
  if (str == "Output" || str == "OUTPUT")
    return PortDirection::OUTPUT;
  return PortDirection::INOUT;
}

std::ostream& operator<<(std::ostream& os, const NodeType& type)
{
  os << toStr(type);
  return os;
}

std::ostream& operator<<(std::ostream& os, const NodeStatus& status)
{
  os << toStr(status);
  return os;
}

std::ostream& operator<<(std::ostream& os, const PortDirection& type)
{
  os << toStr(type);
  return os;
}

std::vector<StringView> splitString(const StringView& strToSplit, char delimeter)
{
  std::vector<StringView> splitted_strings;
  splitted_strings.reserve(4);

  size_t pos = 0;
  while (pos < strToSplit.size())
  {
    size_t new_pos = strToSplit.find_first_of(delimeter, pos);
    if (new_pos == std::string::npos)
    {
      new_pos = strToSplit.size();
    }
    StringView sv = {&strToSplit.data()[pos], new_pos - pos};
    splitted_strings.push_back(sv);
    pos = new_pos + 1;
  }
  return splitted_strings;
}

PortDirection PortInfo::direction() const
{
  return _type;
}

const std::type_index& PortInfo::type() const
{
  return _type_info;
}

Any PortInfo::parseString(const char* str) const
{
  if (_converter)
  {
    return _converter(str);
  }
  return {};
}

Any PortInfo::parseString(const std::string& str) const
{
  if (_converter)
  {
    return _converter(str);
  }
  return {};
}

void PortInfo::setDescription(StringView description)
{
  description_ = static_cast<std::string>(description);
}

void PortInfo::setDefaultValue(StringView default_value_as_string)
{
  default_value_ = static_cast<std::string>(default_value_as_string);
}

const std::string& PortInfo::description() const
{
  return description_;
}

const std::string& PortInfo::defaultValue() const
{
  return default_value_;
}

bool IsAllowedPortName(StringView str)
{
  if (str.empty())
  {
    return false;
  }
  const char first_char = str.data()[0];
  if (!std::isalpha(first_char))
  {
    return false;
  }
  if (str == "name" || str == "ID")
  {
    return false;
  }
  return true;
}

}   // namespace BT
