samples <- 3000
samples_1_3 = samples / 3
samples_2_3 = samples_1_3 * 2
samples_half <- samples / 2
sample_rate <- 48000

factor <- 12.0
start <- 2.0 * factor
mid_1 <- 1.0 * factor
mid_2 <- 0.0 * factor
end <- start

rate <- 0.00001
coef_1 <- exp(log(0.10) / (rate * sample_rate))
coef_1

input <- array(start, samples)

for (n in (2:samples_1_3))
{
  input[n] <- mid_1 + sin(n / 10)
}

for (n in ((samples_1_3 + 1):samples_2_3))
{
  input[n] <- mid_2 + sin(n / 10)
}

for (n in ((samples_2_3 + 1):samples))
{
  input[n] <- end + sin(n / 10)
}

output <- array(0.0, samples)
output[1] <- start

for (n in (2:samples))
{
  output[n] <- (1.0 - coef_1) * output[n - 1] + coef_1 * input[n]
}

plot(input, type="l", col="grey", xlab="Sample", ylab="Level [dB]")
lines(output, type="l", col="blue")


coef_2 <- coef_1
coef_prep <- log(0.10) / (rate * sample_rate)

y1 <- 0.33
x2 <- 3.0

a <- 1.0 / y1
b <- (1 + x2 - a) / x2

output <- array(0.0, samples)
output[1] <- start
last <- start

for (n in (2:samples))
{
  if (input[n] > last)
  {
    output[n] <- (1.0 - coef_2) * output[n - 1] + coef_2 * input[n]
  }
  else
  {
    gr_new <- last - input[n - 1]
    coef_factor <- (a + b * gr_new) / (1 + gr_new)
    coef_curr <- exp(coef_prep * coef_factor)
    output[n] <- (1.0 - coef_curr) * output[n - 1] + coef_curr * input[n]
  }

  last <- output[n]
}

lines(output, type="l", col="red")


## values <- c(1:50 - 1) / 2
## corr <- 100.0 * (a + b * values) / (1 + values)
## plot(values, corr, type="l", xlab="Level [dB]", ylab="Release rate correction [%]")

## abline(v=x2, lty="13", lwd=1, col="red")
## abline(h=100.0, lty="13", lwd=1, col="red")

## abline(v=0, lty="13", lwd=1, col="blue")
## abline(h=100.0 / y1, lty="13", lwd=1, col="blue")
