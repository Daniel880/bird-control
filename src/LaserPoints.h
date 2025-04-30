#ifndef LASER_POINTS_H
#define LASER_POINTS_H

#define LIPKA_DROGA
// #define LIPKA_BERNAD

struct LaserPoint {
  long x;
  long y;
  long time;
};

constexpr const size_t MAX_LASER_POINTS = 50;

#if defined(LIPKA_DROGA)
// "LIPKA_DROGA" pattern
const LaserPoint laserPoints[MAX_LASER_POINTS] = {
    {100, 100, 1000}, // Point 1
    {500, 100, 1000}, // Point 2
    {500, 500, 1000}, // Point 3
    {100, 500, 1000}, // Point 4
    {100, 100, 1000}, // Return to start
    {0, 0, 0},        // End marker (time=0)
                      // Remaining entries initialized to zero by default
};

#elif defined(LIPKA_BERNAD)
// "LIPKA_BERNAD" pattern
const LaserPoint laserPoints[MAX_LASER_POINTS] = {
    {200, 200, 800}, // Point 1
    {600, 200, 800}, // Point 2
    {600, 600, 800}, // Point 3
    {200, 600, 800}, // Point 4
    {400, 400, 800}, // Center point
    {200, 200, 800}, // Return to start
    {0, 0, 0},       // End marker (time=0)
                     // Remaining entries initialized to zero by default
};

#else
// Default pattern if none selected
#warning                                                                       \
    "No laser pattern selected! Define either LIPKA_DROGA or LIPKA_BERNAD in config.h"
const LaserPoint laserPoints[MAX_LASER_POINTS] = {};
#endif

#endif // LASER_POINTS_H