// Copyright 2008, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: vladl@google.com (Vlad Losev)
//
// Tests for Google Test itself. This file verifies that the parameter
// generators objects produce correct parameter sequences and that
// Google Test runtime instantiates correct tests_big_number from those sequences.

#include "gtest/gtest.h"

#if GTEST_HAS_PARAM_TEST

# include <algorithm>
# include <iostream>
# include <list>
# include <sstream>
# include <string>
# include <vector>

// To include gtest-internal-inl.h.
# define GTEST_IMPLEMENTATION_ 1
# include "src/gtest-internal-inl.h"  // for UnitTestOptions
# undef GTEST_IMPLEMENTATION_

# include "test/gtest-param-test_test.h"

using ::std::vector;
using ::std::sort;

using ::testing::AddGlobalTestEnvironment;
using ::testing::Bool;
using ::testing::Message;
using ::testing::Range;
using ::testing::TestWithParam;
using ::testing::Values;
using ::testing::ValuesIn;

# if GTEST_HAS_COMBINE
using ::testing::Combine;
using ::std::tr1::get;
using ::std::tr1::make_tuple;
using ::std::tr1::tuple;
# endif  // GTEST_HAS_COMBINE

using ::testing::internal::ParamGenerator;
using ::testing::internal::UnitTestOptions;

// Prints a value to a string.
//
// TODO(wan@google.com): remove PrintValue() when we move matchers and
// EXPECT_THAT() from Google Mock to Google Test.  At that time, we
// can write EXPECT_THAT(x, Eq(y)) to compare two tuples x and y, as
// EXPECT_THAT() and the matchers know how to print tuples.
template <typename T>
::std::string PrintValue(const T& value) {
  ::std::stringstream stream;
  stream << value;
  return stream.str();
}

# if GTEST_HAS_COMBINE

// These overloads allow printing tuples in our tests_big_number.  We cannot
// define an operator<< for tuples, as that definition needs to be in
// the std namespace in order to be picked up by Google Test via
// Argument-Dependent Lookup, yet defining anything in the std
// namespace in non-STL code is undefined behavior.

template <typename T1, typename T2>
::std::string PrintValue(const tuple<T1, T2>& value) {
  ::std::stringstream stream;
  stream << "(" << get<0>(value) << ", " << get<1>(value) << ")";
  return stream.str();
}

