        SUBROUTINE SYM_GT(RI11,RI22,RI33)
C  IF R > R01+R02   RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,N
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        DIMENSION T(2,3)
	S1 = 0.0
	S2 = 0.0
	S3 = 0.0
	S4 = 0.0
      S5 = 0.0
      S6 = 0.0
        R1A1 = R01/A01
        R1A2 = R01/A02
        R2A1 = R02/A01
        R2A2 = R02/A02
        A1A2 = A01/A02
        A2A1 = A02/A01
        K=0
        DO 14 I=1,N
        K=I
        IF(I.EQ.1) GO TO 14
        IF((IDINT(A2A1*I+0.015)-IDINT(A2A1*I)).GT.0..OR.(
     /   IDINT(A2A1*I-0.015)-IDINT(A2A1*I)).LT.0) GO TO 12
        IF((IDINT(A1A2*I+0.015)-IDINT(A1A2*I)).GT.0..OR.(
     /   IDINT(A1A2*I-0.015)-IDINT(A1A2*I)).LT.0) GO TO 12
   14   CONTINUE
   12   NNN=K-2
        INT = -1
C
        WRITE(*,*) 'N=', NNN
        DO 11 I=1,NNN
           SPA2 = DSIN(PI*I*A2A1)
           SPA1 = DSIN(PI*I*A1A2)
           CPA2 = DCOS(PI*I*A2A1)
           CPA1 = DCOS(PI*I*A1A2)
           T(1,1) =  PIA02/SPA2
           T(2,1) =  PIA01/SPA1
           T(1,2) =  PIA02*PIA02/SPA2/SPA2*CPA2
           T(2,2) =  PIA01*PIA01/SPA1/SPA1*CPA1
           T(1,3) = PIA02*PIA02*PIA02/2*(1+CPA2*CPA2)/SPA2/SPA2/SPA2
           T(2,3) = PIA01*PIA01*PIA01/2*(1+CPA1*CPA1)/SPA1/SPA1/SPA1
         EXA1 = DEXP(-I*R/A01)
         EXA2 = DEXP(-I*R/A02)
         S1 = S1+EXA1/I*DSINH(R1A1*I)*INT*((R02*R02*T(1,1)+(A01/I+R)*
     /   T(1,2)+2.*T(1,3))*DSINH(R2A1*I)-
     /   (2.*R02*T(1,2)+R02*(R+A01/I)*T(1,1))*DCOSH(R2A1*I))
         S2 = S2+EXA2/I*DSINH(R2A2*I)*INT*((R01*R01*T(2,1)+(A02/I+R)*
     /   T(2,2)+2.*T(2,3))*DSINH(R1A2*I)-
     /   (2.*R01*T(2,2)+R01*(R+A02/I)*T(2,1))*DCOSH(R1A2*I))
         S3 = S3+EXA1*DCOSH(R1A1*I)*INT*((R02*R02*T(1,1)+(A01/I+R)*
     /   T(1,2)+2.*T(1,3))*DSINH(R2A1*I)-
     /   (2.*R02*T(1,2)+R02*(R+A01/I)*T(1,1))*DCOSH(R2A1*I))
         S4 = S4+EXA2/I*DSINH(R2A2*I)*INT*((I*R1A2*R01*T(2,1)+
     /   (R*I/A02-1)*T(2,2)+2.*T(2,3)*I/A02-(R+A02/I)*T(2,1))*
     /   DCOSH(R1A2*I)-(2.*R1A2*I*T(2,2)+R01*(I*R/A02-1)*T(2,1))
     /   *DSINH(R1A2*I))
         S5 = S5+EXA2*DCOSH(R2A2*I)*INT*((R01*R01*T(2,1)+(A02/I+R)*
     /   T(2,2)+2.*T(2,3))*DSINH(R1A2*I)-
     /   (2.*R01*T(2,2)+R01*(R+A02/I)*T(2,1))*DCOSH(R1A2*I))
         S6 = S6+EXA1/I*DSINH(R1A1*I)*INT*((I*R2A1*R02*T(1,1)+
     /   (R*I/A01-1)*T(1,2)+2.*T(1,3)*I/A01-(R+A01/I)*T(1,1))*
     /   DCOSH(R2A1*I)-(2.*R2A1*I*T(1,2)+R02*(I*R/A01-1)*T(1,1))
     /   *DSINH(R2A1*I))
         INT = INT*(-1)
 11     CONTINUE
        RI11 = CONSTP1*(CHSH*(S1/A02+S2/A01)+S3/A01/A02+S4/A01)
        RI22 = CONSTP1*A02/A01*
     /   (CHSH2*(S1/A02+S2/A01)+S5/A01/A02+S6/A02)
        RI33 = -CONSTP1/A01*(S1/A02+S2/A01)
        WRITE(*,*) 'S1=', S1,'S2=',S2,'S3=',S3
        WRITE(*,*) 'S4=', S4,'S5=',S5,'S6=',S6
	RETURN
	END
C-------------------------------------------------------------------------
        SUBROUTINE SYM_LT(RI11,RI22,RI33)
C  IF R < R01+R02   RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,N
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        DIMENSION T(2,3)
C        N=60
C        WRITE(*,*) N
        KEY = 1
        IF(R+R01-R02.LT.0.) KEY = -1
        S1 = 0.0
        S2 = 0.0
        S3 = 0.0
        S4 = 0.0
        S5 = 0.0
        S6 = 0.0
        INT = -1
        A1A2 = A01/A02
        A2A1 = A02/A01
        RR2 = R + R02
        R2R2 = R + 2.0*R02
        R2MR = R02 - R
        DR2MR = 2.0*R02 - R
        RR1 = R + R01
        R2R1 = R + 2.0*R01
        R1MR = R01 - R
        DR1MR = 2.0*R01 - R
        K=0
        DO 14 I=1,N
        K=I
        IF(I.EQ.1) GO TO 14
        IF((IDINT(A2A1*I+0.015)-IDINT(A2A1*I)).GT.0..OR.(
     /   IDINT(A2A1*I-0.015)-IDINT(A2A1*I)).LT.0) GO TO 12
        IF((IDINT(A1A2*I+0.015)-IDINT(A1A2*I)).GT.0..OR.(
     /   IDINT(A1A2*I-0.015)-IDINT(A1A2*I)).LT.0) GO TO 12
   14   CONTINUE
   12   NNN=K-2
