/* Copyright 2009-2013 The MathWorks, Inc. */
#define S_FUNCTION_NAME  scd_injection_main
#define S_FUNCTION_LEVEL 2

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
#include "string.h"
    

/* Define different actions */
enum {DIRECT_PASS_ACTION = 0, ADD_ACTION, REPLACE_ACTION, OPEN_ACTION};        
        
/* Define arguments */        
enum {IOTYPE = 0, DATATYPE, DIMENSIONS, PORTDATASIZE, SAMPLETIME, SIGNALINDEX};
#define IOTYPE_ARG(S)         (ssGetSFcnParam(S,IOTYPE))
#define DATATYPE_ARG(S)       (ssGetSFcnParam(S,DATATYPE))
#define DIMENSIONS_ARG(S)     (ssGetSFcnParam(S,DIMENSIONS))
#define PORTDATASIZE_ARG(S)   (ssGetSFcnParam(S,PORTDATASIZE))
#define SAMPLETIME_ARG(S)     (ssGetSFcnParam(S,SAMPLETIME))
#define SIGNALINDEX_ARG(S)    (ssGetSFcnParam(S,SIGNALINDEX))

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================*/
static void mdlInitializeSizes(SimStruct *S)
{
    /* This S-function has 6 parameters:
     * 1) IOType: It is a string that determines the type of action that will be taken
     * by this S-function. It mainly depends on the type of linearization IO, 
     * and for input type IOs, depends on whether or not it is the active input that
     * signal is going to be injected from.
     * 2) DataType: The data type that the second input and output ports will have.
     * 3) Dimensions: The dimensions that second input and output ports will have.
     * 4) PortDataSize: The number of bytes that the second input port has. This will be
     * used when the second input needs to be copied over to the output ports.
     * 5) SampleTime: The sample time that the second input and output ports will share.
     * 6) SignalIndex: The index at which element of the second input we will operate on,
     * it will be used when the second input port is not a scalar. "-1" refers to "all".
     */
    /* Declare necessary variables */
    real_T     *dataType;
    real_T     *dimensions;
    int_T      *intdimensions;
    int_T      dimsCounter = 1;
    DECL_AND_INIT_DIMSINFO(dimsInfo);
    real_T     *sampleTime;
    
    
    ssSetNumSFcnParams(S, 6);
    
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssAllowSignalsWithMoreThan2D(S);
    if (!ssSetNumInputPorts(S, 2)) return;
    if (!ssSetNumOutputPorts(S, 2)) return;
    
    /* First input is a scalar of type double, coming from the output of
     * S-function "injection_input"*/
    ssSetInputPortWidth(S, 0, 1);
	ssSetInputPortDataType(S, 0, SS_DOUBLE);
    
    /* Both input are direct feed through and contiguous */
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortRequiredContiguous(S, 0, 1);
    ssSetInputPortRequiredContiguous(S, 1, 1);
    
    /*********************************************************************/
    /************************* DATA TYPE *********************************/
    /*********************************************************************/
    /* Read the data type from the second parameter and set it to second input
     * port and both output ports. */ 
    dataType = mxGetPr(DATATYPE_ARG(S));
    ssSetInputPortDataType(S, 1, (DTypeId) dataType[0]);
    ssSetOutputPortDataType(S, 0, (DTypeId) dataType[0]);
    ssSetOutputPortDataType(S, 1, (DTypeId) dataType[0]);
    
    /*********************************************************************/
    /************************* DIMENSIONS ********************************/
    /*********************************************************************/
    /* Read the dimension info from the third parameter and set it to second 
     * input and both outputs. */
    dimensions = mxGetPr(DIMENSIONS_ARG(S));
    dimsInfo.numDims = (int_T) dimensions[0];
    intdimensions = (int_T*) malloc(dimsInfo.numDims*sizeof(int_T));    
    dimsInfo.width = 1;
    for (dimsCounter = 0;dimsCounter<dimsInfo.numDims;dimsCounter++) {
        intdimensions[dimsCounter] = (int_T) dimensions[dimsCounter+1];
        dimsInfo.width *= (int_T) dimensions[dimsCounter+1];
    }
    dimsInfo.dims = (int_T*) intdimensions;
    /* Store int dimensions in user data to be able to deallocate */
    ssSetUserData(S,(void*)intdimensions);
    ssSetInputPortDimensionInfo(S,1,&dimsInfo);
    ssSetOutputPortDimensionInfo(S,0,&dimsInfo);
    ssSetOutputPortDimensionInfo(S,1,&dimsInfo);
    
    /*********************************************************************/
    /************************ SAMPLE TIME ********************************/
    /*********************************************************************/
    sampleTime = mxGetPr(SAMPLETIME_ARG(S));
    ssSetNumSampleTimes(S, PORT_BASED_SAMPLE_TIMES);
    ssSetInputPortSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetInputPortSampleTime(S, 1, sampleTime[0]);
    ssSetOutputPortSampleTime(S, 0, sampleTime[0]);
    ssSetOutputPortSampleTime(S, 1, sampleTime[0]);
    ssSetInputPortOffsetTime(S, 1, sampleTime[1]);
    ssSetOutputPortOffsetTime(S, 0, sampleTime[1]);
    ssSetOutputPortOffsetTime(S, 1, sampleTime[1]);
    
    /* Initialize DWork to hold the values of:
	 * 1. Operating point value - generic type
	 * 2. The action that the S-function should take
	 * 3. The logging location, whether input or output of the S-function */
    
    ssSetNumDWork(S,3);
    /* Operating point value */
    ssSetDWorkWidth(S,0,dimsInfo.width);
    ssSetDWorkDataType(S,0,(DTypeId) dataType[0]);
	/* The action */
	ssSetDWorkWidth(S,1,1);
	ssSetDWorkDataType(S,1,SS_INT8);
	/* Logging whether input or output */
	ssSetDWorkWidth(S,2,1);
	ssSetDWorkDataType(S,2,SS_BOOLEAN);

    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetOptions(S,SS_OPTION_CALL_TERMINATE_ON_EXIT);
 
}
static void mdlInitializeSampleTimes(SimStruct *S)
{    
}



