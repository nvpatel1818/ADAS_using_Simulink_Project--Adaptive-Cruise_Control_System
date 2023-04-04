/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ACC_System.c
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

#include "ACC_System.h"
#include "rtwtypes.h"
#include "ACC_System_private.h"
#include <math.h>
#include "ACC_control_output.h"

/* Named constants for Chart: '<S1>/Control Algorithm' */
#define ACC_IN_LeadVehicle_Not_Detected ((uint8_T)3U)
#define ACC_System_IN_ACC_OFF_Mode     ((uint8_T)1U)
#define ACC_System_IN_ACC_ON_Mode      ((uint8_T)2U)
#define ACC_System_IN_ACC_STANDBY_Mode ((uint8_T)3U)
#define ACC_System_IN_NO_ACTIVE_CHILD  ((uint8_T)0U)
#define IN_LeadVehicle_Detected_Follow ((uint8_T)1U)
#define IN_LeadVehicle_Detected_Resume ((uint8_T)2U)
#define IN_LeadVehicle_Not_Detected_Res ((uint8_T)4U)
#define IN_LeadVehicle_Speed_equal_Set_ ((uint8_T)5U)
#define IN_LeadVehicle_Speed_lessthan_S ((uint8_T)6U)

/* Block states (default storage) */
DW_ACC_System_T ACC_System_DW;

/* External outputs (root outports fed by signals with default storage) */
ExtY_ACC_System_T ACC_System_Y;

