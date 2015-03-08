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

  // insert some orders (just to check that we don't crash)
  OrderServer.InsertOrder(10, 12.2, 1, true);
  OrderServer.InsertOrder(11, 13.2, 2, true);
  OrderServer.InsertOrder(12, 14.2, 3, true);
  OrderServer.InsertOrder(13, 15.2, 4, true);
}
