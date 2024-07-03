!#######################################################################
! MSIS(NRL-SOF-014-1) SOFTWARE
! NRLMSISョ empirical atmospheric model software. Use is governed by the
! Open Source Academic Research License Agreement contained in the file
! nrlmsis2.1_license.txt, which is part of this software package. BY
! USING OR MODIFYING THIS SOFTWARE, YOU ARE AGREEING TO THE TERMS AND
! CONDITIONS OF THE LICENSE.  
!#######################################################################

!!! ===========================================================================
!!! NRLMSIS 2.1:
!!! Neutral atmosphere empirical model from the surface to lower exosphere
!!! ===========================================================================

!==================================================================================================
! MSISTEST: Test program for NRLMSIS 2.1
!==================================================================================================
subroutine msistest(iyd, sec, alt, glat, glong, f107a, f107, apd, d, t)

  !use msis_init, only          : msisinit

  implicit none

  integer, parameter          :: nrec = 200

  integer                     :: iyd, mass
  real(4)                     :: sec, alt, glat, glong, stl, f107a, f107, ap(7), apd
  real(4)                     :: d(10),t(2)
  
  character(128)              :: dummy

  !Initialize model
  !call msisinit(parmpath='/home/ando/share/msis21/',parmfile='msis21.parm')

  !Open input and output files, loop through records, and call model
  !open(77,file='msis2.1_test_in.txt',status='old')
  !open(78,file='msis2.1_test_out.txt',status='replace')
  !read(77,*) dummy
  !write(78,'(9a7,10a13,a8)') &
  !  'iyd','sec','alt','glat','glong','stl','f107a','f107','Ap','He','O','N2','O2','Ar','rho','H','N','O*','NO','T'
  !do i = 1,200
  !  read(77,*) iyd,sec,alt,glat,glong,stl,f107a,f107,apd
  ap(1) = apd
  stl = 1.0
  call gtd8d(iyd,sec,alt,glat,glong,stl,f107a,f107,ap,mass,d,t)
  !  write(78,'(2i7,3f7.1,f7.2,3f7.1,10e13.4,f8.2)')  &
  !    iyd,int(sec),alt,glat,glong,stl,f107a,f107,ap(1),d(1:10),t(2)
  !enddo
  !close(77)
  !close(78)

  !stop

end subroutine msistest

subroutine msis_profile(iyd, sec, Nalt, alt_min, alt_step, glat, glong, f107a, f107, apd, d, t)
  implicit none

  integer :: Nalt
  real(4) :: alt_min, alt_step
  integer                     :: iyd, mass
  real(4)                     :: sec, alt, glat, glong, stl, f107a, f107, ap(7), apd
  real(4)                     :: d(Nalt*10),t(Nalt*2), dtmp(10), ttmp(2)
  integer                     :: i, offset_d, offset_t

  ap(1) = apd
  stl = 1.0

  do i = 1, Nalt
     offset_d = (i-1) * 10
     offset_t = (i-1) * 2
     alt = alt_min + (i-1)*alt_step
     call gtd8d(iyd,sec,alt,glat,glong,stl,f107a,f107,ap,mass,dtmp,ttmp)
     d( offset_d+1:offset_d+10 ) = dtmp(1:10)
     t( offset_t+1:offset_t+2 ) = ttmp(1:2)
  enddo

end subroutine msis_profile
