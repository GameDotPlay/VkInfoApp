#pragma once

#include <optional>

struct QueueFamilyIndicies
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value();
    }
};