#ifndef EUREXMODULE_H
#define EUREXMODULE_H

#include <memory>
#include <iostream>

#include "types.h"
#include "emlServer.h"

class EurexModule : public IExecModule
{
private:
  std::unique_ptr<IEmlServer> mEmlServer;
  IRiskChecker* mRiskChecker;
  IExecModuleOrderHandler* mExecModuleOrderHandler;

  // never called directly, only by framework
  void SendInsertToMarket(IEmlServer&, int volume, double price, bool side)
  {
    std::cout << volume << "@" << price << " " << side << std::endl;
  }

  // IExecModule
  void Initialise(IExecModuleOrderHandler* execModuleOrderHandler, IRiskChecker* riskChecker) override
  {
    mRiskChecker = riskChecker;
    mExecModuleOrderHandler = execModuleOrderHandler;
  }

  // IExecModule
  void InsertOrder(int volume, double price, int tag, bool side) override
  {
    // order looks good, ask framework to check (expect callback on success)
    if (!mRiskChecker->CheckInsertOrder(volume, price, tag, side))
    {
      // order insert failed
      mExecModuleOrderHandler->OnOrderError(tag);
    }
  }

public:

  EurexModule()
  {
    // set up handlers
    auto insertFunction = [this](IEmlServer& caller, int volume, double price, bool side)
    {
      SendInsertToMarket(caller, volume, price, side);
    };

    // create eml server
    mEmlServer.reset(new EmlServer<decltype(insertFunction)>(*this, insertFunction));
  }

  // IExecModule
  IEmlServer& GetEmlServer() override
  {
    return *mEmlServer.get();
  }

};

#endif // EUREXMODULE_H
