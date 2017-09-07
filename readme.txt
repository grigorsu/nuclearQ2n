0)  settings.exe: 

  g++ -static -static-libgcc -static-libstdc++ settings.cc -o ../bin/settings.exe
  g++ settings.cc -o ../bin/settings.exe

run: 
  ../bin/setting.exe  -A1 40 -Z1 20 -A2 40 -Z2 18 

Example:
  i :
  ../bin/settings.exe  -A1 40 -Z1 20 -A2 40 -Z2 18 -print console
  Ca[A = 40  ,Z = 20  ]  m: -34846.4          beta: 0            d: 0.55        beta[2+] : 0.123       E[2+] : 3.904  exist: 1
  Ar[A = 40  ,Z = 18  ]  m: -35039.9          beta: 0.251        d: 0.55        beta[2+] : 0.251       E[2+] : 1.461  exist: 1
  Q: 3.37578  MeV E[2+]:   1.525MeV
  Ca[A = 42  ,Z = 20  ]  m: -38547.2          beta: 0.247        d: 0.55        beta[2+] : 0.247       E[2+] : 1.525  exist: 1
  Ar[A = 38  ,Z = 18  ]  m: -34714.8          beta: 0.163        d: 0.55        beta[2+] : 0.163       E[2+] : 2.167  exist: 1
  Q: -13.4048  MeV  E[2+]:   1.208MeV
  Ca[A = 38  ,Z = 20  ]  m: -22058.5          beta: 0            d: 0.55        beta[2+] : 0.125       E[2+] : 2.206  exist: 1
  Ar[A = 42  ,Z = 18  ]  m: -34423            beta: 0.275        d: 0.55        beta[2+] : 0.275       E[2+] : 1.208  exist: 1
   Q2n channel found: Q: 3.37578 E[2+]: 1.525
  Ca[A = 42  ,Z = 20  ]  m: -38547.2          beta: 0.247        d: 0.55        beta[2+] : 0.247       E[2+] : 1.525  exist: 1
  Ar[A = 38  ,Z = 18  ]  m: -34714.8          beta: 0.163        d: 0.55        beta[2+] : 0.163       E[2+] : 2.167  exist: 1
  ii : 
  printf "\n\n\n\n\n\n\n"; g++ settings.cc -o ../bin/settings.exe && ../bin/settings.exe -A1 40 -A2 40 -Z1 20 -Z2 22 -o ../conf/conf.out.txt -print console




1) fort.exe: This programm build potential for 

compile in the folder fort:
  linux:
    gfortran P4C.FOR P4.FOR P12B.FOR P12UU4.FOR -o ../bin/fort.exe
  windows:
    gfortran P4C.FOR P4.FOR P12B.FOR P12UU4.FOR -o ../bin/fort.exe

for static run: 
  linux:
    ???? 
  windows:
    ???? 

