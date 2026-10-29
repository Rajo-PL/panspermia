#include <stdio.h>
#include <stdlib.h>
#include "l0_orbit_governor.h"

int main(void) {
    printf("=== L0-ISM Orbital Satellite Governor Verification ===\n\n");

    l0_orbital_vector_t test_vectors[] = {
        { .s_rho = 0, .s_H = 0, .s_Lambda = 0 },   /* Nominal Passive (Permit 0) */
        { .s_rho = -2, .s_H = 2, .s_Lambda = 0 },  /* Natural Thermal Outgassing (Permit 0) */
        { .s_rho = 0, .s_H = 3, .s_Lambda = -3 },  /* Artificial RESO Correction (Detected 1) */
        { .s_rho = 0, .s_H = 10, .s_Lambda = -10 } /* Boundary Breach (Hard Stop -999) */
    };

    size_t count = sizeof(test_vectors) / sizeof(test_vectors[0]);
    int errors = 0;

    for (size_t i = 0; i < count; i++) {
        int16_t res = l0_verify_orbital_state(&test_vectors[i]);
        printf("Test Case %zu: Vector (%d, %d, %d) -> Action: %d ", 
               i + 1, test_vectors[i].s_rho, test_vectors[i].s_H, test_vectors[i].s_Lambda, res);

        if (res == L0_PERMIT) {
            printf("[PERMIT - Natural Dynamic Balance]\n");
        } else if (res == 1) {
            printf("[DETECTION - Artificial L0 RESO Correction]\n");
        } else if (res == L0_HARD_STOP) {
            printf("[HARD_STOP - Bekenstein Boundary Saturation]\n");
        } else {
            printf("[UNKNOWN]\n");
            errors++;
        }
    }

    if (errors == 0) {
        printf("\nAll L0-ISM Orbital Governor tests passed successfully.\n");
        return EXIT_SUCCESS;
    } else {
        printf("\nVerification FAILED.\n");
        return EXIT_FAILURE;
    }
}
