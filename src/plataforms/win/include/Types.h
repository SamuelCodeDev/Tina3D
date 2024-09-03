#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <string_view>

namespace Tina
{
    using int8 = __int8;
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;

	using uint8 = unsigned __int8;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;

    using string = std::string;
	using string_view = std::string_view;

    union Rect
	{
		struct
		{
			int32 left;
			int32 top;
			int32 right;
			int32 bottom;
		};

		struct
		{
			int32 x1;
			int32 y1;
			int32 x2;
			int32 y2;
		};

		int32 coord[4];
	};

	struct ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};
}

#endif