        program compute_pi
         Implicit None
         include 'omp_lib.h'


         Integer i, argc, nThrd, procs
         Integer*8 n
         Double precision w, x, sum, pi, f, y, truePI, time1, time2
         Character argv1*32
         Character argv2*32
         Integer maxThrd
         Parameter(truePI = 3.1415926535)

         argc = command_argument_count()
         call get_command_argument(1, argv1)
         call get_command_argument(2, argv2)

         READ(argv1, *) nThrd
         READ(argv2, *) n
C       Parameter (n=10000000)
C       Parameter (nThrd=8)
         maxThrd = OMP_GET_MAX_THREADS()
         procs = OMP_GET_NUM_PROCS()

         w = 1.d0/n
         sum = 0.0d0
         time1 = omp_get_wtime() 
!$omp parallel do private(x) num_threads(nThrd)
!$omp& reduction(+:sum)
         do i=1,n
         x = w*(i-0.5d0)
         f = 4.d0/(1.d0 + x*x)
         sum = sum + f
         end do

         pi = w*sum
         time2 = omp_get_wtime()
         Write (6,*) 'Value = ', pi
         Write (6,*) 'Error = ', pi-truePI
         Write (6,*) 'Time = ', time2-time1
         end
