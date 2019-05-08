#ifndef __parameters__
#define __parameters__

class parameters
{
  // Private members

  double eta_factor;
  double lambda_factor;
  double gamma_step;

public:

  // Public members

  long int y;

  double eta;
  double lambda;
  double gamma;


  // Constructors

  parameters (const long int & y, const double & eta, const double & lambda, const double & gamma, const double & eta_factor, const double & lambda_factor, const double & gamma_step);

  // Destructors

  ~parameters () = default;

  // Copy operators

  parameters (const parameters & p);
  parameters & operator = (const parameters & p);

  // Static members

  static void update (parameters & p);

};

#endif // __parameters__
