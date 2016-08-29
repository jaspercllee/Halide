#ifndef HALIDE_ARGUMENT_H
#define HALIDE_ARGUMENT_H

/** \file
 * Defines a type used for expressing the type signature of a
 * generated halide pipeline
 */

#include "Error.h"
#include "Expr.h"
#include "Type.h"
#include "runtime/HalideRuntime.h"

namespace Halide {

/**
 * A struct representing an argument to a halide-generated
 * function. Used for specifying the function signature of
 * generated code.
 */
struct Argument {
    /** The name of the argument */
    std::string name;

    /** An argument is either a primitive type (for parameters), or a
     * buffer pointer.
     *
     * If kind == InputScalar, then type fully encodes the expected type
     * of the scalar argument.
     *
     * If kind == InputBuffer|OutputBuffer, then type.bytes() should be used
     * to determine* elem_size of the buffer; additionally, type.code *should*
     * reflect the expected interpretation of the buffer data (e.g. float vs int),
     * but there is no runtime enforcement of this at present.
     */
    enum Kind {
        InputScalar = halide_argument_kind_input_scalar,
        InputBuffer = halide_argument_kind_input_buffer,
        OutputBuffer = halide_argument_kind_output_buffer
    };
    Kind kind;

    /** If kind == InputBuffer|OutputBuffer, this is the dimensionality of the buffer.
     * If kind == InputScalar, this value is ignored (and should always be set to zero) */
    uint8_t dimensions;

    /** If this is a scalar parameter, then this is its type.
     *
     * If this is a buffer parameter, this is used to determine elem_size
     * of the buffer_t.
     *
     * Note that type.width should always be 1 here. */
    Type type;

    /** If this is a scalar parameter, then these are its default, min, max values.
     * By default, they are left unset, implying "no default, no min, no max". */
    Expr def, min, max;

    Argument() : kind(InputScalar), dimensions(0) {}
    Argument(const std::string &_name, Kind _kind, const Type &_type, uint8_t _dimensions,
                Expr _def = Expr(),
                Expr _min = Expr(),
                Expr _max = Expr()) :
        name(_name), kind(_kind), dimensions(_dimensions), type(_type), def(_def), min(_min), max(_max) {
        user_assert(!(is_scalar() && dimensions != 0))
            << "Scalar Arguments must specify dimensions of 0";
        user_assert(!(is_buffer() && def.defined()))
            << "Scalar default must not be defined for BufferPtr Arguments";
        user_assert(!(is_buffer() && min.defined()))
            << "Scalar min must not be defined for BufferPtr Arguments";
        user_assert(!(is_buffer() && max.defined()))
            << "Scalar max must not be defined for BufferPtr Arguments";
    }

    template<typename T, int D>
    Argument(const Image<T, D> &im) : kind(InputBuffer), dimensions(im.dimensions()), type(im.type()) {}

    bool is_buffer() const { return kind == InputBuffer || kind == OutputBuffer; }
    bool is_scalar() const { return kind == InputScalar; }

    bool is_input() const { return kind == InputScalar || kind == InputBuffer; }
    bool is_output() const { return kind == OutputBuffer; }
};

}

#endif