template <typename T1, typename T2, typename T3>
::std::string PrintValue(const tuple<T1, T2, T3>& value) {
  ::std::stringstream stream;
  stream << "(" << get<0>(value) << ", " << get<1>(value)
         << ", "<< get<2>(value) << ")";
  return stream.str();
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9, typename T10>
::std::string PrintValue(
    const tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& value) {
  ::std::stringstream stream;
  stream << "(" << get<0>(value) << ", " << get<1>(value)
         << ", "<< get<2>(value) << ", " << get<3>(value)
         << ", "<< get<4>(value) << ", " << get<5>(value)
         << ", "<< get<6>(value) << ", " << get<7>(value)
         << ", "<< get<8>(value) << ", " << get<9>(value) << ")";
  return stream.str();
}

# endif  // GTEST_HAS_COMBINE

// Verifies that a sequence generated by the generator and accessed
// via the iterator object matches the expected one using Google Test
// assertions.
template <typename T, size_t N>
void VerifyGenerator(const ParamGenerator<T>& generator,
                     const T (&expected_values)[N]) {
  typename ParamGenerator<T>::iterator it = generator.begin();
  for (size_t i = 0; i < N; ++i) {
    ASSERT_FALSE(it == generator.end())
        << "At element " << i << " when accessing via an iterator "
        << "created with the copy constructor.\n";
    // We cannot use EXPECT_EQ() here as the values may be tuples,
    // which don't support <<.
    EXPECT_TRUE(expected_values[i] == *it)
        << "where i is " << i
        << ", expected_values[i] is " << PrintValue(expected_values[i])
        << ", *it is " << PrintValue(*it)
        << ", and 'it' is an iterator created with the copy constructor.\n";
    it++;
  }
  EXPECT_TRUE(it == generator.end())
        << "At the presumed end of sequence when accessing via an iterator "
        << "created with the copy constructor.\n";

  // Test the iterator assignment. The following lines verify that
  // the sequence accessed via an iterator initialized via the
  // assignment operator (as opposed to a copy constructor) matches
  // just the same.
  it = generator.begin();
  for (size_t i = 0; i < N; ++i) {
    ASSERT_FALSE(it == generator.end())
        << "At element " << i << " when accessing via an iterator "
        << "created with the assignment operator.\n";
    EXPECT_TRUE(expected_values[i] == *it)
        << "where i is " << i
        << ", expected_values[i] is " << PrintValue(expected_values[i])
        << ", *it is " << PrintValue(*it)
        << ", and 'it' is an iterator created with the copy constructor.\n";
    it++;
  }
  EXPECT_TRUE(it == generator.end())
        << "At the presumed end of sequence when accessing via an iterator "
        << "created with the assignment operator.\n";
}

template <typename T>
void VerifyGeneratorIsEmpty(const ParamGenerator<T>& generator) {
  typename ParamGenerator<T>::iterator it = generator.begin();
  EXPECT_TRUE(it == generator.end());

  it = generator.begin();
  EXPECT_TRUE(it == generator.end());
}

// Generator tests_big_number. They test that each of the provided generator functions
// generates an expected sequence of values. The general test pattern
// instantiates a generator using one of the generator functions,
// checks the sequence produced by the generator using its iterator API,
// and then resets the iterator back to the beginning of the sequence
// and checks the sequence again.

// Tests that iterators produced by generator functions conform to the
// ForwardIterator concept.
TEST(IteratorTest, ParamIteratorConformsToForwardIteratorConcept) {
  const ParamGenerator<int> gen = Range(0, 10);
  ParamGenerator<int>::iterator it = gen.begin();

  // Verifies that iterator initialization works as expected.
  ParamGenerator<int>::iterator it2 = it;
  EXPECT_TRUE(*it == *it2) << "Initialized iterators must point to the "
                           << "element same as its source points to";

  // Verifies that iterator assignment works as expected.
  it++;
  EXPECT_FALSE(*it == *it2);
  it2 = it;
  EXPECT_TRUE(*it == *it2) << "Assigned iterators must point to the "
                           << "element same as its source points to";

  // Verifies that prefix operator++() returns *this.
  EXPECT_EQ(&it, &(++it)) << "Result of the prefix operator++ must be "
                          << "refer to the original object";

  // Verifies that the result of the postfix operator++ points to the value
  // pointed to by the original iterator.
  int original_value = *it;  // Have to compute it outside of macro call to be
                             // unaffected by the parameter evaluation order.
  EXPECT_EQ(original_value, *(it++));

  // Verifies that prefix and postfix operator++() advance an iterator
  // all the same.
  it2 = it;
  it++;
  ++it2;
  EXPECT_TRUE(*it == *it2);
}

// Tests that Range() generates the expected sequence.
TEST(RangeTest, IntRangeWithDefaultStep) {
  const ParamGenerator<int> gen = Range(0, 3);
  const int expected_values[] = {0, 1, 2};
  VerifyGenerator(gen, expected_values);
}

// Edge case. Tests that Range() generates the single element sequence
// as expected when provided with range limits that are equal.
TEST(RangeTest, IntRangeSingleValue) {
  const ParamGenerator<int> gen = Range(0, 1);
  const int expected_values[] = {0};
  VerifyGenerator(gen, expected_values);
}

// Edge case. Tests that Range() with generates empty sequence when
// supplied with an empty range.
TEST(RangeTest, IntRangeEmpty) {
  const ParamGenerator<int> gen = Range(0, 0);
  VerifyGeneratorIsEmpty(gen);
}

// Tests that Range() with custom step (greater then one) generates
// the expected sequence.
TEST(RangeTest, IntRangeWithCustomStep) {
  const ParamGenerator<int> gen = Range(0, 9, 3);
  const int expected_values[] = {0, 3, 6};
  VerifyGenerator(gen, expected_values);
}

// Tests that Range() with custom step (greater then one) generates
// the expected sequence when the last element does not fall on the
// upper range limit. Sequences generated by Range() must not have
// elements beyond the range limits.
TEST(RangeTest, IntRangeWithCustomStepOverUpperBound) {
  const ParamGenerator<int> gen = Range(0, 4, 3);
  const int expected_values[] = {0, 3};
  VerifyGenerator(gen, expected_values);
}

// Verifies that Range works with user-defined types that define
// copy constructor, operator=(), operator+(), and operator<().
class DogAdder {
 public:
  explicit DogAdder(const char* a_value) : value_(a_value) {}
  DogAdder(const DogAdder& other) : value_(other.value_.c_str()) {}

  DogAdder operator=(const DogAdder& other) {
    if (this != &other)
      value_ = other.value_;
    return *this;
  }
  DogAdder operator+(const DogAdder& other) const {
    Message msg;
    msg << value_.c_str() << other.value_.c_str();
    return DogAdder(msg.GetString().c_str());
  }
  bool operator<(const DogAdder& other) const {
    return value_ < other.value_;
  }
  const std::string& value() const { return value_; }

 private:
  std::string value_;
};

TEST(RangeTest, WorksWithACustomType) {
  const ParamGenerator<DogAdder> gen =
      Range(DogAdder("cat"), DogAdder("catdogdog"), DogAdder("dog"));
  ParamGenerator<DogAdder>::iterator it = gen.begin();

  ASSERT_FALSE(it == gen.end());
  EXPECT_STREQ("cat", it->value().c_str());

  ASSERT_FALSE(++it == gen.end());
  EXPECT_STREQ("catdog", it->value().c_str());

  EXPECT_TRUE(++it == gen.end());
}

class IntWrapper {
 public:
  explicit IntWrapper(int a_value) : value_(a_value) {}
  IntWrapper(const IntWrapper& other) : value_(other.value_) {}

  IntWrapper operator=(const IntWrapper& other) {
    value_ = other.value_;
    return *this;
  }
  // operator+() adds a different type.
  IntWrapper operator+(int other) const { return IntWrapper(value_ + other); }
  bool operator<(const IntWrapper& other) const {
    return value_ < other.value_;
  }
  int value() const { return value_; }

 private:
  int value_;
};

TEST(RangeTest, WorksWithACustomTypeWithDifferentIncrementType) {
  const ParamGenerator<IntWrapper> gen = Range(IntWrapper(0), IntWrapper(2));
  ParamGenerator<IntWrapper>::iterator it = gen.begin();

  ASSERT_FALSE(it == gen.end());
  EXPECT_EQ(0, it->value());

  ASSERT_FALSE(++it == gen.end());
  EXPECT_EQ(1, it->value());

  EXPECT_TRUE(++it == gen.end());
}

// Tests that ValuesIn() with an array parameter generates
// the expected sequence.
TEST(ValuesInTest, ValuesInArray) {
  int array[] = {3, 5, 8};
  const ParamGenerator<int> gen = ValuesIn(array);
  VerifyGenerator(gen, array);
}

// Tests that ValuesIn() with a const array parameter generates
// the expected sequence.
TEST(ValuesInTest, ValuesInConstArray) {
  const int array[] = {3, 5, 8};
  const ParamGenerator<int> gen = ValuesIn(array);
  VerifyGenerator(gen, array);
}

// Edge case. Tests that ValuesIn() with an array parameter containing a
// single element generates the single element sequence.
TEST(ValuesInTest, ValuesInSingleElementArray) {
  int array[] = {42};
  const ParamGenerator<int> gen = ValuesIn(array);
  VerifyGenerator(gen, array);
}

// Tests that ValuesIn() generates the expected sequence for an STL
// container (vector).
TEST(ValuesInTest, ValuesInVector) {
  typedef ::std::vector<int> ContainerType;
  ContainerType values;
  values.push_back(3);
  values.push_back(5);
  values.push_back(8);
  const ParamGenerator<int> gen = ValuesIn(values);

  const int expected_values[] = {3, 5, 8};
  VerifyGenerator(gen, expected_values);
}

// Tests that ValuesIn() generates the expected sequence.
TEST(ValuesInTest, ValuesInIteratorRange) {
  typedef ::std::vector<int> ContainerType;
  ContainerType values;
  values.push_back(3);
  values.push_back(5);
  values.push_back(8);
  const ParamGenerator<int> gen = ValuesIn(values.begin(), values.end());

  const int expected_values[] = {3, 5, 8};
  VerifyGenerator(gen, expected_values);
}

// Edge case. Tests that ValuesIn() provided with an iterator range specifying a
// single value generates a single-element sequence.
TEST(ValuesInTest, ValuesInSingleElementIteratorRange) {
  typedef ::std::vector<int> ContainerType;
  ContainerType values;
  values.push_back(42);
  const ParamGenerator<int> gen = ValuesIn(values.begin(), values.end());

  const int expected_values[] = {42};
  VerifyGenerator(gen, expected_values);
}

// Edge case. Tests that ValuesIn() provided with an empty iterator range
// generates an empty sequence.
TEST(ValuesInTest, ValuesInEmptyIteratorRange) {
  typedef ::std::vector<int> ContainerType;
  ContainerType values;
  const ParamGenerator<int> gen = ValuesIn(values.begin(), values.end());

  VerifyGeneratorIsEmpty(gen);
}

// Tests that the Values() generates the expected sequence.
TEST(ValuesTest, ValuesWorks) {
  const ParamGenerator<int> gen = Values(3, 5, 8);

  const int expected_values[] = {3, 5, 8};
  VerifyGenerator(gen, expected_values);
}

// Tests that Values() generates the expected sequences from elements of
// different types convertible to ParamGenerator's parameter type.
TEST(ValuesTest, ValuesWorksForValuesOfCompatibleTypes) {
  const ParamGenerator<double> gen = Values(3, 5.0f, 8.0);

  const double expected_values[] = {3.0, 5.0, 8.0};
  VerifyGenerator(gen, expected_values);
}

TEST(ValuesTest, ValuesWorksForMaxLengthList) {
  const ParamGenerator<int> gen = Values(
      10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
      110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
      210, 220, 230, 240, 250, 260, 270, 280, 290, 300,
      310, 320, 330, 340, 350, 360, 370, 380, 390, 400,
      410, 420, 430, 440, 450, 460, 470, 480, 490, 500);

  const int expected_values[] = {
      10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
      110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
      210, 220, 230, 240, 250, 260, 270, 280, 290, 300,
      310, 320, 330, 340, 350, 360, 370, 380, 390, 400,
      410, 420, 430, 440, 450, 460, 470, 480, 490, 500};
  VerifyGenerator(gen, expected_values);
}

// Edge case test. Tests that single-parameter Values() generates the sequence
// with the single value.
TEST(ValuesTest, ValuesWithSingleParameter) {
  const ParamGenerator<int> gen = Values(42);

  const int expected_values[] = {42};
  VerifyGenerator(gen, expected_values);
}

// Tests that Bool() generates sequence (false, true).
TEST(BoolTest, BoolWorks) {
  const ParamGenerator<bool> gen = Bool();

  const bool expected_values[] = {false, true};
  VerifyGenerator(gen, expected_values);
}

# if GTEST_HAS_COMBINE

// Tests that Combine() with two parameters generates the expected sequence.
TEST(CombineTest, CombineWithTwoParameters) {
  const char* foo = "foo";
  const char* bar = "bar";
  const ParamGenerator<tuple<const char*, int> > gen =
      Combine(Values(foo, bar), Values(3, 4));

  tuple<const char*, int> expected_values[] = {
    make_tuple(foo, 3), make_tuple(foo, 4),
    make_tuple(bar, 3), make_tuple(bar, 4)};
  VerifyGenerator(gen, expected_values);
}

// Tests that Combine() with three parameters generates the expected sequence.
TEST(CombineTest, CombineWithThreeParameters) {
  const ParamGenerator<tuple<int, int, int> > gen = Combine(Values(0, 1),
                                                            Values(3, 4),
                                                            Values(5, 6));
  tuple<int, int, int> expected_values[] = {
    make_tuple(0, 3, 5), make_tuple(0, 3, 6),
    make_tuple(0, 4, 5), make_tuple(0, 4, 6),
    make_tuple(1, 3, 5), make_tuple(1, 3, 6),
    make_tuple(1, 4, 5), make_tuple(1, 4, 6)};
  VerifyGenerator(gen, expected_values);
}

// Tests that the Combine() with the first parameter generating a single value
// sequence generates a sequence with the number of elements equal to the
// number of elements in the sequence generated by the second parameter.
TEST(CombineTest, CombineWithFirstParameterSingleValue) {
  const ParamGenerator<tuple<int, int> > gen = Combine(Values(42),
                                                       Values(0, 1));

  tuple<int, int> expected_values[] = {make_tuple(42, 0), make_tuple(42, 1)};
  VerifyGenerator(gen, expected_values);
}

// Tests that the Combine() with the second parameter generating a single value
// sequence generates a sequence with the number of elements equal to the
// number of elements in the sequence generated by the first parameter.
TEST(CombineTest, CombineWithSecondParameterSingleValue) {
  const ParamGenerator<tuple<int, int> > gen = Combine(Values(0, 1),
                                                       Values(42));

  tuple<int, int> expected_values[] = {make_tuple(0, 42), make_tuple(1, 42)};
  VerifyGenerator(gen, expected_values);
}

// Tests that when the first parameter produces an empty sequence,
// Combine() produces an empty sequence, too.
TEST(CombineTest, CombineWithFirstParameterEmptyRange) {
  const ParamGenerator<tuple<int, int> > gen = Combine(Range(0, 0),
                                                       Values(0, 1));
  VerifyGeneratorIsEmpty(gen);
}

// Tests that when the second parameter produces an empty sequence,
// Combine() produces an empty sequence, too.
TEST(CombineTest, CombineWithSecondParameterEmptyRange) {
  const ParamGenerator<tuple<int, int> > gen = Combine(Values(0, 1),
                                                       Range(1, 1));
  VerifyGeneratorIsEmpty(gen);
}

// Edge case. Tests that combine works with the maximum number
// of parameters supported by Google Test (currently 10).
TEST(CombineTest, CombineWithMaxNumberOfParameters) {
  const char* foo = "foo";
  const char* bar = "bar";
  const ParamGenerator<tuple<const char*, int, int, int, int, int, int, int,
                             int, int> > gen = Combine(Values(foo, bar),
                                                       Values(1), Values(2),
                                                       Values(3), Values(4),
                                                       Values(5), Values(6),
                                                       Values(7), Values(8),
                                                       Values(9));

  tuple<const char*, int, int, int, int, int, int, int, int, int>
      expected_values[] = {make_tuple(foo, 1, 2, 3, 4, 5, 6, 7, 8, 9),
                           make_tuple(bar, 1, 2, 3, 4, 5, 6, 7, 8, 9)};
  VerifyGenerator(gen, expected_values);
}

# endif  // GTEST_HAS_COMBINE

// Tests that an generator produces correct sequence after being
// assigned from another generator.
TEST(ParamGeneratorTest, AssignmentWorks) {
  ParamGenerator<int> gen = Values(1, 2);
  const ParamGenerator<int> gen2 = Values(3, 4);
  gen = gen2;

  const int expected_values[] = {3, 4};
  VerifyGenerator(gen, expected_values);
}

// This test verifies that the tests_big_number are expanded and run as specified:
// one test per element from the sequence produced by the generator
// specified in INSTANTIATE_TEST_CASE_P. It also verifies that the test's
// fixture constructor, SetUp(), and TearDown() have run and have been
// supplied with the correct parameters.

// The use of environment object allows detection of the case where no test
// case functionality is run at all. In this case TestCaseTearDown will not
// be able to detect missing tests_big_number, naturally.
template <int kExpectedCalls>
class TestGenerationEnvironment : public ::testing::Environment {
 public:
  static TestGenerationEnvironment* Instance() {
    static TestGenerationEnvironment* instance = new TestGenerationEnvironment;
    return instance;
  }

  void FixtureConstructorExecuted() { fixture_constructor_count_++; }
  void SetUpExecuted() { set_up_count_++; }
  void TearDownExecuted() { tear_down_count_++; }
  void TestBodyExecuted() { test_body_count_++; }

  virtual void TearDown() {
    // If all MultipleTestGenerationTest tests_big_number have been de-selected
    // by the filter flag, the following checks make no sense.
    bool perform_check = false;

    for (int i = 0; i < kExpectedCalls; ++i) {
      Message msg;
      msg << "TestsExpandedAndRun/" << i;
      if (UnitTestOptions::FilterMatchesTest(
             "TestExpansionModule/MultipleTestGenerationTest",
              msg.GetString().c_str())) {
        perform_check = true;
      }
    }
    if (perform_check) {
      EXPECT_EQ(kExpectedCalls, fixture_constructor_count_)
          << "Fixture constructor of ParamTestGenerationTest test case "
          << "has not been run as expected.";
      EXPECT_EQ(kExpectedCalls, set_up_count_)
          << "Fixture SetUp method of ParamTestGenerationTest test case "
          << "has not been run as expected.";
      EXPECT_EQ(kExpectedCalls, tear_down_count_)
          << "Fixture TearDown method of ParamTestGenerationTest test case "
          << "has not been run as expected.";
      EXPECT_EQ(kExpectedCalls, test_body_count_)
          << "Test in ParamTestGenerationTest test case "
          << "has not been run as expected.";
    }
  }

 private:
  TestGenerationEnvironment() : fixture_constructor_count_(0), set_up_count_(0),
                                tear_down_count_(0), test_body_count_(0) {}

  int fixture_constructor_count_;
  int set_up_count_;
  int tear_down_count_;
  int test_body_count_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestGenerationEnvironment);
};