C        WRITE(*,*) 'OK'
        DO 11 I=1,NNN
           SPA2 = DSIN(PI*I*A2A1)
           SPA1 = DSIN(PI*I*A1A2)
           CPA2 = DCOS(PI*I*A2A1)
           CPA1 = DCOS(PI*I*A1A2)
           T(1,1) =  PIA02/SPA2
           T(2,1) =  PIA01/SPA1
           T(1,2) =  PIA02*PIA02/SPA2/SPA2*CPA2
           T(2,2) =  PIA01*PIA01/SPA1/SPA1*CPA1
           T(1,3) = PIA02*PIA02*PIA02/2.*(1.+CPA2*CPA2)/SPA2/SPA2/SPA2
           T(2,3) = PIA01*PIA01*PIA01/2.*(1.+CPA1*CPA1)/SPA1/SPA1/SPA1
           T13 = 2.0*T(1,3)
           T23 = 2.0*T(2,3)
           EXP11 = DEXP(-I*(R+R01+R02)/A01)
           EXP12 = DEXP(-I*(R+R01+R02)/A02)
           EXP21 = DEXP(-I*(-R+R01+R02)/A01)
           EXP22 = DEXP(-I*(-R+R01+R02)/A02)
           EXP31 = DEXP(-I*(R-R01+R02)/A01)
           EXP32 = DEXP(-I*(R-R01+R02)/A02)
           EXP41 = DEXP(-I*KEY*(R+R01-R02)/A01)
           EXP42 = DEXP(-I*KEY*(R+R01-R02)/A02)
          S1 = S1 + INT*(EXP11/I*((RR2+A01/I)*R02*T(1,1)+(R2R2+A01/I)
     /    *T(1,2)+T13)-EXP21/I*((R2MR+A01/I)*R02*T(1,1)+(DR2MR+A01/I)
     /    *T(1,2)+T13)-EXP31/I*((RR2+A01/I)*R02*T(1,1)+(R2R2+A01/I)
     /    *T(1,2)+T13)-KEY*EXP41/I*((R2MR-KEY*A01/I)*R02*T(1,1)+
     /     (A01/I-KEY*DR2MR)*T(1,2)+T13))
          S2 = S2 + INT*(EXP12/I*((RR1+A02/I)*R01*T(2,1)+(R2R1+A02/I)
     /    *T(2,2)+T23)-EXP22/I*((R1MR+A02/I)*R01*T(2,1)+(DR1MR+A02/I)
     /    *T(2,2)+T23)-EXP32/I*((R1MR-A02/I)*R01*T(2,1)+(A02/I-DR1MR)
     /    *T(2,2)+T23)-KEY*EXP42/I*((RR1+KEY*A02/I)*R01*T(2,1)+
     /     (KEY*R2R1+A02/I)*T(2,2)+T23))
          S3 = S3 + INT*(-EXP11/A01*((RR2+A01/I)*R02*T(1,1)+(R2R2+A01/I)
     /    *T(1,2)+T13)+EXP21/A01*((R2MR+A01/I)*R02*T(1,1)+(DR2MR+A01/I)
     /    *T(1,2)+T13)-EXP31/A01*((RR2+A01/I)*R02*T(1,1)+(R2R2+A01/I)
     /    *T(1,2)+T13)+EXP41/A01*((R2MR-KEY*A01/I)*R02*T(1,1)+
     /     (A01/I-KEY*DR2MR)*T(1,2)+T13))
          S4 = S4 + INT*(-EXP12/A02*((RR1+A02/I)*R01*T(2,1)+(R2R1-A02/I)
     /     *T(2,2)+T23-A02/I*(R2R1+A02/I)*T(2,1))
     /    +EXP22/A02*((R1MR+A02/I)*R01*T(2,1)+(DR1MR-A02/I)*T(2,2)
     /     +T23-A02/I*T(2,1)*(DR1MR+A02/I))
     /    -EXP32/A02*((R1MR-A02/I)*R01*T(2,1)+(-A02/I-DR1MR)*T(2,2)+T23
     /     +(DR1MR-A02/I)*T(2,1)*A02/I)
     /    +EXP42/A02*((RR1+KEY*A02/I)*R01*T(2,1)+
     /     (KEY*R2R1-A02/I)*T(2,2)+T23-
     /     KEY*A02/I*T(2,1)*(R2R1+KEY*A02/I)))
          S5 = S5 + INT*(-EXP12/A02*((RR1+A02/I)*R01*T(2,1)+(R2R1+A02/I)
     /    *T(2,2)+T23)+EXP22/A02*((R1MR+A02/I)*R01*T(2,1)+(DR1MR+A02/I)
     /    *T(2,2)+T23)+EXP32/A02*((R1MR-A02/I)*R01*T(2,1)+(-DR1MR+A02/I)
     /    *T(2,2)+T23)-EXP42/A02*((RR1+KEY*A02/I)*R01*T(2,1)+
     /     (A02/I+KEY*R2R1)*T(2,2)+T23))
          S6 = S6 + INT*(-EXP11/A01*((RR2+A01/I)*R02*T(1,1)+(R2R2-A01/I)
     /     *T(1,2)+T13-A01/I*(R2R2+A01/I)*T(1,1))
     /    +EXP21/A01*((R2MR+A01/I)*R02*T(1,1)+(DR2MR-A01/I)*T(1,2)
     /     +T13-A01/I*T(1,1)*(DR2MR+A01/I))
     /    +EXP31/A01*((RR2+A01/I)*R02*T(1,1)+(-A01/I+R2R2)*T(1,2)+T13
     /     -(R2R2+A01/I)*T(1,1)*A01/I)
     /    -EXP41/A01*((R2MR-KEY*A01/I)*R02*T(1,1)+
     /     (-KEY*DR2MR-A01/I)*T(1,2)+T13+
     /     KEY*A01/I*T(1,1)*(DR2MR-KEY*A01/I)))
        INT = INT*(-1)
 11     CONTINUE
        IF(KEY.EQ.1) THEN
        RI11 = CONSTP1/2.*( CHSH* (S1/A02 + S2/A01+
     /   (-R*(-2.*(R01+R02)*(R01+R02)*(R01+R02)+6.*(R01+R02)*R01*R02-
     /   PI*PI*(A01*A01*R01+A02*A02*R02))
     /    -7./20.*PI*PI*PI*PI*(A02**4+A01**4)+
     /   PI*PI*PI*PI*A01*A01*A02*A02/6.+PI*PI*(R01-R02)*(A02*A02*R01-
     /   A01*A01*R02)-PI*PI/2.*(A01*A01+A02*A02)*(R*R+(R01-R02)**2)
     /   -3.*R01*R02*(R*R+(R01-R02)**2)-(3.*R*R+(R01-R02)**2)*
     /   (R01-R02)**2+((R+R01-R02)**4+(R-R01+R02)**4)/8.)/6./A01/A02)
     *    + S3/A02 + S4/A01+
     /   (-R*(-6.*(R01+R02)**2+
     /    6.*R01*R02+6.*R02*(R01+R02)-PI*PI*A01*A01)+
     /   PI*PI*R01*(A02*A02-A01*A01)-
     /   3.*R02*(R*R+(R01-R02)**2)-6.*R01*
     /   R02*(R01-R02)-3.*(R*R+(R01-R02)**2)*(R01-R02))
     /   /6./A01/A02)
        FI = CONSTP1/2.*( CHSH*(-R*(-PI*PI*(A02*A02*R02+A01*A01*R01))-
     /   PI*PI*R01*R02*(A01*A01+A02*A02)+PI*PI*(R01-R02)*(R02*A02*A02-
     /   R01*A01*A01))-R*(-PI*PI*A01*A01)-PI*PI*R02*(A01*A01+A02*A02)+
     /   PI*PI*(R02*A02*A02-R01*A01*A01)-PI*PI*A01*A01*(R01-R02))
     /    /6./A01/A02
        RI22 = CONSTP1*A02/A01/2.*( CHSH2*(S1/A02 + S2/A01+
     /   (-R*(-2.*(R01+R02)*(R01+R02)*(R01+R02)+6.*(R01+R02)*R01*R02-
     /   PI*PI*(A01*A01*R01+A02*A02*R02))
     /    -7./20.*PI*PI*PI*PI*(A02**4+A01**4)+
     /   PI*PI*PI*PI*A01*A01*A02*A02/6.+PI*PI*(R01-R02)*(A02*A02*R01-
     /   A01*A01*R02)-PI*PI/2.*(A01*A01+A02*A02)*(R*R+(R01-R02)**2)
     /   -3.*R01*R02*(R*R+(R01-R02)**2)-(3.*R*R+(R01-R02)**2)*
     /   (R01-R02)**2+((R+R01-R02)**4+(R-R01+R02)**4)/8.)/6./A01/A02)
     *    + S5/A01 + S6/A02+
     /   (-R*(-6.*(R01+R02)**2+
     /    6.*R01*R02+6.*R01*(R01+R02)-PI*PI*A02*A02)+
     /   PI*PI*R02*(A01*A01-A02*A02)-
     /   3.*R01*(R*R+(R01-R02)**2)+6.*R01*
     /   R02*(R01-R02)+3.*(R*R+(R01-R02)**2)*(R01-R02))
     /   /6./A01/A02)
        FI2 = CONSTP1*A02/A01/2.*
     /   ( CHSH2*(-R*(-PI*PI*(A02*A02*R02+A01*A01*R01))-
     /   PI*PI*R01*R02*(A01*A01+A02*A02)+PI*PI*(R01-R02)*(R02*A02*A02-
     /   R01*A01*A01))-R*(-PI*PI*A02*A02)-PI*PI*R01*(A01*A01+A02*A02)-
     /   PI*PI*(R02*A02*A02-R01*A01*A01)+PI*PI*A02*A02*(R01-R02))
     /    /6./A01/A02
        RI33 = -CONSTP1/2./A01*(S1/A02 + S2/A01+
     /   (-R*(-2.*(R01+R02)*(R01+R02)*(R01+R02)+6.*(R01+R02)*R01*R02-
     /   PI*PI*(A01*A01*R01+A02*A02*R02))
     /    -7./20.*PI*PI*PI*PI*(A02**4+A01**4)+
     /   PI*PI*PI*PI*A01*A01*A02*A02/6.+PI*PI*(R01-R02)*(A02*A02*R01-
     /   A01*A01*R02)-PI*PI/2.*(A01*A01+A02*A02)*(R*R+(R01-R02)**2)
     /   -3.*R01*R02*(R*R+(R01-R02)**2)-(3.*R*R+(R01-R02)**2)*
     /   (R01-R02)**2+((R+R01-R02)**4+(R-R01+R02)**4)/8.)/6./A01/A02)
        FI3 = -CONSTP1/2./A01*(-R*(-PI*PI*(A02*A02*R02+A01*A01*R01))-
     /   PI*PI*R01*R02*(A01*A01+A02*A02)+PI*PI*(R01-R02)*(R02*A02*A02-
     /   R01*A01*A01))/6./A01/A02
         ELSE
        RI11 = CONSTP1/2.*( CHSH* (S1/A02 + S2/A01+
     /   R*(2.*(R01+R02)*(R01+R02)*(R01+R02)+
     /    2.*(R01-R02)*(R01-R02)*(R01-R02)-12.*R01*R02*R02+
     /   4.*PI*PI*A01*A01*R01)/6./A01/A02)
     *    + S3/A02 + S4/A01+
     /   (R*(6.*(R01+R02)**2+
     /    6.*(R01-R02)**2-12.*R02*R02+4.*PI*PI*A01*A01))
     /   /6./A01/A02)
        FI = 0.
        RI22 = CONSTP1*A02/A01/2.*( CHSH2*(S1/A02 + S2/A01+
     /   R*(2.*(R01+R02)*(R01+R02)*(R01+R02)+
     /    2.*(R01-R02)*(R01-R02)*(R01-R02)-12.*R01*R02*R02+
     /   4.*PI*PI*A01*A01*R01)/6./A01/A02)
     *    + S5/A01 + S6/A02+
     /   R*(6.*(R01+R02)**2-
     /    6.*(R01-R02)**2-24.*R01*R02)
     /   /6./A01/A02)
        FI2 = 0.
        RI33 = -CONSTP1/2./A01*(S1/A02 + S2/A01+
     /   R*(2.*(R01+R02)*(R01+R02)*(R01+R02)+
     /    2.*(R01-R02)*(R01-R02)*(R01-R02)-12.*R01*R02*R02+
     /   4.*PI*PI*A01*A01*R01)/6./A01/A02)
        FI3 = 0.
        ENDIF
