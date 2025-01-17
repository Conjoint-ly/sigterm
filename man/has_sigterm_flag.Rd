% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/package.R
\name{has_sigterm_flag}
\alias{has_sigterm_flag}
\title{Check Whether R Has Received SIGTERM}
\usage{
has_sigterm_flag()
}
\value{
\code{TRUE} if R has received \code{SIGTERM} and \code{FALSE} if it has not. If
\code{SIGTERM} is not available (e.g. on Windows), the return value is \code{NA}
instead.
}
\description{
On Unix-like operating systems, a process can be sent the \code{SIGTERM} signal
to initiate a graceful shutdown. By default in R, this ends the process.
However, when this package is loaded, we install a signal handler that
prevents this from happening and instead allows the user to check for the
signal with this function.

On Windows, this function will always return \code{NA}.
}
\examples{
# This will be FALSE until the signal has been received,
# or NA on Windows:
has_sigterm_flag()
}
