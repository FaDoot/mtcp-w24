
#include "mros_json.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

template<class T>
inline constexpr bool always_false_v = false;
// Json::operator[]
Json::Value& Json::operator[](const std::string& key) {
    return elts_[key];
}

// Json::operator[] const overload
const Json::Value& Json::operator[](const std::string& key) const {
    auto it = elts_.find(key);
    if (it == elts_.end()) {
        throw std::out_of_range("Key not found in JSON object: " + key);
    }
    return it->second;
}

// Json::operator==
bool Json::operator==(const Json& other) const {
    return elts_ == other.elts_;
}

// Json::toString
std::string Json::toString() const {
    std::string result = "{";
    for (auto iter = elts_.cbegin(); iter != elts_.cend(); ++iter) {
        if (iter != elts_.cbegin()) {
            result += ",";
        }
        result += '"' + iter->first + "\": " + stringify(iter->second); // Serialize each element
    }
    result += "}";
    return result;
}

// Helper for Json::toString converts Json::Value to string based on its underlying type.
std::string Json::stringify(const Json::Value& value) {
    std::ostringstream oss;
    std::visit([&oss](auto&& arg) {
        using T = std::decay_t<decltype(arg)>; // Decay type to get rid of references, const, etc.

        // Serialize integer and floating point numbers directly.
        if constexpr (std::is_same_v<T, int>) {
            oss << arg;
        } else if constexpr (std::is_same_v<T, double>) {
        oss << std::fixed << std::setprecision(6) << arg; // Double, fixed-point notation with 6 decimal places
    } 
        // Serialize strings with quotes around them.
        else if constexpr (std::is_same_v<T, std::string>) {
            oss << '"' << arg << '"';
        }
        // Serialize vectors with recursion for each contained value.
        else if constexpr (std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<double>> || std::is_same_v<T, std::vector<std::string>>) {
            oss << '[';
            for (auto it = arg.begin(); it != arg.end(); ++it) {
                if (it != arg.begin()) {
                    oss << ", ";
                }
                oss << Json::stringify(Json::Value(*it)); // Convert each vector element to Json::Value and recurs.
            }
            oss << ']';
        } 
        // Handle unrecognized types, a compile-time check to alert if a new type is added without handling.
        else {
            static_assert(always_false_v<T>, "Unhandled type in Json::stringify!");
        }
    }, value.data_);
    //cout << oss.str() << endl;
    return oss.str();
}

Json::Value Json::getType(const std::string& str) {
    // This will be a very simplistic guess

    if (str.empty()) return "";  // default to empty string
    
    // Check if the string contains a quote at the beginning and end
    if (str.front() == '"' && str.back() == '"') {
        // Remove the surrounding quotes and return the string
        return str.substr(1, str.size() - 2);
    }

    // Check if the string represents a number
    try {
        if (str.find('.') != std::string::npos) {
            // Contains a decimal point, treat as double
            return std::stod(str);
        } else {
            // Treat as integer
            return std::stoi(str);
        }
    } catch (const std::invalid_argument&) {
        // If std::stoi or std::stod fails because of invalid argument, assume it's a string
        return str.substr(2, str.size() - 3);
    } catch (const std::out_of_range&) {
        // If the number is out of range, it could also be a string
        return str.substr(2, str.size() - 3);
    }

    // If none of the above, unable to determine the type of the JSON value
    throw std::runtime_error("Unable to determine the type of the JSON value");
}
// Json::fromString
Json Json::fromString(const std::string& str) {
    Json json;
    //cout <<   "this is the string2"<<str << endl;
    for (size_t start = 0, end; start < str.size(); start = end + 1) {
            start = str.find('"', start);  // key starts after "
            if (start == std::string::npos) break;  // No more keys
            end = str.find('"', ++start);  // key ends before "
            if (end == std::string::npos) throw std::runtime_error("Unterminated string for key");
            std::string key = str.substr(start, end - start);

            start = str.find(':', end) + 1;   // value starts after :
            if (str[start] == '"') {  // starts with quotation mark; assume string value
                ++start;  // Skip initial quote
                end = str.find('"', start);
                if (end == std::string::npos) throw std::runtime_error("Unterminated string for value");
                std::string value = str.substr(start, end - start);
                json[key] = value;
                end++;  // Skip final quote
            } else {  // assume numerical value
                bool found_comma = false;
                for (end = start; end < str.size(); ++end) {
                    if (str[end] == ',' || str[end] == '}') {
                        found_comma = true;
                        break;
                    }
                }
                if (!found_comma) throw std::runtime_error("Expected comma or closing bracket");
                std::string valueStr = str.substr(start, end - start);
                json[key] = getType(valueStr);
            }
        }
    return json;
}