C            WRITE(*,*) 'CONSTP1', CONSTP1,'CHSH',CHSH,'FI',FI
C             WRITE(*,*) 'R01',R01,'R02',R02,'R',R,'A01',A01,'A02',A02
C             WRITE(*,*) 'RI11PR',RI11,' N',N
C             WRITE(*,*) 'S1',S1,' S2',S2,' S3',S3,' S4',S4
	       RI11 = 0.5*(RI11+FI)
             RI22 = 0.5*(RI22+FI2)
             RI33 = 0.5*(RI33+FI3)
        RETURN
        END
C
        SUBROUTINE SUMEQ_GT(RI11,RI22,RI33)
C IF R>R01+R02 AND A01=A02 RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,N
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/RO/ RO00
        CONSTPE=-4.*PI/3.*RO00*RO00*RO00*A01*A01/R
C        CHSH=-DCOSH(R01/A01)/A01/DSINH(R01/A01)
        S1=0.0
        S2=0.0
        S3=0.0
        R01A=R01/A01
        R02A=R02/A01
        FI=2.*PI*PI/3.+(R01*R01+R02*R02)/A01/A01
        RQ=PI*PI*A01*A01+R01*R01+R02*R02
        DM=DMAX1(R01A,R02A)
        DO 11 I=1,N
        IF(I*DM.GE.600.) GO TO 12
        SR01=DSINH(I*R01A)
        SR02=DSINH(I*R02A)
        CR01=DCOSH(I*R01A)
        CR02=DCOSH(I*R02A)
        S1=S1+DEXP(-I*R/A01)/I*((R*R*R+3.*R*R*A01/I+6.*R*A01*A01/I/I
     /   +6.*A01*A01*A01/I/I/I-3.*A01*A01*(R+A01/I)*FI)*SR01*SR02+
     /   2.*RQ*(R01*CR01*SR02+R02*SR01*CR02)-
     /   2.*R01*R02*(R01*SR01*CR02+R02*CR01*SR02))
        S2=S2+DEXP(-I*R/A01)/I*(-6.*R01*(R+A01/I)*SR01*SR02+
     /   I/A01*(R*R*R+3.*R*R*A01/I+6.*R*A01*A01/I/I
     /   +6.*A01*A01*A01/I/I/I-3.*A01*A01*(R+A01/I)*FI)*CR01*SR02+
     /   4.*R01*(R01*CR01*SR02+R02*SR01*CR02)+2.*RQ*(CR01*SR02+
     /   R01A*I*SR01*SR02+R02A*I*CR01*CR02)-
     /   2.*R02*(R01*SR01*CR02+R02*CR01*SR02)-
     /   2.*R01*R02*(SR01*CR02+R01A*I*CR01*CR02+R02A*I*SR01*SR02))
        S3=S3+DEXP(-I*R/A01)/I*(-6.*R02*(R+A01/I)*SR01*SR02+
     /   I/A01*(R*R*R+3.*R*R*A01/I+6.*R*A01*A01/I/I
     /   +6.*A01*A01*A01/I/I/I-3.*A01*A01*(R+A01/I)*FI)*CR02*SR01+
     /   4.*R02*(R01*CR01*SR02+R02*SR01*CR02)+2.*RQ*(CR02*SR01+
     /   R02A*I*SR01*SR02+R01A*I*CR01*CR02)-
     /   2.*R01*(R01*SR01*CR02+R02*CR01*SR02)-
     /   2.*R01*R02*(SR02*CR01+R02A*I*CR01*CR02+R01A*I*SR01*SR02))
  11    CONTINUE
  12    RI11=CONSTPE*(CHSH*S1+S2)
        RI22=CONSTPE*(CHSH2*S1+S3)
        RI33=-CONSTPE/A01*S1
        WRITE(*,*)'R02',R02,'CHSH2',CHSH2,'I',I,'N',N
        WRITE(*,*)'S1',S1,'S2',S2,'S3',S3
        RETURN
        END