const int test_generation_params[] = {36, 42, 72};

class TestGenerationTest : public TestWithParam<int> {
 public:
  enum {
    PARAMETER_COUNT =
        sizeof(test_generation_params)/sizeof(test_generation_params[0])
  };

  typedef TestGenerationEnvironment<PARAMETER_COUNT> Environment;

  TestGenerationTest() {
    Environment::Instance()->FixtureConstructorExecuted();
    current_parameter_ = GetParam();
  }
  virtual void SetUp() {
    Environment::Instance()->SetUpExecuted();
    EXPECT_EQ(current_parameter_, GetParam());
  }
  virtual void TearDown() {
    Environment::Instance()->TearDownExecuted();
    EXPECT_EQ(current_parameter_, GetParam());
  }

  static void SetUpTestCase() {
    bool all_tests_in_test_case_selected = true;

    for (int i = 0; i < PARAMETER_COUNT; ++i) {
      Message test_name;
      test_name << "TestsExpandedAndRun/" << i;
      if ( !UnitTestOptions::FilterMatchesTest(
                "TestExpansionModule/MultipleTestGenerationTest",
                test_name.GetString())) {
        all_tests_in_test_case_selected = false;
      }
    }
    EXPECT_TRUE(all_tests_in_test_case_selected)
        << "When running the TestGenerationTest test case all of its tests_big_number\n"
        << "must be selected by the filter flag for the test case to pass.\n"
        << "If not all of them are enabled, we can't reliably conclude\n"
        << "that the correct number of tests_big_number have been generated.";

    collected_parameters_.clear();
  }

