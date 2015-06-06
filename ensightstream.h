/****************************************************************************
 * Header File                                                              *
 * Programmer: Huangrui Mo                                                  *
 * - Follow the Google's C/C++ style Guide                                  *
 ****************************************************************************/
/****************************************************************************
 * Header File Guards to Avoid Interdependence
 ****************************************************************************/
#ifndef ARTRACFD_ENSIGHTSTREAM_H_ /* if this is the first definition */
#define ARTRACFD_ENSIGHTSTREAM_H_ /* a unique marker for this header file */
/****************************************************************************
 * Required Header Files
 ****************************************************************************/
#include "commons.h"
/****************************************************************************
 * Data Structure Declarations
 ****************************************************************************/
/****************************************************************************
 * Public Functions Declaration
 ****************************************************************************/
/*
 * Ensight format data exporter
 *
 * Function
 *      Export conservative field data vector variable:
 *      U = [rho, rho_u, rho_v, rho_w, rho_eT]
 *      to primitive variables = [rho, u, v, w, p, T]
 *      to binary data files with Ensight data format.
 * Notice
 *      U is a linear array that stores all the values.
 *      These data are in sequential state 
 *      and can be accessed by linear index math.
 */
extern int WriteComputedDataEnsight(const Real * U, const Space *, 
        const Time *, const Partition *, const Flow *);
/*
 * Ensight format data loader
 *
 * Function
 *      Load computed data from output files which are written in Ensight
 *      format.
 */
extern int LoadComputedDataEnsight(Real *U, const Space *, Time *,
        const Partition *, const Flow *);
#endif
/* a good practice: end file with a newline */
