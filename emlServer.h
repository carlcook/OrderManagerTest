#ifndef EMLSERVER_H
#define EMLSERVER_H

#include "types.h"

template<typename H>
class EmlServer : private IRiskChecker, private IExecModuleOrderHandler, public IEmlServer
{
private:
  IExecModule& mExecModule;
  std::function<void(int, double, bool)> mOrderInserter;
  std::map<int, bool> mCallbacks;
  H mHandler;

  bool CheckLimits(int volume, double price, int tag, bool side)
  {
    return true;
  }

  // IRiskChecker
  bool CheckInsertOrder(int volume, double price, int tag, bool side) override
  {
    if (CheckLimits(volume, price, tag, side))
    {
      mHandler(volume, price, side);
      mOrderInserter(volume, price, side);
      return true;
    }
    return false;
  }

  // IExecModuleOrderHandler
  void OnOrderError(int tag) override
  {
    // TODO log etc
    tag = 0;
  }

  void RegisterOrderInserter(std::function<void (int, double, bool)> orderInserter)
  {
    mOrderInserter = orderInserter;
  }

  void CheckCallback(int tag)
  {
    if (!mCallbacks[tag])
      throw "Risk callback did not happen";
  }

public:
  EmlServer(IExecModule& execModule, H handler)
    : mExecModule(execModule),
      mHandler(handler)
  {
    mExecModule.Initialise(this, this);
  }

  // IEmlServer
  void InsertOrder(int volume, double price, int tag, bool side)
  {
    mCallbacks[tag] = true;
    mExecModule.InsertOrder(volume, price, tag, side);
    CheckCallback(tag);
  }
};

#endif // EMLSERVER_H