  static void TearDownTestCase() {
    vector<int> expected_values(test_generation_params,
                                test_generation_params + PARAMETER_COUNT);
    // Test execution order is not guaranteed by Google Test,
    // so the order of values in collected_parameters_ can be
    // different and we have to sort to compare.
    sort(expected_values.begin(), expected_values.end());
    sort(collected_parameters_.begin(), collected_parameters_.end());

    EXPECT_TRUE(collected_parameters_ == expected_values);
  }

 protected:
  int current_parameter_;
  static vector<int> collected_parameters_;

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestGenerationTest);
};
vector<int> TestGenerationTest::collected_parameters_;

TEST_P(TestGenerationTest, TestsExpandedAndRun) {
  Environment::Instance()->TestBodyExecuted();
  EXPECT_EQ(current_parameter_, GetParam());
  collected_parameters_.push_back(GetParam());
}
INSTANTIATE_TEST_CASE_P(TestExpansionModule, TestGenerationTest,
                        ValuesIn(test_generation_params));

// This test verifies that the element sequence (third parameter of
// INSTANTIATE_TEST_CASE_P) is evaluated in InitGoogleTest() and neither at
// the call site of INSTANTIATE_TEST_CASE_P nor in RUN_ALL_TESTS().  For
// that, we declare param_value_ to be a static member of
// GeneratorEvaluationTest and initialize it to 0.  We set it to 1 in
// main(), just before invocation of InitGoogleTest().  After calling
// InitGoogleTest(), we set the value to 2.  If the sequence is evaluated
// before or after InitGoogleTest, INSTANTIATE_TEST_CASE_P will create a
// test with parameter other than 1, and the test body will fail the
// assertion.
class GeneratorEvaluationTest : public TestWithParam<int> {
 public:
  static int param_value() { return param_value_; }
  static void set_param_value(int param_value) { param_value_ = param_value; }