C
        SUBROUTINE SUMEQ_LT(RI11,RI22,RI33)
C IF R<R01+R02 AND A01=A02 RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,N
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/RO/ RO00
        KEY = 1
        IF(R+R01-R02.LT.0.) KEY = -1
        S1=0.0
        S2=0.0
        S3=0.0
        R1R2=R01+R02
        RR1P2=R*R-R1R2*R1R2
        RR12=R+R01+R02
        R12MR=R01+R02-R
        FI=R01*R02-PI*PI*A01*A01/3.
        R1MR2=R01-R02
        RR1M2=R*R-R1MR2*R1MR2
        FIM=-R01*R02-PI*PI*A01*A01/3.
        DO 11 I=1,N
        SE1=DEXP(-I*(R+R01+R02)/A01)
        SE2=DEXP(-I*(-R+R01+R02)/A01)
        SE3=DEXP(-KEY*I*(R+R01-R02)/A01)
        SE4=DEXP(-I*(R-R01+R02)/A01)
        S1=S1+1./I*(SE1*(R*R*R-3.*R*R1R2*R1R2-2.*R1R2*R1R2*R1R2
     /   +3.*A01*RR1P2/I+6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(RR12+A01/I)*FI)-SE2*(-R*R*R+3.*R*R1R2*R1R2-
     /    2.*R1R2*R1R2*R1R2+3.*A01*RR1P2/I-6.*A01*A01*R/I/I+
     /    6.*A01*A01*A01/I/I/I+6.*(R12MR+A01/I)*FI)-KEY*
     /    SE3*(KEY*R*R*R-KEY*3.*R*R1MR2*R1MR2-KEY*2.*R1MR2*R1MR2*R1MR2+
     /    3.*A01*
     /    RR1M2/I+KEY*6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(KEY*R+KEY*R01-KEY*R02+A01/I)*FIM)-
     /    SE4*(R*R*R-3.*R*R1MR2*R1MR2+
     /    2.*R1MR2*R1MR2*R1MR2+3.*A01*
     /    RR1M2/I+6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(R-R01+R02+A01/I)*FIM))
        S2=S2+1./I*(-I/A01*SE1*(R*R*R-3.*R*R1R2*R1R2-2.*R1R2*R1R2*R1R2
     /   +3.*A01*RR1P2/I+6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(RR12+A01/I)*FI+A01/I*(6.*R*R01+6.*R1R2*R1R2+6.*A01*R01/I)
     /   -6.*A01/I*FI-6.*A01*R02/I*R1R2)+I/A01*SE2*(-R*R*R+
     /    3.*R*R1R2*R1R2-
     /    2.*R1R2*R1R2*R1R2+3.*A01*RR1P2/I-6.*A01*A01*R/I/I+
     /    6.*A01*A01*A01/I/I/I+6.*(R12MR+A01/I)*FI-6.*A01/I*(R*R01-
     /    R1R2*R1R2-A01*R01/I)-6.*A01/I*FI-6.*A01/I*R02*R1R2)+
     /    I/A01*SE3*(KEY*R*R*R-KEY*3.*R*R1MR2*R1MR2-
     /    KEY*2.*R1MR2*R1MR2*R1MR2+3.*A01*
     /    RR1M2/I+KEY*6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(KEY*R+KEY*R01-KEY*R02+A01/I)*FIM+
     /    6.*A01/I*(R*R01+R1MR2*R1MR2+
     /    KEY*A01*R01/I)-6.*A01/I*FIM+6.*A01/I*R02*R1MR2)-
     /    I/A01*SE4*(R*R*R-3.*R*R1MR2*R1MR2+
     /    2.*R1MR2*R1MR2*R1MR2+3.*A01*
     /    RR1M2/I+6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(R-R01+R02+A01/I)*FIM+6.*A01/I*(-R*R01+R1MR2*R1MR2-
     /    A01*R01/I)-6.*A01/I*FIM+6.*A01/I*R02*R1MR2))
        S3=S3+1./I*(-I/A01*SE1*(R*R*R-3.*R*R1R2*R1R2-2.*R1R2*R1R2*R1R2
     /   +3.*A01*RR1P2/I+6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(RR12+A01/I)*FI+A01/I*(6.*R*R02+6.*R1R2*R1R2+6.*A01*R02/I)
     /   -6.*A01/I*FI-6.*A01*R01/I*R1R2)+I/A01*SE2*(-R*R*R+
     /    3.*R*R1R2*R1R2-
     /    2.*R1R2*R1R2*R1R2+3.*A01*RR1P2/I-6.*A01*A01*R/I/I+
     /    6.*A01*A01*A01/I/I/I+6.*(R12MR+A01/I)*FI-6.*A01/I*(R*R02-
     /    R1R2*R1R2-A01*R02/I)-6.*A01/I*FI-6.*A01/I*R01*R1R2)-
     /    I/A01*SE3*(KEY*R*R*R-KEY*3.*R*R1MR2*R1MR2-
     /    KEY*2.*R1MR2*R1MR2*R1MR2+3.*A01*
     /    RR1M2/I+KEY*6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(KEY*R+KEY*R01-KEY*R02+A01/I)*FIM+
     /    6.*A01/I*(-R*R02+R1MR2*R1MR2-
     /    KEY*A01*R02/I)-6.*A01/I*FIM-6.*A01/I*R01*R1MR2)-
     /    I/A01*SE4*(R*R*R-3.*R*R1MR2*R1MR2+
     /    2.*R1MR2*R1MR2*R1MR2+3.*A01*
     /    RR1M2/I+6.*A01*A01*R/I/I+6.*A01*A01*A01/I/I/I+
     /    6.*(R-R01+R02+A01/I)*FIM+6.*A01/I*(R*R02+R1MR2*R1MR2+
     /    A01*R02/I)-6.*A01/I*FIM-6.*A01/I*R01*R1MR2))
  11    CONTINUE
        IF(KEY.EQ.1) THEN
        T1=1./A01*(-R*(-2.*PI*PI*A01*A01*R1R2+6.*R01*R02*R1R2-
     /   2.*R1R2*R1R2*R1R2)-8.*PI**4*A01**4/15.-2.*PI*PI*A01*A01*R01*R02
     /  -(PI*PI*A01*A01+3.*R01*R02)*(R*R+R1MR2*R1MR2)-R1MR2*R1MR2*
     /   (R1MR2*R1MR2+3.*R*R)+((R+R01-R02)**4+(R-R01+R02)**4)/8.)
        T2=1./A01*(-R*(-2.*PI*PI*A01*A01-6.*R01*R01)-
     /  2.*PI*PI*A01*A01*R01-3.*R02*(R*R+R1MR2*R1MR2)
     /  -3.*R1MR2*(2.*R01*R02+R1MR2*R1MR2+R*R))
        T3=1./A01*(-R*(-2.*PI*PI*A01*A01-6.*R02*R02)-
     /  2.*PI*PI*A01*A01*R02-3.*R01*(R*R+R1MR2*R1MR2)
     /  +3.*R1MR2*(2.*R01*R02+R1MR2*R1MR2+R*R))
        ELSE
        T1=1./A01*(-R*(-2.*PI*PI*A01*A01*R1R2+6.*R01*R02*R1R2-
     /   2.*R1R2*R1R2*R1R2)+2.*R*R1MR2*(PI*PI*A01*A01+3.*R01*R02)
     /   +2.*R*R1MR2*R1MR2*R1MR2)
        T2=1./A01*(R*(4.*PI*PI*A01*A01-12.*R02*R02+6.*R1MR2*R1MR2+
     /   6.*R1R2*R1R2))
        T3=1./A01*(R*(-24.*R01*R02-6.*R1MR2*R1MR2+6.*R1R2*R1R2))
