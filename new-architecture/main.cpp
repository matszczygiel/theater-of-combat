#include "component.h"

#include <iostream>

class RenderingComponent : public Component
{
    public:
    void do_work() override {
        std::cout << "Hello components!\n";
    }
};
COMPONENT_REGISTER(RenderingComponent, "RenderingComponent");

int main()
{
    auto comp = Component::create("RenderingComponent");
    comp->do_work();
}