#ifndef CAPTIONOUTPUT_H
#define CAPTTIONOUTPUT_H

#include <QObject>

Q_DECLARE_METATYPE(shared_ptr<OutputCaptionResult>)

struct CaptionOutput {
    shared_ptr<OutputCaptionResult> output_result;
    bool is_clearance;

    CaptionOutput(shared_ptr<OutputCaptionResult> output_result, bool is_clearance);

    CaptionOutput() : is_clearance(false) {}
};

#endif //CAPTIONOUTPUT_H
