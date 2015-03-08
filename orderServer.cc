#ifdef ORDERSERVER_H

template <typename InsertHandler>
OrderServer<InsertHandler>::OrderServer(IMarketModule& marketModule, InsertHandler insertHandler)
        : mMarketModule(marketModule),
          mOrderInsertHandler(insertHandler)
{
  // pass the module the required objects
  mMarketModule.Initialise(this, this);
}

template <typename InsertHandler>
bool OrderServer<InsertHandler>::AttemptInsertOrder(InsertArgs args, int tag)
{
  // record that we received a callback for this tag
  mOrderTagToCallbacks[tag] = true;

  // do the limit checks
  if (CheckLimits(args.mVolume, args.mPrice))
  {
    // call the module's inserter (using the only instance of the access key)
    mOrderInsertHandler(mAccessKey, args);
    return true;
  }
  return false;
}

template <typename InsertArgs>
void OrderServer<InsertArgs>::OnOrderError(int tag)
{
  std::cout << tag << ": order error" << std::endl;
}

template <typename InsertArgs>
bool OrderServer<InsertArgs>::CheckLimits(int volume, double price)
{
  return volume < 1000 && price < 100;
}

template <typename InsertArgs>
void OrderServer<InsertArgs>::VerifyCallback(int tag)
{
  // we should have seen a call to the risk check, if not, bail
  if (!mOrderTagToCallbacks[tag])
    throw "Risk callback did not happen";
}

template <typename InsertArgs>
void OrderServer<InsertArgs>::InsertOrder(int volume, double price, int tag, bool side)
{
  // ask the module to insert the order, resulting in a risk callback
  mMarketModule.InsertOrder(volume, price, tag, side);

  // verify that the callback happened
  VerifyCallback(tag);

  // TODO what about calls to CheckOrderInsert that are unsolicited?
}

#endif
