import math

def pdf(x):
    return math.exp(-(x) ** 2 / (2)) / (math.sqrt(2 * math.pi))

def sum_fun_xk(xk, func):
    return sum([func(each) for each in xk])

def integral(a, b, n, func):
    h = (b - a)/float(n)
    xk = [a + i*h for i in range(1, n)]
    return h/2 * (func(a) + 2 * sum_fun_xk(xk, func) + func(b))

def cdfd(a,b,u,o):
    return integral((a-u)/o,(b-u)/o,10000,pdf)



print 'odd: -3 strip: ', cdfd(-13.,-9., 0.,4.)
print 'odd: -2 strip: ', cdfd( -9.,-5., 0.,4.)
print 'odd: -1 strip: ', cdfd( -5.,-1., 0.,4.)
print 'odd:  0 strip: ', cdfd( -1., 3., 0.,4.)
print 'odd:  1 strip: ', cdfd(  3., 7., 0.,4.)
print 'odd:  2 strip: ', cdfd(  7., 11.,0.,4.)
print 'odd:  3 strip: ', cdfd( 11., 15.,0.,4.)

print 'even: -3 strip: ', cdfd(-15.,-11.,0.,4.)
print 'even: -2 strip: ', cdfd(-11., -7.,0.,4.)
print 'even: -1 strip: ', cdfd( -7.,-3., 0.,4.)
print 'even:  0 strip: ', cdfd( -3., 1., 0.,4.)
print 'even:  1 strip: ', cdfd(  1., 5., 0.,4.)
print 'even:  2 strip: ', cdfd(  5., 9., 0.,4.)
print 'even:  3 strip: ', cdfd(  9.,13., 0.,4.)
