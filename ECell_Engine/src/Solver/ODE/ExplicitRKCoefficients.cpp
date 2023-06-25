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

