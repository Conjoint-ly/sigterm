#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <signal.h>

static volatile sig_atomic_t flag = -1;
static volatile SEXP the_expr;
static volatile SEXP the_rho;

void handler(int signum)
{
  flag++;
}

SEXP install_handler()
{
#ifndef __WIN32 /* Do nothing on Windows, it does not support SIGTERM. */
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = handler;
  sigaction(SIGTERM, &action, NULL);
  flag = 0;
  return Rf_ScalarLogical(TRUE);
#else
  return Rf_ScalarLogical(FALSE);
#endif
}

void my_handler_(int signum)
{
    Rf_eval(the_expr, the_rho);
}

SEXP install_my_handler(SEXP expr, SEXP rho){
#ifndef __WIN32 /* Do nothing on Windows, it does not support SIGTERM. */
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    the_expr = expr;
    the_rho = rho;
    action.sa_handler = my_handler_;
    sigaction(SIGTERM, &action, NULL);
    flag = 0;
    return Rf_ScalarLogical(TRUE);
#else
    return Rf_ScalarLogical(FALSE);
#endif
}

SEXP has_sigterm_flag()
{
  int out;
  switch(flag) {
  case -1:
    out = NA_LOGICAL;
    break;
  case 0:
    out = FALSE;
    break;
  default:
    out = TRUE;
  }
  return Rf_ScalarLogical(out);
}

static const R_CallMethodDef sigterm_entries[] = {
  {"R_install_handler", (DL_FUNC) &install_handler, 0},
  {"R_install_my_handler", (DL_FUNC) &install_my_handler, 2},
  {"R_has_sigterm_flag", (DL_FUNC) &has_sigterm_flag, 0},
  {NULL, NULL, 0}
};

void R_init_sigterm(DllInfo *info) {
  R_registerRoutines(info, NULL, sigterm_entries, NULL, NULL);
  R_useDynamicSymbols(info, FALSE);
}
