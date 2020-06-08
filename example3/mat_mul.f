        program matmult
         Implicit None
         include 'omp_lib.h'

         integer n
         parameter(n=4096)
         Integer i, j, k, argc, nThrd, procs
         Integer*8 m
         Double precision w, x, sum, pi, f, y, truePI, t1, t2
         common /arr/ a, b, c
         double precision a(n, n), b(n, n), c(n, n)
         Character argv1*32
         Character argv2*32
         Integer maxThrd
         real, dimension (:,:), allocatable :: da 
         real, dimension (:,:), allocatable :: db
         real, dimension (:,:), allocatable :: dc
         integer :: s

         argc = command_argument_count()
         call get_command_argument(1, argv1)
         call get_command_argument(2, argv2)

         READ(argv1, *) nThrd
         READ(argv2, *) s
C       Parameter (n=10000000)
C       Parameter (nThrd=8)
         maxThrd = OMP_GET_MAX_THREADS()
         procs = OMP_GET_NUM_PROCS()
         
         allocate ( da(s,s) )
         allocate ( db(s,s) )
         allocate ( dc(s,s) )
         
         do i=1, s
         do j=1, s
         da(i, j)=i*j
         db(i, j)=i*j
         end do
         end do

         t1=omp_get_wtime()
!$omp parallel do shared(da, db, dc) private(i, j, k) num_threads(nThrd)
         do j=1, s
         do i=1, s
         dc(i, j) = 0.0
         do k=1, s
         dc(i, j)=dc(i, j)+da(i, k)*db(k, j)
         end do
         end do
         end do
        
         t2=omp_get_wtime()
         print *, "Time =", t2-t1
         
         IF (s < 6) THEN
            do i = 1, s
            print *, dc(i, :)
            end do
         ELSE
            print *, "Too big"
         END IF
         end