C             WRITE(*,*) 'T1',T1,' T2',T2
        ENDIF
        CONSTPE=-PI/3.*RO00*RO00*RO00*A01*A01/R
C             WRITE(*,*) 'S1',S1,' S2',S2
        RI11=CONSTPE*(CHSH*(S1+T1)+S2+T2)
        RI22=CONSTPE*(CHSH2*(S1+T1)+S3+T3)
        RI33=-CONSTPE/A01*(S1+T1)
        RETURN
        END
C--------------------------------------------------------------------------
        DOUBLE PRECISION FUNCTION FUNUA(P)
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,N
        COMMON/ALPHA/ CONAL,CONAL2
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/RO/ RO00
        COMMON/G/ GAM,GAS
        X=4.*GAM*GAM*P*R
        DJ4 = DSINH(X)/X
        X=2.*GAM*GAM*P*R
        DJ2 = DSINH(X)/X
        EX=1./(1.+DEXP((P-R02)/A02))
        EXA=DEXP(-GAM*GAM*(P*P+R*R))
        FUNUA = (FINN-FEXX)/RO00*(CONAL*EXA*EXA*DJ4*EX+
     /   EXA*DJ2*EX*EX*RO00*CONAL2)*P*P+
     /   FEXX*CONAL2*EXA*DJ2*EX*P*P
	RETURN
	END
