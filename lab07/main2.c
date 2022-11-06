#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

void Trap ( double a , double b , int n , double * global_result_p );

double f(double x){
  return exp(x);
}

//main -> pdf do professor
int main (int argc , char * argv []) {
  double global_result = 0.0;
  double a , b;
  int n;
  //contador de threads
  int thread_count ;
  thread_count = strtol ( argv [1] , NULL , 10) ;
  //input
  printf (" Enter a, b and n\n") ;
  scanf ("%lf %lf %d", &a , &b , &n );
  #pragma omp parallel num_threads(thread_count)
  Trap (a , b , n , & global_result );
  //saída com gresultado+1
  printf (" With n = %d trapezoids , our estimate \n", n) ;
  printf ("of the integral from %g to %g = %.14e\n", a , b , global_result+1 );
  return 0;
} /* main */

//atomic pode ser utilizado pois a regiao critica encaixa no formato necessario
void Trap ( double a , double b , int n , double * global_result_p ){
  double h , x , my_result ;
  double local_a , local_b ;
  int i , local_n ;
  int my_rank = omp_get_thread_num () ;
  int thread_count = omp_get_num_threads () ;
  h = (b -a) /n;
  local_n = n/ thread_count ;
  local_a = a + my_rank * local_n *h;
  local_b = local_a + local_n * h;
  my_result = (f( local_a ) + f( local_b )) /2.0;
  for (i =1; i < local_n ; i ++) {
    x = local_a + i* h;
    my_result += f(x );
    }
    my_result = my_result *h ;
    #pragma omp atomic
    * global_result_p += my_result ;
} /* Trap */
