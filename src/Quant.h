#ifndef HALIDE_QUANT8_H
#define HALIDE_QUANT8_H

/** \file
 *
 * Defines methods for prefixing names in an expression with a prefix string.
 */

#include "IR.h"

namespace Halide {
namespace Internal {

/** Quantize, Dequantize, and quantized-matrix multiplication. */
Expr quant(Expr scale, Expr zp, Expr value);
Expr dequant(Expr scale, Expr zp, Expr k, Expr value);
Expr q8mat(Expr scale, Expr zp, Expr value);

}  // namespace Internal
}  // namespace Halide

#endif
