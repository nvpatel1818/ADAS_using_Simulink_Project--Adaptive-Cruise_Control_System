/* Copyright 2009-2013 The MathWorks, Inc. */
#define S_FUNCTION_NAME  scd_gatherinfo
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes =========================================*/
static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    /* Register the number and type of states the S-Function uses */
    ssSetNumContStates(    S, 0);   /* number of continuous states           */
    ssSetNumDiscStates(    S, 0);   /* number of discrete states             */
    ssAllowSignalsWithMoreThan2D(S);

    /*
     * Configure the input ports. First set the number of input ports. 
     */
    if (!ssSetNumInputPorts(S, 1)) return;    
    if(!ssSetInputPortDimensionInfo(S, 0, DYNAMIC_DIMENSION)) return;
    ssSetInputPortDataType(S, 0, DYNAMICALLY_TYPED);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    /*
     * Configure the output ports. First set the number of output ports.
     */
    if (!ssSetNumOutputPorts(S, 1)) return;
    if(!ssSetOutputPortDimensionInfo(S,0,DYNAMIC_DIMENSION)) return;
    ssSetOutputPortDataType(S, 0, DYNAMICALLY_TYPED);

    ssSetNumSampleTimes(S, PORT_BASED_SAMPLE_TIMES);
    ssSetInputPortSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOutputPortSampleTime(S, 0, INHERITED_SAMPLE_TIME);

    /*
     * Set size of the work vectors.
     */
    ssSetNumRWork(         S, 0);   /* number of real work vector elements   */
    ssSetNumIWork(         S, 0);   /* number of integer work vector elements*/
    ssSetNumPWork(         S, 0);   /* number of pointer work vector elements*/
    ssSetNumModes(         S, 0);   /* number of mode work vector elements   */
    ssSetNumNonsampledZCs( S, 0);   /* number of non-sampled zero crossings   */

    ssSetOptions(S, 0);   /* general options (SS_OPTION_xx)        */

} /* end mdlInitializeSizes */


/*************************************************************************/
/****************** SETTING PORT DIMENSIONS ******************************/
/*************************************************************************/
#if defined(MATLAB_MEX_FILE)

#define MDL_SET_INPUT_PORT_DIMENSION_INFO
void mdlSetInputPortDimensionInfo(SimStruct *S, int_T port, 
        const DimsInfo_T *dimsInfo)
{
    if (port == 0) {
        /* Set the size for all dynamic ports using dimsInfo */
        ssSetInputPortDimensionInfo(S, port,dimsInfo);
        ssSetOutputPortDimensionInfo(S, port, dimsInfo);
    } else {
        return;
    }
} /*mdlSetInputPortDimensionInfo */

#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
void mdlSetOutputPortDimensionInfo(SimStruct *S, int_T port,
        const DimsInfo_T *dimsInfo)
{
    if (port == 0) {
        ssSetOutputPortDimensionInfo(S, port,dimsInfo);
        ssSetInputPortDimensionInfo(S, port,dimsInfo);
    }   
} /* end mdlSetOutputPortDimensionInfo */

#define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
void mdlSetDefaultPortDimensionInfo(SimStruct *S)
{
    DECL_AND_INIT_DIMSINFO(dimsInfo);
    /* Either 2nd input or 2nd output should be already known */
    if (ssGetOutputPortWidth(S, 0) != DYNAMICALLY_SIZED) {
        /* It is the output that is known, get the dimensions first */
        dimsInfo.width   = ssGetOutputPortWidth(S, 0);
        dimsInfo.numDims = ssGetOutputPortNumDimensions(S, 0);
        dimsInfo.dims    = ssGetOutputPortDimensions(S, 0);
        
        /*set second input, first output if unknown */
        if (ssGetInputPortWidth(S, 0) == DYNAMICALLY_SIZED) {
            if(!ssSetInputPortDimensionInfo(S, 0, &dimsInfo)) return;
        }
        return;
     }
     else {
         if (ssGetInputPortWidth(S, 0) != DYNAMICALLY_SIZED) {
             /* It is the input that is known, get the dimensions */
             dimsInfo.width   = ssGetInputPortWidth(S, 0);
             dimsInfo.numDims = ssGetInputPortNumDimensions(S, 0);
             dimsInfo.dims    = ssGetInputPortDimensions(S, 0);
             
             /*set first and second output if unknown */
             if (ssGetOutputPortWidth(S, 0) == DYNAMICALLY_SIZED) {
                 if(!ssSetOutputPortDimensionInfo(S, 0, &dimsInfo)) return;
             }
             return;
         }
         else
         {
             /* Default everything to a scalar signal */
             ssSetOutputPortVectorDimension(S,  0, 1);
             ssSetInputPortVectorDimension(S,  0, 1);
             return;
         }
     }
} /* end mdlSetDefaultPortDimensionInfo */

