#pragma once
namespace miit::algebra {
class Generator {
public:
    virtual ~Generator();
    virtual int generate() = 0;
};
}
