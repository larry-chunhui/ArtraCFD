/****************************************************************************
 * Header File                                                              *
 * Programmer: Huangrui Mo                                                  *
 * - Follow the Google's C/C++ style Guide                                  *
 ****************************************************************************/
/****************************************************************************
 * Header File Guards to Avoid Interdependence
 ****************************************************************************/
#ifndef ARTRACFD_COLLISION_H_ /* if this is the first definition */
#define ARTRACFD_COLLISION_H_ /* a unique marker for this header file */
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
 * Collision Model
 *
 * Function
 *      
 */
extern int ParticleSpatialEvolution(Real *U, Space *, Particle *, 
        Time *, const Partition *, const Flow *);
#endif
/* a good practice: end file with a newline */
