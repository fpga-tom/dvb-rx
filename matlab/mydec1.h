//
// File: mydec1.h
//
// Code generated for Simulink model 'mydec1'.
//
// Model version                  : 1.7
// Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
// C/C++ source code generated on : Tue Jul 25 20:41:36 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_mydec1_h_
#define RTW_HEADER_mydec1_h_
#include <stddef.h>
#include "rtwtypes.h"
#include <cmath>
#include <math.h>
#include <string.h>
#ifndef mydec1_COMMON_INCLUDES_
# define mydec1_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // mydec1_COMMON_INCLUDES_

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
  creal_T Gain[6048];                  // '<Root>/Gain'
  real_T BitDeinterleaver[756];        // '<S2>/Bit Deinterleaver'
  real_T Buffer1_CircBuff[36288];      // '<S2>/Buffer1'
  real_T Buffer2_CircBuf[4536];        // '<S2>/Buffer2'
  real_T stateMetric[64];              // '<S5>/Viterbi Decoder'
  real_T preDecodVect[3024];           // '<S5>/Viterbi Decoder'
  real_T Buffer2[2268];                // '<S2>/Buffer2'
  real_T Buffer1[756];                 // '<S2>/Buffer1'
  real_T GeneralBlockDeinterleaver[36288];// '<S2>/General Block Deinterleaver'
  real_T Demodulator[36288];           // '<S2>/Demodulator'
  real_T GeneralBlockInterleaver[36288];// '<S2>/General Block Interleaver'
  int32_T BuffOff[12];                 // '<S7>/General Multiplexed Deinterleaver1' 
  int32_T BuffBgn[12];                 // '<S7>/General Multiplexed Deinterleaver1' 
  int32_T RegSel;                      // '<S7>/General Multiplexed Deinterleaver1' 
  int32_T Buffer1_memIdx;              // '<S2>/Buffer1'
  int32_T Buffer2_inBufPtrIdx;         // '<S2>/Buffer2'
  int32_T Buffer2_outBufPtrIdx;        // '<S2>/Buffer2'
  int32_T Buffer2_bufferLength;        // '<S2>/Buffer2'
  int32_T tbPtr;                       // '<S5>/Viterbi Decoder'
  uint32_T tbState[10048];             // '<S5>/Viterbi Decoder'
  uint32_T tbInput[10048];             // '<S5>/Viterbi Decoder'
  uint8_T DelayElems[1123];            // '<S7>/General Multiplexed Deinterleaver1' 
  uint8_T Output_DSTATE;               // '<S1>/Output'
  boolean_T Delay_DSTATE[1596];        // '<S5>/Delay'
  boolean_T expEraPuncVect[3024];      // '<S5>/Viterbi Decoder'
} DW;

// Constant parameters (auto storage)
typedef struct {
  // Expression: 0.03
  //  Referenced by: '<Root>/Constant'

  real_T Constant_Value;

  // Computed Parameter: Demodulator_constellation
  //  Referenced by: '<S2>/Demodulator'

  creal_T Demodulator_constellation[64];

  // Computed Parameter: GeneralMultiplexedDeinterleaver
  //  Referenced by: '<S7>/General Multiplexed Deinterleaver1'

  int32_T GeneralMultiplexedDeinterleaver[12];

  // Computed Parameter: Demodulator_s0
  //  Referenced by: '<S2>/Demodulator'

  int32_T Demodulator_s0[192];

  // Computed Parameter: Demodulator_s1
  //  Referenced by: '<S2>/Demodulator'

  int32_T Demodulator_s1[192];

  // Pooled Parameter (Expression: elements)
  //  Referenced by:
  //    '<S2>/General Block Deinterleaver'
  //    '<S2>/General Block Interleaver'

  int32_T pooled2[36288];

  // Computed Parameter: BitDeinterleaver_Elements
  //  Referenced by: '<S2>/Bit Deinterleaver'

  int32_T BitDeinterleaver_Elements[756];

  // Computed Parameter: ViterbiDecoder_StateVec
  //  Referenced by: '<S5>/Viterbi Decoder'

  uint32_T ViterbiDecoder_StateVec[128];

  // Computed Parameter: ViterbiDecoder_OutputVec
  //  Referenced by: '<S5>/Viterbi Decoder'

  uint32_T ViterbiDecoder_OutputVec[128];

  // Computed Parameter: ViterbiDecoder_PuncVec
  //  Referenced by: '<S5>/Viterbi Decoder'

  boolean_T ViterbiDecoder_PuncVec[4];
} ConstP;

// External inputs (root inport signals with auto storage)
typedef struct {
  creal32_T decoderIn[6048];           // '<Root>/decoderIn'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  uint8_T decoderOut[189];             // '<Root>/decoderOut'
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T * volatile errorStatus;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    struct {
      uint8_T TID[3];
    } TaskCounters;
  } Timing;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Constant parameters (auto storage)
extern const ConstP rtConstP;

// Class declaration for model mydec1
class mydec1ModelClass {
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
  mydec1ModelClass();

  // Destructor
  ~mydec1ModelClass();

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
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Data Type Propagation' : Unused code path elimination
//  Block '<S3>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S4>/FixPt Data Type Duplicate1' : Unused code path elimination
//  Block '<S2>/Scope1' : Unused code path elimination


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
//  '<Root>' : 'mydec1'
//  '<S1>'   : 'mydec1/Counter Limited'
//  '<S2>'   : 'mydec1/decoder '
//  '<S3>'   : 'mydec1/Counter Limited/Increment Real World'
//  '<S4>'   : 'mydec1/Counter Limited/Wrap To Zero'
//  '<S5>'   : 'mydec1/decoder /Subsystem'
//  '<S6>'   : 'mydec1/decoder /Subsystem/Outer Deinterleaver'
//  '<S7>'   : 'mydec1/decoder /Subsystem/Outer Deinterleaver/Convolutional Deinterleaver'

#endif                                 // RTW_HEADER_mydec1_h_

//
// File trailer for generated code.
//
// [EOF]
//
