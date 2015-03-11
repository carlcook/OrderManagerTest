#include <memory>
#include <functional>
#include <map>

#include "exampleModule.h"

int main()
{
  // create the example module
  ExampleModule exampleModule;

  // reduce it to its base type pointer
  auto* execModule = static_cast<IMarketModule*>(&exampleModule);

  // get a reference to the order server
  auto& OrderServer = execModule->GetOrderServer();

  // insert an order (just to check that we don't crash)
  OrderServer.InsertOrder(10, 12.2, 1, true);
}
