// @file binfhecontext.h - Header file for BinFHEContext class, which is used
// for Boolean circuit FHE schemes
//
// @author TPOC: contact@palisade-crypto.org
// @copyright Copyright (c) 2019, Duality Technologies Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef BINFHE_BINFHECONTEXT_H
#define BINFHE_BINFHECONTEXT_H

#include <memory>
#include <string>

#include "fhew.h"
#include "lwe.h"
#include "lwecore.h"
#include "math/backend.h"
#include "ringcore.h"
#include "utils/serializable.h"

namespace lbcrypto {

// security levels for predefined parameter sets
enum BINFHEPARAMSET {
  TOY,          // no security
  MEDIUM,     // 108 bits of security for classical and 100 bits for quantum
  STD128_AP,    // Optimized for AP (has higher failure probability for GINX) -
                // more than 128 bits of security for classical
                // computer attacks - uses the same setup as HE standard
  STD128_APOPT, // Optimized for AP (has higher failure probability for GINX) -
                // more than 128 bits of security for classical computer attacks -
                // optimize runtime by finding a non-power-of-two n
  STD128,       // more than 128 bits of security for classical
                // computer attacks - uses the same setup as HE standard
  STD128_OPT,   // more than 128 bits of security for classical computer attacks -
                // optimize runtime by finding a non-power-of-two n
  STD192,       // more than 192 bits of security for classical computer attacks -
                // uses the same setup as HE standard
  STD192_OPT,   // more than 192 bits of security for classical computer attacks -
                // optimize runtime by finding a non-power-of-two n
  STD256,       // more than 256 bits of security for classical computer attacks -
                // uses the same setup as HE standard
  STD256_OPT,   // more than 256 bits of security for classical computer attacks -
                // optimize runtime by finding a non-power-of-two n
  STD128Q,      // more than 128 bits of security for quantum attacks - uses the
                // same setup as HE standard
  STD128Q_OPT,  // more than 128 bits of security for quantum attacks -
                // optimize runtime by finding a non-power-of-two n
  STD192Q,      // more than 192 bits of security for quantum attacks - uses the
                // same setup as HE standard
  STD192Q_OPT,  // more than 192 bits of security for quantum attacks -
                // optimize runtime by finding a non-power-of-two n
  STD256Q,      // more than 256 bits of security for quantum attacks - uses the
                // same setup as HE standard
  STD256Q_OPT,  // more than 256 bits of security for quantum attacks -
                // optimize runtime by finding a non-power-of-two n
  SIGNED_MOD_TEST  // special parameter set for confirming the signed modular
};

// Type of ciphertext generated by the Encrypt method
enum BINFHEOUTPUT {
  FRESH,        // a fresh encryption
  BOOTSTRAPPED  // a freshly encrypted ciphertext is bootstrapped
};

class LWECiphertextImpl;

using LWECiphertext = std::shared_ptr<LWECiphertextImpl>;

using ConstLWECiphertext = const std::shared_ptr<const LWECiphertextImpl>;

class LWEPrivateKeyImpl;

using LWEPrivateKey = std::shared_ptr<LWEPrivateKeyImpl>;

using ConstLWEPrivateKey = const std::shared_ptr<const LWEPrivateKeyImpl>;

using LWEPlaintextModulus = uint64_t;

/**
 * @brief BinFHEContext
 *
 * The wrapper class for Boolean circuit FHE
 */
class BinFHEContext : public Serializable {
 public:
  BinFHEContext() {}

  /**
   * Creates a crypto context using custom parameters.
   * Should be used with care (only for advanced users familiar with LWE
   * parameter selection).
   *
   * @param n lattice parameter for additive LWE scheme
   * @param N ring dimension for RingGSW/RLWE used in bootstrapping
   * @param &q modulus for additive LWE
   * @param &Q modulus for RingGSW/RLWE used in bootstrapping
   * @param &qKS modulus for key switching
   * @param std standard deviation
   * @param baseKS the base used for key switching
   * @param baseG the gadget base used in bootstrapping
   * @param baseR the base used for refreshing
   * @param method the bootstrapping method (AP or GINX)
   * @return creates the cryptocontext
   */
  void GenerateBinFHEContext(uint32_t n, uint32_t N, const NativeInteger &q, 
                             const NativeInteger &Q, const NativeInteger &qKS, double std,
                             uint32_t baseKS, uint32_t baseG, uint32_t baseR,
                             BINFHEMETHOD method = GINX);

  /**
   * Creates a crypto context using predefined parameters sets. Recommended for
   * most users.
   *
   * @param set the parameter set: TOY, MEDIUM, STD128, STD192, STD256
   * @param method the bootstrapping method (AP or GINX)
   * @return create the cryptocontext
   */
  void GenerateBinFHEContext(BINFHEPARAMSET set, BINFHEMETHOD method = GINX);

  void Generate_Default_params();

  /**
   * Gets the refreshing key (used for serialization).
   *
   * @return a shared pointer to the refreshing key
   */
  const std::shared_ptr<RingGSWBTKey> GetRefreshKey() const {
    return m_BTKey.BSkey;
  }

  /**
   * Gets the switching key (used for serialization).
   *
   * @return a shared pointer to the switching key
   */
  const std::shared_ptr<LWESwitchingKey> GetSwitchKey() const {
    return m_BTKey.KSkey;
  }

  /**
   * Generates a secret key for the main LWE scheme
   *
   * @return a shared pointer to the secret key
   */
  LWEPrivateKey KeyGen() const;