C-----------------------------------------------------------------
        SUBROUTINE SUMAL_GT(RI11)
C IF R>R01+R02 AND ALPHA FOR A01 RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,NN
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/RO/ RO00
        COMMON/G/ GAM,GAS
        DIMENSION T(2,3)
        N=40
        INT=-1
        S1=0.0
        S2=0.0
C        R1A1 = R01/A01
C        R1A2 = R01/A02
C        R2A1 = R02/A01
C        R2A2 = R02/A02
        A1A2 = A01/A02
        A2A1 = A02/A01
        K=0
        DO 14 I=1,N
        K=I
        IF(I.EQ.1) GO TO 14
        IF((IDINT(A2A1*I+0.015)-IDINT(A2A1*I)).GT.0..OR.(
     /   IDINT(A2A1*I-0.015)-IDINT(A2A1*I)).LT.0) GO TO 12
        IF((IDINT(A1A2*I+0.015)-IDINT(A1A2*I)).GT.0..OR.(
     /   IDINT(A1A2*I-0.015)-IDINT(A1A2*I)).LT.0) GO TO 12
   14   CONTINUE
   12   N=K-2
        DO 11 I=1,N
           SPA2 = DSIN(PI*I*A2A1)
           SPA1 = DSIN(PI*I*A1A2)
           CPA2 = DCOS(PI*I*A2A1)
           CPA1 = DCOS(PI*I*A1A2)
           T(1,1) =  PIA02/SPA2
           T(2,1) =  PIA01/SPA1
           T(1,2) =  PIA02*PIA02/SPA2/SPA2*CPA2
           T(2,2) =  PIA01*PIA01/SPA1/SPA1*CPA1
           T(1,3) = PIA02*PIA02*PIA02/2*(1+CPA2*CPA2)/SPA2/SPA2/SPA2
           T(2,3) = PIA01*PIA01*PIA01/2*(1+CPA1*CPA1)/SPA1/SPA1/SPA1
           T13 = 2.*T(1,3)
        SE1 = DEXP(-I*R/A01)
        SE2 = DEXP(-I*R/A02)
        SH21 = DSINH(R02*I/A01)
        SH22 = DSINH(R02*I/A02)
        CH21=DCOSH(R02*I/A01)
        S1=S1+INT*SE1*((R02*R02*T(1,1)+(A01/I+R)*T(1,2)+T13)*SH21-
     /   (2.*R02*T(1,2)+R02*(R+A01/I)*T(1,1))*CH21)
        S2=S2+INT*SE2/I*SH22*((R*I/A02-1.)*T(2,2)+2.*I/A02*T(2,3)-
     /    (R+A02/I)*T(2,1))
        INT=-INT
  11    CONTINUE
        CONAL = 8.*PI*RO00*16.*(GAM*GAM/PI)**3*4.*A01/R
        RI11=CONAL*(S1+A02*S2)
        RETURN
        END
