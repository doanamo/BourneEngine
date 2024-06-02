#include "Shared.hpp"

extern bool TestUtility();

int main()
{
    if(!TestUtility())
        return 1;

    return 0;
}
