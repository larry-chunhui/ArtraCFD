/****************************************************************************
 *                              ArtraCFD                                    *
 *                          <By Huangrui Mo>                                *
 * Copyright (C) Huangrui Mo <huangrui.mo@gmail.com>                        *
 * This file is part of ArtraCFD.                                           *
 * ArtraCFD is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 ****************************************************************************/
/****************************************************************************
 * Required Header Files
 ****************************************************************************/
#include "paraview_stream.h"
#include <stdio.h> /* standard library for input and output */
#include <string.h> /* manipulating strings */
#include "paraview.h"
#include "cfd_commons.h"
#include "commons.h"
/****************************************************************************
 * Static Function Declarations
 ****************************************************************************/
static int ReadParaviewDataFile(Time *, ParaviewSet *);
static int ReadParaviewVariableFile(Real *U, const Space *, const Model *,
        const Partition *, ParaviewSet *);
/****************************************************************************
 * Function definitions
 ****************************************************************************/
int ReadComputedDataParaview(Real *U, const Space *space, Time *time,
        const Model *model, const Partition *part)
{
    ParaviewSet paraSet = { /* initialize ParaviewSet environment */
        .baseName = "paraview", /* data file base name */
        .fileName = {'\0'}, /* data file name */
        .floatType = "Float32", /* paraview data type */
        .byteOrder = "LittleEndian" /* byte order of data */
    };
    ReadParaviewDataFile(time, &paraSet);
    ReadParaviewVariableFile(U, space, model, part, &paraSet);
    return 0;
}
static int ReadParaviewDataFile(Time *time, ParaviewSet *paraSet)
{
    FILE *filePointer = NULL;
    filePointer = fopen("restart.pvd", "r");
    if (NULL == filePointer) {
        FatalError("failed to open restart file: restart.pvd...");
    }
    /* read information from file */
    char currentLine[200] = {'\0'}; /* store current line */
    char format[25] = {'\0'}; /* format information */
    /* get rid of redundant lines */
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    /* get restart order number */
    fgets(currentLine, sizeof currentLine, filePointer);
    sscanf(currentLine, "%*s %*s %d", &(time->outputCount)); 
    /* get restart time */
    /* set format specifier according to the type of Real */
    strncpy(format, "%*s %*s %lg", sizeof format); /* default is double type */
    if (sizeof(Real) == sizeof(float)) { /* if set Real as float */
        strncpy(format, "%*s %*s %g", sizeof format); /* float type */
    }
    fgets(currentLine, sizeof currentLine, filePointer);
    sscanf(currentLine, format, &(time->now)); 
    /* get current step number */
    fgets(currentLine, sizeof currentLine, filePointer);
    sscanf(currentLine, "%*s %*s %d", &(time->stepCount)); 
    fclose(filePointer); /* close current opened file */
    /* store updated basename in filename */
    snprintf(paraSet->fileName, sizeof(ParaviewString), "%s%05d", 
            paraSet->baseName, time->outputCount); 
    /* basename is updated here! */
    snprintf(paraSet->baseName, sizeof(ParaviewString), "%s", paraSet->fileName); 
    return 0;
}
static int ReadParaviewVariableFile(Real *U, const Space *space, const Model *model,
        const Partition *part, ParaviewSet *paraSet)
{
    FILE *filePointer = NULL;
    /* current filename */
    snprintf(paraSet->fileName, sizeof(ParaviewString), "%s.vts", paraSet->baseName); 
    filePointer = fopen(paraSet->fileName, "r");
    if (NULL == filePointer) {
        FatalError("failed to load data file...");
    }
    /* get rid of redundant lines */
    char currentLine[200] = {'\0'}; /* store current line */
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    fgets(currentLine, sizeof currentLine, filePointer);
    int idx = 0; /* linear array index math variable */
    ParaviewReal data = 0.0; /* the Paraview data format */
    /* set format specifier according to the type of Real */
    char format[5] = "%lg"; /* default is double type */
    if (sizeof(ParaviewReal) == sizeof(float)) {
        strncpy(format, "%g", sizeof format); /* float type */
    }
    for (int dim = 0; dim < DIMU; ++dim) {
        fgets(currentLine, sizeof currentLine, filePointer);
        for (int k = part->kSub[0]; k < part->kSup[0]; ++k) {
            for (int j = part->jSub[0]; j < part->jSup[0]; ++j) {
                for (int i = part->iSub[0]; i < part->iSup[0]; ++i) {
                    fscanf(filePointer, format, &data);
                    idx = IndexMath(k, j, i, space) * DIMU;
                    switch (dim) {
                        case 0: /* rho */
                            U[idx] = data;
                            break;
                        case 1: /* u */
                            U[idx+1] = U[idx] * data;
                            break;
                        case 2: /* v */
                            U[idx+2] = U[idx] * data;
                            break;
                        case 3: /* w */
                            U[idx+3] = U[idx] * data;
                            break;
                        case 4: /* p */
                            U[idx+4] = 0.5 * (U[idx+1] * U[idx+1] + U[idx+2] * U[idx+2] + U[idx+3] * U[idx+3]) / U[idx] + data / (model->gamma - 1.0);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        fgets(currentLine, sizeof currentLine, filePointer); /* get rid of the end of line of data */
        fgets(currentLine, sizeof currentLine, filePointer);
    }
    fclose(filePointer); /* close current opened file */
    return 0;
}
/* a good practice: end file with a newline */

