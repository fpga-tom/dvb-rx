//
// File: myviterbi.h
//
// Code generated for Simulink model 'myviterbi'.
//
// Model version                  : 1.5
// Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
// C/C++ source code generated on : Sat Jul 29 13:37:56 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_myviterbi_h_
#define RTW_HEADER_myviterbi_h_
#include "rtwtypes.h"
#include <string.h>
#ifndef myviterbi_COMMON_INCLUDES_
# define myviterbi_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // myviterbi_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

#ifndef DEFINED_TYPEDEF_FOR_struct_QhmzmAeprygQHmE6UsjyAG_
#define DEFINED_TYPEDEF_FOR_struct_QhmzmAeprygQHmE6UsjyAG_

typedef struct {
  real_T numInputSymbols;
  real_T numOutputSymbols;
  real_T numStates;
  real_T nextStates[128];
  real_T outputs[128];
} struct_QhmzmAeprygQHmE6UsjyAG;

#endif

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  int32_T stateMetric[64];             // '<Root>/Viterbi Decoder'
  int32_T tbPtr;                       // '<Root>/Viterbi Decoder'
  uint32_T tbState[9792];              // '<Root>/Viterbi Decoder'
  uint32_T tbInput[9792];              // '<Root>/Viterbi Decoder'
  uint8_T preDecodVect[48384];         // '<Root>/Viterbi Decoder'
  boolean_T expEraPuncVect[48384];     // '<Root>/Viterbi Decoder'
  boolean_T ViterbiDecoder[24192];     // '<Root>/Viterbi Decoder'
} DW;

// Constant parameters (auto storage)
typedef struct {
  // Computed Parameter: ViterbiDecoder_StateVec
  //  Referenced by: '<Root>/Viterbi Decoder'

  uint32_T ViterbiDecoder_StateVec[128];

  // Computed Parameter: ViterbiDecoder_OutputVec
  //  Referenced by: '<Root>/Viterbi Decoder'

  uint32_T ViterbiDecoder_OutputVec[128];

  // Computed Parameter: ViterbiDecoder_PuncVec
  //  Referenced by: '<Root>/Viterbi Decoder'

  boolean_T ViterbiDecoder_PuncVec[4];
} ConstP;

// External inputs (root inport signals with auto storage)
typedef struct {
  uint8_T In1[36288];                  // '<Root>/In1'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  uint8_T Out1[3024];                  // '<Root>/Out1'
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T * volatile errorStatus;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Constant parameters (auto storage)
extern const ConstP rtConstP;

// Class declaration for model myviterbi
class myviterbiModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  myviterbiModelClass();

  // Destructor
  ~myviterbiModelClass();

  // Real-Time Model get method
  RT_MODEL * getRTM();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;

  // Real-Time Model
  RT_MODEL rtM;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'myviterbi'

#endif                                 // RTW_HEADER_myviterbi_h_

//
// File trailer for generated code.
//
// [EOF]
//
