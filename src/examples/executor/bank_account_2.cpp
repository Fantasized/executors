#include <experimental/executor>
#include <experimental/thread_pool>
#include <iostream>

using std::experimental::post;
using std::experimental::thread_pool;
using std::experimental::use_future;

// Traditional active object pattern.
// Member functions block until operation is finished.

class bank_account
{
  int balance_ = 0;
  mutable thread_pool pool_{1};

public:
  void deposit(int amount)
  {
    post(pool_,
      use_future([=]
        {
          balance_ += amount;
        })).get();
  }

  void withdraw(int amount)
  {
    post(pool_,
      use_future([=]
        {
          if (balance_ >= amount)
            balance_ -= amount;
        })).get();
  }

  int balance() const
  {
    return post(pool_,
      use_future([=]
        {
          return balance_;
        })).get();
  }
};

int main()
{
  bank_account acct;
  acct.deposit(20);
  acct.withdraw(10);
  std::cout << "balance = " << acct.balance() << "\n";
}
