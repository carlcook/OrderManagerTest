#ifndef ORDERSERVER_H
#define ORDERSERVER_H

#include <map>

#include "types.h"

// some comments:
// * Get rid of all the interfaces
// * Maybe have one handler per order operation type, stored as a static template type per method?

template<typename InsertHandler>
class OrderServer : private IOrderExecutor, private IMarketModuleResponseHandler, public IOrderServer
{
private:
  IMarketModule& mMarketModule;
  std::map<int, bool> mOrderTagToCallbacks;
  InsertHandler mOrderInsertHandler;
  AccessKey mAccessKey;

  // IOrderExecutor
  bool AttemptInsertOrder(const InsertArgs& args, int tag) override;

  // IMarketModuleOrderHandler
  void OnOrderError(int tag) override;

  // IOrderServer
  void InsertOrder(int volume, double price, int tag, bool side);

  // useful private methods
  bool CheckLimits(int volume, double price);
  void VerifyCallback(int tag);

public:
  OrderServer(IMarketModule& marketModule, InsertHandler handler);
};

// pull in the template implementation
#include "orderServer.cc"

#endif // ORDERSERVER_H
