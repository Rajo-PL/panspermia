{
  "result": "/**\n * @file l0_subsea_governor.h\n * @brief Embedded C99 Real-Time Governor for Subsea & Subice L0-ISM Anomaly Detection\n * @details Tailored for ultra-low noise environments: Challenger Deep (Mariana Trench) & Lake Vostok (Antarctica)\n */\n\n#ifndef L0_SUBSEA_GOVERNOR_H\n#define L0_SUBSEA_GOVERNOR_H\n\n#include <stdint.h>\n#include <stdbool.h>\n\n#define L0_BOUND_MIN   (-9)\n#define L0_BOUND_MAX   (+9)\n#define L0_HARD_STOP   (-999)\n#define L0_PERMIT      (0)\n\ntypedef struct {\n    int8_t s_rho;     /* Mass / Density / Thermal State Change */\n    int8_t s_H;       /* Momentum / Dynamic Pressure Jump */\n    int8_t s_Lambda;  /* Local Topological Field State */\n} l0_subsea_vector_t;\n\nstatic inline int16_t l0_verify_subsea_state(const l0_subsea_vector_t* vec) {\n    if (!vec) return L0_HARD_STOP;\n\n    /* 1. Sprawdzenie nasycenia granic Bekensteina [-9, +9] */\n    if (vec->s_rho < L0_BOUND_MIN || vec->s_rho > L0_BOUND_MAX ||\n        vec->s_H < L0_BOUND_MIN   || vec->s_H > L0_BOUND_MAX   ||\n        vec->s_Lambda < L0_BOUND_MIN || vec->s_Lambda > L0_BOUND_MAX) {\n        return L0_HARD_STOP;\n    }\n\n    /* 2. Sprawdzenie Prawa Zachowania Strumienia: s_rho + s_H + s_Lambda == 0 */\n    if ((int16_t)vec->s_rho + (int16_t)vec->s_H + (int16_t)vec->s_Lambda != 0) {\n        return L0_HARD_STOP;\n    }\n\n    /* Wykrycie sztucznej modyfikacji pola topologicznego bez utraty masy (Fulcrum 0 Anomaly) */\n    if (vec->s_rho == 0 && vec->s_H != 0 && vec->s_Lambda != 0) {\n        return 1; /* OZNACZENIE: WYKRYTO SZTUCZNĄ STRUKTURĘ L0 W GŁĘBINIE / ZBIORNIKU SUBGLACJALNYM */\n    }\n\n    return L0_PERMIT;\n}\n\n#endif /* L0_SUBSEA_GOVERNOR_H */\n"
}
``` protocol:
Let's view `test_z3_subsea_subice_audit.py` and `l0-telemetry-ci-v2.yml`.Tool response for view_file:
```json
{
  "result": "#!/usr/bin/env python3\n\"\"\"\nL0-ISM: Subsea & Subice Anomaly Detector\nFile: test_z3_subsea_subice_audit.py\nDescription: SMT Z3 formal verification engine for detecting artificial Layer 0\n             signatures in ultra-low noise environments (Challenger Deep & Lake Vostok).\n\"\"\"\n\nimport time\nfrom z3 import *\n\ndef audit_terrestrial_telemetry(scenario_name: str, s_rho: int, s_H: int, s_Lambda: int):\n    print(f\"\\n=== SMT Z3 Audyt Terestrialny L0-ISM: {scenario_name} ===\")\n    \n    start_time = time.perf_counter()\n    solver = Solver()\n\n    rho = Int('s_rho')       # Thermal / Mass / Density state\n    H = Int('s_H')           # Pressure / Momentum jump\n    Lambda = Int('s_Lambda') # Topological field state\n\n    # 1. 19-State Bounded Lattice S in [-9, +9]\n    for var in [rho, H, Lambda]:\n        solver.add(And(var >= -9, var <= 9))\n\n    # 2. Zero-Sum Flux Conservation Law: s_rho + s_H + s_Lambda == 0\n    solver.add(rho + H + Lambda == 0)\n\n    # 3. Observational Inputs from Deep Sea / Subice Sensors\n    solver.add(rho == s_rho)\n    solver.add(H == s_H)\n    solver.add(Lambda == s_Lambda)\n\n    # 4. Formal Verification\n    result = solver.check()\n    latency_ms = (time.perf_counter() - start_time) * 1000.0\n\n    if result == sat:\n        m = solver.model()\n        print(f\"[STATUS] SAT: Vector physically valid. Latency: {latency_ms:.3f} ms\")\n        print(f\" -> Thermal/Mass (s_rho): {m[rho].as_long()}\")\n        print(f\" -> Momentum/Press (s_H): {m[H].as_long()}\")\n        print(f\" -> Topo-Shift (s_Lambda): {m[Lambda].as_long()}\")\n        \n        if m[Lambda].as_long() != 0 and m[rho].as_long() == 0:\n            print(\" [!] ARTIFICIAL L0 STRUCTURE DETECTED: RESO Operator active in Low-Noise Environment!\")\n    else:\n        print(f\"[STATUS] UNSAT: Invariant Breach / Bekenstein Saturation! Latency: {latency_ms:.3f} ms\")\n        print(\" -> HARDWARE ACTION: -999 HARD_STOP / Quantum Bounce triggered!\")\n\nif __name__ == \"__main__\":\n    # Test 1: Challenger Deep Baseline (Thermal Equilibrium Fulcrum 0)\n    audit_terrestrial_telemetry(\"Rów Mariański - Stan Równowagi (Fulcrum 0)\", s_rho=0, s_H=0, s_Lambda=0)\n\n    # Test 2: Lake Vostok Geothermal Plume (Natural Dynamic Balance)\n    audit_terrestrial_telemetry(\"Jezioro Wostok - Naturalny Pióro Geotermalne\", s_rho=-1, s_H=1, s_Lambda=0)\n\n    # Test 3: Submerged Artificial Facility (RESO Operator Field Modification)\n    audit_terrestrial_telemetry(\"Sztuczna Instalacja Podwodna/Podlodowa (RESO)\", s_rho=0, s_H=4, s_Lambda=-4)\n\n    # Test 4: Bekenstein Limit Breach (|s_i| > 9)\n    audit_terrestrial_telemetry(\"Naruszenie Granicy Bekensteina w Głębinie\", s_rho=0, s_H=12, s_Lambda=-12)\n"
}
``` protocol:
Let's view `l0-telemetry-ci-v2.yml`.Tool response for view_file:
```json
{
  "result": "name: L0-ISM Multi-Environment Telemetry SMT Z3 CI\n\non:\n  push:\n    branches: [ \"main\" ]
    paths:\n      - \"scripts/**\"\n      - \".github/workflows/**\"\n  pull_request:\n    branches: [ \"main\" ]\n\njobs:\n  verify-multi-environment-l0:\n    runs-on: ubuntu-latest\n\n    steps:\n    - name: Checkout Repository\n      uses: actions/checkout@v4\n\n    - name: Set up Python 3.10\n      uses: actions/setup-python@v5\n      with:\n        python-version: \"3.10\"\n\n    - name: Install Python Dependencies\n      run: |\n        python -m pip install --upgrade pip\n        pip install z3-solver ortools numpy\n\n    - name: Compile C99 RTOS State Governors\n      run: |\n        gcc -std=c99 -Wall -Wextra -Werror -I scripts -O3 scripts/l0_orbit_governor_test.c -o scripts/l0_governor_test\n\n    - name: Run C99 RTOS Hardware Verification Driver\n      run: |\n        ./scripts/l0_governor_test\n\n    - name: Execute SMT Z3 Lagrange Orbital Telemetry Audit\n      run: |\n        python scripts/test_z3_lagrange_orbital_audit.py\n\n    - name: Execute SMT Z3 Subsea & Subice Telemetry Audit\n      run: |\n        python scripts/test_z3_subsea_subice_audit.py\n"
}
``` protocol:
Great! I now have all the exact codes for the Subsea & Subice Anomaly Detection Plan (`l0_subsea_governor.h`, `test_z3_subsea_subice_audit.py`, and `.github/workflows/l0-telemetry-ci-v2.yml`).

Let's present them cleanly and directly in Polish (since user requested in Polish: "Wygeneruj tutaj te kody").

I will formatting them nicely with syntax highlighting and clear headings. No meta preambles, direct substance.Oto wyredagowany zestaw kodów źródłowych oraz konfiguracji CI/CD dla **Planu Detekcji Podwodnej i Podlodowej L0-ISM (Rów Mariański / Jezioro Wostok)**:

---

### **1. Nagłówek ISO C99 Pokładowego Zarządcy Głębinowego (`l0_subsea_governor.h`)**

Kod w języku C99 przeznaczony do uruchomienia na procesorach i mikrokontrolerach kapsuł podwodnych/subglacjalnych (Zero-Heap, brak alokacji pamięci dynamicznej, czas wykonania \\(O(1)\\)):

```c
/**
 * @file l0_subsea_governor.h
 * @brief Embedded C99 Real-Time Governor for Subsea & Subice L0-ISM Anomaly Detection
 * @details Tailored for ultra-low noise environments: Challenger Deep (Mariana Trench) & Lake Vostok (Antarctica)
 */

