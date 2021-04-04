#pragma once

namespace Lyre
{

	class IPlatform
	{
	public:
		static float GetTime();
		static float GetDeltaTimeSinceLastCall();
	};

}
