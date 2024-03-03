#ifndef RANDOM_HXX
#define RANDOM_HXX
#include <random>

namespace Poly {

template <typename IntType = int> class Random {
	std::mt19937 gen;
	std::uniform_int_distribution<IntType> rand;

  public:
	Random(IntType a, IntType b) : rand(a, b) {}

	IntType generate() { return rand(gen); }
};

} // namespace Poly

#endif // RANDOM_HXX
