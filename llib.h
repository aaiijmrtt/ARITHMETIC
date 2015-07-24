#ifndef LLIB_H
#define LLIB_H

#include"loat.h"

loat *absolute(loat *n);
loat *bisection(loat *initiala,loat *initialb,loat *accuracy,loat *(*f)(loat*));
loat *newtonraphson(loat *initial,loat *accuracy,loat *(*f)(loat*),loat *(*df)(loat*));
loat ***gaussjordan(int rank,loat *matrix[rank][rank],loat *accuracy);

#endif
