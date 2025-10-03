#pragma once

namespace big
{

	struct ImageDimensions
	{
		int x, y;
		ImageDimensions() { x = y = 0; }
		ImageDimensions(int _x, int _y) { x = _x; y = _y; }
	};

}