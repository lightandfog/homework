program sblas
    
  use mpi
  use timers
  type(TimerType) :: vecaddTime,vecsumTime,trimvTime,totalTime
  
  integer i,j,n,m,k,nsteps
  integer nx
  integer status(MPI_STATUS_SIZE)
  parameter (n=1024*60,m=512)
  parameter (nsteps=1000)
  real*8 a(n),b(n),x(n),y(m),l(m,m)
  real*8 buff(n)
  common /heap/ a,b,x,y,l
  real*8 res

! MPI specific data

  integer myid, numprocs, rc, ierr

! ..Intrinsic functions

! .. Executable statements

! MPI initialisation

  call MPI_INIT( ierr )
  call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
  call MPI_COMM_SIZE( MPI_COMM_WORLD, numprocs, ierr )
  print *, "Process ", myid, " of ", numprocs, " is alive"

  call TimerCreate(vecaddTime,"vecadd")
  call TimerCreate(vecsumTime,"vecsum")
  call TimerCreate(trimvTime,"trimv")
  if (myid.eq.0) call TimerCreate(totalTime,"total")

! Loop over whole program to get reasonable execution time
! (simulates a time-steping code)

! start total timer on master (proc 0)

  if (myid.eq.0) call TimerOn(totalTime)

  do k=1,nsteps

! 'Update' vector data on processor 0

     if (myid.eq.0) call vector_update(n,x,a,b)

! Work out size of data on this processor (not a general solution!)

     nx = n/numprocs

! Distribute the vectors a and b to other processors

     if (myid.eq.0) then
! MASTER
! send parts of the vectors a and b to other processors (x not reqd)

        do i=1,numprocs-1
           call MPI_SEND(a(i*nx+1), nx, MPI_DOUBLE_PRECISION, & 
                I, 0, MPI_COMM_WORLD, ierr)
           call MPI_SEND(b(i*nx+1), nx, MPI_DOUBLE_PRECISION, &
                I, 0, MPI_COMM_WORLD, ierr)
        end do
     else
! SLAVE
! receive data

        call MPI_RECV(a, nx, MPI_DOUBLE_PRECISION, &
             0, 0, MPI_COMM_WORLD, &
             status, ierr)
         call MPI_RECV(b, nx, MPI_DOUBLE_PRECISION, &
              0, 0, MPI_COMM_WORLD, &
              status, ierr)
      end if

! vector addition operation: each processor adds its own parts 
! Note: only master time is collected...

      call TimerOn(vecaddTime)
      call vecadd (nx,x,a,b)
      call TimerOff(vecaddTime)

! Gather result vector on processor 0

      if (myid.eq.0) then
! MASTER
! receive parts of result into the appropriate parts of my x

         do i=1,numprocs-1
            call MPI_RECV(x(i*nx+1), nx, MPI_DOUBLE_PRECISION, &
                 i, 0, MPI_COMM_WORLD, &
                 status, ierr)
         end do

      else
! SLAVE
! send my part of result vector

         call MPI_SEND(x, nx, MPI_DOUBLE_PRECISION, &
              0, 0, MPI_COMM_WORLD, ierr)

      end if

! Rest of the code is performed on processor 0

      if (myid.eq.0) then

! sum operation

         call TimerOn(vecsumTime)
         call vecsum(n,res,x)
         call TimerOff(vecsumTime)

! 'Update' lower triangular matrix and y vector

         call matrix_update(m,y,l)

! triangular matrix times vector operation

         call TimerOn(trimvTime)
         call trimv (m,y,l,a)
         call TimerOff(trimvTime)

      end if

! end time-step loop
   end do

   if (myid == 0) call TimerOff(totalTime)
! print out some results

   do i=1,numprocs
     call MPI_Barrier(MPI_COMM_WORLD, ierr)
     if (myid == (i-1)) then
       if (myid.eq.0) then
          print *,'MASTER output'
          print *,'x(1) = ',x(1),'x(n) = ', x(n)
          print *,'Sum of x() = ',res
          print *,'y(1) = ',y(1),'y(m) = ', y(m)
          call TimerPrint(totalTime)
          call TimerPrint(vecsumTime)
          call TimerPrint(trimvTime)
          print *,'End MASTER output'
          call flush()
! end of processor 0 processing
       end if

       print *,'myid= ', myid
       call TimerPrint(vecaddTime)
       call flush()
     end if
   end do
   call MPI_FINALIZE(ierr)
 end program sblas

! Subroutines

 subroutine vector_update(n,x,a,b)

! .. Scalar arguments
   integer n

! .. Array arguments
   real*8 x(n),a(n),b(n)

! .. Local scalars
   integer i

! .. External functions

   do i=1,n
      x(i) = 0.
      a(i) = 3.142
      b(i) = 3.142
   end do

   return
 end subroutine vector_update

 subroutine matrix_update(m,y,l)

! .. Scalar arguments
   integer m

! .. Array arguments
   real*8 y(m),l(m,m)

! .. Local scalars
   integer i,j

! .. External functions

   do i=1,m
      y(i) = 0.
      do j=1,i
         l(j,i) = 2.
      end do
   end do

   return
 end subroutine matrix_update

 subroutine vecadd (n,x,a,b)

! .. Scalar arguments
   integer n

! .. Array arguments
   real*8 x(n),a(n),b(n)

! .. Local scalars
   integer i

! .. External functions

   do i=1,n
      x(i) = a(i) + b(i)
   end do
   return
 end subroutine vecadd

 subroutine vecsum(n,sum,a)

! .. Scalar arguments 
   integer n
   real*8 sum

! .. Array arguments
   real*8 a(n)

! .. Local scalars
   integer i

   sum = 0.0
   do i=1,n
      sum = sum +a(i)
   end do
   return
 end subroutine vecsum

 subroutine trimv (m,y,l,a)

! .. Scalar arguments
   integer m

! .. Array arguments
   real*8 y(m),l(m,m),a(m)

! .. Local scalars
   integer i,j

! .. External functions

   do i=1,m
      do j=1,i
         y(i) = y(i) + l(j,i)*a(j)
      end do
   end do
   return
 end subroutine trimv

