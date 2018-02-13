#include <core/ic.hpp>

#include <iostream>

int main()
{
    core::ic c(2, 2, 2);
    std::string s;
    c.dump(s);
    std::cout<<s<<std::endl;
    return 0;
}