C
        SUBROUTINE SUMALP_GT(RI11)
C IF R>R01+R02 AND ALPHA FOR A01 RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,NN
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/RO/ RO00
        COMMON/G/ GAM,GAS
        DIMENSION T(2,3)
        N=40
        INT=-1
        S1=0.0
        S2=0.0
        S3=0.0
        S4=0.0
        A1A2 = A01/A02
        A2A1 = A02/A01
        K=0
        DO 14 I=1,N
        K=I
        IF(I.EQ.1) GO TO 14
        IF((IDINT(A2A1*I+0.015)-IDINT(A2A1*I)).GT.0..OR.(
     /   IDINT(A2A1*I-0.015)-IDINT(A2A1*I)).LT.0) GO TO 12
        IF((IDINT(A1A2*I+0.015)-IDINT(A1A2*I)).GT.0..OR.(
     /   IDINT(A1A2*I-0.015)-IDINT(A1A2*I)).LT.0) GO TO 12
   14   CONTINUE
   12   N=K-2
        DO 11 I=1,N
        IF(I*R02/A02.GE.600.) GO TO 15
           SPA2 = DSIN(PI*I*A2A1)
           SPA1 = DSIN(PI*I*A1A2)
           CPA2 = DCOS(PI*I*A2A1)
           CPA1 = DCOS(PI*I*A1A2)
           T(1,1) =  PIA02/SPA2
           T(2,1) =  PIA01/SPA1
           T(1,2) =  PIA02*PIA02/SPA2/SPA2*CPA2
           T(2,2) =  PIA01*PIA01/SPA1/SPA1*CPA1
           T(1,3) = PIA02*PIA02*PIA02/2*(1+CPA2*CPA2)/SPA2/SPA2/SPA2
           T(2,3) = PIA01*PIA01*PIA01/2*(1+CPA1*CPA1)/SPA1/SPA1/SPA1
        T23 = 2.*T(2,3)
        T13 = 2.*T(1,3)
        SE1=DEXP(-I*R/A01)
        SE2=DEXP(-I*R/A02)
        SH21=DSINH(R02*I/A01)
        SH22=DSINH(R02*I/A02)
        CH21=DCOSH(R02*I/A01)
        CH22=DCOSH(R02*I/A02)
        S1=S1+INT*SE2/I*SH22*(I/A02*((A02/I+R)*T(2,2)+T23)-2.*T(2,2)-
     /   (R+A02/I)*T(2,1))
        S2=S2+INT*SE1*((R02*R02*T(1,1)+(A01/I+R)*T(1,2)+T13)*SH21-
     /    (2.*R02*T(1,2)+R02*(R+A01/I)*T(1,1))*CH21)
        S3=S3+INT*SE2*CH22*(((A02/I+R)*T(2,2)+T23)*I/A02-2.*T(2,2)-
     /    (R+A02/I)*T(2,1))
        S4=S4+INT*SE1*(-(2.*R02*I/A01*T(1,2)+R02*(I*R/A01-1.)*T(1,1))
     /  *SH21
     /  +(I*R02*R02/A01*T(1,1)+(R*I/A01-1.)*T(1,2)+2.*I/A01*T(1,3)-
     /   (R+A01/I)*T(1,1))*CH21)
        INT=-INT
  11    CONTINUE
C        CHSH=-DCOSH(R02/A02)/A02/DSINH(R02/A02)
  15    CONALP = -128.*PI*RO00*RO00*A01*A02/R*(GAM*GAM/PI)**(1.5)
        RI11=CONALP*(CHSH2*A02*(S1+S2/A02)+S3+S4)
        RETURN
        END
C-------------------------------------------------------------------------
        SUBROUTINE SUMAL_LT(RI11)
C IF R<R01+R02 AND ALPHA FOR A01 RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,NN
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/RO/ RO00
        COMMON/G/ GAM,GAS
        DIMENSION T(2,3)
        N = 40
        INT=-1
        S1=0.0
        A01 = 1./DSQRT(2D0)/GAS/GAM
        PIA01 = PI*A01
        A1A2 = A01/A02
        A2A1 = A02/A01
        K=0
        DO 14 I=1,N
        K=I
        IF(I.EQ.1) GO TO 14
        IF((IDINT(A2A1*I+0.015)-IDINT(A2A1*I)).GT.0..OR.(
     /   IDINT(A2A1*I-0.015)-IDINT(A2A1*I)).LT.0) GO TO 12
        IF((IDINT(A1A2*I+0.015)-IDINT(A1A2*I)).GT.0..OR.(
     /   IDINT(A1A2*I-0.015)-IDINT(A1A2*I)).LT.0) GO TO 12
   14   CONTINUE
   12   N=K-2
        DO 11 I=1,N
           SPA2 = DSIN(PI*I*A2A1)
           SPA1 = DSIN(PI*I*A1A2)
           CPA2 = DCOS(PI*I*A2A1)
           CPA1 = DCOS(PI*I*A1A2)
           T(1,1) =  PIA02/SPA2
           T(2,1) =  PIA01/SPA1
           T(1,2) =  PIA02*PIA02/SPA2/SPA2*CPA2
           T(2,2) =  PIA01*PIA01/SPA1/SPA1*CPA1
           T(1,3) = PIA02*PIA02*PIA02/2*(1+CPA2*CPA2)/SPA2/SPA2/SPA2
           T(2,3) = PIA01*PIA01*PIA01/2*(1+CPA1*CPA1)/SPA1/SPA1/SPA1
        SE1=DEXP(-I*(R+R02)/A01)
        SE12=DEXP(-I*(R02-R)/A01)
        SE2=DEXP(-I*(R+R02)/A02)
        SE22=DEXP(-I*(R02-R)/A02)
        S1=S1+INT*(-SE1*((R+A01/I+R02)*R02*T(1,1)+(R+A01/I+2*R02)*T(1,2)
     /   +2.*T(1,3))+SE12*((-R+A01/I+R02)*R02*T(1,1)+
     /    (-R+A01/I+2.*R02)*T(1,2)+2.*T(1,3))-SE2*(-A02/I*(R+A02/I)*
     /    T(2,1)+(R-A02/I)*T(2,2)+2.*T(2,3))+SE22*(-A02/I*(-R+A02/I)*
     /    T(2,1)-(R+A02/I)*T(2,2)+2.*T(2,3)))
        INT=-INT
  11    CONTINUE
        CONAL = 4.*PI*RO00*16.*(GAM*GAM/PI)**3*4.*A01/R
        RI11=CONAL*(S1+R/12.*(4.*PI*PI*A01*A01))
        RETURN
        END
