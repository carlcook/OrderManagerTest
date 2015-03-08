#ifndef EUREXMODULE_H
#define EUREXMODULE_H

#include <memory>
#include <iostream>

#include "types.h"
#include "orderServer.h"

class ExampleModule : public IMarketModule
{
private:
  // we own the order server
  std::unique_ptr<IOrderServer> mOrderServer;

  // required interface instances
  IOrderExecutor* mOrderExecutor;
  IMarketModuleResponseHandler* mMarketModuleResponseHandler;

  // IExecModule
  void Initialise(IMarketModuleResponseHandler*, IOrderExecutor*) override;
  void InsertOrder(int volume, double price, int tag, bool side) override;
  IOrderServer& GetOrderServer() override;

  // Handles for order operations. Can only be called by framework,
  // as Key is impossible to instantiate
  void SendInsertToMarket(AccessKey, InsertArgs args);

public:
  ExampleModule();

// give SetupHandlers method private access (makes life easier)
template <typename Module> friend void SetupOrderHandlers(Module*);
};

#endif // EUREXMODULE_H