#define MDL_START
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   */
static void mdlStart(SimStruct *S) 
{
    /* In this function, we will populate permanent settings in work vector
     * such as action type, so that we do not do such things multiple times
     * in mdlOutputs. */
    
    /* Declare variables and read parameters */
    char      insertiontype[80];
    int       portDataSize = (int) *mxGetPr(PORTDATASIZE_ARG(S));
    /* Read DWork vectors */
	int8_T    *action = (int8_T*) ssGetDWork(S,1);
	boolean_T *logging = (boolean_T*) ssGetDWork(S,2);
    
    /* Variables to define possible actions */
    boolean_T   isDirectPass;       /* Directly pass the input to outputs */
    boolean_T   isAdd;              /* Add injection signal on the top of input */
    boolean_T   isReplace;          /* Speak out injection signal ignoring input */
    boolean_T   isOpen;             /* Open the loop, speak out op. value */
    
    mxGetString(IOTYPE_ARG(S),insertiontype,80);
    



     
	 /* Determine the type of action and logging location so that
	  * we do not have to run these ifs at each time step */
	 
    /* Check where to log */
    logging[0] = ((strcmp(insertiontype,"InputOutput") == 0) ||
                   (strcmp(insertiontype,"InputOutputInactive") == 0) ||
                   (strcmp(insertiontype,"InputOutputOpen") == 0) ||
                   (strcmp(insertiontype,"InputOutputInactiveOpen") == 0));
	 
     /* Determine the action that needs to be taken */
    isDirectPass = ((strcmp(insertiontype,"None") == 0) ||
                    (strcmp(insertiontype,"InputInactive") == 0) ||
                    (strcmp(insertiontype,"Output") == 0) ||
                    (strcmp(insertiontype,"InputOutputInactive") == 0) ||
                    (strcmp(insertiontype,"OutputInputInactive") == 0) );
	if (isDirectPass) {
        action[0] = DIRECT_PASS_ACTION;
        return;
	}
    isAdd = ((strcmp(insertiontype,"Input") == 0) ||
             (strcmp(insertiontype,"InputOutput") == 0) ||
             (strcmp(insertiontype,"OutputInput") == 0));
	if (isAdd) {
        action[0] = ADD_ACTION;
		return;
    }

    isReplace = ((strcmp(insertiontype,"InputOpen") == 0) ||
                 (strcmp(insertiontype,"InputOutputOpen") == 0) ||
                 (strcmp(insertiontype,"OutputInputOpen") == 0));
	if (isReplace) {
        action[0] = REPLACE_ACTION;
		return;
	 }

    isOpen = ((strcmp(insertiontype,"InputInactiveOpen") == 0) ||
              (strcmp(insertiontype,"OutputOpen") == 0) ||
              (strcmp(insertiontype,"InputOutputInactiveOpen") == 0) ||
              (strcmp(insertiontype,"OutputInputInactiveOpen") == 0) ||
              (strcmp(insertiontype,"Open") == 0) );
	if (isOpen) {
        action[0] = OPEN_ACTION;
		return;
    }    

  }
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Declare variables and read parameters */
    int     signalIndex = ((int) *mxGetPr(SIGNALINDEX_ARG(S)))-1; /* Adjust for zero-based indexing */
    int     portDataSize = (int) *mxGetPr(PORTDATASIZE_ARG(S));
    
    /* Read the DWork variables */
    void      *opvalue = ssGetDWork(S,0);
    int8_T    *action = (int8_T*) ssGetDWork(S,1);
    boolean_T *isLogOutput = (boolean_T*) ssGetDWork(S,2);
    
    /* Input & Output variables as VOID for copying with MEMCPY */
    const void        *u = ssGetInputPortSignal(S,1);     /* 2nd input port */
    void              *y = ssGetOutputPortSignal(S,1);       /* 2nd output port */
    void              *y_log = ssGetOutputPortSignal(S,0);
    
    /* Now read input and output with data type double */
    real_T        *opvalue_real = (real_T*) ssGetDWork(S,0);
    const real_T  *u_real = ssGetInputPortRealSignal(S,1);
    const real_T  *u_input_real = ssGetInputPortRealSignal(S,0);
    real_T        *y_real = ssGetOutputPortRealSignal(S,1);
    real_T        *ylog_real = ssGetOutputPortRealSignal(S,0);
    
    if (ssGetT(S) == ssGetTStart(S)) {
        /* At t=0, initialize the op. point value to the initial value of second input port */
        const void *u = ssGetInputPortSignal(S,1);
        (void)memcpy(opvalue,u,portDataSize);
    }
    
    /* Initially copy the second input to the both outputs */
    (void)memcpy(y,u,portDataSize);
    (void)memcpy(y_log,u,portDataSize);
    /* If it is DirectPass, there is nothing else to be done: return */
    if (action[0] == DIRECT_PASS_ACTION) {
        if (isLogOutput[0])
            /* Copy all elements of y to y_log */
            (void)memcpy(y_log,y,portDataSize);
        return;
    }
    if (action[0] == OPEN_ACTION) {
        /* Speak out the operating point value */
        (void)memcpy(y,opvalue,portDataSize); 
        /* Handle the logging if output is to be logged */
        if (isLogOutput[0])
            /* Copy all elements of y to y_log */
            (void)memcpy(y_log,y,portDataSize);
        return;
    }
     
    /* Take the action */
    if (action[0] == ADD_ACTION) {
        /* Add the injection signal on the top of input */
        y_real[signalIndex] = u_real[signalIndex]+u_input_real[0];
        if (isLogOutput[0])
            /* Copy all elements of y to y_log */
            (void)memcpy(y_log,y,portDataSize);
        return;
    }
    if (action[0] == REPLACE_ACTION) {
        /* Speak out the operating point value */
        (void)memcpy(y,opvalue,portDataSize);
        /* Speak out the injection signal ignoring the input at the specified index */
        y_real[signalIndex] = opvalue_real[signalIndex]+u_input_real[0];
        /* Handle the logging and return */
        if (isLogOutput[0])
            /* Copy all elements of y to y_log */
            (void)memcpy(y_log,y,portDataSize);
        return;
    }
}
static void mdlTerminate(SimStruct *S)
{
    /* Deallocate int dimensions stored in user data*/
    int_T *intdimensions = ssGetUserData(S);
    free(intdimensions); /*sbcheck::termok*/
}


/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

/* LocalWords:  IOs nd
 */
