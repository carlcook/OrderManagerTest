#include <memory>
#include <functional>
#include <map>

#include "eurexModule.h"

int main()
{
	std::unique_ptr<EurexModule> eurexModule(new EurexModule());
	auto& emlServer = eurexModule->GetEmlServer();
	emlServer.InsertOrder(10, 12.2, 1, true);
}

// TODO can we turn this into templates instead of lambdas?
// TODO order class
// TODO token to check that RG has been called from module
// TODO token to check that we have been called by EML server callback?
// TODO variadic template arguments
// TODO use std::move instead of copies
