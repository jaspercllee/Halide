#include "Quant8.h"
#include "IRMutator.h"
#include "IROperator.h"

namespace Halide {
namespace Internal {

using std::string;

// Prefix all names in an expression with some string.
class QuantExpr : public IRMutator {
    using IRMutator::visit;
    Expr scale, zp;
    Expr visit(const Variable *v) override {
        debug(3) << "Quant visit vname " << " = " << v->name << "\n";
        debug(3) << "Quant visit scale " << " = " << scale << "\n";
        debug(3) << "Quant visit zp " << " = " << zp << "\n";
        if (v->param.defined()) {
            return v;
        } else
            return v;
    }
public:
    QuantExpr(Expr s, Expr z) : scale(s), zp(z) {}
};


Expr quant(Expr scale, Expr zp, Expr value) {
    debug(3) << "In quant value " << " = " << value << "\n";


    zp = saturating_cast(UInt(8), zp);


    if(scale.defined()) {
        value = value/scale;
        value = ceil(value);
        value = Cast::make(Int(32), value);
        value = value + zp;
        value = clamp(value, 0, 255);
        debug(3) << "****check int8**** " << "\n";
//***   saturating_cast
//        value = Cast::make(UInt(8), value);
        debug(3) << "****check int8**** " << "\n";
    }
    debug(3) << "In quant value " << " = " << value << "\n";
    QuantExpr q(scale, zp);
    debug(3) << "In quant value " << " = " << value << "\n";
    return q.mutate(value);
}


class DequantExpr : public IRMutator {
    using IRMutator::visit;
    Expr scale, zp;
    Expr visit(const Variable *v) override {
        debug(3) << "Quant visit vname " << " = " << v->name << "\n";
        debug(3) << "Quant visit scale " << " = " << scale << "\n";
        debug(3) << "Quant visit zp " << " = " << zp << "\n";
        if (v->param.defined()) {
            return v;
        } else {
            return v;
        }
    }
public:
    DequantExpr(Expr s, Expr z) : scale(s), zp(z) {}
};


Expr dequant(Expr scale, Expr zp, Expr value) {
    debug(3) << "In dequant value " << " = " << value << "\n";
//    value = saturating_cast(Int(32), value);
    if(scale.defined()) {
        //value = Cast::make(UInt(8), value);


        //ADD
        value = (value - 2*zp) * scale;

        //MUL
//        value = (value - zp*zp) * scale;
        value = Cast::make(Int(32), value);
    }
    debug(3) << "In dequant value " << " = " << value << "\n";
    DequantExpr q(scale, zp);
    debug(3) << "In dequant value " << " = " << value << "\n";
    return q.mutate(value);
}


class QuantMatExpr : public IRMutator {
    using IRMutator::visit;
    Expr scale, zp;
    Expr visit(const Variable *v) override {
        debug(3) << "Quant_mat visit vname " << " = " << v->name << "\n";
        debug(3) << "Quant_mat visit scale " << " = " << scale << "\n";
        debug(3) << "Quant_mat visit zp " << " = " << zp << "\n";
        if (v->param.defined()) {
            return v;
        } else
            return v;
    }
public:
    QuantMatExpr(Expr s, Expr z) : scale(s), zp(z) {}
};


Expr q8mat(Expr scale, Expr zp, Expr value) {
    debug(3) << "in quant_mat" << " = " << value << "\n";


//    zp = saturating_cast(UInt(8), zp);


    if(scale.defined()) {
        debug(3) << "in quant_mat" << " = " << value << "\n";
    }


    debug(3) << "in quant_mat" << " = " << value << "\n";
    QuantMatExpr q(scale, zp);
    debug(3) << "in quant_mat" << " = " << value << "\n";
    return q.mutate(value);
}


}  // namespace Internal
}  // namespace Halide