#ifndef L0_SUBSEA_GOVERNOR_H
#define L0_SUBSEA_GOVERNOR_H

#include <stdint.h>
#include <stdbool.h>

#define L0_BOUND_MIN   (-9)
#define L0_BOUND_MAX   (+9)
#define L0_HARD_STOP   (-999)
#define L0_PERMIT      (0)

typedef struct {
    int8_t s_rho;     /* Mass / Density / Thermal State Change */
    int8_t s_H;       /* Momentum / Dynamic Pressure Jump */
    int8_t s_Lambda;  /* Local Topological Field State */
} l0_subsea_vector_t;

/**
 * @brief Weryfikuje inwarianty L0 dla telemetrii podwodnej/podlodowej
 * @param vec Wskaźnik na wektor stanu próbki telemetrycznej
 * @return L0_PERMIT (0), DETECT_RESO (1), lub L0_HARD_STOP (-999)
 */
static inline int16_t l0_verify_subsea_state(const l0_subsea_vector_t* vec) {
    if (!vec) return L0_HARD_STOP;

    /* 1. Sprawdzenie nasycenia granic Bekensteina [-9, +9] */
    if (vec->s_rho < L0_BOUND_MIN || vec->s_rho > L0_BOUND_MAX ||
        vec->s_H < L0_BOUND_MIN   || vec->s_H > L0_BOUND_MAX   ||
        vec->s_Lambda < L0_BOUND_MIN || vec->s_Lambda > L0_BOUND_MAX) {
        return L0_HARD_STOP;
    }

    /* 2. Sprawdzenie Prawa Zachowania Strumienia: s_rho + s_H + s_Lambda == 0 */
    if ((int16_t)vec->s_rho + (int16_t)vec->s_H + (int16_t)vec->s_Lambda != 0) {
        return L0_HARD_STOP;
    }

    /* 3. Wykrycie sztucznej modyfikacji pola topologicznego bez utraty masy (Fulcrum 0 Anomaly) */
    if (vec->s_rho == 0 && vec->s_H != 0 && vec->s_Lambda != 0) {
        return 1; /* OZNACZENIE: WYKRYTO SZTUCZNĄ STRUKTURĘ L0 W GŁĘBINIE / ZBIORNIKU SUBGLACJALNYM */
    }

    return L0_PERMIT;
}

#endif /* L0_SUBSEA_GOVERNOR_H */
