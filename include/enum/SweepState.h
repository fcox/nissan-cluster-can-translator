#ifndef SWEEP_STATE_H
#define SWEEP_STATE_H

enum class SweepState {
    Rising,
    Falling,
    PausedTop,
    PausedBottom,
    Stopped
};

#endif