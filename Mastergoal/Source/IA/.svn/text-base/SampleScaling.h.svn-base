#ifndef SAMPLE_SCALING_H
#define SAMPLE_SCALING_H

#include <ga/GAScaling.h>
#include <ga/GAPopulation.h>
#include "Definitions.h"

namespace AI
{
	class SampleScaling : public GAScalingScheme
	{
	public:
		GADefineIdentity("SampleScaling", GAID_SAMPLE_SCALING);

		SampleScaling() {}

		SampleScaling(const SampleScaling& arg) { copy(arg); }

		SampleScaling& operator=(const GAScalingScheme & arg) { copy(arg); return *this; }

		virtual ~SampleScaling() {}

		virtual GAScalingScheme * clone () const { return new SampleScaling(*this); }

		virtual void evaluate(const GAPopulation &p);

		virtual void copy(const GAScalingScheme& arg)
		{
			if(&arg != this && sameClass(arg))
			{
				GAScalingScheme::copy(arg);
			}
		}
	};
}

#endif
