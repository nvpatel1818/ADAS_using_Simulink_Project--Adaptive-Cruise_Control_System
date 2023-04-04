/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ACC_System_private.h
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

#ifndef RTW_HEADER_ACC_System_private_h_
#define RTW_HEADER_ACC_System_private_h_
#include "rtwtypes.h"

/* Imported (extern) block signals */
extern uint8_T CameraInput_LeadVehicle;/* '<Root>/CameraInput_LeadVehicle' */
extern uint8_T RadarInput_LeadVehicle; /* '<Root>/RadarInput_LeadVehicle' */
extern uint8_T CameraInput_DriveVehicle;/* '<Root>/CameraInput_DriveVehicle' */
extern uint8_T RadarInput_DriveVehicle;/* '<Root>/RadarInput_DriveVehicle' */
extern uint8_T Time_Gap;               /* '<Root>/Time_Gap' */
extern uint8_T Set_Speed;              /* '<Root>/Set_Speed' */
extern boolean_T CruiseSwitch;         /* '<Root>/CruiseSwitch' */
extern uint8_T Set_Gap;                /* '<Root>/Set_Gap' */
extern boolean_T SetSwitch;            /* '<Root>/SetSwitch' */
extern real_T rt_roundd_snf(real_T u);

#endif                                 /* RTW_HEADER_ACC_System_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
