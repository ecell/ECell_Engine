#pragma once

namespace ECellEngine::Solvers::ODE
{
	/*!
	@brief Structure to hold the coefficients of an explicit Runge-Kutta method.
	*/
	struct ExplicitRKCoefficients
	{
		/*!
		@brief The order of the method.
		*/
		unsigned short order = 0;

		/*!
		@brief The order of the polynom used for dense output.
		*/
		unsigned short denseOutputOrder = 0;

		/*!
		@brief The order of the less precise estimation.
		@details This is used for error estimation.
		*/
		unsigned short estimationsMinOrder = 0;

		/*!
		@brief The number of stages of the method to reach the order.
		*/
		unsigned short stages = 0;

		/*!
		@brief The number of a_{i,j} parameters for each stage.
		@details This is the result of the Sum_{j=1}^{stages} (j-1)
		*/
		const unsigned short NBAS_PER_STAGE[6] = { 0, 1, 3, 6, 10 };//to be extended for higher order methods

		/*!
		@brief The coefficients k_{i} for each equation.
		@details The array is of size nbEquations * stages. The definition of
		 this array mimmics a matrix of @a nbEquations rows and @a stages columns.
		*/
		float* ks = nullptr;
		
		/*!
		@biref The coefficients a_{i,j} of all stages and all ks.
		@details The array is of size ::NBAS_PER_STAGE[stages] with the coefficient
				for stage 2, then the 2 coefficient of stage 3, then the 3 coefficients
				of stage 4, then the 6 coefficients of stage 5, etc.
		*/
		float* as = nullptr;

		/*!
		@brief The coefficients b_{i} for all stages of the high order solution.
		*/
		float* bs = nullptr;

		/*!
		@brief The coefficients b_{i} for all stages of the low order solution.
		@details This is used for error estimation.
		*/
		float* bs2 = nullptr;

		/*!
		@brief The coefficients of the polynomial b_{j}(theta) for the dense output.
		@details It is of size stages * polynomial order. The definition of
		 this array mimmics a matrix of @a stages rows and @a polynomial order columns.
		*/
		float* bsp = nullptr;

		/*!
		@brief The coefficients c_{i} for all stages.
		@remarks Added for exhaustivness, but currently not used. Might be removed.
		*/
		float* cs = nullptr;

		ExplicitRKCoefficients() noexcept = default;

		~ExplicitRKCoefficients()
		{
			delete[] ks;
			delete[] as;
			delete[] bs;
			delete[] bs2;
			delete[] bsp;
			delete[] cs;
		}

		/*!
		@brief Computes the sum of the a_{stage,j}*k_{j} coefficients for j in [1, s-1].
		@details The minimal value of _stage is 2, the maximal value is ::stages. This
				is valid for a unique equation informed by @p _eqIdx. In particuler,
				it allows select the appropriate k_{j} in ::ks.
		@param _eqIdx The index of the equation for which we compute a coefficient
				k.
		@param _stage The stage of the coefficient k we are computing.
		*/
		float ComputekSumForStage(const unsigned short _eqIdx, const unsigned short _stage) noexcept;

		/*!
		@brief Computes the sum of the bs_{i}*k_{i} coefficients for i in [1, s].
		@details This is valid for a unique equation informed by @p _eqIdx.
				In particuler, it allows select the appropriate k_{i} in ::ks.
		@param _eqIdx The index of the equation for which we compute a coefficient
				k.
		@param bs The array of bs_{i} coefficients. Either ::bs or ::bs2.
		*/
		float ComputekSumForSolution(const unsigned short _eqIdx, const float bs[]) noexcept;

		/*!
		@brief Handles the clean up of the various arrays.
		@details Includes ks, as, bs, bs2 and cs.
		*/
		void Delete() noexcept;

		/*!
		@brief Sets the coefficients to the classical RK4 method.
		*/
		void SetToClassicRK4(const unsigned short _systemSize) noexcept;

		/*!
		@brief Sets the coefficients to the Dormand-Prince 5(4) method (DOPRI54).
		*/
		void SetToDormandPrince54(const unsigned short _systemSize) noexcept;

		/*!
		@brief Sets the coefficients to the Merson4 method.
		@details This is a 4th order method with 5 stages. It supports
				 error estimation and dense output by direct Hermite
				 interpolation.
		*/
		void SetToMerson4(const unsigned short _systemSize) noexcept;
	};
}