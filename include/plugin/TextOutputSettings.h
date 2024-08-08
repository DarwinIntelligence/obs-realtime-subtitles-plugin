#ifndef TEXTOUTPUTSETTINGS_H
#define TEXTOUTPUTSETTINGS_H

using namespace std;

#include <QObject>

struct TextOutputSettings {
    bool enabled;
    string text_source_name;
    uint line_length;
    uint line_count;
    bool insert_punctuation;
    CapitalizationType capitalization;

    void print(const char *line_prefix = "") const {
        printf("%sTextOutputSettings\n", line_prefix);
        printf("%s  enabled: %d\n", line_prefix, enabled);
        printf("%s  text_source_name: %s\n", line_prefix, text_source_name.c_str());
        printf("%s  line_length: %d\n", line_prefix, line_length);
        printf("%s  line_count: %d\n", line_prefix, line_count);
        printf("%s  insert_punctuation: %d\n", line_prefix, insert_punctuation);
        printf("%s  capitalization: %d\n", line_prefix, capitalization);
    }

    bool isValid() const {
        return !text_source_name.empty() && line_count && line_length;
    }

    bool isValidEnabled() const {
        return enabled && isValid();
    }

    bool operator==(const TextOutputSettings &rhs) const {
        return enabled == rhs.enabled &&
               text_source_name == rhs.text_source_name &&
               line_length == rhs.line_length &&
               line_count == rhs.line_count &&
               insert_punctuation == rhs.insert_punctuation &&
               capitalization == rhs.capitalization;
    }

    bool operator!=(const TextOutputSettings &rhs) const {
        return !(rhs == *this);
    }
};

#endif //TEXTOUTPUTSETTINGS_H
