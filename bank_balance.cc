// Compile with: g++ -O3 bank_balance.cc -o bank_balance
// Created to solve this mathematical puzzle:
//  https://youtube.com/watch?v=ILrqPpLpwpE
// To solve that puzzle exactly, run like so:
//  ./bank_balance 1000000

// Program for solving the following problem:
//  - deposit 'a' units on day 1
//  - deposit 'b' units on day 2
//  - day 3, and every subsequent day, is the sum of the values of the previous
//    two days.
//  - find the smallest 'a', 'b' such that a target sum is reached as *slowly*
//    as possible, while still being reached exactly.
//
// Consider the progression of this sum symbolically:
//  day 1: 1a
//  day 2:      1b
//  day 3: 1a + 1b
//  day 4: 1a + 2b
//  day 5: 2a + 3b
//  day 6: 3a + 5b
//  day 7: 5a + 8b
//  day 8: 8a + 13b
// Notice that the coefficients follow the fibonacci sequence! However, the
// 'a' coefficient sequence is one behind the 'b' coefficient sequence, because
// it started with {1, 0, 1} instead of simply {0, 1}.
//
// To be formal, every day the sum has the following form:
//  target_sum = a*fib(n-1) + b*fib(n)
// where n increases by 1 each day.
// We're looking for the solution that maximizes n.
//
// This algorithm works by doing the following:
//  Start with the largest possible n: the n such that fib(n-1) <= target_sum
//  for each n, try every possible a value, and solve for the corresponding
//  b value. If it's an integer, we're done.
//
// Overall, this is a very efficient algorithm: when total_sum is
// one hundred trillion, this takes 0.21 seconds to find the solution on my
// machine.

#include <iostream>
#include <string>
#include <vector>

// Compute all fibonacci numbers <= max.
std::vector<int64_t> GetFibonacci(int64_t max) {
  std::vector<int64_t> ret;
  if (max == 0) return {0};

  ret.push_back(0);
  ret.push_back(1);
  while (ret.back() <= max) {
    ret.push_back(ret[ret.size()-1] + ret[ret.size()-2]);
  }
  ret.pop_back();

  return ret;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <target_sum>" << std::endl;
    return 1;
  }

  int64_t target_sum = std::stoll(argv[1]);

  // Precompute all the fibonacci numbers up to the target sum.
  std::vector<int64_t> fibs = GetFibonacci(target_sum);

  // Special case: if target_sum exactly equals a fibonacci number, the optimal
  // result is to deposit 1 unit on day 1 and 0 units on day 2. However, the
  // below algorithm starts with fib(n-1) for the first day and fib(n) for the
  // second, and moves downward; therefore, fib(n) is never considered for the
  // first day and we will miss the optimal result. So, handle that here.
  if (fibs.back() == target_sum) {
    std::cout << "days: " << fibs.size() << "\n";
    std::cout << "first deposit:  1\n";
    std::cout << "second deposit: 0\n";
    return 0;
  }

  // 'index' is the day number.
  for (int64_t index = fibs.size() - 2; index >= 0; --index) {
    int64_t fib1 = fibs[index];
    int64_t fib2 = fibs[index+1];

    // i is the deposit on the first day, j is the deposit on the second.
    for (int64_t i = 0; i*fib1 <= target_sum; ++i) {
      int64_t second_deposit_accumulated = target_sum - (i*fib1);
      if (second_deposit_accumulated % fib2 == 0) {
        int64_t j = second_deposit_accumulated / fib2;
        std::cout << "days: " << index << "\n";
        std::cout << "first deposit:  " << i << "\n";
        std::cout << "second deposit: " << j << "\n";
        return 0;
      }
    }
  }

  std::cout << "No solution found." << std::endl;

  return 0;
}
