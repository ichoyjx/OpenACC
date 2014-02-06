/*
 * framework for functional test, NL did this...
 */

test_1() {
  ...
  return err1;
}

test_2() {
  ...
  return err2;
}

test_3() {
  ...
  return err3;
}

batch_test() {
  err = 0;
  err += test_1();
  err += test_2();
  err += test_3();

  return err;
}

int main() {
  return batch_test();
}
