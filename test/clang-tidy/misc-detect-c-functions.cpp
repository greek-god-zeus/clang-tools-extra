// RUN: %check_clang_tidy %s misc-detect-c-functions %t

namespace std{
double exp(double);
float abs(float);
}

double exp(double);
float fabs(float);
int foo();

struct Foo {
    double exp(double);
};

void function()
{
    std::exp(5);
    exp(5);
    // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: this function has a corresponding std version. Consider using it (std::exp) [misc-detect-c-functions]
    // CHECK-FIXES: {{^}}    std::exp(5);{{$}}

    std::abs(-1);
    fabs(-1);
    // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: this function has a better version. Consider using it (std::abs) [misc-detect-c-functions]
    // CHECK-FIXES: {{^}}    std::abs(-1);{{$}}

    Foo bar;
    bar.exp(5);

    foo();
}
