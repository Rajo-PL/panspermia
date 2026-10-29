/**
 * @file l0_orbit_governor.h
 * @brief Embedded C99 Real-Time Orbital Governor for L0-ISM Anomaly Detection
 */

#ifndef L0_ORBIT_GOVERNOR_H
#define L0_ORBIT_GOVERNOR_H

#include <stdint.h>
#include <stdbool.h>

#define L0_BOUND_MIN   (-9)
#define L0_BOUND_MAX   (+9)
#define L0_HARD_STOP   (-999)
#define L0_PERMIT      (0)

typedef struct {
    int8_t s_rho;     /* Mass / Density state */
    int8_t s_H;       /* Momentum / Acceleration state */
    int8_t s_Lambda;  /* Topological field state */
} l0_orbital_vector_t;

static inline int16_t l0_verify_orbital_state(const l0_orbital_vector_t* vec) {
    if (!vec) return L0_HARD_STOP;

    /* 1. Sprawdzenie nasycenia granic siatki Bekensteina [-9, +9] */
    if (vec->s_rho < L0_BOUND_MIN || vec->s_rho > L0_BOUND_MAX ||
        vec->s_H < L0_BOUND_MIN   || vec->s_H > L0_BOUND_MAX   ||
        vec->s_Lambda < L0_BOUND_MIN || vec->s_Lambda > L0_BOUND_MAX) {
        return L0_HARD_STOP;
    }

    /* 2. Sprawdzenie Prawa Zachowania Strumienia: s_rho + s_H + s_Lambda == 0 */
    if ((int16_t)vec->s_rho + (int16_t)vec->s_H + (int16_t)vec->s_Lambda != 0) {
        return L0_HARD_STOP;
    }

    /* Wykrycie sztucznej korekty pędu bez utraty masy */
    if (vec->s_rho == 0 && vec->s_H != 0 && vec->s_Lambda != 0) {
        return 1; /* OZNACZENIE: WYKRYTO SZTUCZNĄ STRUKTURĘ L0 */
    }

    return L0_PERMIT;
}

#endif /* L0_ORBIT_GOVERNOR_H */
