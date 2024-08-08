#ifndef CAPTIONOUTPUTCONTROL_H
#define CAPTIONOUTPUTCONTROL_H

#include "CaptionOutput.h"

template<typename T>
struct CaptionOutputControl {
    moodycamel::BlockingConcurrentQueue<CaptionOutput> caption_queue;
    volatile bool stop = false;
    T arg;

    CaptionOutputControl(T arg) : arg(arg) {}

    void stop_soon();

    ~CaptionOutputControl();
};

#endif //CAPTIONOUTPUTCONTROL_H
