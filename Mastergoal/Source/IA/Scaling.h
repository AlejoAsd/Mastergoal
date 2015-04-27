#ifndef SCALING_H
#define SCALING_H

#include <GA/GAScaling.h>

class PruebaScaling : public GAScalingScheme
{
public:
	GADefineIdentity("PruebaScaling", 210);

	PruebaScaling(){}
	PruebaScaling(const PruebaScaling & arg){copy(arg);}
	PruebaScaling & operator=(const GAScalingScheme & arg)
	{ copy(arg); return *this; }
	virtual ~PruebaScaling() {}
	virtual GAScalingScheme * clone() const 
	{ return new PruebaScaling(*this); }
	virtual void evaluate(const GAPopulation & p);
	virtual void copy(const GAScalingScheme & arg){	}

};

void 
PruebaScaling::evaluate(const GAPopulation & p) {
	float f;
	for(int i=0; i<p.size(); i++)
	{
		f = p.individual(i).score();
		if(f < 0) f = 0;
			p.individual(i).fitness(f);
	}
}



#endif
