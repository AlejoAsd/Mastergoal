#ifndef SAMPLE_POP_DATA_H
#define SAMPLE_POP_DATA_H

#include <ga/GAEvalData.h>
#include <ga/GAPopulation.h>

namespace AI
{
	class SamplePopData : public GAEvalData
	{
	public:
		SamplePopData(GAPopulation *sample);

		SamplePopData(const SamplePopData& data);

		virtual ~SamplePopData() {}

		SamplePopData& operator=(const SamplePopData& data) { copy(data); return *this; }

		virtual SamplePopData* clone() const;

		virtual void copy(const GAEvalData& data);

		inline GAPopulation* Sample() const { return sample; }

		inline GAPopulation* Sample(GAPopulation *sample) { return this->sample = sample; }

	protected:
		GAPopulation *sample;
	};
}

#endif
