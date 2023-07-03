/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "quantizer.h"
#include "srsran/ofh/compression/iq_compressor.h"
#include "srsran/ofh/compression/iq_decompressor.h"

namespace srsran {
namespace ofh {

/// Implementation of the Block Floating Point IQ data compression.
class iq_compression_bfp_impl : public iq_compressor, public iq_decompressor
{
public:
  // Constructor.
  explicit iq_compression_bfp_impl(float iq_scaling_ = 1.0) : iq_scaling(iq_scaling_) {}

  // See interface for the documentation.
  virtual void
  compress(span<compressed_prb> output, span<const cf_t> input, const ru_compression_params& params) override;

  // See interface for the documentation.
  virtual void
  decompress(span<cf_t> output, span<const compressed_prb> input, const ru_compression_params& params) override;

protected:
  /// Number of quantized samples per resource block.
  static constexpr size_t NOF_SAMPLES_PER_PRB = 2 * NOF_SUBCARRIERS_PER_RB;

  /// \brief Determines the exponent to be used in BFP compression.
  ///
  /// First, it determines the maximum left shift that will still preserve the original value (implemented as a counter
  /// of leading zeros without a sign bit). Then, it determines an exponent given the requested \c data_width.
  ///
  /// \param[in] in_sample  16bit input value for which the exponent is calculated.
  /// \param[in] data_width BFP compression bit width.
  /// \return An exponent to be applied to the input sample to compress it to \c data_width bits.
  static unsigned determine_exponent(uint16_t x, unsigned data_width)
  {
    srsran_assert(data_width < 17, "Passed IQ data width exceeds 16 bits");

    unsigned max_shift       = MAX_IQ_WIDTH - data_width;
    unsigned lz_without_sign = max_shift;

    if ((x > 0) && (max_shift > 0)) {
      // TODO: use a wrapper that checks whether this builtin is actually available and provides a fallback option.
      lz_without_sign = __builtin_clz(x) - 16U - 1U;
    }
    int raw_exp = std::min(max_shift, lz_without_sign);
    return std::max(0, static_cast<int>(MAX_IQ_WIDTH - data_width) - raw_exp);
  }

  /// \brief Compresses one resource block using the generic implementation of the algorithm
  /// from O-RAN.WG4.CUS, Annex A.1.2.
  ///
  /// \param[out] c_prb      Compressed PRB.
  /// \param input_quantized Span of quantized IQ samples of a resource block to be compressed.
  /// \param data_width      BFP compression bit width.
  static void compress_prb_generic(compressed_prb& c_prb, span<const int16_t> input_quantized, unsigned data_width);

  /// \brief Decompresses one resource block using a generic implementation of the algorithm
  /// from O-RAN.WG4.CUS, Annex A.1.3.
  ///
  /// \param[out] output Span of decompressed complex samples of a resource block (12 samples).
  /// \param[in]  c_prb  Compressed PRB.
  /// \param[in]  q      Quantizer object.
  /// \param data_width  Bit width of compressed samples.
  static void
  decompress_prb_generic(span<cf_t> output, const compressed_prb& c_prb, const quantizer& q, unsigned data_width);

  /// Quantizes complex float samples using the specified bit width.
  ///
  /// \param[out] out      Quantized samples.
  /// \param[in] in        Span of input float samples.
  /// \param[in] bit_width Number of significant bits used by the quantized samples.
  void quantize_input(span<int16_t> out, span<const float> in);

private:
  /// Scaling factor applied to IQ data prior to quantization.
  const float iq_scaling;
};

} // namespace ofh
} // namespace srsran
