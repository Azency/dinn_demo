// @file  simple-integers-serial.cpp - Simple example for BFVrns (integer
// arithmetic) with serialization.
// Refer to the simple-real-numbers-serial file for an example of how to use
// this in a "client-server" setup
// @author TPOC: contact@palisade-crypto.org
//
// @copyright Copyright (c) 2019, New Jersey Institute of Technology (NJIT))
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

#include "palisade.h"

// header files needed for serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "pubkeylp-ser.h"
#include "scheme/bfvrns/bfvrns-ser.h"

using namespace lbcrypto;

const std::string DATAFOLDER = "demoData";

int main() {
  std::cout << "This program requres the subdirectory `" << DATAFOLDER
            << "' to exist, otherwise you will get "
            << "an error writing serializations." << std::endl;
  // Sample Program: Step 1: Set CryptoContext

  // Set the main parameters
  int plaintextModulus = 65537;
  double sigma = 3.2;
  SecurityLevel securityLevel = HEStd_128_classic;
  uint32_t depth = 2;

  // Instantiate the crypto context
  CryptoContext<DCRTPoly> cryptoContext =
      CryptoContextFactory<DCRTPoly>::genCryptoContextBFVrns(
          plaintextModulus, securityLevel, sigma, 0, depth, 0, OPTIMIZED);

  // Enable features that you wish to use
  cryptoContext->HESea_Enable(ENCRYPTION);
  cryptoContext->HESea_Enable(SHE);

  std::cout << "\nThe cryptocontext has been generated." << std::endl;

  // Serialize cryptocontext
  if (!Serial::SerializeToFile(DATAFOLDER + "/cryptocontext.txt", cryptoContext,
                               SerType::BINARY)) {
    std::cerr << "Error writing serialization of the crypto context to "
                 "cryptocontext.txt"
              << std::endl;
    return 1;
  }
  std::cout << "The cryptocontext has been serialized." << std::endl;

  // Sample Program: Step 2: Key Generation

  // Initialize Public Key Containers
  LPKeyPair<DCRTPoly> keyPair;

  // Generate a public/private key pair
  keyPair = cryptoContext->HESea_KeyGen();

  std::cout << "The key pair has been generated." << std::endl;

  // Serialize the public key
  if (!Serial::SerializeToFile(DATAFOLDER + "/key-public.txt",
                               keyPair.publicKey, SerType::BINARY)) {
    std::cerr << "Error writing serialization of public key to key-public.txt"
              << std::endl;
    return 1;
  }
  std::cout << "The public key has been serialized." << std::endl;

  // Serialize the secret key
  if (!Serial::SerializeToFile(DATAFOLDER + "/key-private.txt",
                               keyPair.secretKey, SerType::BINARY)) {
    std::cerr << "Error writing serialization of private key to key-private.txt"
              << std::endl;
    return 1;
  }
  std::cout << "The secret key has been serialized." << std::endl;

  // Generate the relinearization key
  cryptoContext->HESea_EvalMultKeyGen(keyPair.secretKey);

  std::cout << "The eval mult keys have been generated." << std::endl;

  // Serialize the relinearization (evaluation) key for homomorphic
  // multiplication
  std::ofstream emkeyfile(DATAFOLDER + "/" + "key-eval-mult.txt",
                          std::ios::out | std::ios::binary);
  if (emkeyfile.is_open()) {
    if (cryptoContext->HESea_SerializeEvalMultKey(emkeyfile, SerType::BINARY) == false) {
      std::cerr << "Error writing serialization of the eval mult keys to "
                   "key-eval-mult.txt"
                << std::endl;
      return 1;
    }
    std::cout << "The eval mult keys have been serialized." << std::endl;

    emkeyfile.close();
  } else {
    std::cerr << "Error serializing eval mult keys" << std::endl;
    return 1;
  }

  // Generate the rotation evaluation keys
  cryptoContext->HESea_EvalAtIndexKeyGen(keyPair.secretKey, {1, 2, -1, -2});

  std::cout << "The rotation keys have been generated." << std::endl;

  // Serialize the rotation keyhs
  std::ofstream erkeyfile(DATAFOLDER + "/" + "key-eval-rot.txt",
                          std::ios::out | std::ios::binary);
  if (erkeyfile.is_open()) {
    if (cryptoContext->HESea_SerializeEvalAutomorphismKey(erkeyfile, SerType::BINARY) == false) {
      std::cerr << "Error writing serialization of the eval rotation keys to "
                   "key-eval-rot.txt"
                << std::endl;
      return 1;
    }
    std::cout << "The eval rotation keys have been serialized." << std::endl;

    erkeyfile.close();
  } else {
    std::cerr << "Error serializing eval rotation keys" << std::endl;
    return 1;
  }

  // Sample Program: Step 3: Encryption

  // First plaintext vector is encoded
  std::vector<int64_t> vectorOfInts1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  Plaintext plaintext1 = cryptoContext->HESea_MakePackedPlaintext(vectorOfInts1);
  // Second plaintext vector is encoded
  std::vector<int64_t> vectorOfInts2 = {3, 2, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  Plaintext plaintext2 = cryptoContext->HESea_MakePackedPlaintext(vectorOfInts2);
  // Third plaintext vector is encoded
  std::vector<int64_t> vectorOfInts3 = {1, 2, 5, 2, 5, 6, 7, 8, 9, 10, 11, 12};
  Plaintext plaintext3 = cryptoContext->HESea_MakePackedPlaintext(vectorOfInts3);

  std::cout << "Plaintext #1: " << plaintext1 << std::endl;
  std::cout << "Plaintext #2: " << plaintext2 << std::endl;
  std::cout << "Plaintext #3: " << plaintext3 << std::endl;

  // The encoded vectors are encrypted
  auto ciphertext1 = cryptoContext->HESea_Encrypt(keyPair.publicKey, plaintext1);
  auto ciphertext2 = cryptoContext->HESea_Encrypt(keyPair.publicKey, plaintext2);
  auto ciphertext3 = cryptoContext->HESea_Encrypt(keyPair.publicKey, plaintext3);

  std::cout << "The plaintexts have been encrypted." << std::endl;

  if (!Serial::SerializeToFile(DATAFOLDER + "/" + "ciphertext1.txt",
                               ciphertext1, SerType::BINARY)) {
    std::cerr
        << "Error writing serialization of ciphertext 1 to ciphertext1.txt"
        << std::endl;
    return 1;
  }
  std::cout << "The first ciphertext has been serialized." << std::endl;

  if (!Serial::SerializeToFile(DATAFOLDER + "/" + "ciphertext2.txt",
                               ciphertext2, SerType::BINARY)) {
    std::cerr
        << "Error writing serialization of ciphertext 2 to ciphertext2.txt"
        << std::endl;
    return 1;
  }
  std::cout << "The second ciphertext has been serialized." << std::endl;

  if (!Serial::SerializeToFile(DATAFOLDER + "/" + "ciphertext3.txt",
                               ciphertext3, SerType::BINARY)) {
    std::cerr
        << "Error writing serialization of ciphertext 3 to ciphertext3.txt"
        << std::endl;
    return 1;
  }
  std::cout << "The third ciphertext has been serialized." << std::endl;


  // Sample Program: Step 4: Evaluation

  // PALISADE maintains an internal map of CryptoContext objects which are
  // indexed by a tag and the tag is applied to both the CryptoContext and some
  // of the keys. When deserializing a context, PALISADE checks for the tag and
  // if it finds it in the CryptoContext map, it will return the stored version.
  // Hence, we need to clear the context and clear the keys.
  cryptoContext->HESea_ClearEvalMultKeys();
  cryptoContext->HESea_ClearEvalAutomorphismKeys();
  lbcrypto::CryptoContextFactory<lbcrypto::DCRTPoly>::ReleaseAllContexts();

  // Deserialize the crypto context
  CryptoContext<DCRTPoly> cc;
  if (!Serial::DeserializeFromFile(DATAFOLDER + "/cryptocontext.txt", cc,
                                   SerType::BINARY)) {
    std::cerr << "I cannot read serialization from "
              << DATAFOLDER + "/cryptocontext.txt" << std::endl;
    return 1;
  }
  std::cout << "The cryptocontext has been deserialized." << std::endl;

  LPPublicKey<DCRTPoly> pk;
  if (Serial::DeserializeFromFile(DATAFOLDER + "/key-public.txt", pk,
                                  SerType::BINARY) == false) {
    std::cerr << "Could not read public key" << std::endl;
    return 1;
  }
  std::cout << "The public key has been deserialized." << std::endl;

  std::ifstream emkeys(DATAFOLDER + "/key-eval-mult.txt",
                       std::ios::in | std::ios::binary);
  if (!emkeys.is_open()) {
    std::cerr << "I cannot read serialization from "
              << DATAFOLDER + "/key-eval-mult.txt" << std::endl;
    return 1;
  }
  if (cc->HESea_DeserializeEvalMultKey(emkeys, SerType::BINARY) == false) {
    std::cerr << "Could not deserialize the eval mult key file" << std::endl;
    return 1;
  }
  std::cout << "Deserialized the eval mult keys." << std::endl;

  std::ifstream erkeys(DATAFOLDER + "/key-eval-rot.txt",
                       std::ios::in | std::ios::binary);
  if (!erkeys.is_open()) {
    std::cerr << "I cannot read serialization from "
              << DATAFOLDER + "/key-eval-rot.txt" << std::endl;
    return 1;
  }
  if (cc->HESea_DeserializeEvalAutomorphismKey(erkeys, SerType::BINARY) == false) {
    std::cerr << "Could not deserialize the eval rotation key file"
              << std::endl;
    return 1;
  }
  std::cout << "Deserialized the eval rotation keys." << std::endl;

  Ciphertext<DCRTPoly> ct1;
  if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext1.txt", ct1,
                                  SerType::BINARY) == false) {
    std::cerr << "Could not read the ciphertext" << std::endl;
    return 1;
  }
  std::cout << "The first ciphertext has been deserialized." << std::endl;

  Ciphertext<DCRTPoly> ct2;
  if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext2.txt", ct2,
                                  SerType::BINARY) == false) {
    std::cerr << "Could not read the ciphertext" << std::endl;
    return 1;
  }
  std::cout << "The second ciphertext has been deserialized." << std::endl;

  Ciphertext<DCRTPoly> ct3;
  if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext3.txt", ct3,
                                  SerType::BINARY) == false) {
    std::cerr << "Could not read the ciphertext" << std::endl;
    return 1;
  }
  std::cout << "The third ciphertext has been deserialized." << std::endl;

  // Homomorphic additions
  auto ciphertextAdd12 = cc->HESea_EvalAdd(ct1, ct2);//iphertext2);
  auto ciphertextAddResult = cc->HESea_EvalAdd(ciphertextAdd12, ct3);//iphertext3);

  // Homomorphic multiplications
  auto ciphertextMul12 = cc->HESea_EvalMult(ct1, ct2);//iphertext2);
  auto ciphertextMultResult = cc->HESea_EvalMult(ciphertextMul12, ct3);//iphertext3);

  // Homomorphic rotations
  auto ciphertextRot1 = cc->HESea_EvalAtIndex(ct1, 1);
  auto ciphertextRot2 = cc->HESea_EvalAtIndex(ct1, 2);
  auto ciphertextRot3 = cc->HESea_EvalAtIndex(ct1, -1);
  auto ciphertextRot4 = cc->HESea_EvalAtIndex(ct1, -2);

  // Sample Program: Step 5: Decryption

  LPPrivateKey<DCRTPoly> sk;
  if (Serial::DeserializeFromFile(DATAFOLDER + "/key-private.txt", sk,
                                  SerType::BINARY) == false) {
    std::cerr << "Could not read secret key" << std::endl;
    return 1;
  }
  std::cout << "The secret key has been deserialized." << std::endl;

  // Decrypt the result of additions
  Plaintext plaintextAddResult;
  cc->HESea_Decrypt(sk, ciphertextAddResult, &plaintextAddResult);

  // Decrypt the result of multiplications
  Plaintext plaintextMultResult;
  cc->HESea_Decrypt(sk, ciphertextMultResult, &plaintextMultResult);

  // Decrypt the result of rotations
  Plaintext plaintextRot1;
  cc->HESea_Decrypt(sk, ciphertextRot1, &plaintextRot1);
  Plaintext plaintextRot2;
  cc->HESea_Decrypt(sk, ciphertextRot2, &plaintextRot2);
  Plaintext plaintextRot3;
  cc->HESea_Decrypt(sk, ciphertextRot3, &plaintextRot3);
  Plaintext plaintextRot4;
  cc->HESea_Decrypt(sk, ciphertextRot4, &plaintextRot4);

  // Shows only the same number of elements as in the original plaintext vector
  // By default it will show all coefficients in the BFV-encoded polynomial
  plaintextRot1->SetLength(vectorOfInts1.size());
  plaintextRot2->SetLength(vectorOfInts1.size());
  plaintextRot3->SetLength(vectorOfInts1.size());
  plaintextRot4->SetLength(vectorOfInts1.size());

  // Output results
  std::cout << "\nResults of homomorphic computations" << std::endl;
  std::cout << "#1 + #2 + #3: " << plaintextAddResult << std::endl;
  std::cout << "#1 * #2 * #3: " << plaintextMultResult << std::endl;
  std::cout << "Left rotation of #1 by 1: " << plaintextRot1 << std::endl;
  std::cout << "Left rotation of #1 by 2: " << plaintextRot2 << std::endl;
  std::cout << "Right rotation of #1 by 1: " << plaintextRot3 << std::endl;
  std::cout << "Right rotation of #1 by 2: " << plaintextRot4 << std::endl;
  return 0;
}
