#include "Shared.hpp"
#include "Common/Algorithms/Sorting.hpp"

TEST_DEFINE("Common.Sorting", "InsertionSort")
{
    Array<int> array = { 4, 1, 3, 2 };
    InsertionSort(array.GetBeginPtr(), array.GetEndPtr());
    TEST_TRUE(array[0] == 1);
    TEST_TRUE(array[1] == 2);
    TEST_TRUE(array[2] == 3);
    TEST_TRUE(array[3] == 4);
}
