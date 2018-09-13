#include <migraph/gpu/write_literals.hpp>
#include <migraph/iterator_for.hpp>
#include <migraph/gpu/hip.hpp>
#include <migraph/instruction.hpp>
#include <migraph/pass_config.hpp>

namespace migraph {

namespace gpu {

struct hip_load_literal
{
    shape s;
    std::size_t n = 0;
    std::string name() const { return "hip::load_literal"; }
    shape compute_shape(const std::vector<shape>& inputs) const
    {
        check_shapes{inputs}.has(0);
        return s;
    }
    argument compute(context& ctx, const shape&, const std::vector<argument>&) const
    {
        return ctx.literals.at(n);
    }
};

void write_literals::apply(program& p) const
{
    if(!enabled(MIGRAPH_DISABLE_MEMORY_COLORING{}))
        return;

    assert(ctx != nullptr);
    for(auto ins : iterator_for(p))
    {
        if(ins->op.name() == "@literal")
        {
            argument a    = to_gpu(ins->lit.get_argument());
            std::size_t n = ctx->literals.size();
            ctx->literals.push_back(a);
            p.replace_instruction(ins, hip_load_literal{a.get_shape(), n});
        }
    }
}
} // namespace gpu
} // namespace migraph
