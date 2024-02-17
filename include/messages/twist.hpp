#ifndef MESSAGES_TWIST_HPP
#define MESSAGES_TWIST_HPP

#include "mros_json.hpp"

namespace Messages {

struct Twist2d {
        double dx, dy, dtheta;
       
        static void toJson(Json& json, const Twist2d& twist) {
            json["dx"] = Json::Value(twist.dx);
            json["dy"] = Json::Value(twist.dy);
            json["dtheta"] = Json::Value(twist.dtheta);
        }

        static void fromJson(Twist2d &twist, const Json& json) {
            twist.dx = json["dx"].get<double>();
            twist.dy = json["dy"].get<double>();
            twist.dtheta = json["dtheta"].get<double>();
        }
    };
}
#endif