#include <memory>
#include <functional>
#include <map>

#include "eurexModule.h"

int main()
{
	EurexModule eurexModule;
	auto& emlServer = eurexModule.GetEmlServer();
	emlServer.InsertOrder(10, 12.2, 1, true);
	emlServer.InsertOrder(10, 12.2, 2, true);
	emlServer.InsertOrder(10, 12.2, 3, true);
	emlServer.InsertOrder(10, 12.2, 4, true);
}

// TODO order class?
// TODO variadic template arguments? Needed?
// TODO use std::move instead of copies (or is this not required?)