  /**
   * Generates a secret key used in bootstrapping
   * @return a shared pointer to the secret key
   */
  LWEPrivateKey KeyGenN() const;

  /**
   * Encrypts a bit using a secret key (symmetric key encryption)
   *
   * @param sk - the secret key
   * @param &m - the plaintext
   * @param output - FRESH to generate fresh ciphertext, BOOTSTRAPPED to
   * generate a refreshed ciphertext (default)
   * @return a shared pointer to the ciphertext
   */
  LWECiphertext Encrypt(ConstLWEPrivateKey sk, const LWEPlaintext &m,
                        BINFHEOUTPUT output = BOOTSTRAPPED) const;

  /**
   * Decrypts a ciphertext using a secret key
   *
   * @param sk the secret key
   * @param ct the ciphertext
   * @param *result plaintext result
   */
  void Decrypt(ConstLWEPrivateKey sk, ConstLWECiphertext ct,
               LWEPlaintext *result) const;

  /**
   * Generates a switching key to go from a secret key with (Q,N) to a secret
   * key with (q,n)
   *
   * @param sk new secret key
   * @param skN old secret key
   * @return a shared pointer to the switching key
   */
  std::shared_ptr<LWESwitchingKey> KeySwitchGen(ConstLWEPrivateKey sk,
                                                ConstLWEPrivateKey skN) const;

  /**
   * Generates boostrapping keys
   *
   * @param sk secret key
   */
  void BTKeyGen(ConstLWEPrivateKey sk);

  /**
   * Loads bootstrapping keys in the context (typically after deserializing)
   *
   * @param key struct with the bootstrapping keys
   */
  void BTKeyLoad(const RingGSWEvalKey &key) { m_BTKey = key; }

  /**
   * Clear the bootstrapping keys in the current context
   */
  void ClearBTKeys() {
    m_BTKey.BSkey.reset();
    m_BTKey.KSkey.reset();
  }

  /**
   * Evaluates a binary gate (calls bootstrapping as a subroutine)
   *
   * @param gate the gate; can be AND, OR, NAND, NOR, XOR, or XNOR
   * @param ct1 first ciphertext
   * @param ct2 second ciphertext
   * @return a shared pointer to the resulting ciphertext
   */
  LWECiphertext EvalBinGate(const BINGATE gate, ConstLWECiphertext ct1,
                            ConstLWECiphertext ct2) const;

  /**
   * Bootstraps a ciphertext (without peforming any operation)
   *
   * @param ct1 ciphertext to be bootstrapped
   * @return a shared pointer to the resulting ciphertext
   */
  LWECiphertext Bootstrap(ConstLWECiphertext ct1) const;

  /**
   * Evaluates NOT gate
   *
   * @param ct1 the input ciphertext
   * @return a shared pointer to the resulting ciphertext
   */
  LWECiphertext EvalNOT(ConstLWECiphertext ct1) const;

  /**
   * Evaluates constant gate
   *
   * @param value the Boolean value to output
   * @return a shared pointer to the resulting ciphertext
   */
  LWECiphertext EvalConstant(bool value) const;


  /**
   * Encrypt message without noise
  */
  LWECiphertext TraivlEncrypt(LWEPlaintext value, LWEPlaintextModulus p);

  /**
    * Evaluates SignFunc
    */
  LWECiphertext MyEvalSigndFunc(ConstLWECiphertext ct, LWEPlaintextModulus p) const;

  /**
    * Encrypt message with modulus p
  
    */
  LWECiphertext Encrypt(ConstLWEPrivateKey sk, const LWEPlaintext& m, const LWEPlaintextModulus& p, BINFHEOUTPUT output);

  /**
    * Decrypt message with modulus p
    */
  void Decrypt(ConstLWEPrivateKey sk, ConstLWECiphertext ct, LWEPlaintext* result, LWEPlaintextModulus p) const;



  const std::shared_ptr<RingGSWCryptoParams> GetParams() { return m_params; }

  const std::shared_ptr<LWEEncryptionScheme> GetLWEScheme() {
    return m_LWEscheme;
  }

  const std::shared_ptr<RingGSWAccumulatorScheme> GetRingGSWScheme() {
    return m_RingGSWscheme;
  }

  template <class Archive>
  void save(Archive &ar, std::uint32_t const version) const {
    ar(::cereal::make_nvp("params", m_params));
  }

  template <class Archive>
  void load(Archive &ar, std::uint32_t const version) {
    if (version > SerializedVersion()) {
      PALISADE_THROW(deserialize_error,
                     "serialized object version " + std::to_string(version) +
                         " is from a later version of the library");
    }
    ar(::cereal::make_nvp("params", m_params));
  }

  std::string SerializedObjectName() const { return "RingGSWBTKey"; }
  static uint32_t SerializedVersion() { return 1; }

 private:
  // Shared pointer to Ring GSW + LWE parameters
  std::shared_ptr<RingGSWCryptoParams> m_params;

  // Shared pointer to the underlying additive LWE scheme
  std::shared_ptr<LWEEncryptionScheme> m_LWEscheme;

  // Shared pointer to the underlying RingGSW/RLWE scheme
  std::shared_ptr<RingGSWAccumulatorScheme> m_RingGSWscheme;

  // Struct containing the bootstrapping keys
  RingGSWEvalKey m_BTKey;
};

}  // namespace lbcrypto

#endif
