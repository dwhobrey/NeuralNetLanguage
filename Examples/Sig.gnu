title "Inverted Sigmoid"
nobars
xlab "Error"
ylab "Activity"
set samples 500
top = 1.0
bot = 0.0
gain = 12.0
cc = -0.6
sigmoid(x) = bot + (top-bot)/(1.0+exp(-gain * (cc+x)))
invsig(x) = 1.0 - sigmoid(x)
plot [0:5] sigmoid(x)