/* Real-time model */
static RT_MODEL_ACC_System_T ACC_System_M_;
RT_MODEL_ACC_System_T *const ACC_System_M = &ACC_System_M_;
real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Model step function */
void ACC_System_step(void)
{
  /* Sum: '<S3>/Add' incorporates:
   *  Inport: '<Root>/CameraInput_LeadVehicle'
   *  Inport: '<Root>/RadarInput_LeadVehicle'
   */
  LeadVehicle_Speed = (uint8_T)(CameraInput_LeadVehicle + RadarInput_LeadVehicle);

  /* UnitDelay: '<S1>/Unit Delay' */
  Acceleration_Mode = ACC_System_Y.Acceleration_Mode_h;

  /* Sum: '<S4>/Add' incorporates:
   *  Inport: '<Root>/CameraInput_DriveVehicle'
   *  Inport: '<Root>/RadarInput_DriveVehicle'
   */
  DriveVehicle_Speed = (uint8_T)((uint8_T)(CameraInput_DriveVehicle +
    RadarInput_DriveVehicle) + Acceleration_Mode);

  /* SignalConversion: '<S4>/Signal Conversion' incorporates:
   *  Inport: '<Root>/RadarInput_DriveVehicle'
   */
  LeadVehicle_Detected = RadarInput_DriveVehicle;

  /* Chart: '<S1>/Control Algorithm' incorporates:
   *  Inport: '<Root>/CruiseSwitch'
   *  Inport: '<Root>/SetSwitch'
   *  Inport: '<Root>/Set_Gap'
   *  Inport: '<Root>/Set_Speed'
   *  Inport: '<Root>/Time_Gap'
   *  UnitDelay: '<S1>/Unit Delay'
   */
  if (ACC_System_DW.is_active_c3_ACC_System == 0U) {
    ACC_System_DW.is_active_c3_ACC_System = 1U;
    ACC_System_DW.is_c3_ACC_System = ACC_System_IN_ACC_OFF_Mode;
    ACC_System_Y.Acceleration_Mode_h = 0U;
  } else {
    switch (ACC_System_DW.is_c3_ACC_System) {
     case ACC_System_IN_ACC_OFF_Mode:
      ACC_System_Y.Acceleration_Mode_h = 0U;
      if (CruiseSwitch) {
        ACC_System_DW.is_c3_ACC_System = ACC_System_IN_ACC_STANDBY_Mode;
        ACC_System_Y.Acceleration_Mode_h = 1U;
      }
      break;

     case ACC_System_IN_ACC_ON_Mode:
      {
        if (!SetSwitch) {
          ACC_System_DW.is_ACC_ON_Mode = ACC_System_IN_NO_ACTIVE_CHILD;
          ACC_System_DW.is_c3_ACC_System = ACC_System_IN_ACC_STANDBY_Mode;
          ACC_System_Y.Acceleration_Mode_h = 1U;
        } else if (!CruiseSwitch) {
          ACC_System_DW.is_ACC_ON_Mode = ACC_System_IN_NO_ACTIVE_CHILD;
          ACC_System_DW.is_c3_ACC_System = ACC_System_IN_ACC_OFF_Mode;
          ACC_System_Y.Acceleration_Mode_h = 0U;
        } else {
          switch (ACC_System_DW.is_ACC_ON_Mode) {
           case IN_LeadVehicle_Detected_Follow:
            ACC_System_Y.Acceleration_Mode_h = 2U;
            if (LeadVehicle_Detected == 0) {
              ACC_System_DW.is_ACC_ON_Mode = ACC_IN_LeadVehicle_Not_Detected;
              ACC_System_Y.Acceleration_Mode_h = 1U;
            } else if (((LeadVehicle_Detected == 1) && (LeadVehicle_Speed <
                         Set_Speed)) || (Time_Gap < Set_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Speed_lessthan_S;
              ACC_System_Y.Acceleration_Mode_h = 4U;
            }
            break;

           case IN_LeadVehicle_Detected_Resume:
            ACC_System_Y.Acceleration_Mode_h = 3U;
            if (LeadVehicle_Detected == 0) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Not_Detected_Res;
              ACC_System_Y.Acceleration_Mode_h = 1U;
            } else if ((DriveVehicle_Speed < Set_Speed) && (LeadVehicle_Speed >
                        DriveVehicle_Speed) && (Time_Gap >= Set_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Speed_equal_Set_;
              ACC_System_Y.Acceleration_Mode_h = 5U;
            } else if ((DriveVehicle_Speed == Set_Speed) && (LeadVehicle_Speed >=
                        Set_Speed) && (Time_Gap >= Set_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Detected_Follow;
              ACC_System_Y.Acceleration_Mode_h = 2U;
            }
            break;

           case ACC_IN_LeadVehicle_Not_Detected:
            ACC_System_Y.Acceleration_Mode_h = 1U;
            if ((LeadVehicle_Detected == 1) && (DriveVehicle_Speed == Set_Speed)
                && (LeadVehicle_Speed >= Set_Speed) && (Time_Gap >= Set_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Detected_Follow;
              ACC_System_Y.Acceleration_Mode_h = 2U;
            } else if (((LeadVehicle_Detected == 1) && (LeadVehicle_Speed <
                         Set_Speed)) || (Time_Gap < Set_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Speed_lessthan_S;
              ACC_System_Y.Acceleration_Mode_h = 4U;
            }
            break;

           case IN_LeadVehicle_Not_Detected_Res:
            ACC_System_Y.Acceleration_Mode_h = 1U;
            break;

           case IN_LeadVehicle_Speed_equal_Set_:
            ACC_System_Y.Acceleration_Mode_h = 5U;
            if (((LeadVehicle_Speed < Set_Speed) && (LeadVehicle_Speed <
                  DriveVehicle_Speed)) || ((int32_T)rt_roundd_snf(0.75 * (real_T)
                  Set_Gap) == Time_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Speed_lessthan_S;
              ACC_System_Y.Acceleration_Mode_h = 4U;
            } else if (((DriveVehicle_Speed < Set_Speed) && (LeadVehicle_Speed >
              DriveVehicle_Speed)) || (Time_Gap >= Set_Gap)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Detected_Resume;
              ACC_System_Y.Acceleration_Mode_h = 3U;
            } else if ((LeadVehicle_Detected == 0) || (DriveVehicle_Speed <=
                        Set_Speed)) {
              ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Not_Detected_Res;
              ACC_System_Y.Acceleration_Mode_h = 1U;
            }
            break;

           default:
            {
              int32_T tmp;
              int32_T tmp_0;
              uint8_T tmp_1;
              uint8_T tmp_2;

              /* case IN_LeadVehicle_Speed_lessthan_Set_Speed: */
              ACC_System_Y.Acceleration_Mode_h = 4U;
              tmp = (int32_T)rt_roundd_snf((real_T)LeadVehicle_Speed * 1.25);
              tmp_0 = (int32_T)rt_roundd_snf(1.25 * (real_T)Set_Gap);
              if (tmp < 256) {
                tmp_1 = (uint8_T)tmp;
              } else {
                tmp_1 = MAX_uint8_T;
              }

              if (tmp_0 < 256) {
                tmp_2 = (uint8_T)tmp_0;
              } else {
                tmp_2 = MAX_uint8_T;
              }

              if ((tmp_1 >= DriveVehicle_Speed) && ((int32_T)rt_roundd_snf
                   ((real_T)LeadVehicle_Speed * 0.75) <= DriveVehicle_Speed) &&
                  (DriveVehicle_Speed < Set_Speed) && (Time_Gap <= tmp_2) &&
                  (Time_Gap >= (int32_T)rt_roundd_snf(0.75 * (real_T)Set_Gap)))
              {
                ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Speed_equal_Set_;
                ACC_System_Y.Acceleration_Mode_h = 5U;
              } else if ((LeadVehicle_Detected == 0) && (DriveVehicle_Speed ==
                          Set_Speed)) {
                ACC_System_DW.is_ACC_ON_Mode = ACC_IN_LeadVehicle_Not_Detected;
                ACC_System_Y.Acceleration_Mode_h = 1U;
              }
            }
            break;
          }
        }
      }
      break;

     default:
      /* case IN_ACC_STANDBY_Mode: */
      ACC_System_Y.Acceleration_Mode_h = 1U;
      if (!CruiseSwitch) {
        ACC_System_DW.is_c3_ACC_System = ACC_System_IN_ACC_OFF_Mode;
        ACC_System_Y.Acceleration_Mode_h = 0U;
      } else if (SetSwitch) {
        ACC_System_DW.is_c3_ACC_System = ACC_System_IN_ACC_ON_Mode;
        ACC_System_DW.is_ACC_ON_Mode = IN_LeadVehicle_Detected_Follow;
        ACC_System_Y.Acceleration_Mode_h = 2U;
      }
      break;
    }
  }

  /* End of Chart: '<S1>/Control Algorithm' */
}

/* Model initialize function */
void ACC_System_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void ACC_System_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