 private:
  static int param_value_;
};
int GeneratorEvaluationTest::param_value_ = 0;

TEST_P(GeneratorEvaluationTest, GeneratorsEvaluatedInMain) {
  EXPECT_EQ(1, GetParam());
}
INSTANTIATE_TEST_CASE_P(GenEvalModule,
                        GeneratorEvaluationTest,
                        Values(GeneratorEvaluationTest::param_value()));

// Tests that generators defined in a different translation unit are
// functional. Generator extern_gen is defined in gtest-param-test_test2.cc.
extern ParamGenerator<int> extern_gen;
class ExternalGeneratorTest : public TestWithParam<int> {};
TEST_P(ExternalGeneratorTest, ExternalGenerator) {
  // Sequence produced by extern_gen contains only a single value
  // which we verify here.
  EXPECT_EQ(GetParam(), 33);
}
INSTANTIATE_TEST_CASE_P(ExternalGeneratorModule,
                        ExternalGeneratorTest,
                        extern_gen);

// Tests that a parameterized test case can be defined in one translation
// unit and instantiated in another. This test will be instantiated in
// gtest-param-test_test2.cc. ExternalInstantiationTest fixture class is
// defined in gtest-param-test_test.h.
TEST_P(ExternalInstantiationTest, IsMultipleOf33) {
  EXPECT_EQ(0, GetParam() % 33);
}

