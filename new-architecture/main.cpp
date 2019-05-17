#include "component.h"

#include <iostream>
#include <string>

class RenderingComponent : public Registrable<RenderingComponent, std::string> {
   public:
    using Registrable_base = Registrable<RenderingComponent, std::string>;
    void do_work() {
        std::cout << "Hello components!\n";
    }
};
TOC_REGISTER_CLASS(RenderingComponent::Registrable_base, RenderingComponent);

int main() {
    auto comp = RenderingComponent::Registrable_base::create("RenderingComponent");
    comp->do_work();
}
