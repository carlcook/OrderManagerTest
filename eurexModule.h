#ifndef EUREXMODULE_H
#define EUREXMODULE_H

#include <memory>
#include <iostream>

#include "types.h"
#include "emlServer.h"

class EurexModule : public IExecModule
{
private:
  // we own the EML server
  std::unique_ptr<IEmlServer> mEmlServer;

  // required interface instances
  IOrderChecker* mOrderChecker;
  IExecModuleOrderHandler* mExecModuleOrderHandler;

  // IExecModule
  void Initialise(IExecModuleOrderHandler* execModuleOrderHandler, IOrderChecker* orderChecker) override;
  void InsertOrder(int volume, double price, int tag, bool side) override;
  IEmlServer& GetEmlServer() override;

  // Handles for order operations. Can only be called by framework,
  // as Key is impossible to instantiate
  void SendInsertToMarket(AccessKey, InsertArgs args);

public:
  EurexModule();

// give SetupHandlers method private access (makes life easier)
template <typename Module> friend void SetupHandlers(Module*);
};

#endif // EUREXMODULE_H
