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
#include "data_probe.h"
#include <stdio.h> /* standard library for input and output */
#include <stdlib.h> /* support for abs operation */
#include "cfd_commons.h"
#include "commons.h"
/****************************************************************************
 * Function definitions
 ****************************************************************************/
int WriteFieldDataAtProbes(const Time *time, const Space *space, const Model *model)
{
    if (0 == time->probeN) {
        return 0;
    }
    FILE *filePointer = NULL;
    String fileName = {'\0'};
    const Partition *restrict part = &(space->part);
    const Node *const node = space->node;
    int idx = 0; /* linear array index math variable */
    int idxOld = 0; /* linear array index math variable */
    Real Uo[DIMUo] = {0.0};
    const IntVec nMin = {part->ns[PIN][X][MIN] - 1, part->ns[PIN][Y][MIN] - 1, part->ns[PIN][Z][MIN] - 1};
    const IntVec nMax = {part->ns[PIN][X][MAX] + 1, part->ns[PIN][Y][MAX] + 1, part->ns[PIN][Z][MAX] + 1};
    const RealVec sMin = {part->domain[X][MIN], part->domain[Y][MIN], part->domain[Z][MIN]};
    const RealVec d = {part->d[X], part->d[Y], part->d[Z]};
    const RealVec dd = {part->dd[X], part->dd[Y], part->dd[Z]};
    const int ng = part->ng;
    RealVec p1 = {0.0};
    RealVec p2 = {0.0};
    RealVec dl = {0.0};
    int stepN = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    for (int n = 0; n < time->probeN; ++n) {
        snprintf(fileName, sizeof(fileName), "%s%d.%05d", "probe", n + 1, time->countStep);
        filePointer = fopen(fileName, "w");
        if (NULL == filePointer) {
            FatalError("failed to write data at probes...");
        }
        fprintf(filePointer, "# points  x  y  z  rho  u  v  w  p T\n"); 
        p1[X] = time->probe[n][0];
        p1[Y] = time->probe[n][1];
        p1[Z] = time->probe[n][2];
        p2[X] = time->probe[n][3];
        p2[Y] = time->probe[n][4];
        p2[Z] = time->probe[n][5];
        stepN = MaxInt(time->probe[n][ENTRYPROBE-1] - 1, 1);
        dl[X] = (p2[X] - p1[X]) / (Real)(stepN);
        dl[Y] = (p2[Y] - p1[Y]) / (Real)(stepN);
        dl[Z] = (p2[Z] - p1[Z]) / (Real)(stepN);
        idxOld = -1; /* used to avoid repeating node for tiny step sizes */
        for (int m = 0; m <= stepN; ++m) {
            i = ValidNodeSpace(NodeSpace(p1[X] + m * dl[X], sMin[X], dd[X], ng), nMin[X], nMax[X]);
            j = ValidNodeSpace(NodeSpace(p1[Y] + m * dl[Y], sMin[Y], dd[Y], ng), nMin[Y], nMax[Y]);
            k = ValidNodeSpace(NodeSpace(p1[Z] + m * dl[Z], sMin[Z], dd[Z], ng), nMin[Z], nMax[Z]);
            idx = IndexNode(k, j, i, part->n[Y], part->n[X]);
            if (idxOld == idx) {
                continue;
            }
            idxOld = idx; /* record */
            p2[X] = PointSpace(i, sMin[X], d[X], ng);
            p2[Y] = PointSpace(j, sMin[Y], d[Y], ng);
            p2[Z] = PointSpace(k, sMin[Z], d[Z], ng);
            PrimitiveByConservative(model->gamma, model->gasR, node[idx].U[TO], Uo);
            fprintf(filePointer, "%d  %.6g  %.6g  %.6g  %.6g  %.6g  %.6g  %.6g  %.6g  %.6g\n",
                    m, p2[X], p2[Y], p2[Z], Uo[0], Uo[1], Uo[2], Uo[3], Uo[4], Uo[5]); 
        }
        fclose(filePointer); /* close current opened file */
    }
    return 0;
}
/* a good practice: end file with a newline */

