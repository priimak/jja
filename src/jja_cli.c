#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jja.h"

// compare equality of two strings
#define streq(str1, str2) (strcmp((str1),(str2)) == 0)

// used for checking that command line parater was set if it is not set
// then prints out error message and exists program
#define check_param(param, name) \
  do { \
    if( (param) < -900 ) { \
      fprintf(stderr, "ERROR: Parameter \"%s\" must be set.\n", (name));  \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

// method: 1 - simple euler; 2 - second order runge-kutta; 4 - forth order runge-kutta;
int method = 1;

// indicates what data are we to display
int display = -999;

// initial value of dc current
ffloat Idc_start = -999;

// final value of dc current
ffloat Idc_end = -999;

// dc current step
ffloat dIdc = -999;

// frequency of ac current
ffloat Omega_1 = -999;

// amplitude of ac current
ffloat i_1 = -999;

// initial value of phase difference
ffloat phi_start = -999;

// number of periods used to compute absorption
// Q. Why should this be greater than 1?
int nAv = -999;

// resonator frequecy
ffloat Omega_0 = -999;

// normalized active resistance of the resonator
ffloat r = -999;

// quantity proportional resonator capacitance (or volume?)
ffloat b = -9999;

// ?
ffloat h;

// time step
ffloat dt = -999;

// time from which averaging starts
ffloat t_start = -999;

// output will go into file defined by this variable
// if it is set to "-" (default value)  or "stdout" then it will go to stdout
// and if it is set "stderr" it will go to stderr
// for all other values appropriate file name will be open
// if file name starts with '+' then file will be open for append
char *out_file = (char *)"stdout";

// actual output file handle
FILE *out = NULL;

// gpu device number
int device;

int quiet = 0;

void parse_cmd(int argc, char **argv) {
  char *rf_name = NULL;
  char *separator = (char *)"=";
  char *name  = NULL;
  char *value = NULL;
  for( int i = 1; i <= argc; i++ ) {
    name  = strtok(argv[i], separator);
    value = strtok(NULL, separator);
    if( name == NULL || value == NULL ) {
      break;
    }

    if( streq(name, "display")     ) { display      = atoi(value);          } else
    if( streq(name, "method")      ) { method       = atoi(value);          } else
    if( streq(name, "Idc_start")   ) { Idc_start    = parse_ffloat(value);  } else
      //    if( streq(name, "Idc_end")     ) { Idc_end      = strtod(value,  NULL); } else
      //    if( streq(name, "dIdc")        ) { dIdc         = strtod(value,  NULL); } else
    if( streq(name, "Omega_1")     ) { Omega_1      = parse_ffloat(value);  } else
    if( streq(name, "i_1")         ) { i_1          = parse_ffloat(value);  } else
    if( streq(name, "phi_start")   ) { phi_start    = parse_ffloat(value);  } else
    if( streq(name, "nAv")         ) { nAv          = atoi(value); } else
    if( streq(name, "Omega_0")     ) { Omega_0      = parse_ffloat(value);  } else
    if( streq(name, "b")           ) { b            = parse_ffloat(value);  } else
    if( streq(name, "r")           ) { r            = parse_ffloat(value);  } else
      //    if( streq(name, "h")           ) { h            = strtod(value,  NULL); } else
    if( streq(name, "dt")          ) { dt           = parse_ffloat(value);  } else
    if( streq(name, "t-start")     ) { t_start      = atoi(value);          } else 
    if( streq(name, "quiet")       ) { quiet        = 1;                    } else
    if( streq(name, "device")      ) { device       = atoi(value);          } else
    if( streq(name, "o")           ) { out_file     = strdup(value);        }
  }

  if( display < -900 ) { 
    fprintf(stderr, "ERROR: Parameter \"display\" must be set.\n");  
    exit(EXIT_FAILURE); 
  }

  check_param(display,   "display");
  check_param(Idc_start, "Idc_start");
  //  check_param(Idc_end,   "Idc_end");
  //  check_param(dIdc,      "dIdc");
  check_param(Omega_1,   "Omega_1");
  check_param(i_1,       "i_1");
  check_param(phi_start, "phi_start");
  check_param(nAv,       "nAv");
  check_param(Omega_0,   "Omega_0");
  check_param(b,         "b");
  check_param(r,         "r");
  check_param(h,         "h");
  check_param(dt,        "dt");
  check_param(t_start,   "t-start");

  // validate display value
  if( display != 1 && display != 2 ) {
    fprintf(stderr, "ERROR: Invalid value of display= parameter. Possible values are 1, ... .\n");
    exit(EXIT_FAILURE);
  }

  // validate method value
  if( method != 1 && method != 2 && method != 4 ) {
    fprintf(stderr, "ERROR: Invalid value of method= parameter. Possible values are 1, 2 and 4\n");
    exit(EXIT_FAILURE);
  }
  // ensure t-start > 0
  if( t_start <= 0 ) {
    fprintf(stderr, "ERROR: Invalid value of t-start=%f parameter. it must be greater than 0.\n", t_start);
    exit(EXIT_FAILURE);
  }

  // try to open output file
  if( streq(out_file, "stdout") || streq(out_file, "-") ) { 
    out = stdout;   

  } else if( streq(out_file, "stderr") ) { 
    out = stderr; 

  } else {
    // open actual file
    if( strncmp("+", out_file, strlen("+")) == 0 ) {
      // open for append
      out = fopen(out_file+1, "a");
    } else {
      out = fopen(out_file, "w");
    }

    if( out == NULL ) {
      fprintf(stderr, "ERROR: Faled to open file \"%s\" for writing.\n", out_file);
      perror("ERROR");
      exit(EXIT_FAILURE);
    }
  }

  if( out == NULL ) {
    fprintf(stderr, "ERROR: Faled to open file for writing.");
    exit(EXIT_FAILURE);
  }
} // end of parse_cmd(...)
