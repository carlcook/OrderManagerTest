#ifdef EMLSERVER_H

template <typename InsertHandler>
EmlServer<InsertHandler>::EmlServer(IExecModule& execModule, InsertHandler handler)
        : mExecModule(execModule),
          mOrderInsertHandler(handler)
{
  // pass the module the required objects
  mExecModule.Initialise(this, this);
}

template <typename InsertHandler>
bool EmlServer<InsertHandler>::CheckInsertOrder(InsertArgs args, int tag)
{
  // record that we received a callback for this tag
  mTagToCallbacks[tag] = true;

  // do the limit checks
  if (CheckLimits(args.volume, args.price))
  {
    // call the module's inserter
    mOrderInsertHandler(mKey, args);
    return true;
  }
  return false;
}

template <typename InsertArgs>
void EmlServer<InsertArgs>::OnOrderError(int tag)
{
  std::cout << tag << ": order error" << std::endl;
}

template <typename InsertArgs>
bool EmlServer<InsertArgs>::CheckLimits(int volume, double price)
{
  return volume < 1000 && price < 100;
}

template <typename InsertArgs>
void EmlServer<InsertArgs>::CheckCallback(int tag)
{
  if (!mTagToCallbacks[tag])
    throw "Risk callback did not happen";
}

template <typename InsertArgs>
void EmlServer<InsertArgs>::InsertOrder(int volume, double price, int tag, bool side)
{
  // ask the module to insert the order, resulting in a risk callback
  mExecModule.InsertOrder(volume, price, tag, side);

  // verify that the callback happened
  CheckCallback(tag);
}

#endif
