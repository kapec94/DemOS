AC_INIT([DemOS-Tools], [0.1], [mateusz.kaplon@gmail.com])

AC_DEFUN([MY_AC_CHECK_PROG],
         [{ AC_CHECK_PROG($1, [$2], [ok])
            if test x"$$1" != x"ok" ; then
                AC_MSG_ERROR([Please install $2 before building])
            fi }]
)

AC_CONFIG_FILES([create-disk-image.sh], [chmod +x create-disk-image.sh])

MY_AC_CHECK_PROG([HAS_DD], [dd])
MY_AC_CHECK_PROG([HAS_PARTED], [parted])
MY_AC_CHECK_PROG([HAS_MKFS_VFAT], [mkfs.vfat])

AC_OUTPUT
