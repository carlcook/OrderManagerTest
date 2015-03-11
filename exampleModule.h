#ifndef EXAMPLEMODULE_H
#define EXAMPLEMODULE_H

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

  // IMarketModule
  void Initialise(IMarketModuleResponseHandler*, IOrderExecutor*) override;
  void InsertOrder(int volume, double price, int tag, bool side) override;
  IOrderServer& GetOrderServer() override;

  // Handles for order operations. Can only be called by framework,
  // as Key is impossible to instantiate
  void SendInsertToMarket(AccessKey, const InsertArgs &args);

  // Extension data for order inserts
  struct ExtendedInsertArgs : public InsertArgs
  {
    ExtendedInsertArgs(const int& volume, const double& price, const bool& side, const int& id, const std::string& message)
      : InsertArgs({volume, price, side}),
        mId(id), mMessage(message)
    {
    }

    const int& mId;
    const std::string& mMessage;
  };

public:
  ExampleModule();

// give SetupHandlers method private access (makes life easier)
template <typename Module> friend void SetupOrderHandlers(Module*);
};

#endif // EXAMPLEMODULE_H