#endif /* MATLAB_MEX_FILE */

/*************************************************************************/
/****************** SETTING PORT SAMPLE TIMES ****************************/
/*************************************************************************/

#define MDL_SET_INPUT_PORT_SAMPLE_TIME
#if defined(MDL_SET_INPUT_PORT_SAMPLE_TIME) && defined(MATLAB_MEX_FILE)
/* Function: mdlSetInputPortSampleTime ==================================*/
void mdlSetInputPortSampleTime(SimStruct *S,
                                      int_T     portIdx,
                                      real_T    sampleTime,
                                      real_T    offsetTime)
{
    if (portIdx == 0) {
        ssSetInputPortSampleTime(S, portIdx, sampleTime);
        ssSetInputPortOffsetTime(S, portIdx, offsetTime);
		/* Set the second outport port sample time temporarily to this,
		 * it will eventually be set in mdlSetOutputPortSampleTime.
		 * We have to do it temporarily here because there are cases 
		 * when mdlSetOutputPortSampleTime is not called at all,e.g.,
		 * when the output of S-function is connected to a scope. */     
        ssSetOutputPortSampleTime(S, 0, sampleTime);
        ssSetOutputPortOffsetTime(S, 0, offsetTime);
    } 
} /* end mdlSetInputPortSampleTime */
#endif /* MDL_SET_INPUT_PORT_SAMPLE_TIME */

#define MDL_SET_OUTPUT_PORT_SAMPLE_TIME
#if defined(MDL_SET_OUTPUT_PORT_SAMPLE_TIME) && defined(MATLAB_MEX_FILE)
/* Function: mdlSetOutputPortSampleTime =================================*/
void mdlSetOutputPortSampleTime(SimStruct *S,
                                      int_T     portIdx,
                                      real_T    sampleTime,
                                      real_T    offsetTime)
{
    /* Set input port sample time as well */
    if (portIdx == 0) {
        ssSetOutputPortSampleTime(S, portIdx, sampleTime);
        ssSetOutputPortOffsetTime(S, portIdx, offsetTime);
        ssSetInputPortSampleTime(S, 0, sampleTime);
        ssSetInputPortOffsetTime(S, 0, offsetTime);
    }
} /* end mdlSetOutputPortSampleTime */
#endif /* MDL_SET_OUTPUT_PORT_SAMPLE_TIME */

/* Function: mdlInitializeSampleTimes ===================================*/
void mdlInitializeSampleTimes(SimStruct *S)
{
} /*end mdlInitializeSampleTimes */

/*************************************************************************/
/****************** SETTING PORT DATA TYPES ******************************/
/*************************************************************************/

#define MDL_SET_INPUT_PORT_DATA_TYPE
#if defined(MDL_SET_INPUT_PORT_DATA_TYPE) && defined(MATLAB_MEX_FILE)
  /* Function: mdlSetInputPortDataType ==================================*/
  static void mdlSetInputPortDataType(SimStruct *S, int portIndex,DTypeId dType)
  {
    ssSetInputPortDataType(S,portIndex,dType);
    ssSetOutputPortDataType(S,portIndex,dType);
  } /* end mdlSetInputPortDataType */
#endif /* MDL_SET_INPUT_PORT_DATA_TYPE */

#define MDL_SET_OUTPUT_PORT_DATA_TYPE  
#if defined(MDL_SET_OUTPUT_PORT_DATA_TYPE) && defined(MATLAB_MEX_FILE)
  /* Function: mdlSetOutputPortDataType =================================*/
  static void mdlSetOutputPortDataType(SimStruct *S,int portIndex,DTypeId dType)
  {
    ssSetOutputPortDataType(S,portIndex,dType);
    ssSetInputPortDataType(S,portIndex,dType);
  } /* end mdlSetOutputPortDataType */
#endif /* MDL_SET_OUTPUT_PORT_DATA_TYPE */

/* Function: mdlOutputs =================================================*/
static void mdlOutputs(SimStruct *S, int_T tid)
{
} /* end mdlOutputs */

/* Function: mdlTerminate ===============================================*/
static void mdlTerminate(SimStruct *S)
{
}

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
