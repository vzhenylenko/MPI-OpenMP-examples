      Program Example3a
       Implicit None
       Include 'mpif.h'
       Integer Size, Rank, Ierr, I, N, Status(MPI_STATUS_SIZE), argc
       Integer*8 nPart
       Double Precision Sum, GSum, A, B, time1, time2, timeL, timeI,
     $ Al, Bl, X, Yl, Yr, F, ISum, TRUE, timeT 
       Character argv*32
C      Пределы интегрирования
       Parameter (A=0, B=1)
       Parameter (TRUE=0.565985876838710)
       
C      Подынтегральная функция
       F(x)=(x*x*x+x)/(x*x*x*x+1)
       Call MPI_INIT(Ierr)
       Call MPI_COMM_SIZE(MPI_COMM_WORLD, Size, Ierr)
       Call MPI_COMM_RANK(MPI_COMM_WORLD, Rank, Ierr)
       
       argc = iargc()
       CALL getarg(1,argv)
       READ(argv,*) nPart
C      «0»-процесс засекает время
       time1 = MPI_WTime()
C      Каждый процесс определяет свои пределы интегрирования
C      и число интервалов разбиения
       Al = A+(B-A)*Rank/Size
       Bl = Al+(B-A)/Size
C        N = 1000000000/Size

       N = nPart/Size
C      Каждый процесс определяет свою частичную сумму ...
       Sum = 0
       Yl = F(Al)
       Do I = 1,N
       X = Al+(Bl-Al)*I/N
       Yr = F(X)
       Sum = Sum + Yr + Yl
       Yl = Yr
       End Do

C      «0»-процесс получает частичные суммы и определяет результат
       If (Rank.eq.0) Then
       timeT = MPI_WTime() - time1
       GSum = Sum
       Do I=1,Size-1
       Call MPI_RECV(ISum, 1, MPI_DOUBLE_PRECISION,
     $ MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, Status, Ierr)
       Call MPI_RECV(timeI, 1, MPI_DOUBLE_PRECISION,
     $ MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, Status, Ierr)
       GSum = GSum + ISum
       timeT = timeT + timeI
       End Do
       time2 = MPI_WTime()
       GSum = GSum/(N*Size)/2*(B-A)
C        Write (6,*) 'Result= ',GSum,' Error= ',TRUE-GSum,
C      $' Time=',time2 - time1
       Write (6,*) 'Result = ',GSum,' Error = ',TRUE-GSum
       Write (6,*) 'Time = ', time2 - time1
       
       else
       timeL = MPI_WTime() - time1
       Call MPI_SEND(Sum, 1, MPI_DOUBLE_PRECISION, 0,
     $ 0, MPI_COMM_WORLD, Ierr)
       Call MPI_SEND(timeL, 1, MPI_DOUBLE_PRECISION, 0,
     $ 0, MPI_COMM_WORLD, Ierr)
       End If
       Call MPI_FINALIZE(Ierr)
       Stop
       End
