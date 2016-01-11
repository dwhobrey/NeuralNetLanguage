title "Inverted Sigmoid"
nobars
xlab "Error"
ylab "Activity"
set samples 500
top = 1.0
bot = 0.0
gain = 150.0
cc = -0.08
sigmoid(x) = bot + (top-bot)/(1.0+exp(-gain * (cc+x)))
mm = 10.0
rr(x) = 0.5
ss(x) = rr(x)*mm
kk(x) = 2.0*ss(x)*ss(x)
ccc(x) = 2.5066 * ss(x)
gauss(x) = exp(-(((x-mm)*(x-mm))/kk(x)))/ccc(x)
ngauss(x) = exp(-(((x-mm)*(x-mm))/kk(x)))