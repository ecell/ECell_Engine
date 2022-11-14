#pragma once

#include <math.h>

extern "C" {
#include "revrand.h"
}

namespace ECellEngine::Maths
{
	class ReversibleRNG
	{
	private:
		rng_state rng;

	public:

		ReversibleRNG(){}

		/*
		@brief Samples a number according to an exponential distribution of parameter @p _lambda.
		@param _lambda The parameter of the exponential distribution.
		@returns The sample as a float.
		*/
		inline const float Exponential(const float& _lambda)
		{
			return -log(random_uniform(&rng)) / _lambda;
		}

		/*
		* @brief Calls the API of revrand to initialize the RNG.
		* @param _seed The seed to use for the initialization of the RNG.
		*/
		inline void Initialize(const unsigned long& _seed)
		{
			init_state(_seed, &rng);
		}

		/*
		@brief Calls the API of revrand to reverse the RNG.
		*/
		inline void Reverse()
		{
			reverse(&rng);
		}
	};
}