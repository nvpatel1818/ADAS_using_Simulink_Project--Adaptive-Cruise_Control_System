/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ACC_System.h
 *
 * Code generated for Simulink model 'ACC_System'.
 *
 * Model version                  : 1.35
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Sat Apr  1 00:48:55 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_ACC_System_h_
#define RTW_HEADER_ACC_System_h_
#ifndef ACC_System_COMMON_INCLUDES_
#define ACC_System_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* ACC_System_COMMON_INCLUDES_ */

#include "ACC_System_types.h"

/* Includes for objects with custom storage classes */
#include "ACC_control_output.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint8_T is_active_c3_ACC_System;     /* '<S1>/Control Algorithm' */
  uint8_T is_c3_ACC_System;            /* '<S1>/Control Algorithm' */
  uint8_T is_ACC_ON_Mode;              /* '<S1>/Control Algorithm' */
} DW_ACC_System_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint8_T Acceleration_Mode_h;         /* '<Root>/Acceleration_Mode' */
} ExtY_ACC_System_T;

/* Real-time Model Data Structure */
struct tag_RTM_ACC_System_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_ACC_System_T ACC_System_DW;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_ACC_System_T ACC_System_Y;

/* Model entry point functions */
extern void ACC_System_initialize(void);
extern void ACC_System_step(void);
extern void ACC_System_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ACC_System_T *const ACC_System_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Scope' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ACC_System'
 * '<S1>'   : 'ACC_System/Subsystem'
 * '<S2>'   : 'ACC_System/Subsystem/Control Algorithm'
 * '<S3>'   : 'ACC_System/Subsystem/Subsystem'
 * '<S4>'   : 'ACC_System/Subsystem/Subsystem1'
 */
#endif                                 /* RTW_HEADER_ACC_System_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
