#ifndef EMLSERVER_H
#define EMLSERVER_H

#include <map>

#include "types.h"

template<typename InsertHandler>
class EmlServer : private IOrderChecker, private IExecModuleOrderHandler, public IEmlServer
{
private:
  IExecModule& mExecModule;
  std::map<int, bool> mTagToCallbacks;
  InsertHandler mOrderInsertHandler;
  AccessKey mKey;

  // IOrderChecker
  bool CheckInsertOrder(InsertArgs args, int tag) override;

  // IExecModuleOrderHandler
  void OnOrderError(int tag) override;

  // IEmlServer
  void InsertOrder(int volume, double price, int tag, bool side);

  // useful private methods
  bool CheckLimits(int volume, double price);
  void CheckCallback(int tag);

public:
  EmlServer(IExecModule& execModule, InsertHandler handler);
};

#include "emlServer.cc"

#endif // EMLSERVER_H
