#pragma once
#include <cstdint>
#include <chrono>

namespace ExCore {
    class Time {
    public:
        Time() : _clock(), _timeP(), _time(), _frameNum(), _delta() {
            reset();
        }

        void reset() {
            _frameNum = 0;
            _delta = 0;
            _time = 0;
            _timeP = _clock.now();
        }

        void tick() {
            auto prev = _timeP;
            _timeP = _clock.now();

            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(prev - _timeP).count();
            _delta = ms / 10000.0;

            _time += _delta;
            _frameNum++;
        }

        template<typename P = double>
        P getDeltaTime(const uint8_t space) const { return P(_delta); }

        template<typename P = double>
        P getTime(const uint8_t space) const { return P(_time); }

    private:
        uint64_t _frameNum;
        std::chrono::high_resolution_clock _clock;
        double _delta;
        double _time;
        std::chrono::steady_clock::time_point _timeP;
    };
}