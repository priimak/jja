#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "constants.h"
#include "jja.h"
#include "jja_cli.h"

extern int method;
extern int display;
extern ffloat Idc_start;
extern ffloat Idc_end;
extern ffloat dIdc;
extern ffloat Omega_1;
extern ffloat i_1;
extern ffloat phi_start;
extern int nAv;
extern ffloat Omega_0;
extern ffloat b;
extern ffloat r;
extern int n;
extern ffloat dt;
extern ffloat t_start;
extern char *out_file;
extern FILE *out;
extern int device;
extern int quiet;

ffloat dt_over_2;
ffloat Omega_0_squared;
ffloat l;
ffloat pi2; // 2 * PI

void euler(ffloat* q, ffloat* phi, ffloat* q_1, ffloat t, ffloat i_dc);
void rg2(ffloat* q, ffloat* phi, ffloat* q_1, ffloat t, ffloat i_dc);
void rg4(ffloat* q, ffloat* phi, ffloat* q_1, ffloat t, ffloat i_dc);

int main(int argc, char **argv) {
  parse_cmd(argc, argv);

  dt_over_2 = dt / 2;
  Omega_0_squared = Omega_0 * Omega_0;
  l = 1.0 / ( b * Omega_0_squared );
  pi2 = 2 * PI;

  ffloat t_end = t_start +  nAv * 2 * PI / Omega_1; 

  ffloat q = 0;
  ffloat phi = phi_start;
  ffloat q_1 = 0;

  fprintf(out, 
	  "# ./bin/1jja_c display=%d method=%d Idc_start=%f Omega_1=%f i_1=%f phi_start=%f \
nAv=%d Omega_0=%f b=%f r=%f dt=%f t-start=%f o=%s\n", 
	  display, method, Idc_start, Omega_1, i_1, phi_start, nAv, Omega_0, b, r, dt, t_start, 
	  out_file);
  fprintf(out, "# Start t_end=%f l=%f\n", t_end, l);
  if (display == 1) {
    fprintf(out, "#      t        q      phi      q_1\n");
  } else {
    fprintf(out, "#   i_dc     v_av\n");
  }

  ffloat i_dc = Idc_start;
  ffloat v_av = 0;
  
  ffloat t = 0;
  for (;;) {
    if (display == 1) {
      fprintf(out, "%2.6f %f %f %f\n", t, q, phi, q_1);
    }
    if( method == 1 ) {
      euler(&q, &phi, &q_1, t, i_dc);
    } else if( method == 2) {
      rg2(&q, &phi, &q_1, t, i_dc);
    } else { 
      rg4(&q, &phi, &q_1, t, i_dc);
    }

    
    if (phi > pi2) {
      phi = phi - pi2;
    }

    t = t + dt;

    if (t >= t_start) {
      break;
    }
  }

  ffloat t_av_started_at = t;
  ffloat phi_at_t_minus_dt = phi;
  for (;;) {
    if (display == 1) {
      fprintf(out, "%2.6f %f %f %f\n", t, q, phi, q_1);
    }
    if( method == 1 ) {
      euler(&q, &phi, &q_1, t, i_dc);
    } else if( method == 2) {
      rg2(&q, &phi, &q_1, t, i_dc);
    } else { 
      rg4(&q, &phi, &q_1, t, i_dc);
    }

    // compute v_av
    v_av = v_av + ( sin(phi_at_t_minus_dt) + sin(phi) ) * dt_over_2;
    phi_at_t_minus_dt = phi;

    if (t >= t_end) {
      break;
    }

    if (phi > pi2) {
      phi = phi - pi2;
    }

    t = t + dt;
  }
  v_av = v_av / (t_end - t_av_started_at);
  if (display == 2) {
    fprintf(out, "%f %f\n", i_dc, v_av);
  }
}

// Simple Euler method
void euler(ffloat* q, ffloat* phi, ffloat* q_1, ffloat t, ffloat i_dc) {
  ffloat q_next = (*q) + dt * (*q_1);
  ffloat phi_next = (*phi) + dt * ( i_dc + i_1 * cos(Omega_1 * t) - sin(*phi) - (*q_1)); 
  *q_1 = (*q_1) 
    + dt * ( 
        i_dc/l + i_1/l * cos(Omega_1 * t) - (1 + r)/l * (*q_1) - 1/l * sin(*phi) - Omega_0 * Omega_0 * (*q) 
    );

  *q = q_next;
  *phi = phi_next;
}

