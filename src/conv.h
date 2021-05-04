#pragma once

enum class convert_direction
{
    FROM_UTF8_TO_LATIN2,
    FROM_LATIN2_TO_UTF8,
};

void convert_from_utf8_to_latin2(const std::string& from, std::string& to);
void convert_from_latin2_to_utf8(const std::string& from, std::string& to);
