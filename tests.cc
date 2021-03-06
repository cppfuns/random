#include "Random.h"
using namespace eosblox;

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

static inline void assertSeedEq(const Random &gen, const uint64_t seed)
{
  assert(gen.seed() == seed);
}

static inline void assertNextEq(Random &gen, const uint64_t value)
{
  assert(gen.next() == value);
}

static inline bool doubleEqual(const double a, const double b)
{
  constexpr double epsilon = 0.0000001;
  return std::abs(a - b) <= ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
}

static inline void assertNextDoubleEq(Random &gen, const double value)
{
  assert(doubleEqual(gen.nextDouble(), value));
}

static inline void assertNextInRangeEq(Random &gen, const uint64_t min, const uint64_t max,
                                       const uint64_t value)
{
  assert(gen.nextInRange(min, max) == value);
}

template <typename Container>
static inline void assertShuffleEq(Random &gen, Container &data, const Container &value)
{
  gen.shuffle(data);
  assert(data == value);
}

void testSeedAccumulation()
{
  Random gen(1);
  assertSeedEq(gen, 1);

  // Integer.
  gen.accumSeed(1);
  assertSeedEq(gen, 11295943761408656531LLU);

  // Initializer list.
  gen.accumSeedRange({1, 2, 3});
  assertSeedEq(gen, 18083823154816788002LLU);

  // General ranges.
  gen.accumSeedRange(std::string("Hello, World!"));
  assertSeedEq(gen, 7880214006551886600LLU);

  const std::string str("Hello, World!");
  gen.accumSeedRange(str);
  assertSeedEq(gen, 11498166657420254716LLU);

  gen.accumSeedRange(std::vector<int>{1, 2, 3});
  assertSeedEq(gen, 4175012130207295590LLU);

  // Arrays.
  const int arr[3]{1, 2, 3};
  gen.accumSeedArray(arr);
  assertSeedEq(gen, 4326221722869819695LLU);
}

void testNext()
{
  {
    Random gen(1);
    assertNextEq(gen, 17243114145050246623LLU);
    assertNextEq(gen, 5516405404208600209LLU);
    assertNextEq(gen, 4897608421861629041LLU);
    assertNextEq(gen, 8669024630748878125LLU);
    assertNextEq(gen, 13562114715725036114LLU);
  }
  {
    Random gen(111970);
    assertNextEq(gen, 8987405948927848382LLU);
    assertNextEq(gen, 4621043010224079033LLU);
    assertNextEq(gen, 15658672667220659145LLU);
    assertNextEq(gen, 14814678247671488241LLU);
    assertNextEq(gen, 849456367364724273LLU);
  }
  {
    Random gen(1);
    gen.accumSeedRange(std::string("Hello, World!"));
    assertNextEq(gen, 14607242223934345243LLU);
    assertNextEq(gen, 12308690566605765243LLU);
    assertNextEq(gen, 6585550730876598598LLU);
    assertNextEq(gen, 5507355692446769180LLU);
    assertNextEq(gen, 16383900445848122088LLU);
  }
}

void testNextDouble()
{
  Random gen(42);
  assertNextDoubleEq(gen, 8.485680e-02);
  assertNextDoubleEq(gen, 3.902738e-01);
  assertNextDoubleEq(gen, 2.926740e-01);
  assertNextDoubleEq(gen, 7.088247e-01);
  assertNextDoubleEq(gen, 8.789325e-01);
}

void testNextInRange()
{
  Random gen(42);
  assertNextInRangeEq(gen, 10, 20, 10);
  assertNextInRangeEq(gen, 10, 20, 13);
  assertNextInRangeEq(gen, 10, 20, 12);
  assertNextInRangeEq(gen, 10, 20, 17);
  assertNextInRangeEq(gen, 10, 20, 18);
}

void testShuffle()
{
  {
    Random gen(42);
    std::string str("Hello, World!");
    assertShuffleEq(gen, str, std::string("llo,lrH!do eW"));
    assertShuffleEq(gen, str, std::string("Hlr ,ldoWel!o"));
    assertShuffleEq(gen, str, std::string("l,Woordl Hle!"));
    assertShuffleEq(gen, str, std::string("dllooe,lHr !W"));
    assertShuffleEq(gen, str, std::string("oeH!d,rllWo l"));
  }
  {
    Random gen(42);
    using IVec = std::vector<int>;
    IVec vec{1, 2, 3, 4, 5};
    assertShuffleEq(gen, vec, IVec{1, 2, 5, 4, 3});
    assertShuffleEq(gen, vec, IVec{4, 5, 1, 2, 3});
    assertShuffleEq(gen, vec, IVec{1, 4, 2, 3, 5});
    assertShuffleEq(gen, vec, IVec{1, 3, 5, 4, 2});
    assertShuffleEq(gen, vec, IVec{3, 5, 4, 1, 2});
  }
}

int main(int argc, char **argv)
{
  testSeedAccumulation();
  testNext();
  testNextDouble();
  testNextInRange();
  testShuffle();
  return 0;
}
