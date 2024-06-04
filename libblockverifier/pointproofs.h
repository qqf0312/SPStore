#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
extern "C"{

#define COMMIT_LEN 49

#define COMMIT_LEN 97

#define POINTPROOFSG1_LEN 48

#define POINTPROOFSG1_LEN 96

#define POINTPROOFSG2_LEN 96

#define POINTPROOFSG2_LEN 48

#define PROOF_LEN 49

#define PROOF_LEN 97

/**
 * deserialized prover parameter struct
 */
typedef struct {
  void *data;
} pointproofs_pp;

/**
 * values
 */
typedef struct {
  const uint8_t *data;
  size_t len;
} pointproofs_value;

/**
 * deserialized commitment struct
 */
typedef struct {
  void *data;
} pointproofs_commitment;

/**
 * serialized commitment struct
 */
typedef struct {
  uint8_t *data;
  uintptr_t len;
} pointproofs_commitment_bytes;

/**
 * serialized prover parameter struct
 */
typedef struct {
  uint8_t *data;
  uintptr_t len;
} pointproofs_pp_bytes;

/**
 * deserialized proof struct
 */
typedef struct {
  void *data;
} pointproofs_proof;

/**
 * serialized proof struct
 */
typedef struct {
  uint8_t *data;
  uintptr_t len;
} pointproofs_proof_bytes;

/**
 * deserialized verifier parameter struct
 */
typedef struct {
  void *data;
} pointproofs_vp;

/**
 * serialized verifer parameter struct
 */
typedef struct {
  uint8_t *data;
  uintptr_t len;
} pointproofs_vp_bytes;

/**
 * non-serialized
 */
typedef struct {
  pointproofs_pp prover;
  pointproofs_vp verifier;
} pointproofs_params;

/**
 * Generate a commitment
 */
int32_t pointproofs_commit(pointproofs_pp prover,
                           const pointproofs_value *values,
                           uintptr_t n,
                           pointproofs_commitment *commit);

/**
 * Deserializeing bytes into commitments
 */
int32_t pointproofs_commit_deserial(pointproofs_commitment_bytes commit_bytes,
                                    pointproofs_commitment *commit);

/**
 * Serializing commitments into bytes
 */
int32_t pointproofs_commit_serial(pointproofs_commitment commit,
                                  pointproofs_commitment_bytes *bytes);

/**
 * update an existing commitment
 */
int32_t pointproofs_commit_update(pointproofs_pp prover,
                                  pointproofs_commitment com,
                                  size_t changed_idx,
                                  pointproofs_value val_old,
                                  pointproofs_value val_new,
                                  pointproofs_commitment *new_com);

/**
 * Free commitment
 */
void pointproofs_free_commit(pointproofs_commitment commit);

void pointproofs_free_commit_string(pointproofs_commitment_bytes buf);

void pointproofs_free_pp_string(pointproofs_pp_bytes buf);

/**
 * Free proof
 */
void pointproofs_free_proof(pointproofs_proof proof);

void pointproofs_free_proof_string(pointproofs_proof_bytes buf);

/**
 * Free prover parameter
 */
void pointproofs_free_prover_params(pointproofs_pp pp);

/**
 * Free verifier parameter
 */
void pointproofs_free_verifier_params(pointproofs_vp vp);

void pointproofs_free_vp_string(pointproofs_vp_bytes buf);

/**
 * Generating a pair of parameters
 */
int32_t pointproofs_paramgen(const uint8_t *seedbuf,
                             size_t seedlen,
                             uint8_t ciphersuite,
                             size_t n,
                             pointproofs_params *param);

int32_t pointproofs_pp_deserial(pointproofs_pp_bytes pprover, pointproofs_pp *prover);

/**
 * Serializing a prove parameter into bytes
 */
int32_t pointproofs_pp_serial(pointproofs_pp pprover, pointproofs_pp_bytes *bytes);

/**
 * Deserializeing bytes into proofs
 */
int32_t pointproofs_proof_deserial(pointproofs_proof_bytes proof_bytes, pointproofs_proof *proof);

/**
 * Serializing proofs into bytes
 */
int32_t pointproofs_proof_serial(pointproofs_proof proof, pointproofs_proof_bytes *bytes);

/**
 * update an existing proof
 */
int32_t pointproofs_proof_update(pointproofs_pp prover,
                                 pointproofs_proof proof,
                                 size_t idx,
                                 size_t changed_idx,
                                 pointproofs_value val_old,
                                 pointproofs_value val_new,
                                 pointproofs_proof *new_proof);

/**
 * Generate a proof
 */
int32_t pointproofs_prove(pointproofs_pp prover,
                          const pointproofs_value *values,
                          uintptr_t n,
                          size_t idx,
                          pointproofs_proof *proof);

/**
 * aggregate proofs within a same commitment
 */
int32_t pointproofs_same_commit_aggregate(pointproofs_commitment com,
                                          const pointproofs_proof *proofs,
                                          const size_t *set,
                                          const pointproofs_value *values,
                                          size_t nvalues,
                                          size_t param_n,
                                          pointproofs_proof *agg_proof);

/**
 * verify an aggregated proof within a same commitment
 */
bool pointproofs_same_commit_batch_verify(pointproofs_vp verifier,
                                          pointproofs_commitment com,
                                          pointproofs_proof proof,
                                          const size_t *set,
                                          const pointproofs_value *values,
                                          size_t nvalues);

/**
 * verify the proof against the value and commitment
 */
bool pointproofs_verify(pointproofs_vp verifier,
                        pointproofs_commitment com,
                        pointproofs_proof proof,
                        pointproofs_value value,
                        size_t idx);

int32_t pointproofs_vp_deserial(pointproofs_vp_bytes pverifier, pointproofs_vp *verifier);

/**
 * Serializing a prove parameter into bytes
 */
int32_t pointproofs_vp_serial(pointproofs_vp pverifier, pointproofs_vp_bytes *bytes);

/**
 * aggregated proofs cross commitments
 */
int32_t pointproofs_x_commit_aggregate_full(const pointproofs_commitment *com,
                                            const pointproofs_proof *proof,
                                            const size_t *set,
                                            const pointproofs_value *values,
                                            const size_t *commit_indices,
                                            size_t no_commits,
                                            size_t param_n,
                                            pointproofs_proof *x_proof);

/**
 * aggregated proofs cross commitments
 */
int32_t pointproofs_x_commit_aggregate_partial(const pointproofs_commitment *com,
                                               const pointproofs_proof *agg_proof,
                                               const size_t *set,
                                               const pointproofs_value *values,
                                               const size_t *commit_indices,
                                               size_t no_commits,
                                               size_t param_n,
                                               pointproofs_proof *x_proof);

/**
 * verify an aggregated proof across commitments
 */
bool pointproofs_x_commit_batch_verify(pointproofs_vp verifier,
                                       const pointproofs_commitment *com,
                                       pointproofs_proof proof,
                                       const size_t *set,
                                       const pointproofs_value *values,
                                       const size_t *commit_indices,
                                       size_t no_commits);
}