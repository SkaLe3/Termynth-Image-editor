#pragma once

#include <cstdint>

enum class EAttribute : uint8_t
{
	None = 0,
	Bold = 1,
	Dim = 1 << 1,
	Italic = 1 << 2,
	Underline = 1 << 3,
	Blink = 1 << 4,
	Inverse = 1 << 5,
	DefaultFg = 1 << 6,
	DefaultBg = 1 << 7
};