// Tests that a parameterized test case can be instantiated with multiple
// generators.
class MultipleInstantiationTest : public TestWithParam<int> {};
TEST_P(MultipleInstantiationTest, AllowsMultipleInstances) {
}
INSTANTIATE_TEST_CASE_P(Sequence1, MultipleInstantiationTest, Values(1, 2));
INSTANTIATE_TEST_CASE_P(Sequence2, MultipleInstantiationTest, Range(3, 5));

// Tests that a parameterized test case can be instantiated
// in multiple translation units. This test will be instantiated
// here and in gtest-param-test_test2.cc.
// InstantiationInMultipleTranslationUnitsTest fixture class
// is defined in gtest-param-test_test.h.
TEST_P(InstantiationInMultipleTranslaionUnitsTest, IsMultipleOf42) {
  EXPECT_EQ(0, GetParam() % 42);
}
INSTANTIATE_TEST_CASE_P(Sequence1,
                        InstantiationInMultipleTranslaionUnitsTest,
                        Values(42, 42*2));

// Tests that each iteration of parameterized test runs in a separate test
// object.
class SeparateInstanceTest : public TestWithParam<int> {
 public:
  SeparateInstanceTest() : count_(0) {}

  static void TearDownTestCase() {
    EXPECT_GE(global_count_, 2)
        << "If some (but not all) SeparateInstanceTest tests_big_number have been "
        << "filtered out this test will fail. Make sure that all "
        << "GeneratorEvaluationTest are selected or de-selected together "
        << "by the test filter.";
  }

