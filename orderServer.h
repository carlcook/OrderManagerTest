#ifndef ORDERSERVER_H
#define ORDERSERVER_H

#include <map>

#include "types.h"

template<typename InsertHandler>
class OrderServer : private IOrderExecutor, private IMarketModuleResponseHandler, public IOrderServer
{
private:
  IMarketModule& mMarketModule;
  std::map<int, bool> mOrderTagToCallbacks;
  InsertHandler mOrderInsertHandler;
  AccessKey mAccessKey;

  // IOrderExecutor
  bool AttemptInsertOrder(InsertArgs args, int tag) override;

  // IExecModuleOrderHandler
  void OnOrderError(int tag) override;

  // IOrderServer
  void InsertOrder(int volume, double price, int tag, bool side);

  // useful private methods
  bool CheckLimits(int volume, double price);
  void VerifyCallback(int tag);

public:
  OrderServer(IMarketModule& execModule, InsertHandler handler);
};

// pull in the template implementation
#include "orderServer.cc"

#endif // ORDERSERVER_H
