#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H

template<typename T>
struct OutputWriter {
    std::recursive_mutex control_change_mutex;
    std::shared_ptr<CaptionOutputControl<T>> control;

    void clear();

    void set_control(const std::shared_ptr<CaptionOutputControl<T>> &set_control);

    bool enqueue(const CaptionOutput &output);
};

#endif //OUTPUTWRITER_H