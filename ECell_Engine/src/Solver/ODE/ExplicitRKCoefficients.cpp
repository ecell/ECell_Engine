#include "Solver/ODE/ExplicitRKCoefficients.hpp"

float ECellEngine::Solvers::ODE::ExplicitRKCoefficients::ComputekSumForStage(const unsigned short _eqIdx, const unsigned short _stage) noexcept
{
	float res = 0;
	for (unsigned short s = 0; s < _stage - 1; ++s)
	{
		res += as[NBAS_PER_STAGE[_stage - 2] + s] * ks[_eqIdx+s];
	}
	return res;
}

float ECellEngine::Solvers::ODE::ExplicitRKCoefficients::ComputekSumForSolution(const unsigned short _eqIdx, float bs[]) noexcept
{
	float res = 0;
	for (unsigned short s = 0; s < stages; ++s)
	{
		res += bs[s] * ks[_eqIdx+s];
	}
	return res;
}

void ECellEngine::Solvers::ODE::ExplicitRKCoefficients::Delete() noexcept
{
	delete[] ks;
	delete[] as;
	delete[] bs;
	delete[] bs2;
	delete[] cs;
}

void ECellEngine::Solvers::ODE::ExplicitRKCoefficients::SetToClassicRK4(const unsigned short _systemSize) noexcept
{
	Delete();
	order = 4;
	estimationsMinOrder = 4;//does not matter
	stages = 4;
	ks = new float[_systemSize * 4];
	as = new float[6] { 0.5f, 0.f, 0.5f, 0.f, 0.f, 1.0f };
	bs = new float[4] { 1.f / 6.f, 1.f / 3.f, 1.f / 3.f, 1.f / 6.f };
	bs2 = new float[4] { 0.f, 0.f, 0.f, 0.f };
	cs = new float[4] { 0.f, 0.5f, 0.5f, 1.0f };
}

void ECellEngine::Solvers::ODE::ExplicitRKCoefficients::SetToDormandPrince54(const unsigned short _systemSize) noexcept
{
	Delete();
	order = 5;
	estimationsMinOrder = 4;
	stages = 7;
	ks = new float[_systemSize * 4];
	as = new float[21] {
		0.2f,
		3.f / 40.f, 9.f / 40.f,
		44.f / 45.f, -56.f / 15.f, 32.f/9.f,
		19372.f / 6561.f, -25360.f / 2187.f, 64448.f / 6561.f, -212.f / 729.f,
		9017.f / 3168.f, -355.f / 33.f, 46732.f / 5247.f, 49.f / 176.f, -5103.f / 18656.f,
		35.f / 384.f, 0.f, 500.f / 1113.f, 125.f / 192.f, -2187.f / 6784.f, 11.f / 84.f};
	bs = new float[7] { 35.f / 384.f, 0.f, 500.f / 1113.f, 125.f / 192.f, -2187.f / 6784.f, 11.f / 84.f, 0.f };
	bs2 = new float[7] { 5179.f / 57600.f, 0.f, 7571.f / 16695.f, 393.f / 640.f, -92097.f / 339200.f, 187.f / 2100.f, 1.f/40.f };
	cs = new float[7] { 0.2f, 0.3f, 0.8f, 8.f/9.f, 1.f, 1.f };
}

void ECellEngine::Solvers::ODE::ExplicitRKCoefficients::SetToMerson4(const unsigned short _systemSize) noexcept
{
	Delete();
	order = 4;
	estimationsMinOrder = 3;
	stages = 5;
	ks = new float[_systemSize * 5];
	as = new float[10] {1.f / 3.f, 1.f / 6.f, 1.f / 6.f, 0.125f, 0.f, 0.375f, 0.5f, 0.f, -1.5f, 2.f};
	bs = new float[5] {1.f / 6.f, 0.f, 0.f, 2.f / 3.f, 1.f / 6.f};
	bs2 = new float[5] {0.1f, 0.f, 0.3f, 0.4f, 0.2f};
	cs = new float[5] {0.f, 1.f / 3.f, 1.f / 3.f, 0.5f, 1.0f};
}

