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
 */

/* Based on Micheal Wolfe's slides:
 * http://www.openacc.org.php53-2.ord1-1.websitetestlink.com/sites/default/files/PGI-Wolfe-GTC-Notes.pdf
 *
 */
/////////////////// First ////////////////////
//////////////////////////////////////////////
#pragma acc routine
extern void matvec(float* v,float* x,... );
...
#pragma acc parallel loop ...
for( int i = 0; i < n; ++i )
  matvec( v, x, i, n );
//////////////////////////////////////////////
#pragma acc routine
matvec(...){
#pragma acc parallel loop
  for( int i = 0; i < n; ++i ){...}
//////////////////////////////////////////////
//////////////////////////////////////////////

/////////////////// Second ///////////////////
//////////////////////////////////////////////
#pragma acc routine
  extern void matvec(float* v,float* x,... );
  ...
#pragma acc parallel num_gangs(1)
    {
      matvec( v0, x0, i, n );
      matvec( v1, x1, i, n );
      matvec( v2, x2, i, n );
    }
//////////////////////////////////////////////
#pragma acc routine
    matvec(...){
#pragma acc parallel loop
      for( int i = 0; i < n; ++i ){...}
//////////////////////////////////////////////
//////////////////////////////////////////////

/*
 * The second case shows that accelerator creates a new
 * thread to launch all the parallelism kernels.
 *
 * This is a good way, the point is ... ?
 */
