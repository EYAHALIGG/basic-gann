#include "ann.h"
#include <iostream>

#define xy(x,y,w) ((x) + (y) * (w))

ann::ann(unsigned _inum, unsigned _hnum, unsigned _onum, float(*_func)(float))
{
	inum = _inum;
	hnum = _hnum;
	onum = _onum;
	func = _func;
	is = (float*)malloc(inum*sizeof(*is));
	hs = (float*)malloc(hnum*sizeof(*hs));
	os = (float*)malloc(onum*sizeof(*os));
	i2h = (float*)malloc(inum*hnum*sizeof(*i2h));
	h2o = (float*)malloc(hnum*onum*sizeof(*h2o));
	randomWeights();
}

ann::~ann()
{
	free(is);
	free(hs);
	free(os);
	free(i2h);
	free(h2o);
}

void ann::feedForward(float * inputs)
{
	memcpy(is, inputs, inum*sizeof(*is));
	float sum;
	for (unsigned k = 0; k < hnum; k++)
	{
		sum = 0;
		for (unsigned i = 0; i < inum; i++)
			sum += i2h[xy(i, k, inum)] * is[i];
		hs[k] = func(sum);
	}
	for (unsigned k = 0; k < onum; k++)
	{
		sum = 0;
		for (unsigned i = 0; i < hnum; i++)
			sum += h2o[xy(i, k, hnum)] * hs[i];
		os[k] = func(sum);
	}
}

void ann::randomWeights()
{
	unsigned i2hnum = inum*hnum;
	unsigned h2onum = hnum*onum;
	for (unsigned i = 0; i < i2hnum; i++) { i2h[i] = (float)rand() / RAND_MAX; }
	for (unsigned i = 0; i < h2onum; i++) { h2o[i] = (float)rand() / RAND_MAX; }
}

void ann::report()
{
	std::cout << "inum: " << inum << std::endl;
	std::cout << "hnum: " << hnum << std::endl;
	std::cout << "onum: " << onum << std::endl;
	std::cout << "is:" << std::endl;
	for (unsigned i = 0; i < inum; i++) { std::cout << is[i] << std::endl; }	
	std::cout << "hs:" << std::endl;
	for (unsigned i = 0; i < hnum; i++) { std::cout << hs[i] << std::endl; }
	std::cout << "os:" << std::endl;
	for (unsigned i = 0; i < onum; i++) { std::cout << os[i] << std::endl; }
	unsigned i2hnum = inum*hnum;
	unsigned h2onum = hnum*onum;
	std::cout << "i2h:" << std::endl;
	for (unsigned i = 0; i < i2hnum; i++) { std::cout << i2h[i] << std::endl; }
	std::cout << "h2o:" << std::endl;
	for (unsigned i = 0; i < h2onum; i++) { std::cout << h2o[i] << std::endl; }
}