C
        SUBROUTINE SUMALP_LT(RI11)
C IF R<R01+R02 AND ALPHA FOR A01 RI11 HAS THE SUCH TYPE
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        COMMON/AAA/ R,CONST12,CONST13,CONST22,CONST23,PI,NN
        COMMON/BBB/ A01,A02,R01,R02,PIA01,PIA02
        COMMON/CON/ CONSTP,CONSTP2,CONSTP1,CHSH,CHSH2,FINN,FEXX
        COMMON/RO/ RO00
        COMMON/G/ GAM,GAS
        DIMENSION T(2,3)
C        READ (5,2) N
C  2     FORMAT (I5)
        N = 40
        INT=-1
        S1=0.0
        S2=0.0
        S3=0.0
        S4=0.0
        A1A2 = A01/A02
        A2A1 = A02/A01
        K=0
        DO 14 I=1,N
        K=I
        IF(I.EQ.1) GO TO 14
        IF((IDINT(A2A1*I+0.015)-IDINT(A2A1*I)).GT.0..OR.(
     /   IDINT(A2A1*I-0.015)-IDINT(A2A1*I)).LT.0) GO TO 12
        IF((IDINT(A1A2*I+0.015)-IDINT(A1A2*I)).GT.0..OR.(
     /   IDINT(A1A2*I-0.015)-IDINT(A1A2*I)).LT.0) GO TO 12
   14   CONTINUE
   12   N=K-2
        DO 11 I=1,N
           SPA2 = DSIN(PI*I*A2A1)
           SPA1 = DSIN(PI*I*A1A2)
           CPA2 = DCOS(PI*I*A2A1)
           CPA1 = DCOS(PI*I*A1A2)
           T(1,1) =  PIA02/SPA2
           T(2,1) =  PIA01/SPA1
           T(1,2) =  PIA02*PIA02/SPA2/SPA2*CPA2
           T(2,2) =  PIA01*PIA01/SPA1/SPA1*CPA1
           T(1,3) = PIA02*PIA02*PIA02/2*(1+CPA2*CPA2)/SPA2/SPA2/SPA2
           T(2,3) = PIA01*PIA01*PIA01/2*(1+CPA1*CPA1)/SPA1/SPA1/SPA1
        T23 = 2.*T(2,3)
        T13 = 2.*T(1,3)
        SE1=DEXP(-I*(R+R02)/A01)
        SE12=DEXP(-I*(R02-R)/A01)
        SE2=DEXP(-I*(R+R02)/A02)
        SE22=DEXP(-I*(R02-R)/A02)
        S1=S1+INT*(-SE1*((R+A01/I+R02)*R02*T(1,1)+(R+A01/I+2.*R02)
     /   *T(1,2)+T13)+SE12*((-R+A01/I+R02)*R02*T(1,1)+(-R+A01/I+2.*R02)*
     /    T(1,2)+T13))
        S2=S2+INT*(-SE2*((R-A02/I)*T(2,2)+T23-A02/I*(R+A02/I)*T(2,1))+
     /    SE22*((-R-A02/I)*T(2,2)+T23-A02/I*(-R+A02/I)*T(2,1)))
        S3=S3+INT*(SE1*((R+A01/I+R02)*R02*T(1,1)+(R-A01/I+2.*R02)*T(1,2)
     /    +T13-A01/I*(R+A01/I+2.*R02)*T(1,1))-SE12*((-R+A01/I+R02)*R02*
     /     T(1,1)+(-R-A01/I+2.*R02)*T(1,2)+T13-A01/I*(-R+A01/I+2.*R02)*
     /     T(1,1)))*I
        S4=S4+INT*(SE2*((R-A02/I)*T(2,2)+T23-A02/I*(R+A02/I)*T(2,1))-
     /     SE22*((-R-A02/I)*T(2,2)+T23-A02/I*(-R+A02/I)*T(2,1)))*I
        INT=-INT
  11    CONTINUE
        CONALP = -4.*PI*RO00*RO00*16.*(GAM*GAM/PI)**(1.5)*A01*A02/R
C        CHSH=-DCOSH(R02/A02)/A02/DSINH(R02/A02)
        RI11=CONALP*(CHSH2*(S1+S2+R/12.*(
     /   4.*PI*PI*A01*A01))+S3/A01+S4/A02)
        RETURN
        END
C-------------------------------------------------------------------------
