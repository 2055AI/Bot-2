#pragma once
#include "lib16868C/devices/abstractEncoder.hpp"
#include "lib16868C/util/math.hpp"
#include "okapi/api.hpp"

namespace lib16868C {
class OpticalEncoder : private okapi::ADIEncoder, public AbstractEncoder {
	public:
		OpticalEncoder(const OpticalEncoder& enc);
		OpticalEncoder(int portTop, int portBottom, bool reverse);
		OpticalEncoder(int smartPort, int portTop, int portBottom, bool reverse);

		double get() override;
		void resetZero() override;
		double getVelocity() override;
	
	private:
		double prevTicks = 0;
		uint prevTime = 0;
};
} // namespace lib16868C