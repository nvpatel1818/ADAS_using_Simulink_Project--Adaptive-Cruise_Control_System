/* Copyright 2009-2013 The MathWorks, Inc. */
#define S_FUNCTION_NAME  scd_injection_input
#define S_FUNCTION_LEVEL 2

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
#include "string.h"

/* Define the parameters */
enum {INPUT_SIGNAL = 0,IS_VARIABLE_TS,SAMPLE_TIME};
#define INPUT_SIGNAL_ARG(S)      (ssGetSFcnParam(S,INPUT_SIGNAL))
#define IS_VARIABLE_TS_ARG(S)    (ssGetSFcnParam(S,IS_VARIABLE_TS))
#define SAMPLE_TIME_ARG(S)       (ssGetSFcnParam(S,SAMPLE_TIME))
        
/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================*/
static void mdlInitializeSizes(SimStruct *S)
{
    /* This S-function has 3 parameters:
     * InputWKSVariable - The name of the variable in workspace which holds the input
     * as 1 or 2 column matrix in the form of [u] or [t u]. 2 column format is used 
     * when variable sample time option is set to 1 as we need timestamps.
     * IsVariableTs - If 1, this S-function will be set to be variable sample time.
     * if 0, it will have the discrete rate specified in the third parameter:
     * InputSampleTime - sample time of input signal, ignored for variable sample time */
    ssSetNumSFcnParams(S, 3);
    
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    

    if (!ssSetNumInputPorts(S, 0)) return;

    if (!ssSetNumOutputPorts(S, 1)) return;
    /* Output is always a scalar of type double */
    ssSetOutputPortWidth(S, 0, 1);
	ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    
    ssSetNumSampleTimes(S, 1);
    /* Initialize DWork to hold the current index of the time/input*/
    ssSetNumDWork(S,1);
    ssSetDWorkWidth(S,0,1);
    ssSetDWorkDataType(S,0,7); /* SS_UINT32 */
    
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetOptions(S, 0);
    
}
/* Function: mdlInitializeSampleTimes =====================================
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    const real_T  *isVariableTs = mxGetPr(IS_VARIABLE_TS_ARG(S));
    const real_T  *sampletime = mxGetPr(SAMPLE_TIME_ARG(S));
    if (isVariableTs[0] == 1) { 
        ssSetSampleTime(S,0,VARIABLE_SAMPLE_TIME);
    }
    else {
        ssSetSampleTime(S,0, (time_T) sampletime[0]);
        ssSetOffsetTime(S,0, 0.0);
    }
}

#define MDL_GET_TIME_OF_NEXT_VAR_HIT
static void mdlGetTimeOfNextVarHit(SimStruct *S)
{
    time_T timeOfNextHit;
    uint32_T *curindex = (uint32_T*) ssGetDWork(S,0);  
    const real_T *inputsignal = mxGetPr(INPUT_SIGNAL_ARG(S));
    /* Determine the next hit from time vector if we are not at the final time step.*/ 
    if (ssGetT(S) != ssGetTFinal(S)) {
             
        timeOfNextHit = inputsignal[curindex[0]+1]; 
    }
    else {
        /* If we are at the final step, set it to 1 second later since we will not hit it in any case.*/
        timeOfNextHit = ssGetT(S)+1;
    }
    ssSetTNext(S, timeOfNextHit);
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
/* Function: mdlStart =====================================================
 */
static void mdlStart(SimStruct *S)
  {
    uint32_T *curindex = (uint32_T*) ssGetDWork(S,0);
    /* Initialize the index to zero */
    curindex[0] = 0;
  }
#endif /*  MDL_START */

/* Function: mdlOutputs ===================================================
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Declare variables to read the data */
    size_t          numRows = mxGetM(INPUT_SIGNAL_ARG(S));
    real_T          *signalvalues = mxGetPr(INPUT_SIGNAL_ARG(S));
    real_T          *y = ssGetOutputPortRealSignal(S,0);
    /* Get the current index */
    uint32_T    *curindex = (uint32_T*) ssGetDWork(S,0);
    
    /* If it is variable time step, the input signal is in the form of 
     * [t u], 2 column. If it is not variable sample time, it is only [u] with
     * a single column. */
    if (ssGetSampleTime(S,0) != VARIABLE_SAMPLE_TIME) {
        /* The input is expected to be of a single column */
        if (curindex[0]>= numRows)
            /* Speak out the final value for later instances */
            y[0] = signalvalues[numRows-1];
        else
            y[0] = signalvalues[curindex[0]];
        /* Increment the index */
        curindex[0]++;
    }
    else {
        /* The input is expected to be of 2 columns */
        if (curindex[0]>= numRows)
            /* Speak out the final value for later instances, accountting 
             * for time column. */
            y[0] = signalvalues[2*numRows-1];
        else
            y[0] = signalvalues[curindex[0]+numRows];
        curindex[0]++;  
    }   
}

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


