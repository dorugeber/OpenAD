subroutine  solve(n,A,x,b,trans)
  implicit none
  integer :: n
  double precision :: A(n,n), x(n), b(n)
  logical trans
  ! local 
  integer :: i,j
  ! umfpack specific setup: 
  integer, parameter :: umfKeepSize=20
  integer, dimension(umfKeepSize) ::   umfKeep
  integer, parameter :: umfIcntlSize=20
  integer, dimension(umfIcntlSize)::   umfIcntl
  integer, parameter :: umfInfoSize=40
  integer, dimension(umfInfoSize)::   umfInfo
  integer, parameter :: umfCntlSize=10
  double precision, dimension(umfCntlSize)::   umfCntl
  integer, parameter :: umfRinfoSize=20
  double precision, dimension(umfRinfoSize)::   umfRinfo
  ! matrix values for triplrt form 
  integer, parameter :: umfValSize=100 ! simply fixed here for this example
  double precision, dimension(umfValSize)::   umfVal
  ! index values for triplet form
  integer, parameter :: umfIndexSize=100 ! simply fixed here for this example
  integer, dimension(umfIndexSize)::   umfIndex
  ! work array for back substitution
  double precision, dimension(2*n)::   umfWork
  umfKeep = 0
  umfCntl = 0
  umfIcntl = 0
  ! init
  call UMD21I(umfKeep,umfCntl,umfIcntl)
  ! explicitly reshape the matrix 
  ! and set the index values as needed 
  ! by umfpack
  do j=1,n
     do i=1,n
        umfVal((j-1)*n+i)=A(i,j)
        umfIndex((j-1)*n+i)=i     ! row
        umfIndex((j-1)*n+i+n*n)=j ! column
     end do
  end do
  call UMD2FA(  & 
       n,       & ! rank 
       n*n,     & ! nnz
       1,       & ! keep the input matrix intact
       trans,   & ! transposed matrix?
       umfValSize, & 
       umfIndexSize, &  
       umfVal,  &  
       umfIndex, & 
       umfKeep, &
       umfCntl, & 
       umfIcntl, & 
       umfInfo, & 
       umfRinfo)
  if (umfInfo (1) .lt. 0) print *, 'error in UMD2FA'
  call UMD2SO(  & 
       n,       & ! rank 
       0,       & ! no iterative refinement
       .false., & ! transposed again?
       umfValSize, & 
       umfIndexSize, &  
       umfVal,  &  
       umfIndex, & 
       umfKeep, &
       b, & 
       x, & 
       umfWork, & 
       umfCntl, & 
       umfIcntl, & 
       umfInfo, & 
       umfRinfo)  
  if (umfInfo (1) .lt. 0) print *, 'error in UMD2SO'
end subroutine
