#include "Quant.h"
#include "IRMutator.h"
#include "IROperator.h"

namespace Halide {
namespace Internal {

using std::string;

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

//    zp = saturating_cast(Int(8), zp);
    if(scale.defined()) {
        //Expr tmp = value/scale+zp;
        
        scale = Cast::make(Float(32), scale);
//        value = Div::make(value, scale);
//        value = Add::make(value, zp);
        value = Cast::make(Float(32), Div::make(value, scale));
        value = Add::make(value, Cast::make(Float(32), zp));

//        value = Cast::make(Int(32), value);
//        value = round(value);
//        value = clamp(value, 0, 255);
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
    Expr scale, zp, k;
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
    DequantExpr(Expr s, Expr z, Expr k) : scale(s), zp(z), k(k) {}
};


Expr dequant(Expr scale, Expr zp, Expr k, Expr value) {
    debug(3) << "In dequant value " << " = " << value << "\n";
//    value = saturating_cast(Int(32), value);
    if(scale.defined()) {
        value = Cast::make(Float(32), value);

        //MUL
        value = (value - k*zp) * scale;
    }
    debug(3) << "In dequant value " << " = " << value << "\n";
    DequantExpr q(scale, zp, k);
    debug(3) << "In dequant value " << " = " << value << "\n";
    return q.mutate(value);
}


class QuantMatExpr : public IRMutator {
    using IRMutator::visit;
    Expr scale, zp, a, b, c;
/*
    Expr visit(const Add *op) override {
        a = mutate(op->a);
        b = mutate(op->b);
        c = Cast::make(op->type, 0+b);
        return c;
    }
*/
    Expr visit(const Mul *op) override {
        debug(3) << "xxxxxx" << "\n";
        debug(3) << "quant_mat visit scale " << " = " << scale << "\n";
        debug(3) << "quant_mat visit zp6 " << " = " << zp << "\n";
//        a = mutate(op->a);
//        b = mutate(op->b);
        debug(3) << "quant_mat Mul op->a " << " = " << op->a << "\n";
        debug(3) << "quant_mat Mul op->b " << " = " << op->b << "\n";

//        c = Cast::make(op->type, zp + scale * ( zp * zp - zp * op->a - zp * op->b + op->a * op->b ));
        c = Cast::make(op->type, zp + scale * ((op->a - zp) * (op->b - zp)));
//        c = clamp(c, 0, 255);
//	        c = Cast::make(UInt(8), c);
        debug(3) << "quant_mat Mul9:c " << " = " << c << "\n";

        return c;
    }


public:
    QuantMatExpr(Expr s, Expr z) : scale(s), zp(z) {}
};


Expr q8mat(Expr scale, Expr zp, Expr value) {
    debug(3) << "in quant_mat1" << " = " << value << "\n";
    QuantMatExpr q(scale, zp);
    debug(3) << "in quant_mat4" << " = " << value << "\n";
    return q.mutate(value);
}


}  // namespace Internal
}  // namespace Halide
