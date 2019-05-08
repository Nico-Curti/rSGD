#include <parameters.h>

parameters :: parameters (const long int & y, const double & eta, const double & lambda, const double & gamma, const double & eta_factor, const double & lambda_factor, const double & gamma_step) : eta_factor (eta_factor), lambda_factor (lambda_factor), gamma_step (gamma_step), y (y), eta (eta), lambda (lambda), gamma (gamma)
{
}

parameters :: parameters (const parameters & p)
{
  y = p.y;
  eta = p.eta;
  lambda = p.lambda;
  gamma = p.gamma;
  eta_factor = p.eta_factor;
  lambda_factor = p.lambda_factor;
  gamma_step = p.gamma_step;
}

parameters & parameters :: operator = (const parameters & p)
{
  y = p.y;
  eta = p.eta;
  lambda = p.lambda;
  gamma = p.gamma;
  eta_factor = p.eta_factor;
  lambda_factor = p.lambda_factor;
  gamma_step = p.gamma_step;
  return *this;
}

void parameters :: update (parameters & p)
{
  p.eta *= p.eta_factor;
  p.lambda *= p.lambda_factor;
  p.gamma += p.gamma_step;
}