 protected:
  int count_;
  static int global_count_;
};
int SeparateInstanceTest::global_count_ = 0;

TEST_P(SeparateInstanceTest, TestsRunInSeparateInstances) {
  EXPECT_EQ(0, count_++);
  global_count_++;
}
INSTANTIATE_TEST_CASE_P(FourElemSequence, SeparateInstanceTest, Range(1, 4));

// Tests that all instantiations of a test have named appropriately. Test
// defined with TEST_P(TestCaseName, TestName) and instantiated with
// INSTANTIATE_TEST_CASE_P(SequenceName, TestCaseName, generator) must be named
// SequenceName/TestCaseName.TestName/i, where i is the 0-based index of the
// sequence element used to instantiate the test.
class NamingTest : public TestWithParam<int> {};

TEST_P(NamingTest, TestsReportCorrectNamesAndParameters) {
  const ::testing::TestInfo* const test_info =
     ::testing::UnitTest::GetInstance()->current_test_info();

  EXPECT_STREQ("ZeroToFiveSequence/NamingTest", test_info->test_case_name());

  Message index_stream;
  index_stream << "TestsReportCorrectNamesAndParameters/" << GetParam();
  EXPECT_STREQ(index_stream.GetString().c_str(), test_info->name());

  EXPECT_EQ(::testing::PrintToString(GetParam()), test_info->value_param());
}

