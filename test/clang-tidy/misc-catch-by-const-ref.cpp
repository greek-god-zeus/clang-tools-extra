// RUN: %check_clang_tidy %s misc-catch-by-const-ref %t


class logic_error {
public:
  logic_error(const char *message) {}
};

void check()
{}

void catchbyRef() {
  try {
    check();
  } catch (logic_error& e) {
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: catch handler catches by non const reference; catching by const-reference may be more efficient [misc-catch-by-const-ref]
  }
}

void catchbyConstRef() {
  try {
    check();
  } catch (const logic_error& e) {
  }
}
