// include/messages/pose.hpp
#ifndef MESSAGES_POSE_HPP
#define MESSAGES_POSE_HPP

#include "mros_json.hpp"

namespace Messages {

struct Pose2d {
    double x, y, theta;
    static void toJson(Json &json, const Pose2d &pose) {
        json["x"] = pose.x;
        json["y"] = pose.y;
        json["theta"] = pose.theta;
        
    }

   
    static void fromJson(Pose2d &pose, const Json& json) {
        pose.x = json["x"].get<double>();
        pose.y = json["y"].get<double>();
        pose.theta = json["theta"].get<double>();
      
    }
};

} // namespace Messages

#endif // MESSAGES_POSE_HPP