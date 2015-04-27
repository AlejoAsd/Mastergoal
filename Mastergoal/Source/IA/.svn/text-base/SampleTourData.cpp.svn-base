#include "stdafx.h"
#include "SampleTourData.h"

namespace AI
{
	SampleTourData::SampleTourData(int size) : GAEvalData()
	{		
		this->size = size;
		this->beats = new GAGenome*[size]; 
		this->draws = new GAGenome*[size];
		this->loses = new GAGenome*[size];
		this->nBeats = 0;
		this->nDraws = 0;
		this->nLoses = 0;
	}

	SampleTourData::SampleTourData(const SampleTourData& data)
	{
		this->beats = NULL;
		this->draws = NULL;
		this->loses = NULL;
		copy(data);	
	}

	SampleTourData::~SampleTourData()
	{		
		if (this->beats)
		{
			delete[] beats;
			beats = NULL;
		}
		if (this->draws)
		{
			delete[] draws;
			draws = NULL;
		}
		if (this->loses)
		{
			delete[] loses;
			loses = NULL;
		}
	}

	SampleTourData* SampleTourData::clone() const
	{
		return new SampleTourData(*this);
	}

	void SampleTourData::copy(const GAEvalData &evalData)
	{
		SampleTourData& data = (SampleTourData&)evalData;
		this->size = data.size;

		if (!this->beats)
			this->beats = new GAGenome*[size];
		if (!this->draws)
			this->draws = new GAGenome*[size];
		if (!this->loses)
			this->loses = new GAGenome*[size];
		this->nBeats = data.nBeats;
		this->nDraws = data.nDraws;
		this->nLoses = data.nLoses;

		for (int i = 0; i < size; i++)
		{
			this->beats[i] = data.beats[i];
			this->draws[i] = data.draws[i];
			this->loses[i] = data.loses[i];
		}
	}

	SampleTourData& SampleTourData::operator=(const SampleTourData& data) 
	{ 
		copy(data); return *this; 
	}
}
