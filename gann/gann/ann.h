#pragma once
class ann
{
public:
	unsigned inum, hnum, onum;
	float *is, *hs, *os;
	float *i2h, *h2o;
	float(*func)(float);
	ann(unsigned _inum, unsigned _hnum, unsigned _onum, float(*_func)(float));
	~ann();
	void feedForward(float* inputs);
	void randomWeights();
	void report();
};