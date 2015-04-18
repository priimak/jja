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
  double pi2 = pi * pi;

  double at,dt1,Idc0,Idcn,dIdc,Idc,w1,I1,f0,T1,tn,t0,h,t,b,q0,q10,f1,L,r,a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,V,Vs,q1s,P,Psc,q1sc,dVs,dq1s,dPsc,dq1sc,Ijj,w0;
  double f,q,q1;
  int n;
  cout << "Idc0=";
  cin >> Idc0;
  cout << "Idcn=";
  cin >> Idcn;
  cout << "dI=";
  cin >> dIdc;
  cout << "w1=";
  cin >> w1;
  cout << "I1=";
  cin >> I1;
  cout << "f0=";
  cin >> f0;
  cout << "n=";
  cin >> n;
  cout << "w0=";
  cin >> w0;
  cout << "b=";
  cin >> b;
  cout << "r=";
  cin >> r;
  cout << "dt=";
  cin >> h;
  cout << "dt1=";
  cin >> dt1;
  cout << "at=";
  cin >> at;
  res.open("absorpsingleres.txt", std::ios_base::app);
  L=1/(b*w0*w0);
  Idc=Idc0;
  t0=0;
  T1=2*pi/w1;
  tn=at+n*T1;

  while(Idc<=Idcn) {
    cout << "Idc=" << Idc << ", l=" << l << "\n";
    t=t0;
    q=0;
    f=f0;
    q1=0;
    Vs=0;
    q1s=0;
    Psc=0;
    q1sc=0;
    while(t<=at) {
      f1=f;
      q10=q1;
      a1=phi1(q1);
      b1=phi2(Idc,I1,w1,t,f,q1);
      c1=phi3(L,w0,r,Idc,I1,w1,t,q,f,q1);

      a2=phi1(q1+dt1*c1/2);
      b2=phi2(Idc,I1,w1,t+dt1/2,f+dt1*b1/2,q1+dt1*c1/2);
      c2=phi3(L,w0,r,Idc,I1,w1,t+dt1/2,q+dt1*a1/2,f+dt1*b1/2,q1+dt1*c1/2);

      a3=phi1(q1+dt1*c2/2);
      b3=phi2(Idc,I1,w1,t+dt1/2,f+dt1*b2/2,q1+dt1*c2/2);
      c3=phi3(L,w0,r,Idc,I1,w1,t+dt1/2,q+dt1*a2/2,f+dt1*b2/2,q1+dt1*c2/2);
      a4=phi1(q1+dt1*c3);
      b4=phi2(Idc,I1,w1,t+dt1,f+dt1*b3,q1+dt1*c3);
      c4=phi3(L,w0,r,Idc,I1,w1,t+dt1,q+dt1*a3,f+dt1*b3,q1+dt1*c3);
      q=q+dt1*(a1+2*a2+2*a3+a4)/6;
      f=f+dt1*(b1+2*b2+2*b3+b4)/6;
      q1=q1+dt1*(c1+2*c2+2*c3+c4)/6;
      t=t+dt1;
      
      if (f > pi2) {
        f = f - pi2;
      }

      res << t << "     " << q << "     " << f << "     " << q1 << "\n";
    }
    /*
    while(t<=tn) {
      f1=f;
      q10=q1;
      a1=phi1(q1);
      b1=phi2(Idc,I1,w1,t,f,q1);
      c1=phi3(L,w0,r,Idc,I1,w1,t,q,f,q1);
      a2=phi1(q1+h*c1/2);
      b2=phi2(Idc,I1,w1,t+h/2,f+h*b1/2,q1+h*c1/2);
      c2=phi3(L,w0,r,Idc,I1,w1,t+h/2,q+h*a1/2,f+h*b1/2,q1+h*c1/2);
      a3=phi1(q1+h*c2/2);
      b3=phi2(Idc,I1,w1,t+h/2,f+h*b2/2,q1+h*c2/2);
      c3=phi3(L,w0,r,Idc,I1,w1,t+h/2,q+h*a2/2,f+h*b2/2,q1+h*c2/2);
      a4=phi1(q1+h*c3);
      b4=phi2(Idc,I1,w1,t+h,f+h*b3,q1+h*c3);
      c4=phi3(L,w0,r,Idc,I1,w1,t+h,q+h*a3,f+h*b3,q1+h*c3);
      q=q+h*(a1+2*a2+2*a3+a4)/6;
      f=f+h*(b1+2*b2+2*b3+b4)/6;
      q1=q1+h*(c1+2*c2+2*c3+c4)/6;
      t=t+h;
      dVs=(sin(f1)+sin(f))*h/2;
      Vs=Vs+dVs;
      dq1s=(q10+q1)*h/2;
      q1s=q1s+dq1s;
      dPsc=(sin(f1)*cos(w1*(t-h))+sin(f)*cos(w1*t))*h/2;
      Psc=Psc+dPsc;
      dq1sc=(q10*cos(w1*(t-h))+q1*cos(w1*t))*h/2;
      q1sc=q1sc+dq1sc;
    }
    V=Idc-Vs/(n*T1)-q1s/(n*T1);
    P=I1/2-Psc/(n*T1)-q1sc/(n*T1);
    Ijj=Idc-q1s/(n*T1);
    //res << Idc << "     " << Ijj << "     " << V << "     " << P << "\n";
    */
    Idc=Idc+dIdc;
  }
  res.close();
  return 0;
}

double phi1(double q1) {
  double F;
  F=q1;
  return F;
}

double phi2(double Idc, double I1, double w1, double t, double f, double q1) {
  double F;
  F = Idc + I1*cos(w1*t) - sin(f) - q1;
  return F;
}

double phi3(double L,double w0, double r, double Idc, double I1, double w1, double t, double q, double f, double q1) {
  double F;
  F = (Idc+I1*cos(w1*t)-sin(f))/L - (r+1)/L*q1 - q*w0*w0;
  return F;
}