// Second-order Runge-Kutta
void rg2(ffloat* q, ffloat* phi, ffloat* q_1, ffloat t, ffloat i_dc) {
  // step to the center
  ffloat q_center = (*q) + dt_over_2 * (*q_1);
  ffloat phi_center = (*phi) + dt_over_2 * ( i_dc + i_1 * cos(Omega_1 * t) - sin(*phi) - (*q_1)); 
  ffloat q_1_center = (*q_1) 
    + dt_over_2 * ( 
        i_dc/l + i_1/l * cos(Omega_1 * t) - (1 + r)/l * (*q_1) - 1/l * sin(*phi) - Omega_0_squared * (*q) 
    );

  // step from t to t + dt using central values
  *q = (*q) + dt * q_1_center;
  *phi = (*phi) + dt * ( i_dc + i_1 * cos(Omega_1 * (t + dt_over_2)) - sin(phi_center) - q_1_center); 
  *q_1 = (*q_1) 
    + dt * ( 
        i_dc/l + i_1/l * cos(Omega_1 * (t + dt_over_2)) - (1 + r)/l * q_1_center 
        - 1/l * sin(phi_center) - Omega_0_squared * q_center 
    );
}

// Forth-order Runge-Kutta
void rg4(ffloat* q, ffloat* phi, ffloat* q_1, ffloat t, ffloat i_dc) {
  ffloat q_k1 = (*q_1);
  ffloat phi_k1 = i_dc + i_1 * cos(Omega_1 * t) 
    - sin(*phi) 
    - (*q_1);
  ffloat q_1_k1 = i_dc/l + i_1/l * cos(Omega_1 * t) 
    - (1 + r)/l * (*q_1) 
    - 1/l * sin(*phi) 
    - Omega_0_squared * (*q);

  ffloat q_k2 = (*q_1) + dt_over_2 * q_1_k1;
  ffloat phi_k2 = i_dc + i_1 * cos(Omega_1 * ( t + dt_over_2 )) 
    - sin((*phi) + dt_over_2 * phi_k1) 
    - ((*q_1) + dt_over_2 * q_1_k1);
  ffloat q_1_k2 = i_dc/l + i_1/l * cos(Omega_1 * ( t + dt_over_2 )) 
    - (1 + r)/l * ((*q_1) + dt_over_2 * q_1_k1) 
    - 1/l * sin((*phi) + dt_over_2 * phi_k1) 
    - Omega_0_squared * ((*q) + dt_over_2 * q_k1);

  ffloat q_k3 = (*q_1) + dt_over_2 * q_1_k2;
  ffloat phi_k3 = i_dc + i_1 * cos(Omega_1 * ( t + dt_over_2 ))
    - sin((*phi) + dt_over_2 * phi_k2)
    - ((*q_1) + dt_over_2 * q_1_k2);
  ffloat q_1_k3 = i_dc/l + i_1/l * cos(Omega_1 * ( t + dt_over_2 )) 
    - (1 + r)/l * ((*q_1) + dt_over_2 * q_1_k2) 
    - 1/l * sin((*phi) + dt_over_2 * phi_k2) 
    - Omega_0_squared * ((*q) + dt_over_2 * q_k2);

  ffloat q_k4 = (*q_1) + dt * q_1_k3;
  ffloat phi_k4 = i_dc + i_1 * cos(Omega_1 * ( t + dt ))
    - sin((*phi) + dt * phi_k3)
    - ((*q_1) + dt * q_1_k3);
  ffloat q_1_k4 = i_dc/l + i_1/l * cos(Omega_1 * ( t + dt )) 
    - (1 + r)/l * ((*q_1) + dt * q_1_k3) 
    - 1/l * sin((*phi) + dt * phi_k3) 
    - Omega_0_squared * ((*q) + dt * q_k3);

  *q = (*q) + dt * (q_k1 + 2 * q_k2 + 2 * q_k3 + q_k4) / 6;
  *phi = (*phi) + dt * (phi_k1 + 2 * phi_k2 + 2 * phi_k3 + phi_k4) / 6;
  *q_1 = (*q_1) + dt * (q_1_k1 + 2 * q_1_k2 + 2 * q_1_k3 + q_1_k4) / 6;
}
