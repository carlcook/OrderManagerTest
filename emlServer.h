#ifndef EMLSERVER_H
#define EMLSERVER_H

#include <iostream>

#include "types.h"

template<typename InsertHandler>
class EmlServer : private IRiskChecker, private IExecModuleOrderHandler, public IEmlServer
{
private:
  IExecModule& mExecModule;
  std::map<int, bool> mTagToCallbacks;
  InsertHandler mOrderInsertHandler;

  // IRiskChecker
  bool CheckInsertOrder(int volume, double price, int tag, bool side) override
  {
    // record that we received a callback for this tag
    mTagToCallbacks[tag] = true;

    // do the limit checks
    if (CheckLimits(volume, price))
    {
      // now call the module's inserter
      // TODO what about passing extra data?
      mOrderInsertHandler(*this, volume, price, side);
      return true;
    }
    return false;
  }

  // IExecModuleOrderHandler
  void OnOrderError(int tag) override
  {
    std::cout << tag << ": order error" << std::endl;
  }

  // useful private methods
  bool CheckLimits(int volume, double price)
  {
    return volume < 1000 && price < 100;
  }

  void CheckCallback(int tag)
  {
    if (!mTagToCallbacks[tag])
      throw "Risk callback did not happen";
  }

public:
  EmlServer(IExecModule& execModule, InsertHandler handler)
    : mExecModule(execModule),
      mOrderInsertHandler(handler)
  {
    // pass the module the required objects
    mExecModule.Initialise(this, this);
  }

  // IEmlServer
  void InsertOrder(int volume, double price, int tag, bool side)
  {
    // ask the module to insert the order, resulting in a risk callback
    mExecModule.InsertOrder(volume, price, tag, side);

    // verify that the callback happened
    CheckCallback(tag);
  }
};

#endif // EMLSERVER_H
