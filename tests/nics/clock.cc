#include <iostream>
#include "Clock.h"

int main() {
  Clock test(10);

  cout << "Initial time: " << test.remaining_seconds() << endl;

  test.start();
  sleep(1);
  cout << "Remaining after 1 second: " << test.remaining_seconds() << endl;
  cout << "Elapsed after 1 second: " << test.elapsed_seconds() << endl;
  test.stop();
  cout << "Remaining after stop: " << test.remaining_seconds() << endl;
  cout << "Elapsed after stop: " << test.elapsed_seconds() << endl;

  return 0;
}
