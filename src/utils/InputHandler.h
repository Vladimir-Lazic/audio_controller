#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "AudioDefinitions.h"
#include "Utils.h"

#include <optional>
#include <iostream>

class InputHandler {
public:
    std::optional<AudioTask> process(const std::string& str) const;
};

#endif // INPUT_HANDLER_H
