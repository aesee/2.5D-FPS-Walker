#include "Colors.h"

const COLORREF Colors::Black = RGB(0, 0, 0);

const COLORREF Colors::Blue = RGB(255, 0, 0);

const COLORREF Colors::MediumBlue = RGB(205, 0, 0);

const COLORREF Colors::DarkBlue = RGB(139, 0, 0);

const COLORREF Colors::Navy = RGB(128, 0, 0);

const COLORREF Colors::MidnightBlue = RGB(112, 25, 25);

COLORREF Colors::Grey(INT32 Saturation)
{
	return RGB(22 * Saturation, 22 * Saturation, 22 * Saturation);
}
