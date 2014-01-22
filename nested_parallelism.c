/*
 * Spec 2.0 introduces a new concept -- nested parallelism
 *
 * "On some devices, the accelerator may also create and launch
 *  parallel kernels, allowing for nested parallelism. In that
 *  case, the OpenACC directives may be executed by a host thread
 *  or an accelerator thread. This specification uses the term
 *  local thread or local memory to mean the thread that executes
 *  the directive, or the memory associated with that thread,
 *  whether thatthread executes on the host or on the accelerator."
 *
 * A more concise way to interpret:
 *     nested parallelism using parallel or kernels region
 *     containing another parallel or kernels region
 *
 *
 */
