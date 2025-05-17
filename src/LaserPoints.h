#ifndef LASER_POINTS_H
#define LASER_POINTS_H

#define LIPKA_DROGA
// #define LIPKA_BERNAD

struct LaserPoint {
  long x;
  long y;
  long time;
};

constexpr const size_t MAX_LASER_POINTS = 20;

#if defined(LIPKA_DROGA)
// "LIPKA_DROGA" pattern
const LaserPoint laserPoints[MAX_LASER_POINTS] = {
    {-9470, 6860, 1000},  // Slupek
    {-8050, 8190, 1000},  // prawo gora
    {-4350, 8090, 1000},  // prawo dol
    {-8860, 8100, 1000},  // 1 gora
    {-4300, 7260, 1000},  // 1 dol
    {-12072, 7831, 1000}, // 2 gora
    {-5440, 4680, 1000},  // 2 dol, na prawo od skrzyni
    {-16770, 7391, 1000}, // 3 gora, na wprost od skrzyni
    // START LEWEJ CZESCI POLA
    {-30970, 4260, 1000}, // 3 dol, na lewo od skrzyni
    {-22500, 6890, 1000}, // 4 gora
    {-30400, 6090, 1000}, // 4 dol, na lewo od skrzyni
    {-27427, 6695, 1000}, // drzewko 2
    {-30360, 6490, 1000},
    {-27710, 6800, 1000}, // 5 gora , ostatni gora
    {-30290, 6670, 1000}, // naroznik przy sasiedzie
    {-27710, 6800, 1000}, // 5 gora , ostatni gora, jeszcze raz
    // {-28331, 6660, 1000}, // drzewko 1
    // {-27427, 6695, 1000}, // drzewko 2
    {-30360, 6490, 1000}, // lewo od skrzyni na powrocie

    {-9450, 6900, 2000}, // Point 1
    {0, 0, 0},           // End marker (time=0)
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