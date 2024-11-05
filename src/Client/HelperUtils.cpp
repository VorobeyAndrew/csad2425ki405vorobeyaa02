#include "HelperUtils.h"
#include <iostream>

std::string HelperUtils::toLowerCase(std::string& text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
	return text;
}