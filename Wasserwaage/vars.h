/* gesammelte globale Variablen
 */
#ifndef _VARS_H
#define _VARS_H

int nb; // number of clients
float fp_corr,fr_corr;
float z1,z2,z3,z4;
bool do_calib=false;  // compass
float  keilhoehe=KEIL_HOEHE;
float fp=0,fr=0;    // auch von Calib (http) benutzt

#endif
