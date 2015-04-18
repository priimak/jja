#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath> 
using namespace std;

const double pi=3.1415926535897932384626433832795;

double phi1(double q1);

double phi2(double Idc, double I1, double w1, double t, double f, double q1);

double phi3(double L,double w0, double r, double Idc, double I1, double w1, double t, double q, double f, double q1);

std::ofstream res;

int main() {
  double pi2 = 2 * pi;
  double at, dt1, Idc0, Idc, w1, I1, f0, T1, tn, t0, t, b, q0, q10, f1, L, r;
  double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, V, Vs, q1s, P, Psc;
  double q1sc, dVs, dq1s, dPsc, dq1sc, Ijj, w0,  f, q, q1, dt;
  int n;
  cout << "Idc0=";
  cin >> Idc0;
  cout << "w1=";
  cin >> w1;
  cout << "I1=";
  cin >> I1;
  cout << "f0=";
  cin >> f0;
  cout << "w0=";
  cin >> w0;
  cout << "b=";
  cin >> b;
  cout << "r=";
  cin >> r;
  cout << "dt=";
  cin >> dt;
  cout << "at=";
  cin >> at;

  res.open("absorpsingleres.txt", std::ios_base::app);

  L=1/(b*w0*w0);
  Idc=Idc0;
  t0=0;
  T1=2*pi/w1;
  tn=at+n*T1;

  // initial conditions
  t=t0;
  q=0;
  f=f0;
  q1=0;
  Vs=0;
  q1s=0;
  Psc=0;
  q1sc=0;

  res << "./a.out Idc_start=" << Idc0 << " Omega_1=" << w1 
      << " i_1=" << I1 << " phi_start=" << f0 << " Omega_0=" << w0
      << " b=" << b << " r=" << r << " dt=" << dt << " t-start=" << at << "\n";
  res << "./a.out Idc0=" << Idc0 << " w1=" << w1 
      << " I1=" << I1 << " f0=" << f0 << " w0=" << w0
      << " b=" << b << " r=" << r << " dt=" << dt << " at=" << at << "\n";
  res << "# t_end=" << at << " l=" << L << "\n";
  while (t <= at) {
    f1 = f;
    q10 = q1;
    a1 = phi1(q1);
    b1 = phi2(Idc, I1, w1, t, f, q1);
    c1 = phi3(L, w0, r, Idc, I1, w1, t, q, f, q1);

    a2 = phi1(q1 + dt * c1/2);
    b2 = phi2(Idc, I1, w1, t + dt / 2, f + dt * b1 / 2, q1 + dt * c1 / 2);
    c2 = phi3(L, w0, r, Idc, I1, w1, t + dt / 2, q + dt * a1 / 2, f + dt * b1 / 2, q1 + dt * c1 / 2);

    a3 = phi1(q1 + dt * c2 / 2);
    b3 = phi2(Idc, I1, w1, t + dt / 2, f + dt * b2 / 2, q1 + dt * c2 / 2);
    c3 = phi3(L, w0, r, Idc, I1, w1, t + dt / 2, q + dt * a2 / 2, f + dt * b2 / 2, q1 + dt * c2 / 2);

    a4 = phi1(q1 + dt * c3);
    b4 = phi2(Idc, I1, w1, t + dt, f + dt * b3, q1 + dt * c3);
    c4 = phi3(L, w0, r, Idc, I1, w1, t + dt, q + dt * a3, f + dt * b3, q1 + dt * c3);

    q = q + dt * (a1 + 2 * a2 + 2 * a3 + a4) / 6;
    f = f + dt * (b1 + 2 * b2 + 2 * b3 + b4) / 6;
    q1 = q1 + dt * (c1 + 2 * c2 + 2 * c3 + c4) / 6;

    if (f > pi2) {
      f = f - pi2;
    }

    res << t << "     " << q << "     " << f << "     " << q1 << "\n";
          
    t = t  +  dt;
  }

  res.close();
  return 0;
}

double phi1(double q1) {
  double F;
  F = q1;
  return F;
}

double phi2(double Idc, double I1, double w1, double t, double f, double q1) {
  double F;
  F = Idc + I1 * cos(w1 * t) - sin(f) - q1;
  return F;
}

double phi3(double L,double w0, double r, double Idc, double I1, double w1, double t, double q, double f, double q1)
{
  double F;
  F = ( Idc + I1 * cos(w1 * t) - sin(f) ) / L - (r + 1) / L * q1 - q * w0 * w0;
  return F;
}
