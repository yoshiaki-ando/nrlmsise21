subroutine wrap_msisinit()
  use msis_init, only : msisinit

  call msisinit(parmpath='/home/ando/share/msis21/',parmfile='msis21.parm')

end subroutine wrap_msisinit
