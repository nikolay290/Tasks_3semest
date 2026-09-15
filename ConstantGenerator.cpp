#include "D:\ConstantGenerator.h"
namespace miit::algebra {
ConstantGenerator::ConstantGenerator(int value) : value_(value) {}
int ConstantGenerator::generate() { return value_; }
}