INSTANTIATE_TEST_CASE_P(ZeroToFiveSequence, NamingTest, Range(0, 5));

// Class that cannot be streamed into an ostream.  It needs to be copyable
// (and, in case of MSVC, also assignable) in order to be a test parameter
// type.  Its default copy constructor and assignment operator do exactly
// what we need.
class Unstreamable {
 public:
  explicit Unstreamable(int value) : value_(value) {}

 private:
  int value_;
};

class CommentTest : public TestWithParam<Unstreamable> {};

TEST_P(CommentTest, TestsCorrectlyReportUnstreamableParams) {
  const ::testing::TestInfo* const test_info =
     ::testing::UnitTest::GetInstance()->current_test_info();

  EXPECT_EQ(::testing::PrintToString(GetParam()), test_info->value_param());
}

INSTANTIATE_TEST_CASE_P(InstantiationWithComments,
                        CommentTest,
                        Values(Unstreamable(1)));

// Verify that we can create a hierarchy of test fixtures, where the base
// class fixture is not parameterized and the derived class is. In this case
// ParameterizedDerivedTest inherits from NonParameterizedBaseTest.  We
// perform simple tests_big_number on both.
class NonParameterizedBaseTest : public ::testing::Test {
 public:
  NonParameterizedBaseTest() : n_(17) { }
 protected:
  int n_;
};

class ParameterizedDerivedTest : public NonParameterizedBaseTest,
                                 public ::testing::WithParamInterface<int> {
 protected:
  ParameterizedDerivedTest() : count_(0) { }
  int count_;
  static int global_count_;
};

int ParameterizedDerivedTest::global_count_ = 0;

TEST_F(NonParameterizedBaseTest, FixtureIsInitialized) {
  EXPECT_EQ(17, n_);
}

TEST_P(ParameterizedDerivedTest, SeesSequence) {
  EXPECT_EQ(17, n_);
  EXPECT_EQ(0, count_++);
  EXPECT_EQ(GetParam(), global_count_++);
}

class ParameterizedDeathTest : public ::testing::TestWithParam<int> { };

TEST_F(ParameterizedDeathTest, GetParamDiesFromTestF) {
  EXPECT_DEATH_IF_SUPPORTED(GetParam(),
                            ".* value-parameterized test .*");
}

INSTANTIATE_TEST_CASE_P(RangeZeroToFive, ParameterizedDerivedTest, Range(0, 5));

#endif  // GTEST_HAS_PARAM_TEST

TEST(CompileTest, CombineIsDefinedOnlyWhenGtestHasParamTestIsDefined) {
#if GTEST_HAS_COMBINE && !GTEST_HAS_PARAM_TEST
  FAIL() << "GTEST_HAS_COMBINE is defined while GTEST_HAS_PARAM_TEST is not\n"
#endif
}

int main(int argc, char **argv) {
#if GTEST_HAS_PARAM_TEST
  // Used in TestGenerationTest test case.
  AddGlobalTestEnvironment(TestGenerationTest::Environment::Instance());
  // Used in GeneratorEvaluationTest test case. Tests that the updated value
  // will be picked up for instantiating tests_big_number in GeneratorEvaluationTest.
  GeneratorEvaluationTest::set_param_value(1);
#endif  // GTEST_HAS_PARAM_TEST

  ::testing::InitGoogleTest(&argc, argv);

#if GTEST_HAS_PARAM_TEST
  // Used in GeneratorEvaluationTest test case. Tests that value updated
  // here will NOT be used for instantiating tests_big_number in
  // GeneratorEvaluationTest.
  GeneratorEvaluationTest::set_param_value(2);
#endif  // GTEST_HAS_PARAM_TEST

  return RUN_ALL_TESTS();
}
