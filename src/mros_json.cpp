
#include "mros_json.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

template<class T>
inline constexpr bool always_false_v = false;

Json::Value& Json::operator[](const std::string& key) {
    return elts_[key];
}

const Json::Value& Json::operator[](const std::string& key) const {
    auto it = elts_.find(key);
    if (it == elts_.end()) {
        throw std::out_of_range("Key not found in JSON object: " + key);
    }
    return it->second;
}


bool Json::operator==(const Json& other) const {
    return elts_ == other.elts_;
}


std::string Json::toString() const {
    std::string result = "{";
    for (auto iter = elts_.cbegin(); iter != elts_.cend(); ++iter) {
        if (iter != elts_.cbegin()) {
            result += ",";
        }
        result += '"' + iter->first + "\": " + stringify(iter->second); 
    }
    result += "}";
    return result;
}

std::string Json::stringify(const Json::Value& value) {
    std::ostringstream oss;
    std::visit([&oss](auto&& arg) {
        using T = std::decay_t<decltype(arg)>; 

     
        if constexpr (std::is_same_v<T, int>) {
            oss << arg;
        } else if constexpr (std::is_same_v<T, double>) {
        oss << std::fixed << std::setprecision(6) << arg; 
    } 
        else if constexpr (std::is_same_v<T, std::string>) {
            oss << '"' << arg << '"';
        }
        else if constexpr (std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<double>> || std::is_same_v<T, std::vector<std::string>>) {
            oss << '[';
            for (auto it = arg.begin(); it != arg.end(); ++it) {
                if (it != arg.begin()) {
                    oss << ", ";
                }
                oss << Json::stringify(Json::Value(*it)); 
            }
            oss << ']';
        } 
        
        else {
            static_assert(always_false_v<T>, "Unhandled type in Json::stringify!");
        }
    }, value.data_);
    //cout << oss.str() << endl;
    return oss.str();
}

Json::Value Json::getType(const std::string& str) {
    

    if (str.empty()) return "";  
    
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }

    try {
        if (str.find('.') != std::string::npos) {
            return std::stod(str);
        } else {
           
            return std::stoi(str);
        }
    } catch (const std::invalid_argument&) {
        return str.substr(2, str.size() - 3);
    } catch (const std::out_of_range&) {
        return str.substr(2, str.size() - 3);
    }

    throw std::runtime_error("Unable to determine the type of the JSON value");
}
Json Json::fromString(const std::string& str) {
    Json json;
    //cout <<   "this is the string2"<<str << endl;
    for (size_t start = 0, end; start < str.size(); start = end + 1) {
            start = str.find('"', start);  
            if (start == std::string::npos) 
                break;  
            end = str.find('"', ++start);  
            if (end == std::string::npos) throw std::runtime_error("Unterminated string for key");
            std::string key = str.substr(start, end - start);

            start = str.find(':', end) + 1;  
            if (str[start] == '"') {  
                ++start;  
                end = str.find('"', start);
                if (end == std::string::npos) throw std::runtime_error("Unterminated string for value");
                std::string value = str.substr(start, end - start);
                json[key] = value;
                end++;  
            } else { 
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