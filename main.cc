#include <memory>
#include <functional>
#include <map>

#include "eurexModule.h"

int main()
{
  EurexModule eurexModule;
  auto* execModule = (IExecModule*)&eurexModule;
  auto& emlServer = execModule->GetEmlServer();
  emlServer.InsertOrder(10, 12.2, 1, true);
  emlServer.InsertOrder(10, 12.2, 2, true);
  emlServer.InsertOrder(10, 12.2, 3, true);
  emlServer.InsertOrder(10, 12.2, 4, true);
}
