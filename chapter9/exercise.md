# Chapter 9 Exercise

## 9.1

Original
    real = 1000, eff  = 0, sav  = 0, fs   = 0

a. real=2000, effective=2000, saved=2000, file-system=2000
    privilege user with setuid change all id

b. real=1000, effective=2000, saved=2000, file-system=2000
    change effective id to 2000, file-system id follows effective id value.
    Since effective id doesn't equal to old real id, saved id become new effective id.

c. real=1000, effective=2000, saved=   0, file-system=2000
    seteuid(2000) = setresuid(-1,2000,-1)

d. real=1000, effective=   0, saved=   0, file-system=2000

e. real=1000, effective=2000, saved=3000, file-system=2000
    change effective id to 2000, file-system id follows effective id value.

## 9.2

Nope, privilege user's effective uid is 0
Even though its real uid is 0, it have to do ``seteuid(0)`` to get it's privilege back.

##  9.3

![Code](./p_3.c)

## 9.4 

```c
pid_t saved_uid = geteuid();

// setuid
setuid(getuid());       /* X Y Y -> X X Y (for unprivilege) */
setuid(saved_uid);      /* X X Y -> X Y Y (for unprivilege) */
                        /* cannot do that */

// seteuid
seteuid(getuid());      /* X Y Y -> X X Y */
seteuid(saved_uid);     /* X X Y -> X Y Y */
                        /* cannot do that */
// setreuid
setreuid(-1, getuid());      /* X Y Y -> X X Y */
setreuid(-1, saved_uid);     /* X X Y -> X Y Y */
setreuid(getuid(), getuid()) /* X Y Y -> X X X */

// setresuid
setresuid(-1, getuid(), -1);             /* X Y Y -> X X Y */
setresuid(-1, saved_uid, -1);            /* X X Y -> X Y Y */
setresuid(getuid(), getuid(), getuid()); /* X Y Y -> X X X */

```

## 9.5

```c
pid_t saved_uid = geteuid();

// setuid
                        /* X 0 0 -> X X 0 cannot do that */
                        /* X X 0 -> X 0 0 cannot do that */
setuid(getuid());       /* X 0 0 -> X X X */

// seteuid
seteuid(getuid());      /* X 0 0 -> X X 0 */
seteuid(saved_uid);     /* X X 0 -> X 0 0 */
                        /* X 0 0 -> X X X cannot do that */ 

// setreuid
setreuid(-1, getuid());         /* X 0 0 -> X X 0 */
setreuid(-1, saved_uid);        /* X X 0 -> X 0 0 */
setreuid(getuid(), getuid());   /* X 0 0 -> X X X */

// setresuid
setresuid(-1, getuid(), -1);                /* X 0 0 -> X X 0 */
setresuid(-1, saved_uidd, -1);              /* X X 0 -> X 0 0 */
setresuid(getuid(), getuid(), getuid());    /* X 0 0 -> X X X */
```
