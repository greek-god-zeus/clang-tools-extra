// RUN: %check_clang_tidy %s misc-detect-c-functions %t

namespace std{
double exp(double);
float abs(float);
}

double exp(double);
float fabs(float);

void function()
{
    std::exp(5);
    exp(5);
    std::abs(-1);
    fabs(-1);
}
