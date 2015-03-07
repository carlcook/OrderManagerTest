#ifndef EUREXMODULE_H
#define EUREXMODULE_H

#include "types.h"
#include "emlServer.h"

class EurexModule : public IExecModule
{
private:
  std::unique_ptr<IEmlServer> mEmlServer;
  IRiskChecker* mRiskChecker;
  IExecModuleOrderHandler* mExecModuleOrderHandler;

  // never called directly, only by framework
  void SendInsertToMarket(int volume, double price, bool side)
  {
    // TCP send
  }

public:

  EurexModule()
  {
    auto fn = [this](int volume, double price, bool side) { SendInsertToMarket(volume, price, side);};
    auto b = new EmlServer<decltype(fn)>(*this, fn);
    mEmlServer.reset(b);
  }

  IEmlServer& GetEmlServer() override
  {
    return *mEmlServer.get();
  }

  void Initialise(IExecModuleOrderHandler* execModuleOrderHandler, IRiskChecker* riskChecker) override
  {
    mRiskChecker = riskChecker;
    mExecModuleOrderHandler =execModuleOrderHandler;
    mExecModuleOrderHandler->RegisterOrderInserter([this](int volume, double price, bool side) { SendInsertToMarket(volume, price, side);});
  }

  void InsertOrder(int volume, double price, int tag, bool side) override
  {
    // order looks good, ask framework to check (expect callback on success)
    if (!mRiskChecker->CheckInsertOrder(volume, price, tag, side))
    {
      mExecModuleOrderHandler->OnOrderError(tag);
    }
  }
};

#endif // EUREXMODULE_H
