#include "Data/Reaction.hpp"

void ECellEngine::Data::Reaction::AddProduct(Operand* _sp)
{
    products.push_back(_sp->GetID());
    onProductDestroySubTokens.push_back(std::move(_sp->onDestroy += std::bind(&ECellEngine::Data::Reaction::OnProductDestroy, this, std::placeholders::_1)));
}

void ECellEngine::Data::Reaction::AddReactant(Operand* _sp)
{
    reactants.push_back(_sp->GetID());
    onReactantDestroySubTokens.push_back(std::move(_sp->onDestroy += std::bind(&ECellEngine::Data::Reaction::OnReactantDestroy, this, std::placeholders::_1)));
}

void ECellEngine::Data::Reaction::OnProductDestroy(Operand* _sp)
{
    int idx = 0;
    while (idx < products.size() && products[idx] != _sp->GetID())
    {
        ++idx;
    }

    if (idx < products.size())
    {
        products.erase(products.begin() + idx);
        onProductDestroySubTokens.erase(onProductDestroySubTokens.begin() + idx);
    }
    else
    {
        ECellEngine::Logging::Logger::LogError("Reaction (%s) OnProductDestroy: Product (ID: %llu) not found in the list of products.", GetName(), _sp->GetID());
    }
    
}

void ECellEngine::Data::Reaction::OnReactantDestroy(Operand* _sp)
{
    int idx = 0;
    while (idx < reactants.size() && reactants[idx] != _sp->GetID())
    {
        ++idx;
    }

    if (idx < reactants.size())
    {
        reactants.erase(reactants.begin() + idx);
        onReactantDestroySubTokens.erase(onReactantDestroySubTokens.begin() + idx);
    }
    else
    {
        ECellEngine::Logging::Logger::LogError("Reaction (%s) OnReactantDestroy: Reactant (ID: %llu) not found in the list of reactants.", GetName(), _sp->GetID());
    }
}