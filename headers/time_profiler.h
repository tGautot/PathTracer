#ifndef TIME_PROFILER_H
#define TIME_PROFILER_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <sys/time.h>

#define WHOLE_EXEC 0
#define SPHERE_COLLISION_PROFILE_ID 1
#define AABB_COLLISION_PROFILE_ID 2

namespace prfl{

    static std::map<int, uint64_t> times;
    static std::map<int, uint64_t> segments;
    static std::map<int, std::string> names;
    static int longestNameLen = 1;
    static int namesSize = 15;
    static int timesSize = 15;

    inline uint64_t get_time(){
        timeval tv;
        gettimeofday(&tv, 0);
        return tv.tv_usec/1000;
    }

    inline void create_profile(int profile_id){
        times[profile_id] = 0LL;
    }

    inline void create_profile(int profile_id, std::string s){
        times[profile_id] = 0LL;
        names[profile_id] = s;
    }

    inline void start_profiling_segment(int profile_id){
        segments[profile_id] = get_time();
    }

    inline void end_profiling_segment(int profile_id){
        uint64_t curr = get_time();
        times[profile_id] += curr - segments[profile_id];
        segments[profile_id] = curr;
    }
    inline void set_profile_name(int profile_id, std::string name){
        names[profile_id] = name;
    }

    inline uint64_t get_profile_time(int profile_id){
        return times[profile_id];
    }

    inline void print_profile_time(int profile_id, std::ostream& out = std::clog){
        out << get_profile_time(profile_id);
    }

    void print_full_profiler_info(std::ostream& out = std::clog){
        using std::endl;
        out << "SIMPLE PROFILER RESULTS" << endl;
        out << "╔" << std::string(namesSize, '=') << "╦" << std::string(timesSize + 4, '=') << "╗" << endl;
        for(auto it = times.begin(); it != times.end(); it++){
            int profile_id = it->first;
            std::string name;
            if(names.find(profile_id) != names.end()){
                name = names[profile_id];
            } else {
                name = std::to_string(profile_id);
            }
            int nameLen = name.length();
            nameLen = nameLen > namesSize ? namesSize : nameLen;

            std::string timeStr = std::to_string(it->second);
            int timeLen = timeStr.length();
            timeLen = timeLen > timesSize ? timesSize : timeLen;

            out << "║" << name << std::string(namesSize - nameLen, ' ') 
                << "║" << std::string(timesSize - timeLen, ' ') << timeStr << " ms ║" << endl; ; 
        }
        out << "╚" << std::string(namesSize, '=') << "╩" << std::string(timesSize + 4, '=') << "╝" << endl;
        
    }
}

#endif