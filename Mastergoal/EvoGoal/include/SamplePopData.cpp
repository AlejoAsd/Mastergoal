#include "stdafx.h"
#include "SamplePopData.h"

namespace AI
{
	SamplePopData::SamplePopData(GAPopulation *sample) : GAEvalData()
	{
		this->sample = sample;
	}

	SamplePopData::SamplePopData(const SamplePopData& data) : GAEvalData(data)
	{
		copy(data);	
	}

	SamplePopData* SamplePopData::clone() const
	{
		return new SamplePopData(*this);
	}

	void SamplePopData::copy(const GAEvalData &evalData)
	{
		SamplePopData& data = (SamplePopData&)evalData;

		this->sample = data.sample;
	}
}
