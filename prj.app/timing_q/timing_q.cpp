#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

void slow_motion()
{
    static int a[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    // Generate Γ(13) == 12! permutations:
    while (std::ranges::next_permutation(a).found) {}
}

int main()
{
    using namespace std::literals; // enables literal suffixes, e.g. 24h, 1ms, 1s.

    const std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();

    const std::time_t t_c = std::chrono::system_clock::to_time_t(now - 24h);
    std::cout << "24 hours ago, the time was "
              << std::put_time(std::localtime(&t_c), "%F %T.\n") << std::flush;

    const std::chrono::time_point<std::chrono::steady_clock> start =
            std::chrono::steady_clock::now();

    std::cout << "Different clocks are not comparable: \n"
                 "  System time: " << now.time_since_epoch() << "\n"
                                                                "  Steady time: " << start.time_since_epoch() << '\n';

    slow_motion();

    const auto end = std::chrono::steady_clock::now();
    std::cout
            << "Slow calculations took "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start) << " ≈ "
            << (end - start) / 1ms << "ms ≈ " // almost equivalent form of the above, but
            << (end - start) / 1s << "s.\n";  // using milliseconds and seconds accordingly
}